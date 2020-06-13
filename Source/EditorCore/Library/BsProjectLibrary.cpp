//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Library/BsProjectLibrary.h"
#include "FileSystem/BsFileSystem.h"
#include "Error/BsException.h"
#include "Resources/BsResources.h"
#include "Resources/BsResourceManifest.h"
#include "Importer/BsImporter.h"
#include "Library/BsProjectResourceMeta.h"
#include "Resources/BsResources.h"
#include "Importer/BsImporter.h"
#include "Importer/BsImportOptions.h"
#include "Serialization/BsFileSerializer.h"
#include "Serialization/BsSerializedObject.h"
#include "Serialization/BsBinaryDiff.h"
#include "Debug/BsDebug.h"
#include "Library/BsProjectLibraryEntries.h"
#include "Resources/BsResource.h"
#include "BsEditorApplication.h"
#include "Material/BsShader.h"
#include "Image/BsTexture.h"
#include "String/BsUnicode.h"
#include "CoreThread/BsCoreThread.h"
#include "Threading/BsTaskScheduler.h"
#include "RenderAPI/BsRenderTexture.h"
#include "Renderer/BsRendererUtility.h"
#include <regex>

using namespace std::placeholders;

namespace bs
{
	void coreGeneratePreviewPyramid(const SPtr<ct::Texture>& input, const SPtr<ct::RenderTexture>* outputs, 
		UINT32 numOutputs)
	{
		if(numOutputs == 0)
			return;

		gProfilerGPU().beginFrame();
		ct::RenderAPI& rapi = ct::RenderAPI::instance();

		rapi.setRenderTarget(outputs[0]);
		ct::gRendererUtility().blit(input);

		for(UINT32 i = 1; i < numOutputs; i++)
		{
			rapi.setRenderTarget(outputs[i]);
			ct::gRendererUtility().blit(outputs[i - 1]->getColorTexture(0), Rect2I::EMPTY, false, false, true);
		}

		rapi.submitCommandBuffer(nullptr);
		gProfilerGPU().endFrame(true);
	}

	ProjectResourceIcons generatePreviewIcons(Resource& resource)
	{
		ProjectResourceIcons icons;

		const UINT32 typeId = resource.getTypeId();
		if(typeId == TID_Texture)
		{
			Texture& texture = static_cast<Texture&>(resource);
			SPtr<Texture> texturePtr = static_pointer_cast<Texture>(texture.getThisPtr());

			UINT32 sizes[] = { 256, 192, 128, 96, 64, 48, 32, 16 };

			constexpr UINT32 numSizes = (UINT32)bs_size(sizes);
			SPtr<RenderTexture> rts[numSizes];
			SPtr<ct::RenderTexture> coreRts[numSizes];
			for(UINT32 i = 0; i < numSizes; i++)
			{
				TEXTURE_DESC rtDesc;
				rtDesc.usage |= TU_CPUREADABLE; 
				rtDesc.width = sizes[i];
				rtDesc.height = sizes[i];

				rts[i] = RenderTexture::create(rtDesc, false);
				coreRts[i] = rts[i]->getCore();
			}

			gCoreThread().queueCommand(
				[coreTex = texturePtr->getCore(), coreRts, numSizes]()
			{
				coreGeneratePreviewPyramid(coreTex, coreRts, numSizes);
			}, CTQF_BlockUntilComplete);

			icons.icon256 = rts[0]->getColorTexture(0);
			icons.icon192 = rts[1]->getColorTexture(0);
			icons.icon128 = rts[2]->getColorTexture(0);
			icons.icon96 = rts[3]->getColorTexture(0);
			icons.icon64 = rts[4]->getColorTexture(0);
			icons.icon48 = rts[5]->getColorTexture(0);
			icons.icon32 = rts[6]->getColorTexture(0);
			icons.icon16 = rts[7]->getColorTexture(0);
		}

		return icons;
	}

	const Path TEMP_DIR = "Temp/";
	const Path INTERNAL_TEMP_DIR = PROJECT_INTERNAL_DIR + TEMP_DIR;

	const Path ProjectLibrary::RESOURCES_DIR = "Resources/";
	const Path ProjectLibrary::INTERNAL_RESOURCES_DIR = PROJECT_INTERNAL_DIR + RESOURCES_DIR;
	const char* ProjectLibrary::LIBRARY_ENTRIES_FILENAME = "ProjectLibrary.asset";
	const char* ProjectLibrary::RESOURCE_MANIFEST_FILENAME = "ResourceManifest.asset";

	ProjectLibrary::LibraryEntry::LibraryEntry()
		:type(LibraryEntryType::Directory)
	{ }

	ProjectLibrary::LibraryEntry::LibraryEntry(const Path& path, const String& name, DirectoryEntry* parent, 
		LibraryEntryType type)
		:type(type), path(path), elementName(name), elementNameHash(bs_hash(UTF8::toLower(name))), parent(parent)
	{ }

	ProjectLibrary::FileEntry::FileEntry(const Path& path, const String& name, DirectoryEntry* parent)
		:LibraryEntry(path, name, parent, LibraryEntryType::File)
	{ }

	ProjectLibrary::DirectoryEntry::DirectoryEntry(const Path& path, const String& name, DirectoryEntry* parent)
		:LibraryEntry(path, name, parent, LibraryEntryType::Directory)
	{ }

	ProjectLibrary::ProjectLibrary()
		: mRootEntry(nullptr), mIsLoaded(false)
	{
		mRootEntry = bs_ushared_ptr_new<DirectoryEntry>(mResourcesFolder, mResourcesFolder.getTail(), nullptr);
	}

	ProjectLibrary::~ProjectLibrary()
	{
		_finishQueuedImports(true);
		clearEntries();
	}

	UINT32 ProjectLibrary::checkForModifications(const Path& fullPath)
	{
		UINT32 resourcesToImport = 0;

		if (!mResourcesFolder.includes(fullPath))
			return resourcesToImport; // Folder not part of our resources path, so no modifications

		if(mRootEntry == nullptr)
			mRootEntry = bs_ushared_ptr_new<DirectoryEntry>(mResourcesFolder, mResourcesFolder.getTail(), nullptr);

		Path pathToSearch = fullPath;
		USPtr<LibraryEntry> entry = findEntry(pathToSearch);
		if (entry == nullptr) // File could be new, try to find parent directory entry
		{
			if (FileSystem::exists(pathToSearch))
			{
				if (isMeta(pathToSearch))
				{
					Path sourceFilePath = pathToSearch;
					sourceFilePath.setExtension("");

					if (!FileSystem::isFile(sourceFilePath))
					{
						BS_LOG(Warning, Editor, "Found a .meta file without a corresponding resource. Deleting.");

						FileSystem::remove(pathToSearch);
					}
				}
				else
				{
					Path parentDirPath = pathToSearch.getParent();
					entry = findEntry(parentDirPath);

					// Cannot find parent directory. Create the needed hierarchy.
					DirectoryEntry* entryParent = nullptr;
					DirectoryEntry* newHierarchyParent = nullptr;
					if (entry == nullptr)
						createInternalParentHierarchy(pathToSearch, &newHierarchyParent, &entryParent);
					else
						entryParent = static_cast<DirectoryEntry*>(entry.get());

					if (FileSystem::isFile(pathToSearch))
						addResourceInternal(entryParent, pathToSearch);
					else if (FileSystem::isDirectory(pathToSearch))
					{
						addDirectoryInternal(entryParent, pathToSearch);
						resourcesToImport += checkForModifications(pathToSearch);
					}
				}
			}
		}
		else if(entry->type == LibraryEntryType::File)
		{
			if(FileSystem::isFile(entry->path))
			{
				FileEntry* resEntry = static_cast<FileEntry*>(entry.get());
				if(reimportResourceInternal(resEntry))
					resourcesToImport++;
			}
			else
				deleteResourceInternal(static_pointer_cast<FileEntry>(entry));
		}
		else if(entry->type == LibraryEntryType::Directory) // Check folder and all subfolders for modifications
		{
			if(!FileSystem::isDirectory(entry->path))
			{
				deleteDirectoryInternal(static_pointer_cast<DirectoryEntry>(entry));
			}
			else
			{
				Stack<DirectoryEntry*> todo;
				todo.push(static_cast<DirectoryEntry*>(entry.get()));

				Vector<Path> childFiles;
				Vector<Path> childDirectories;
				Vector<bool> existingEntries;
				Vector<USPtr<LibraryEntry>> toDelete;

				while(!todo.empty())
				{
					DirectoryEntry* currentDir = todo.top();
					todo.pop();

					existingEntries.clear();
					existingEntries.resize(currentDir->mChildren.size());
					for(UINT32 i = 0; i < (UINT32)currentDir->mChildren.size(); i++)
						existingEntries[i] = false;

					childFiles.clear();
					childDirectories.clear();

					FileSystem::getChildren(currentDir->path, childFiles, childDirectories);
			
					for(auto& filePath : childFiles)
					{
						if(isMeta(filePath))
						{
							Path sourceFilePath = filePath;
							sourceFilePath.setExtension("");

							if(!FileSystem::isFile(sourceFilePath))
							{
								BS_LOG(Warning, Editor, "Found a .meta file without a corresponding resource. Deleting.");

								FileSystem::remove(filePath);
							}
						}
						else
						{
							FileEntry* existingEntry = nullptr;
							UINT32 idx = 0;
							for(auto& child : currentDir->mChildren)
							{
								if(child->type == LibraryEntryType::File && child->path == filePath)
								{
									existingEntries[idx] = true;
									existingEntry = static_cast<FileEntry*>(child.get());
									break;
								}

								idx++;
							}

							if(existingEntry != nullptr)
							{
								if(reimportResourceInternal(existingEntry))
									resourcesToImport++;
							}
							else
							{
								addResourceInternal(currentDir, filePath);
								resourcesToImport++;
							}
						}
					}

					for(auto& dirPath : childDirectories)
					{
						DirectoryEntry* existingEntry = nullptr;
						UINT32 idx = 0;
						for(auto& child : currentDir->mChildren)
						{
							if(child->type == LibraryEntryType::Directory && child->path == dirPath)
							{
								existingEntries[idx] = true;
								existingEntry = static_cast<DirectoryEntry*>(child.get());
								break;
							}

							idx++;
						}

						if(existingEntry == nullptr)
							addDirectoryInternal(currentDir, dirPath);
					}

					{
						for(UINT32 i = 0; i < (UINT32)existingEntries.size(); i++)
						{
							if(existingEntries[i])
								continue;

							toDelete.push_back(currentDir->mChildren[i]);
						}

						for(auto& child : toDelete)
						{
							if(child->type == LibraryEntryType::Directory)
								deleteDirectoryInternal(static_pointer_cast<DirectoryEntry>(child));
							else if(child->type == LibraryEntryType::File)
								deleteResourceInternal(static_pointer_cast<FileEntry>(child));
						}

						toDelete.clear();
					}

					for(auto& child : currentDir->mChildren)
					{
						if(child->type == LibraryEntryType::Directory)
							todo.push(static_cast<DirectoryEntry*>(child.get()));
					}
				}
			}
		}

		return resourcesToImport;
	}

	USPtr<ProjectLibrary::FileEntry> ProjectLibrary::addResourceInternal(DirectoryEntry* parent, const Path& filePath, 
		const SPtr<ImportOptions>& importOptions, bool forceReimport, bool synchronous)
	{
		USPtr<FileEntry> newResource = bs_ushared_ptr_new<FileEntry>(filePath, filePath.getTail(), parent);
		parent->mChildren.push_back(newResource);

		reimportResourceInternal(newResource.get(), importOptions, forceReimport, false, synchronous);
		onEntryAdded(newResource->path);

		return newResource;
	}

	USPtr<ProjectLibrary::DirectoryEntry> ProjectLibrary::addDirectoryInternal(DirectoryEntry* parent, const Path& dirPath)
	{
		USPtr<DirectoryEntry> newEntry = bs_ushared_ptr_new<DirectoryEntry>(dirPath, dirPath.getTail(), parent);
		parent->mChildren.push_back(newEntry);

		onEntryAdded(newEntry->path);
		return newEntry;
	}

	void ProjectLibrary::deleteResourceInternal(USPtr<FileEntry> resource)
	{
		if(resource->meta != nullptr)
		{
			auto& resourceMetas = resource->meta->getResourceMetaData();
			for(auto& entry : resourceMetas)
			{
				const UUID& uuid = entry->getUUID();

				Path path;
				if (mResourceManifest->uuidToFilePath(uuid, path))
				{
					if (FileSystem::isFile(path))
						FileSystem::remove(path);

					mResourceManifest->unregisterResource(uuid);
				}

				mUUIDToPath.erase(uuid);
			}
		}

		Path metaPath = getMetaPath(resource->path);
		if (FileSystem::isFile(metaPath))
			FileSystem::remove(metaPath);

		DirectoryEntry* parent = resource->parent;
		auto findIter = std::find_if(parent->mChildren.begin(), parent->mChildren.end(), 
			[&] (const USPtr<LibraryEntry>& entry) { return entry == resource; });

		parent->mChildren.erase(findIter);

		Path originalPath = resource->path;
		onEntryRemoved(originalPath);

		const auto iterQueuedImport = mQueuedImports.find(resource.get());
		if(iterQueuedImport != mQueuedImports.end())
			iterQueuedImport->second->canceled = true;

		removeDependencies(resource.get());
		*resource = FileEntry();

		reimportDependants(originalPath);
	}

	void ProjectLibrary::deleteDirectoryInternal(USPtr<DirectoryEntry> directory)
	{
		if(directory == mRootEntry)
			mRootEntry = nullptr;

		Vector<USPtr<LibraryEntry>> childrenToDestroy = directory->mChildren;
		for(auto& child : childrenToDestroy)
		{
			if(child->type == LibraryEntryType::Directory)
				deleteDirectoryInternal(static_pointer_cast<DirectoryEntry>(child));
			else
				deleteResourceInternal(static_pointer_cast<FileEntry>(child));
		}

		DirectoryEntry* parent = directory->parent;
		if(parent != nullptr)
		{
			auto findIter = std::find_if(parent->mChildren.begin(), parent->mChildren.end(), 
				[&] (const USPtr<LibraryEntry>& entry) { return entry == directory; });

			parent->mChildren.erase(findIter);
		}

		onEntryRemoved(directory->path);
		*directory = DirectoryEntry();
	}

	bool ProjectLibrary::reimportResourceInternal(FileEntry* fileEntry, const SPtr<ImportOptions>& importOptions,
		bool forceReimport, bool pruneResourceMetas, bool synchronous)
	{
		Path metaPath = fileEntry->path;
		metaPath.setFilename(metaPath.getFilename() + ".meta");

		// If the file doesn't have meta-data, attempt to read it from a meta-file, if one exists. This can only happen
		// if library data is obsolete (e.g. when adding files from another copy of the project)
		if(fileEntry->meta == nullptr)
		{
			if(FileSystem::isFile(metaPath))
			{
				FileDecoder fs(metaPath);
				SPtr<IReflectable> loadedMeta = fs.decode();

				if(loadedMeta != nullptr && loadedMeta->isDerivedFrom(ProjectFileMeta::getRTTIStatic()))
				{
					const SPtr<ProjectFileMeta>& fileMeta = std::static_pointer_cast<ProjectFileMeta>(loadedMeta);
					fileEntry->meta = fileMeta;

					auto& resourceMetas = fileEntry->meta->getResourceMetaData();

					if (!resourceMetas.empty())
					{
						mUUIDToPath[resourceMetas[0]->getUUID()] = fileEntry->path;

						for (UINT32 i = 1; i < (UINT32)resourceMetas.size(); i++)
						{
							const SPtr<ProjectResourceMeta>& entry = resourceMetas[i];
							mUUIDToPath[entry->getUUID()] = fileEntry->path + entry->getUniqueName();
						}
					}
				}
			}
		}

		if (!isUpToDate(fileEntry) || forceReimport)
		{
			// Note: If resource is native we just copy it to the internal folder. We could avoid the copy and 
			// load the resource directly from the Resources folder but that requires complicating library code.
			const bool isNativeResource = isNative(fileEntry->path);

			SPtr<ImportOptions> curImportOptions = nullptr;
			if (importOptions == nullptr && !isNativeResource)
			{
				if (fileEntry->meta != nullptr)
					curImportOptions = fileEntry->meta->getImportOptions();
				else
					curImportOptions = Importer::instance().createImportOptions(fileEntry->path);
			}
			else
				curImportOptions = importOptions;

			SPtr<QueuedImport> queuedImport = bs_shared_ptr_new<QueuedImport>();
			queuedImport->filePath = fileEntry->path;
			queuedImport->importOptions = curImportOptions;
			queuedImport->pruneMetas = pruneResourceMetas;
			queuedImport->native = isNativeResource;
			queuedImport->timestamp = std::time(nullptr);

			// If import is already queued for this file make the tasks dependant so they don't execute at the same time, 
			// and so they execute in the proper order
			SPtr<Task> dependency;

			const auto iterFind = mQueuedImports.find(fileEntry);
			if (iterFind != mQueuedImports.end())
			{
				dependency = iterFind->second->importTask;

				// Need this reference just so the dependency is kept alive, otherwise it goes out of scope when we
				// remove or overwrite it from mQueuedImports map
				queuedImport->dependsOn = iterFind->second;

				// Note: We should cancel the task here so it doesn't run unnecessarily. But if the task is already
				// running it shouldn't be canceled as dependencies still need to wait on it (since cancelling a
				// running task doesn't actually stop it). Yet there is currently no good wait to check if task
				// is currently running. 

				// Dependency being imported async but we want the current resource right away. Wait until dependency is
				// done otherwise when dependency finishes it will overwrite whatever we write now.
				if(synchronous && dependency)
				{
					if (finishQueuedImport(fileEntry, *iterFind->second, true))
						mQueuedImports.erase(iterFind);
				}
			}
				
			// Needs to be pass a weak pointer to worker methods since internally it holds a reference to the task itself, 
			// and we can't have the task closure holding a reference back, otherwise it leaks
			std::weak_ptr<QueuedImport> queuedImportWeak = queuedImport;

			if(!isNativeResource)
			{
				// Find UUIDs for any existing sub-resources
				if (fileEntry->meta != nullptr)
				{
					const Vector<SPtr<ProjectResourceMeta>>& resourceMetas = fileEntry->meta->getAllResourceMetaData();
					for (auto& entry : resourceMetas)
						queuedImport->resources.emplace_back(entry->getUniqueName(), nullptr, entry->getUUID());
				}

				// Perform import, register the resources and their UUID in the QueuedImport structure and save the
				// resource on disk
				const auto importAsync = [queuedImportWeak, &projectFolder = mProjectFolder, &mutex = mQueuedImportMutex]()
				{
					SPtr<QueuedImport> queuedImport = queuedImportWeak.lock();

					Vector<SubResourceRaw> importedResources = gImporter()._importAll(queuedImport->filePath, 
						queuedImport->importOptions);

					if (!importedResources.empty())
					{
						Path outputPath = projectFolder;
						outputPath.append(INTERNAL_TEMP_DIR);

						if (!FileSystem::isDirectory(outputPath))
							FileSystem::createDir(outputPath);

						for (auto& entry : importedResources)
						{
							String subresourceName = entry.name;
							Path::stripInvalid(subresourceName);

							UUID uuid;
							{
								// Any access to queuedImport->resources must be locked
								Lock lock(mutex);

								auto iterFind = std::find_if(queuedImport->resources.begin(), queuedImport->resources.end(),
									[&subresourceName](const QueuedImportResource& importResource)
								{
									return importResource.name == subresourceName;
								});

								if (iterFind != queuedImport->resources.end())
									iterFind->resource = entry.value;
								else
								{
									queuedImport->resources.push_back(QueuedImportResource(entry.name, entry.value,
										UUID::EMPTY));

									iterFind = queuedImport->resources.end() - 1;
								}

								if (iterFind->uuid.empty())
									iterFind->uuid = UUIDGenerator::generateRandom();

								uuid = iterFind->uuid;
							}

							const String uuidStr = uuid.toString();

							outputPath.setFilename(uuidStr + ".asset");
							gResources()._save(entry.value, outputPath, true);
						}
					}
				};

				if(!synchronous)
				{
					queuedImport->importTask = Task::create("ProjectLibraryImport", importAsync, TaskPriority::Normal,
						dependency);
				}
				else
					importAsync();
			}
			else
			{
				// If meta exists make sure it is registered in the manifest before load, otherwise it will get assigned a new UUID.
				// This can happen if library isn't properly saved before exiting the application.
				if (fileEntry->meta != nullptr)
				{
					auto& resourceMetas = fileEntry->meta->getResourceMetaData();
					if(!resourceMetas.empty())
						mResourceManifest->registerResource(resourceMetas[0]->getUUID(), fileEntry->path);
				}

				const auto importAsync = [queuedImportWeak, &projectFolder = mProjectFolder, &mutex = mQueuedImportMutex]()
				{
					// Don't load dependencies because we don't need them, but also because they might not be in the
					// manifest which would screw up their UUIDs.
					SPtr<QueuedImport> queuedImport = queuedImportWeak.lock();
					HResource resource = gResources().load(queuedImport->filePath, ResourceLoadFlag::KeepSourceData);

					if (resource.isLoaded(false))
					{
						Path outputPath = projectFolder;
						outputPath.append(INTERNAL_TEMP_DIR);

						if (!FileSystem::isDirectory(outputPath))
							FileSystem::createDir(outputPath);

						{
							// Any access to queuedImport->resources must be locked
							Lock lock(mutex);

							queuedImport->resources.push_back(QueuedImportResource("primary", resource));
						}

						const String uuidStr = resource.getUUID().toString();

						outputPath.setFilename(uuidStr + ".asset");
						gResources()._save(resource.getInternalPtr(), outputPath, true);
					}
				};

				if(!synchronous)
				{
					queuedImport->importTask = Task::create("ProjectLibraryImport", importAsync, TaskPriority::Normal,
						dependency);
				}
				else
					importAsync();
			}

			if(!synchronous)
			{
				TaskScheduler::instance().addTask(queuedImport->importTask);
				mQueuedImports[fileEntry] = queuedImport;
			}

			if(synchronous)
				finishQueuedImport(fileEntry, *queuedImport, true);

			return true;
		}

		return false;
	}

	bool ProjectLibrary::finishQueuedImport(FileEntry* fileEntry, const QueuedImport& import, bool wait)
	{
		if (import.importTask != nullptr && !import.importTask->isComplete())
		{
			if (wait)
				import.importTask->wait();
			else
				return false;
		}

		// We wait on canceled task to finish and then just discard the results because any dependant tasks need to be
		// aware this tasks exists, so we can't just remove it straight away.
		if (import.canceled)
			return true;

		Path metaPath = fileEntry->path;
		metaPath.setFilename(metaPath.getFilename() + ".meta");

		SPtr<SerializedObject> orgMetaData;
		Vector<SPtr<ProjectResourceMeta>> existingMetas;
		if (fileEntry->meta == nullptr) // Build a brand new meta-file
			fileEntry->meta = ProjectFileMeta::create(import.importOptions);
		else // Existing meta-file, which needs to be updated
		{
			orgMetaData = SerializedObject::create(*fileEntry->meta);

			// Remove existing dependencies (they will be re-added later)
			removeDependencies(fileEntry);

			existingMetas = fileEntry->meta->getAllResourceMetaData();

			fileEntry->meta->clearResourceMetaData();
			fileEntry->meta->mImportOptions = import.importOptions;
		}

		fileEntry->lastUpdateTime = import.timestamp;

		Path internalResourcesPath = mProjectFolder;
		internalResourcesPath.append(INTERNAL_RESOURCES_DIR);

		if (!FileSystem::isDirectory(internalResourcesPath))
			FileSystem::createDir(internalResourcesPath);

		Path tempResourcesPath = mProjectFolder;
		tempResourcesPath.append(INTERNAL_TEMP_DIR);

		// See which sub-resource metas need to be updated, removed or added based on the new resource set
		bool isFirst = true;
		for (const auto& entry : import.resources)
		{
			// Entries with no resources are sub-resources that used to exist in this file, but haven't been imported
			// this time
			if (!entry.resource)
				continue;

			// Copy the resource file from the temporary directory
			const String uuidStr = entry.uuid.toString();

			tempResourcesPath.setFilename(uuidStr + ".asset");
			internalResourcesPath.setFilename(uuidStr + ".asset");

			FileSystem::move(tempResourcesPath, internalResourcesPath);

			String name = entry.name;
			Path::stripInvalid(name);

			const ProjectResourceIcons icons = generatePreviewIcons(*entry.resource);

			bool foundMeta = false;
			for (auto iterMeta = existingMetas.begin(); iterMeta != existingMetas.end();)
			{
				const SPtr<ProjectResourceMeta>& metaEntry = *iterMeta;

				if (name == metaEntry->getUniqueName())
				{
					if(!foundMeta)
					{
						// Make sure the UUID we used for saving the resource matches the current one (should always
						// be true unless the meta-data somehow changes while the async import is happening)
						assert(entry.uuid == metaEntry->getUUID());

						HResource importedResource = gResources()._getResourceHandle(metaEntry->getUUID());

						gResources().update(importedResource, entry.resource);

						metaEntry->setPreviewIcons(icons);
						fileEntry->meta->add(metaEntry);
					}

					foundMeta = true;
					iterMeta = existingMetas.erase(iterMeta);
				}
				else
					++iterMeta;
			}

			if (!foundMeta)
			{
				HResource importedResource;

				// Native resources are always expected to have a handle since Resources::load was called during
				// the 'import' step
				if (import.native)
					importedResource = entry.handle;
				else
					importedResource = gResources()._createResourceHandle(entry.resource, entry.uuid);

				SPtr<ResourceMetaData> subMeta = entry.resource->getMetaData();
				const UINT32 typeId = entry.resource->getTypeId();
				const UUID& UUID = importedResource.getUUID();

				SPtr<ProjectResourceMeta> resMeta = ProjectResourceMeta::create(name, UUID, typeId,
					icons, subMeta);
				fileEntry->meta->add(resMeta);
			}

			// Update UUID to path mapping
			if (isFirst)
				mUUIDToPath[entry.uuid] = fileEntry->path;
			else
				mUUIDToPath[entry.uuid] = fileEntry->path + name;

			isFirst = false;

			// Register path in manifest
			mResourceManifest->registerResource(entry.uuid, internalResourcesPath);
		}

		// Keep resource metas that we are not currently using, in case they get restored so their references
		// don't get broken
		if (!import.pruneMetas)
		{
			for (auto& metaEntry : existingMetas)
				fileEntry->meta->addInactive(metaEntry);
		}

		// Note: Ideally we replace this with a specialized BinaryCompare method
		bool metaModified = true;
		if(orgMetaData != nullptr)
		{
			SPtr<SerializedObject> newMetaData = SerializedObject::create(*fileEntry->meta);
			BinaryDiff diffHandler;
			SPtr<SerializedObject> diff = diffHandler.generateDiff(orgMetaData, newMetaData);

			metaModified = diff != nullptr;
		}

		if(metaModified)
		{
			// Save the meta file
			FileEncoder fs(metaPath);
			fs.encode(fileEntry->meta.get());
		}

		// Register any dependencies this resource depends on
		addDependencies(fileEntry);

		// Notify the outside world import is doen
		onEntryImported(fileEntry->path);

		// Queue any resources dependant on this one for import
		reimportDependants(fileEntry->path);

		return true;
	}

	void ProjectLibrary::_finishQueuedImports(bool wait)
	{
		for(auto iter = mQueuedImports.begin(); iter != mQueuedImports.end();)
		{
			if(finishQueuedImport(iter->first, *iter->second, wait))
				iter = mQueuedImports.erase(iter);
			else
				++iter;
		}
	}

	bool ProjectLibrary::isUpToDate(FileEntry* resource) const
	{
		SPtr<QueuedImport> queuedImport;

		if(resource->meta == nullptr)
		{
			// Allow no meta if import in progress
			const auto iterFind = mQueuedImports.find(resource);
			if(iterFind == mQueuedImports.end())
				return false;

			queuedImport = iterFind->second;
		}
		else
		{
			auto& resourceMetas = resource->meta->getResourceMetaData();
			for (auto& resMeta : resourceMetas)
			{
				Path internalPath;
				if (!mResourceManifest->uuidToFilePath(resMeta->getUUID(), internalPath))
					return false;

				if (!FileSystem::isFile(internalPath))
					return false;
			}
		}

		// Note: We're keeping separate update times for queued imports. This allows the import to be cancelled (either by
		// user or by app crashing), without updating the actual update time. This way the systems knows to try to reimport
		// the resource on the next check. At the same time we don't want our checkForModifications function to keep
		// trying to reimport a resource if it's already been queued for import.
		const std::time_t lastUpdateTime = queuedImport ? queuedImport->timestamp : resource->lastUpdateTime;
		const std::time_t lastModifiedTime = FileSystem::getLastModifiedTime(resource->path);

		return lastModifiedTime <= lastUpdateTime;
	}

	Vector<USPtr<ProjectLibrary::LibraryEntry>> ProjectLibrary::search(const String& pattern)
	{
		return search(pattern, {});
	}

	Vector<USPtr<ProjectLibrary::LibraryEntry>> ProjectLibrary::search(const String& pattern, const Vector<UINT32>& typeIds)
	{
		Vector<USPtr<LibraryEntry>> foundEntries;

		std::regex escape("[.^$|()\\[\\]{}*+?\\\\]");
		String replace("\\\\&");
		String escapedPattern = std::regex_replace(pattern, escape, replace, std::regex_constants::match_default | std::regex_constants::format_sed);

		// For some reason MSVC stdlib implementation requires a different pattern than stdlib one
#if BS_PLATFORM == BS_PLATFORM_WIN32
		std::regex wildcard("\\\\\\*");
#else
		std::regex wildcard("\\\\\\\\\\*");
#endif
		String wildcardReplace(".*");
		String searchPattern = std::regex_replace(escapedPattern, wildcard, ".*");

		std::regex searchRegex(searchPattern, std::regex_constants::ECMAScript | std::regex_constants::icase);

		Stack<DirectoryEntry*> todo;
		todo.push(mRootEntry.get());
		while (!todo.empty())
		{
			DirectoryEntry* dirEntry = todo.top();
			todo.pop();

			for (auto& child : dirEntry->mChildren)
			{
				if (std::regex_match(child->elementName, searchRegex))
				{
					if (typeIds.empty())
						foundEntries.push_back(child);
					else
					{
						if (child->type == LibraryEntryType::File)
						{
							FileEntry* childFileEntry = static_cast<FileEntry*>(child.get());
							if (childFileEntry->meta != nullptr)
							{
								auto& resourceMetas = childFileEntry->meta->getResourceMetaData();
								for (auto& typeId : typeIds)
								{
									bool found = false;
									for (auto& resMeta : resourceMetas)
									{
										if (resMeta->getTypeID() == typeId)
										{
											foundEntries.push_back(child);
											found = true;
											break;
										}
									}

									if (found)
										break;
								}
							}
						}
					}
				}

				if (child->type == LibraryEntryType::Directory)
				{
					DirectoryEntry* childDirEntry = static_cast<DirectoryEntry*>(child.get());
					todo.push(childDirEntry);
				}
			}
		}

		std::sort(foundEntries.begin(), foundEntries.end(), 
			[&](const USPtr<LibraryEntry>& a, const USPtr<LibraryEntry>& b) 
		{ 
			return a->elementName.compare(b->elementName) < 0;
		});

		return foundEntries;
	}

	USPtr<ProjectLibrary::LibraryEntry> ProjectLibrary::findEntry(const Path& path) const
	{
		Path relPath;
		const Path* searchPath;
		if (path.isAbsolute())
		{
			if (!mResourcesFolder.includes(path))
				return nullptr;

			relPath = path.getRelative(mRootEntry->path);
			searchPath = &relPath;
		}
		else
			searchPath = &path;

		BS_ASSERT(mRootEntry->path == mResourcesFolder);

		UINT32 numElems = searchPath->getNumDirectories() + (searchPath->isFile() ? 1 : 0);
		UINT32 idx = 0;

		USPtr<LibraryEntry> rootLibEntry = mRootEntry;
		USPtr<LibraryEntry>* current = &rootLibEntry;
		while (current != nullptr)
		{
			if (idx == numElems)
				return *current;

			const String& curElem =
				(searchPath->isFile() && idx == (numElems - 1)) ? searchPath->getFilename() : (*searchPath)[idx];

			if ((*current)->type == LibraryEntryType::Directory)
			{
				DirectoryEntry* dirEntry = static_cast<DirectoryEntry*>(current->get());
				size_t curElemHash = bs_hash(UTF8::toLower(curElem));

				current = nullptr;
				for (auto& child : dirEntry->mChildren)
				{
					if(curElemHash != child->elementNameHash)
						continue;

					if (Path::comparePathElem(curElem, child->elementName))
					{
						idx++;
						current = &child;
						break;
					}
				}
			}
			else // Found file
			{
				// If this is next to last element, next entry is assumed to be a sub-resource name, which we ignore
				if (idx == (numElems - 1))
					return *current;
				else
					break; // Not a valid path
			}
		}

		return nullptr;
	}

	bool ProjectLibrary::isSubresource(const Path& path) const
	{
		UINT32 numElems = path.getNumDirectories() + (path.isFile() ? 1 : 0);

		if (numElems <= 1)
			return false;

		Path filePath = path;
		filePath.makeParent();

		LibraryEntry* entry = findEntry(filePath).get();
		return entry != nullptr && entry->type == LibraryEntryType::File;
	}

	SPtr<ProjectResourceMeta> ProjectLibrary::findResourceMeta(const Path& path) const
	{
		UINT32 numElems = path.getNumDirectories() + (path.isFile() ? 1 : 0);

		// Check if it is a subresource path
		if(numElems > 1)
		{
			Path filePath = path;
			filePath.makeParent();

			LibraryEntry* entry = findEntry(filePath).get();
			if (entry == nullptr)
				return nullptr;

			// Entry is a subresource
			if (entry->type == LibraryEntryType::File)
			{
				FileEntry* fileEntry = static_cast<FileEntry*>(entry);
				if (fileEntry->meta == nullptr)
					return nullptr;

				auto& resourceMetas = fileEntry->meta->getResourceMetaData();
				for(auto& resMeta : resourceMetas)
				{
					if (resMeta->getUniqueName() == path.getTail())
						return resMeta;
				}

				// Found the file but no subresource or meta information
				return nullptr;
			}
			else // Entry not a subresource
			{
				DirectoryEntry* dirEntry = static_cast<DirectoryEntry*>(entry);
				for (auto& child : dirEntry->mChildren)
				{
					if (Path::comparePathElem(path.getTail(), child->elementName))
					{
						if (child->type == LibraryEntryType::File)
						{
							FileEntry* fileEntry = static_cast<FileEntry*>(child.get());
							if (fileEntry->meta == nullptr)
								return nullptr;

							return fileEntry->meta->getResourceMetaData()[0];
						}
					}
				}

				return nullptr;
			}
		}

		// Not a subresource path, load directly
		{
			LibraryEntry* entry = findEntry(path).get();
			if (entry == nullptr || entry->type == LibraryEntryType::Directory)
				return nullptr;

			FileEntry* fileEntry = static_cast<FileEntry*>(entry);
			if (fileEntry->meta == nullptr)
				return nullptr;

			auto& resourceMetas = fileEntry->meta->getResourceMetaData();

			if(!resourceMetas.empty())
				return resourceMetas[0];

			return nullptr;
		}
	}

	Path ProjectLibrary::uuidToPath(const UUID& uuid) const
	{
		auto iterFind = mUUIDToPath.find(uuid);

		if (iterFind != mUUIDToPath.end())
			return iterFind->second;

		return Path::BLANK;
	}

	void ProjectLibrary::createEntry(const HResource& resource, const Path& path)
	{
		if (resource == nullptr)
			return;

		Path assetPath = path;
		if (path.isAbsolute())
		{
			if (!getResourcesFolder().includes(path))
				return;

			assetPath = path.getRelative(getResourcesFolder());
		}

		deleteEntry(assetPath);

		resource->setName(path.getFilename(false));

		Path absPath = assetPath.getAbsolute(getResourcesFolder());
		Resources::instance().save(resource, absPath, false);

		Path parentDirPath = absPath.getParent();
		USPtr<LibraryEntry> parentEntry = findEntry(parentDirPath);

		// Register parent hierarchy if not found
		DirectoryEntry* entryParent = nullptr;
		if (parentEntry == nullptr)
			createInternalParentHierarchy(absPath, nullptr, &entryParent);
		else
			entryParent = static_cast<DirectoryEntry*>(parentEntry.get());

		addResourceInternal(entryParent, absPath, nullptr, true, true);
	}

	void ProjectLibrary::saveEntry(const HResource& resource)
	{
		if (resource == nullptr)
			return;

		Path filePath = uuidToPath(resource.getUUID());
		if(filePath.isEmpty())
		{
			BS_LOG(Warning, Editor, "Trying to save a resource that hasn't been registered with the project library. "
				"Call ProjectLibrary::create first.");
			return;
		}

		filePath.makeAbsolute(getResourcesFolder());

		Resources::instance().save(resource, filePath, true);

		LibraryEntry* fileEntry = findEntry(filePath).get();
		if(fileEntry)
			reimportResourceInternal(static_cast<FileEntry*>(fileEntry), nullptr, true, false, true);
	}

	void ProjectLibrary::createFolderEntry(const Path& path)
	{
		Path fullPath = path;
		if (fullPath.isAbsolute())
		{
			if (!mResourcesFolder.includes(fullPath))
				return;
		}
		else
			fullPath.makeAbsolute(mResourcesFolder);

		if (FileSystem::isDirectory(fullPath))
			return; // Already exists

		FileSystem::createDir(fullPath);
		Path parentPath = fullPath.getParent();

		DirectoryEntry* newEntryParent = nullptr;
		USPtr<LibraryEntry> newEntryParentLib = findEntry(parentPath);
		if (newEntryParentLib != nullptr)
		{
			assert(newEntryParentLib->type == LibraryEntryType::Directory);
			newEntryParent = static_cast<DirectoryEntry*>(newEntryParentLib.get());
		}

		DirectoryEntry* newHierarchyParent = nullptr;
		if (newEntryParent == nullptr) // New path parent doesn't exist, so we need to create the hierarchy
			createInternalParentHierarchy(fullPath, &newHierarchyParent, &newEntryParent);

		addDirectoryInternal(newEntryParent, fullPath);
	}

	void ProjectLibrary::moveEntry(const Path& oldPath, const Path& newPath, bool overwrite)
	{
		Path oldFullPath = oldPath;
		if (!oldFullPath.isAbsolute())
			oldFullPath.makeAbsolute(mResourcesFolder);

		Path newFullPath = newPath;
		if (!newFullPath.isAbsolute())
			newFullPath.makeAbsolute(mResourcesFolder);

		Path parentPath = newFullPath.getParent();
		if (!FileSystem::isDirectory(parentPath))
		{
			BS_LOG(Warning, Editor, "Move operation failed. Destination directory \"{0}\" doesn't exist.", parentPath);
			return;
		}

		if(FileSystem::isFile(oldFullPath) || FileSystem::isDirectory(oldFullPath))
			FileSystem::move(oldFullPath, newFullPath, overwrite);

		Path oldMetaPath = getMetaPath(oldFullPath);
		Path newMetaPath = getMetaPath(newFullPath);

		USPtr<LibraryEntry> oldEntry = findEntry(oldFullPath);
		if(oldEntry != nullptr) // Moving from the Resources folder
		{
			// Moved outside of Resources, delete entry & meta file
			if (!mResourcesFolder.includes(newFullPath))
			{
				if(oldEntry->type == LibraryEntryType::File)
					deleteResourceInternal(static_pointer_cast<FileEntry>(oldEntry));
				else if(oldEntry->type == LibraryEntryType::Directory)
					deleteDirectoryInternal(static_pointer_cast<DirectoryEntry>(oldEntry));
			}
			else // Just moving internally
			{
				onEntryRemoved(oldEntry->path);

				USPtr<FileEntry> fileEntry = nullptr;
				if (oldEntry->type == LibraryEntryType::File)
				{
					fileEntry = static_pointer_cast<FileEntry>(oldEntry);
					removeDependencies(fileEntry.get());

					// Update uuid <-> path mapping
					if(fileEntry->meta != nullptr)
					{
						auto& resourceMetas = fileEntry->meta->getResourceMetaData();

						if (!resourceMetas.empty())
						{
							mUUIDToPath[resourceMetas[0]->getUUID()] = newFullPath;

							for (UINT32 i = 1; i < (UINT32)resourceMetas.size(); i++)
							{
								SPtr<ProjectResourceMeta> resMeta = resourceMetas[i];

								const UUID& UUID = resMeta->getUUID();
								mUUIDToPath[UUID] = newFullPath + resMeta->getUniqueName();
							}
						}
					}
				}

				if(FileSystem::isFile(oldMetaPath))
					FileSystem::move(oldMetaPath, newMetaPath);

				DirectoryEntry* parent = oldEntry->parent;
				auto findIter = std::find(parent->mChildren.begin(), parent->mChildren.end(), oldEntry);
				if(findIter != parent->mChildren.end())
					parent->mChildren.erase(findIter);

				Path parentPath = newFullPath.getParent();

				DirectoryEntry* newEntryParent = nullptr;
				USPtr<LibraryEntry> newEntryParentLib = findEntry(parentPath);
				if(newEntryParentLib != nullptr)
				{
					assert(newEntryParentLib->type == LibraryEntryType::Directory);
					newEntryParent = static_cast<DirectoryEntry*>(newEntryParentLib.get());
				}

				DirectoryEntry* newHierarchyParent = nullptr;
				if(newEntryParent == nullptr) // New path parent doesn't exist, so we need to create the hierarchy
					createInternalParentHierarchy(newFullPath, &newHierarchyParent, &newEntryParent);

				newEntryParent->mChildren.push_back(oldEntry);
				oldEntry->parent = newEntryParent;
				oldEntry->path = newFullPath;
				oldEntry->elementName = newFullPath.getTail();
				oldEntry->elementNameHash = bs_hash(UTF8::toLower(oldEntry->elementName));

				if(oldEntry->type == LibraryEntryType::Directory) // Update child paths
				{
					Stack<LibraryEntry*> todo;
					todo.push(oldEntry.get());

					while(!todo.empty())
					{
						LibraryEntry* curEntry = todo.top();
						todo.pop();

						DirectoryEntry* curDirEntry = static_cast<DirectoryEntry*>(curEntry);
						for(auto& child : curDirEntry->mChildren)
						{
							child->path = child->parent->path;
							child->path.append(child->elementName);

							if(child->type == LibraryEntryType::Directory)
								todo.push(child.get());
						}
					}
				}

				onEntryAdded(oldEntry->path);

				if (fileEntry != nullptr)
				{
					reimportDependants(oldFullPath);
					reimportDependants(newFullPath);
				}
			}
		}
		else // Moving from outside of the Resources folder (likely adding a new resource)
		{
			checkForModifications(newFullPath);
		}
	}

	void ProjectLibrary::copyEntry(const Path& oldPath, const Path& newPath, bool overwrite)
	{
		Path oldFullPath = oldPath;
		if (!oldFullPath.isAbsolute())
			oldFullPath.makeAbsolute(mResourcesFolder);

		Path newFullPath = newPath;
		if (!newFullPath.isAbsolute())
			newFullPath.makeAbsolute(mResourcesFolder);

		if (!FileSystem::exists(oldFullPath))
			return;

		FileSystem::copy(oldFullPath, newFullPath, overwrite);

		// Copying a file/folder outside of the Resources folder, no special handling needed
		if (!mResourcesFolder.includes(newFullPath))
			return;

		Path parentPath = newFullPath.getParent();

		DirectoryEntry* newEntryParent = nullptr;
		LibraryEntry* newEntryParentLib = findEntry(parentPath).get();
		if (newEntryParentLib != nullptr)
		{
			assert(newEntryParentLib->type == LibraryEntryType::Directory);
			newEntryParent = static_cast<DirectoryEntry*>(newEntryParentLib);
		}

		// If the source is outside of Resources folder, just plain import the copy
		LibraryEntry* oldEntry = findEntry(oldFullPath).get();
		if (oldEntry == nullptr)
		{
			checkForModifications(newFullPath);
			return;
		}

		// Both source and destination are within Resources folder, need to preserve import options on the copies
		if (FileSystem::isFile(newFullPath))
		{
			assert(oldEntry->type == LibraryEntryType::File);
			FileEntry* oldResEntry = static_cast<FileEntry*>(oldEntry);

			SPtr<ImportOptions> importOptions;
			if (oldResEntry->meta != nullptr)
				importOptions = oldResEntry->meta->getImportOptions();

			addResourceInternal(newEntryParent, newFullPath, importOptions, true);
		}
		else
		{
			assert(oldEntry->type == LibraryEntryType::File);
			DirectoryEntry* oldDirEntry = static_cast<DirectoryEntry*>(oldEntry);

			DirectoryEntry* newDirEntry = addDirectoryInternal(newEntryParent, newFullPath).get();

			Stack<std::tuple<DirectoryEntry*, DirectoryEntry*>> todo;
			todo.push(std::make_tuple(oldDirEntry, newDirEntry));

			while (!todo.empty())
			{
				auto current = todo.top();
				todo.pop();

				DirectoryEntry* sourceDir = std::get<0>(current);
				DirectoryEntry* destDir = std::get<1>(current);

				for (auto& child : sourceDir->mChildren)
				{
					Path childDestPath = destDir->path;
					childDestPath.append(child->path.getTail());

					if (child->type == LibraryEntryType::File)
					{
						FileEntry* childResEntry = static_cast<FileEntry*>(child.get());

						SPtr<ImportOptions> importOptions;
						if (childResEntry->meta != nullptr)
							importOptions = childResEntry->meta->getImportOptions();

						addResourceInternal(destDir, childDestPath, importOptions, true);
					}
					else // Directory
					{
						DirectoryEntry* childSourceDirEntry = static_cast<DirectoryEntry*>(child.get());
						DirectoryEntry* childDestDirEntry = addDirectoryInternal(destDir, childDestPath).get();

						todo.push(std::make_tuple(childSourceDirEntry, childDestDirEntry));
					}
				}
			}
		}
	}

	void ProjectLibrary::deleteEntry(const Path& path)
	{
		Path fullPath = path;
		if (!fullPath.isAbsolute())
			fullPath.makeAbsolute(mResourcesFolder);

		if(FileSystem::exists(fullPath))
			FileSystem::remove(fullPath);

		USPtr<LibraryEntry> entry = findEntry(fullPath);
		if(entry != nullptr)
		{
			if(entry->type == LibraryEntryType::File)
				deleteResourceInternal(static_pointer_cast<FileEntry>(entry));
			else if(entry->type == LibraryEntryType::Directory)
				deleteDirectoryInternal(static_pointer_cast<DirectoryEntry>(entry));
		}
	}

	void ProjectLibrary::reimport(const Path& path, const SPtr<ImportOptions>& importOptions, bool forceReimport,
		bool synchronous)
	{
		LibraryEntry* entry = findEntry(path).get();
		if (entry != nullptr)
		{
			if (entry->type == LibraryEntryType::File)
			{
				FileEntry* resEntry = static_cast<FileEntry*>(entry);
				reimportResourceInternal(resEntry, importOptions, forceReimport, synchronous);
			}
		}
	}

	float ProjectLibrary::getImportProgress(const Path& path) const
	{
		LibraryEntry* entry = findEntry(path).get();
		if (entry == nullptr)
			return 0.0f;

		if(entry->type == LibraryEntryType::Directory)
			return 1.0f;

		// Note: Only supporting binary progress reporting for now
		const auto iterFind = mQueuedImports.find(static_cast<FileEntry*>(entry));
		return iterFind != mQueuedImports.end() ? 0.0f : 1.0f;
	}

	void ProjectLibrary::cancelImport()
	{
		for(auto& entry : mQueuedImports)
			entry.second->canceled = true;
	}

	void ProjectLibrary::waitForQueuedImport(FileEntry* fileEntry)
	{
		const auto iterFind = mQueuedImports.find(fileEntry);
		if (iterFind != mQueuedImports.end())
		{
			if (finishQueuedImport(fileEntry, *iterFind->second, true))
				mQueuedImports.erase(iterFind);
		}
	}

	void ProjectLibrary::setIncludeInBuild(const Path& path, bool include)
	{
		LibraryEntry* entry = findEntry(path).get();

		if (entry == nullptr || entry->type == LibraryEntryType::Directory)
			return;

		auto fileEntry = static_cast<FileEntry*>(entry);

		// Any queued imports will overwrite the meta file, so make sure they finish first
		waitForQueuedImport(fileEntry);

		if (fileEntry->meta == nullptr)
			return;

		fileEntry->meta->setIncludeInBuild(include);

		Path metaPath = fileEntry->path;
		metaPath.setFilename(metaPath.getFilename() + ".meta");

		FileEncoder fs(metaPath);
		fs.encode(fileEntry->meta.get());
	}

	void ProjectLibrary::setUserData(const Path& path, const SPtr<IReflectable>& userData)
	{
		LibraryEntry* entry = findEntry(path).get();

		if (entry == nullptr || entry->type == LibraryEntryType::Directory)
			return;

		auto fileEntry = static_cast<FileEntry*>(entry);

		// Any queued imports will overwrite the meta file, so make sure they finish first
		waitForQueuedImport(fileEntry);

		SPtr<ProjectResourceMeta> resMeta = findResourceMeta(path);
		
		if (resMeta == nullptr)
			return;

		resMeta->mUserData = userData;

		Path metaPath = fileEntry->path;
		metaPath.setFilename(metaPath.getFilename() + ".meta");

		FileEncoder fs(metaPath);
		fs.encode(fileEntry->meta.get());
	}

	Vector<USPtr<ProjectLibrary::FileEntry>> ProjectLibrary::getResourcesForBuild() const
	{
		Vector<USPtr<FileEntry>> output;

		Stack<DirectoryEntry*> todo;
		todo.push(mRootEntry.get());

		while (!todo.empty())
		{
			DirectoryEntry* directory = todo.top();
			todo.pop();

			for (auto& child : directory->mChildren)
			{
				if (child->type == LibraryEntryType::File)
				{
					FileEntry* resEntry = static_cast<FileEntry*>(child.get());
					if (resEntry->meta != nullptr && resEntry->meta->getIncludeInBuild())
						output.push_back(static_pointer_cast<FileEntry>(child));
				}
				else if (child->type == LibraryEntryType::Directory)
				{
					todo.push(static_cast<DirectoryEntry*>(child.get()));
				}
			}
		}

		return output;
	}

	HResource ProjectLibrary::load(const Path& path)
	{
		SPtr<ProjectResourceMeta> meta = findResourceMeta(path);
		if (meta == nullptr)
			return HResource();

		ResourceLoadFlags loadFlags = ResourceLoadFlag::Default | ResourceLoadFlag::KeepSourceData;

		const UUID& resUUID = meta->getUUID();
		return gResources().loadFromUUID(resUUID, false, loadFlags);
	}

	void ProjectLibrary::createInternalParentHierarchy(const Path& fullPath, DirectoryEntry** newHierarchyRoot, 
		DirectoryEntry** newHierarchyLeaf)
	{
		Path parentPath = fullPath;

		DirectoryEntry* newEntryParent = nullptr;
		Stack<Path> parentPaths;
		do 
		{
			Path newParentPath = parentPath.getParent();

			if(newParentPath == parentPath)
				break;

			LibraryEntry* newEntryParentLib = findEntry(newParentPath).get();
			if(newEntryParentLib != nullptr)
			{
				assert(newEntryParentLib->type == LibraryEntryType::Directory);
				newEntryParent = static_cast<DirectoryEntry*>(newEntryParentLib);

				break;
			}

			parentPaths.push(newParentPath);
			parentPath = newParentPath;

		} while (true);

		assert(newEntryParent != nullptr); // Must exist

		if(newHierarchyRoot != nullptr)
			*newHierarchyRoot = newEntryParent;

		while(!parentPaths.empty())
		{
			Path curPath = parentPaths.top();
			parentPaths.pop();

			newEntryParent = addDirectoryInternal(newEntryParent, curPath).get();
		}

		if(newHierarchyLeaf != nullptr)
			*newHierarchyLeaf = newEntryParent;
	}

	Path ProjectLibrary::getMetaPath(const Path& path) const
	{
		Path metaPath = path;
		metaPath.setFilename(metaPath.getFilename() + ".meta");

		return metaPath;
	}

	bool ProjectLibrary::isMeta(const Path& fullPath) const
	{
		return fullPath.getExtension() == ".meta";
	}

	bool ProjectLibrary::isNative(const Path& path) const
	{
		String extension = path.getExtension();

		return extension == ".asset" || extension == ".prefab";
	}

	void ProjectLibrary::unloadLibrary()
	{
		if (!mIsLoaded)
			return;

		_finishQueuedImports(true);

		mProjectFolder = Path::BLANK;
		mResourcesFolder = Path::BLANK;

		clearEntries();
		mRootEntry = bs_ushared_ptr_new<DirectoryEntry>(mResourcesFolder, mResourcesFolder.getTail(), nullptr);

		mDependencies.clear();
		gResources().unregisterResourceManifest(mResourceManifest);
		mResourceManifest = nullptr;
		mIsLoaded = false;
	}

	void ProjectLibrary::makeEntriesRelative()
	{
		// Make all paths relative before saving
		std::function<void(LibraryEntry*, const Path&)> makeRelative =
			[&](LibraryEntry* entry, const Path& root)
		{
			entry->path.makeRelative(root);

			if (entry->type == LibraryEntryType::Directory)
			{
				DirectoryEntry* dirEntry = static_cast<DirectoryEntry*>(entry);
				for (auto& child : dirEntry->mChildren)
					makeRelative(child.get(), root);
			}
		};

		Path root = getResourcesFolder();
		makeRelative(mRootEntry.get(), root);
	}

	void ProjectLibrary::makeEntriesAbsolute()
	{
		std::function<void(LibraryEntry*, const Path&)> makeAbsolute =
			[&](LibraryEntry* entry, const Path& root)
		{
			entry->path.makeAbsolute(root);

			if (entry->type == LibraryEntryType::Directory)
			{
				DirectoryEntry* dirEntry = static_cast<DirectoryEntry*>(entry);
				for (auto& child : dirEntry->mChildren)
					makeAbsolute(child.get(), root);
			}
		};

		Path root = getResourcesFolder();
		makeAbsolute(mRootEntry.get(), root);
	}

	void ProjectLibrary::saveLibrary()
	{
		if (!mIsLoaded)
			return;

		// Make all paths relative before saving
		makeEntriesRelative();		
		SPtr<ProjectLibraryEntries> libEntries = ProjectLibraryEntries::create(mRootEntry);

		Path libraryEntriesPath = mProjectFolder;
		libraryEntriesPath.append(PROJECT_INTERNAL_DIR);
		libraryEntriesPath.append(LIBRARY_ENTRIES_FILENAME);

		FileEncoder fs(libraryEntriesPath);
		fs.encode(libEntries.get());

		// Restore absolute entry paths
		makeEntriesAbsolute();

		Path resourceManifestPath = mProjectFolder;
		resourceManifestPath.append(PROJECT_INTERNAL_DIR);
		resourceManifestPath.append(RESOURCE_MANIFEST_FILENAME);

		ResourceManifest::save(mResourceManifest, resourceManifestPath, mProjectFolder);
	}

	void ProjectLibrary::loadLibrary()
	{
		unloadLibrary();

		mProjectFolder = gEditorApplication().getProjectPath();
		mResourcesFolder = mProjectFolder;
		mResourcesFolder.append(RESOURCES_DIR);

		mRootEntry = bs_ushared_ptr_new<DirectoryEntry>(mResourcesFolder, mResourcesFolder.getTail(), nullptr);

		Path libraryEntriesPath = mProjectFolder;
		libraryEntriesPath.append(PROJECT_INTERNAL_DIR);
		libraryEntriesPath.append(LIBRARY_ENTRIES_FILENAME);

		if(FileSystem::exists(libraryEntriesPath))
		{
			FileDecoder fs(libraryEntriesPath);
			SPtr<ProjectLibraryEntries> libEntries = std::static_pointer_cast<ProjectLibraryEntries>(fs.decode());

			mRootEntry = libEntries->getRootEntry();
			mRootEntry->parent = nullptr;
		}

		// Entries are stored relative to project folder, but we want their absolute paths now
		makeEntriesAbsolute();

		// Load resource manifest
		Path resourceManifestPath = mProjectFolder;
		resourceManifestPath.append(PROJECT_INTERNAL_DIR);
		resourceManifestPath.append(RESOURCE_MANIFEST_FILENAME);

		if (FileSystem::exists(resourceManifestPath))
			mResourceManifest = ResourceManifest::load(resourceManifestPath, mProjectFolder);
		else
			mResourceManifest = ResourceManifest::create("ProjectLibrary");

		gResources().registerResourceManifest(mResourceManifest);

		// Load all meta files
		Stack<DirectoryEntry*> todo;
		todo.push(mRootEntry.get());

		Vector<USPtr<LibraryEntry>> deletedEntries;

		while(!todo.empty())
		{
			DirectoryEntry* curDir = todo.top();
			todo.pop();

			for(auto& child : curDir->mChildren)
			{
				if(child->type == LibraryEntryType::File)
				{
					USPtr<FileEntry> resEntry = static_pointer_cast<FileEntry>(child);
					
					if (FileSystem::isFile(resEntry->path))
					{
						if (resEntry->meta == nullptr)
						{
							Path metaPath = resEntry->path;
							metaPath.setFilename(metaPath.getFilename() + ".meta");

							if (FileSystem::isFile(metaPath))
							{
								FileDecoder fs(metaPath);
								SPtr<IReflectable> loadedMeta = fs.decode();

								if (loadedMeta != nullptr && loadedMeta->isDerivedFrom(ProjectFileMeta::getRTTIStatic()))
								{
									SPtr<ProjectFileMeta> fileMeta = std::static_pointer_cast<ProjectFileMeta>(loadedMeta);
									resEntry->meta = fileMeta;
								}
							}
						}

						if (resEntry->meta != nullptr)
						{
							auto& resourceMetas = resEntry->meta->getResourceMetaData();

							if (!resourceMetas.empty())
							{
								mUUIDToPath[resourceMetas[0]->getUUID()] = resEntry->path;

								for (UINT32 i = 1; i < (UINT32)resourceMetas.size(); i++)
								{
									SPtr<ProjectResourceMeta> entry = resourceMetas[i];
									mUUIDToPath[entry->getUUID()] = resEntry->path + entry->getUniqueName();
								}
							}
						}

						addDependencies(resEntry.get());
					}
					else
						deletedEntries.push_back(resEntry);
				}
				else if(child->type == LibraryEntryType::Directory)
				{
					if (FileSystem::isDirectory(child->path))
						todo.push(static_cast<DirectoryEntry*>(child.get()));
					else
						deletedEntries.push_back(child);
				}
			}
		}

		// Remove entries that no longer have corresponding files
		for (auto& deletedEntry : deletedEntries)
		{
			if (deletedEntry->type == LibraryEntryType::File)
				deleteResourceInternal(static_pointer_cast<FileEntry>(deletedEntry));
			else
				deleteDirectoryInternal(static_pointer_cast<DirectoryEntry>(deletedEntry));
		}

		// Clean up internal library folder from obsolete files
		Path internalResourcesFolder = mProjectFolder;
		internalResourcesFolder.append(INTERNAL_RESOURCES_DIR);

		if (FileSystem::exists(internalResourcesFolder))
		{
			Vector<Path> toDelete;
			auto processFile = [&](const Path& file)
			{
				UUID uuid = UUID(file.getFilename(false));
				if (mUUIDToPath.find(uuid) == mUUIDToPath.end())
				{
					mResourceManifest->unregisterResource(uuid);
					toDelete.push_back(file);
				}

				return true;
			};

			FileSystem::iterate(internalResourcesFolder, processFile);

			for (auto& entry : toDelete)
				FileSystem::remove(entry);
		}

		mIsLoaded = true;
	}

	void ProjectLibrary::clearEntries()
	{
		if (mRootEntry == nullptr)
			return;

		std::function<void(LibraryEntry*)> invalidateRecursive =
			[&](LibraryEntry* entry)
		{
			if (entry->type == LibraryEntryType::Directory)
			{
				DirectoryEntry* dirEntry = static_cast<DirectoryEntry*>(entry);

				for (auto& child : dirEntry->mChildren)
					invalidateRecursive(child.get());

				*dirEntry = DirectoryEntry();
			}
			else
			{
				FileEntry* fileEntry = static_cast<FileEntry*>(entry);
				*fileEntry = FileEntry();
			}
		};

		assert(mQueuedImports.empty());

		invalidateRecursive(mRootEntry.get());
		mRootEntry = nullptr;
	}

	Vector<Path> ProjectLibrary::getImportDependencies(const FileEntry* entry)
	{
		Vector<Path> output;

		if (entry->meta == nullptr)
			return output;

		auto& resourceMetas = entry->meta->getResourceMetaData();
		for(auto& resMeta : resourceMetas)
		{
			if (resMeta->getTypeID() == TID_Shader)
			{
				SPtr<ShaderMetaData> metaData = std::static_pointer_cast<ShaderMetaData>(resMeta->getResourceMetaData());

				for (auto& include : metaData->includes)
					output.push_back(include);
			}
		}

		return output;
	}

	void ProjectLibrary::addDependencies(const FileEntry* entry)
	{
		Vector<Path> dependencies = getImportDependencies(entry);
		for (auto& dependency : dependencies)
			mDependencies[dependency].push_back(entry->path);
	}

	void ProjectLibrary::removeDependencies(const FileEntry* entry)
	{
		Vector<Path> dependencies = getImportDependencies(entry);
		for (auto& dependency : dependencies)
		{
			Vector<Path>& curDependencies = mDependencies[dependency];
			auto iterRemove = std::remove_if(curDependencies.begin(), curDependencies.end(),
				[&](const Path& x)
			{
				return x == entry->path;
			});

			curDependencies.erase(iterRemove, curDependencies.end());
		}
	}

	void ProjectLibrary::reimportDependants(const Path& entryPath)
	{
		auto iterFind = mDependencies.find(entryPath);
		if (iterFind == mDependencies.end())
			return;

		// Make a copy since we might modify this list during reimport
		Vector<Path> dependencies = iterFind->second;
		for (auto& dependency : dependencies)
		{
			LibraryEntry* entry = findEntry(dependency).get();
			if (entry != nullptr && entry->type == LibraryEntryType::File)
			{
				FileEntry* resEntry = static_cast<FileEntry*>(entry);

				SPtr<ImportOptions> importOptions;
				if (resEntry->meta != nullptr)
					importOptions = resEntry->meta->getImportOptions();

				reimportResourceInternal(resEntry, importOptions, true);
			}
		}
	}

	BS_ED_EXPORT ProjectLibrary& gProjectLibrary()
	{
		return ProjectLibrary::instance();
	}
}