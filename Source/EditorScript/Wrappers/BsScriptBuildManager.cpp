//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptBuildManager.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "Image/BsTexture.h"
#include "BsMonoUtil.h"
#include "Wrappers/BsScriptPlatformInfo.h"
#include "FileSystem/BsFileSystem.h"
#include "Utility/BsIconUtility.h"
#include "CoreThread/BsCoreThread.h"
#include "Utility/BsGameSettings.h"
#include "Serialization/BsFileSerializer.h"
#include "Library/BsProjectLibrary.h"
#include "Library/BsProjectResourceMeta.h"
#include "Resources/BsResources.h"
#include "Scene/BsPrefab.h"
#include "BsEditorApplication.h"
#include "Resources/BsResourceManifest.h"
#include "Resources/BsBuiltinResources.h"
#include "Scene/BsSceneObject.h"
#include "Debug/BsDebug.h"
#include "Resources/BsGameResourceManager.h"

namespace bs
{
	ScriptBuildManager::ScriptBuildManager(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptBuildManager::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GetAvailablePlatforms", (void*)&ScriptBuildManager::internal_GetAvailablePlatforms);
		metaData.scriptClass->addInternalCall("Internal_GetActivePlatform", (void*)&ScriptBuildManager::internal_GetActivePlatform);
		metaData.scriptClass->addInternalCall("Internal_SetActivePlatform", (void*)&ScriptBuildManager::internal_SetActivePlatform);
		metaData.scriptClass->addInternalCall("Internal_GetActivePlatformInfo", (void*)&ScriptBuildManager::internal_GetActivePlatformInfo);
		metaData.scriptClass->addInternalCall("Internal_GetPlatformInfo", (void*)&ScriptBuildManager::internal_GetPlatformInfo);
		metaData.scriptClass->addInternalCall("Internal_GetFrameworkAssemblies", (void*)&ScriptBuildManager::internal_GetFrameworkAssemblies);
		metaData.scriptClass->addInternalCall("Internal_GetMainExecutable", (void*)&ScriptBuildManager::internal_GetMainExecutable);
		metaData.scriptClass->addInternalCall("Internal_GetDefines", (void*)&ScriptBuildManager::internal_GetDefines);
		metaData.scriptClass->addInternalCall("Internal_GetNativeBinaries", (void*)&ScriptBuildManager::internal_GetNativeBinaries);
		metaData.scriptClass->addInternalCall("Internal_GetBuildFolder", (void*)&ScriptBuildManager::internal_GetBuildFolder);
		metaData.scriptClass->addInternalCall("Internal_InjectIcons", (void*)&ScriptBuildManager::internal_InjectIcons);
		metaData.scriptClass->addInternalCall("Internal_PackageResources", (void*)&ScriptBuildManager::internal_PackageResources);
		metaData.scriptClass->addInternalCall("Internal_CreateStartupSettings", (void*)&ScriptBuildManager::internal_CreateStartupSettings);
	}

	MonoArray* ScriptBuildManager::internal_GetAvailablePlatforms()
	{
		const Vector<PlatformType>& availableType = BuildManager::instance().getAvailablePlatforms();

		ScriptArray outArray = ScriptArray::create<UINT32>((UINT32)availableType.size());
		UINT32 idx = 0;
		for (auto& type : availableType)
			outArray.set(idx++, type);

		return outArray.getInternal();
	}

	PlatformType ScriptBuildManager::internal_GetActivePlatform()
	{
		return BuildManager::instance().getActivePlatform();
	}

	void ScriptBuildManager::internal_SetActivePlatform(PlatformType value)
	{
		BuildManager::instance().setActivePlatform(value);
	}

	MonoObject* ScriptBuildManager::internal_GetActivePlatformInfo()
	{
		return ScriptPlatformInfo::create(BuildManager::instance().getActivePlatformInfo());
	}

	MonoObject* ScriptBuildManager::internal_GetPlatformInfo(PlatformType type)
	{
		return ScriptPlatformInfo::create(BuildManager::instance().getPlatformInfo(type));
	}

	MonoArray* ScriptBuildManager::internal_GetFrameworkAssemblies(PlatformType type)
	{
		Vector<String> frameworkAssemblies = BuildManager::instance().getFrameworkAssemblies(type);

		ScriptArray outArray = ScriptArray::create<String>((UINT32)frameworkAssemblies.size());
		UINT32 idx = 0;
		for (auto& assemblyName : frameworkAssemblies)
			outArray.set(idx++, MonoUtil::stringToMono(assemblyName));

		return outArray.getInternal();
	}

	MonoString* ScriptBuildManager::internal_GetMainExecutable(PlatformType type)
	{
		return MonoUtil::stringToMono(BuildManager::instance().getMainExecutable(type).toString());
	}

	MonoString* ScriptBuildManager::internal_GetDefines(PlatformType type)
	{
		return MonoUtil::stringToMono(BuildManager::instance().getDefines(type));
	}

	MonoArray* ScriptBuildManager::internal_GetNativeBinaries(PlatformType type)
	{
		Vector<Path> paths = BuildManager::instance().getNativeBinaries(type);

		UINT32 numEntries = (UINT32)paths.size();
		ScriptArray outArray = ScriptArray::create<String>(numEntries);
		for (UINT32 i = 0; i < numEntries; i++)
		{
			outArray.set(i, MonoUtil::stringToMono(paths[i].toString()));
		}

		return outArray.getInternal();
	}

	MonoString* ScriptBuildManager::internal_GetBuildFolder(ScriptBuildFolder folder, PlatformType platform)
	{
		Path path;

		if (folder == ScriptBuildFolder::FrameworkAssemblies)
		{
			Path assemblyFolder = MonoManager::instance().getFrameworkAssembliesFolder();

			Path sourceFolder = BuildManager::instance().getBuildFolder(BuildFolder::SourceRoot, platform);
			path = assemblyFolder.makeRelative(sourceFolder);
		}
		else if (folder == ScriptBuildFolder::Mono)
		{
			Path monoEtcFolder = MonoManager::instance().getMonoEtcFolder();

			Path sourceFolder = BuildManager::instance().getBuildFolder(BuildFolder::SourceRoot, platform);
			path = monoEtcFolder.makeRelative(sourceFolder);
		}
		else
		{
			BuildFolder nativeFolderType = BuildFolder::SourceRoot;
			switch (folder)
			{
			case ScriptBuildFolder::SourceRoot:
				nativeFolderType = BuildFolder::SourceRoot;
				break;
			case ScriptBuildFolder::DestinationRoot:
				nativeFolderType = BuildFolder::DestinationRoot;
				break;
			case ScriptBuildFolder::NativeBinaries:
				nativeFolderType = BuildFolder::NativeBinaries;
				break;
			case ScriptBuildFolder::BansheeDebugAssemblies:
				nativeFolderType = BuildFolder::BansheeDebugAssemblies;
				break;
			case ScriptBuildFolder::BansheeReleaseAssemblies:
				nativeFolderType = BuildFolder::BansheeReleaseAssemblies;
				break;
			case ScriptBuildFolder::Data:
				nativeFolderType = BuildFolder::Data;
				break;
			default:
				break;
			}

			path = BuildManager::instance().getBuildFolder(nativeFolderType, platform);
		}

		return MonoUtil::stringToMono(path.toString());
	}

	void ScriptBuildManager::internal_InjectIcons(MonoString* filePath, ScriptPlatformInfo* info)
	{
		if (info == nullptr)
			return;

		Path executablePath = MonoUtil::monoToString(filePath);

		Map<UINT32, SPtr<PixelData>> icons;
		SPtr<PlatformInfo> platformInfo = info->getPlatformInfo();
		switch (platformInfo->type)
		{
		case PlatformType::Windows:
		{
			SPtr<WinPlatformInfo> winPlatformInfo = std::static_pointer_cast<WinPlatformInfo>(platformInfo);

			struct IconData
			{
				UINT32 size;
				SPtr<PixelData> pixels;
			};

			IconData textures[] =
			{ 
				{ 16, nullptr },
				{ 32, nullptr },
				{ 48, nullptr },
				{ 64, nullptr },
				{ 96, nullptr },
				{ 128, nullptr },
				{ 192, nullptr },
				{ 256, nullptr }
			};

			HTexture icon = static_resource_cast<Texture>(gResources().loadFromUUID(winPlatformInfo->icon.getUUID()));
			if (icon.isLoaded())
			{
				auto& texProps = icon->getProperties();

				SPtr<PixelData> pixels = texProps.allocBuffer(0, 0);
				icon->readData(pixels);
				gCoreThread().submit(true);

				for (auto& entry : textures)
				{
					entry.pixels = PixelData::create(entry.size, entry.size, 1, PF_RGBA8);
					PixelUtil::scale(*pixels, *entry.pixels);

					icons[entry.size] = entry.pixels;
				}
			}
		}
			break;
		default:
			break;
		}

		IconUtility::updateIconExe(executablePath, icons);
	}

	void ScriptBuildManager::internal_PackageResources(MonoString* buildFolder, ScriptPlatformInfo* info)
	{
		UnorderedSet<Path> usedResources;
		SPtr<ResourceMapping> resourceMap = ResourceMapping::create();

		// Get all resources manually included in build
		Vector<USPtr<ProjectLibrary::FileEntry>> buildResources = gProjectLibrary().getResourcesForBuild();
		for (auto& entry : buildResources)
		{
			if (entry->meta == nullptr)
			{
				BS_LOG(Warning, Editor, "Cannot include resource in build, missing meta file for: {0}", entry->path);
				continue;
			}

			auto& resourceMetas = entry->meta->getResourceMetaData();
			for(auto& resMeta : resourceMetas)
			{
				Path resourcePath;
				if (gResources().getFilePathFromUUID(resMeta->getUUID(), resourcePath))
					usedResources.insert(resourcePath);
				else
					BS_LOG(Warning, Editor, "Cannot include resource in build, missing imported asset for: {0}", entry->path);
			}
		}

		// Include main scene
		SPtr<PlatformInfo> platformInfo;

		if (info != nullptr)
			platformInfo = info->getPlatformInfo();

		if (platformInfo != nullptr)
		{
			Path resourcePath;
			if (gResources().getFilePathFromUUID(platformInfo->mainScene.getUUID(), resourcePath))
				usedResources.insert(resourcePath);
			else
				BS_LOG(Warning, Editor, "Cannot include main scene in build, missing imported asset.");
		}

		// Find dependencies of all resources
		Vector<Path> newResources;
		for (auto& entry : usedResources)
			newResources.push_back(entry);

		while (!newResources.empty())
		{
			Vector<Path> allDependencies;
			for (auto& entry : newResources)
			{
				Vector<UUID> curDependencies = gResources().getDependencies(entry);
				for (auto& entry : curDependencies)
				{
					Path resourcePath;
					if (gResources().getFilePathFromUUID(entry, resourcePath))
					{
						if (usedResources.find(resourcePath) == usedResources.end())
						{
							allDependencies.push_back(resourcePath);
							usedResources.insert(resourcePath);
						}
					}
				}
			}

			newResources = allDependencies;
		} 

		// Copy resources
		Path buildPath = MonoUtil::monoToString(buildFolder);

		Path outputPath = buildPath;
		outputPath.append(GAME_RESOURCES_FOLDER_NAME);

		FileSystem::createDir(outputPath);

		Path libraryDir = gProjectLibrary().getResourcesFolder();
		for (auto& entry : usedResources)
		{
			UUID uuid;

			const bool found = gResources().getUUIDFromFilePath(entry, uuid);
			BS_ASSERT(found);

			Path sourcePath = gProjectLibrary().uuidToPath(uuid);
			if (sourcePath.isEmpty()) // Resource not part of library, meaning its built-in and we don't need to copy those here
				continue;

			SPtr<ProjectResourceMeta> resMeta = gProjectLibrary().findResourceMeta(sourcePath);
			assert(resMeta != nullptr);

			Path destPath = outputPath;
			destPath.setFilename(entry.getFilename());

			// Create library -> packaged resource mapping
			Path relSourcePath = sourcePath;
			if (sourcePath.isAbsolute())
				relSourcePath.makeRelative(libraryDir);

			Path relDestPath = GAME_RESOURCES_FOLDER_NAME;
			relDestPath.setFilename(entry.getFilename());

			resourceMap->add(relSourcePath, relDestPath);

			// If resource is prefab make sure to update it in case any of the prefabs it is referencing changed
			if (resMeta->getTypeID() == TID_Prefab)
			{
				bool reload = gResources().isLoaded(uuid);

				HPrefab prefab = static_resource_cast<Prefab>(gProjectLibrary().load(sourcePath));
				prefab->_updateChildInstances();

				// Clear prefab diffs as they're not used in standalone
				Stack<HSceneObject> todo;
				todo.push(prefab->_getRoot());

				while (!todo.empty())
				{
					HSceneObject current = todo.top();
					todo.pop();

					current->_clearPrefabDiff();

					UINT32 numChildren = current->getNumChildren();
					for (UINT32 i = 0; i < numChildren; i++)
					{
						HSceneObject child = current->getChild(i);
						todo.push(child);
					}
				}

				gResources().save(prefab, destPath, false);

				// Need to unload this one as we modified it in memory, and we don't want to persist those changes past
				// this point
				gResources().release(prefab);

				if (reload)
					gProjectLibrary().load(sourcePath);
			}
			else
				FileSystem::copy(entry, destPath);
		}

		// Save icon
		Path iconFolder = BuiltinResources::getIconFolder();

		Path sourceRoot = BuildManager::instance().getBuildFolder(BuildFolder::SourceRoot, platformInfo->type);
		iconFolder.makeRelative(sourceRoot);

		Path destRoot = BuildManager::instance().getBuildFolder(BuildFolder::DestinationRoot, platformInfo->type);
		Path destIconFile = destRoot;
		destIconFile.append(iconFolder);
		destIconFile.setFilename(String(BuiltinResources::IconTextureName) + ".asset");

		switch (platformInfo->type)
		{
		case PlatformType::Windows:
		{
			SPtr<WinPlatformInfo> winPlatformInfo = std::static_pointer_cast<WinPlatformInfo>(platformInfo);
			
			HTexture icon = static_resource_cast<Texture>(gResources().loadFromUUID(winPlatformInfo->icon.getUUID()));
			if (icon != nullptr)
				gResources().save(icon, destIconFile, true);
		}
			break;
		default:
			break;
		};

		// Save manifest
		Path manifestPath = outputPath;
		manifestPath.append(GAME_RESOURCE_MANIFEST_NAME);

		Path internalResourcesFolder = gEditorApplication().getProjectPath();
		internalResourcesFolder.append(PROJECT_INTERNAL_DIR);

		SPtr<ResourceManifest> manifest = gProjectLibrary()._getManifest();
		ResourceManifest::save(manifest, manifestPath, internalResourcesFolder);

		// Save resource map
		Path mappingPath = outputPath;
		mappingPath.append(GAME_RESOURCE_MAPPING_NAME);

		FileEncoder fe(mappingPath);
		fe.encode(resourceMap.get());
	}

	void ScriptBuildManager::internal_CreateStartupSettings(MonoString* buildFolder, ScriptPlatformInfo* info)
	{
		SPtr<PlatformInfo> platformInfo;

		if (info != nullptr)
			platformInfo = info->getPlatformInfo();

		SPtr<GameSettings> gameSettings;
		if (platformInfo != nullptr)
		{
			gameSettings = bs_shared_ptr_new<GameSettings>();
			gameSettings->mainSceneUUID = platformInfo->mainScene.getUUID();
			gameSettings->fullscreen = platformInfo->fullscreen;
			gameSettings->resolutionWidth = platformInfo->windowedWidth;
			gameSettings->resolutionHeight = platformInfo->windowedHeight;

			switch (platformInfo->type)
			{
			case(PlatformType::Windows) :
			{
				SPtr<WinPlatformInfo> winPlatformInfo = std::static_pointer_cast<WinPlatformInfo>(platformInfo);
				gameSettings->titleBarText = winPlatformInfo->titlebarText;
			}
				break;
			default:
				break;
			}
		}

		Path outputPath = MonoUtil::monoToString(buildFolder);
		outputPath.append(GAME_SETTINGS_NAME);

		FileEncoder fe(outputPath);
		fe.encode(gameSettings.get());
	}
}
