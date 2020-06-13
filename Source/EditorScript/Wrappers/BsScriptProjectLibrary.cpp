//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptProjectLibrary.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "Wrappers/BsScriptResource.h"
#include "Resources/BsResources.h"
#include "Library/BsProjectResourceMeta.h"
#include "BsScriptResourceManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsEditorApplication.h"
#include "Serialization/BsManagedSerializableObject.h"
#include "Reflection/BsRTTIType.h"
#include "BsManagedResourceMetaData.h"

#include "Generated/BsScriptProjectResourceIcons.generated.h"
#include "Generated/BsScriptImportOptions.editor.generated.h"

using namespace std::placeholders;

namespace bs
{
	ScriptProjectLibrary::OnEntryChangedThunkDef ScriptProjectLibrary::OnEntryAddedThunk;
	ScriptProjectLibrary::OnEntryChangedThunkDef ScriptProjectLibrary::OnEntryRemovedThunk;
	ScriptProjectLibrary::OnEntryChangedThunkDef ScriptProjectLibrary::OnEntryImportedThunk;

	HEvent ScriptProjectLibrary::mOnEntryAddedConn;
	HEvent ScriptProjectLibrary::mOnEntryRemovedConn;
	HEvent ScriptProjectLibrary::mOnEntryImportedConn;

	ScriptProjectLibrary::ScriptProjectLibrary(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptProjectLibrary::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_Refresh", (void*)&ScriptProjectLibrary::internal_Refresh);
		metaData.scriptClass->addInternalCall("Internal_FinalizeImports", (void*)&ScriptProjectLibrary::internal_FinalizeImports);
		metaData.scriptClass->addInternalCall("Internal_Create", (void*)&ScriptProjectLibrary::internal_Create);
		metaData.scriptClass->addInternalCall("Internal_Load", (void*)&ScriptProjectLibrary::internal_Load);
		metaData.scriptClass->addInternalCall("Internal_Save", (void*)&ScriptProjectLibrary::internal_Save);
		metaData.scriptClass->addInternalCall("Internal_GetRoot", (void*)&ScriptProjectLibrary::internal_GetRoot);
		metaData.scriptClass->addInternalCall("Internal_Reimport", (void*)&ScriptProjectLibrary::internal_Reimport);
		metaData.scriptClass->addInternalCall("Internal_GetImportProgress", (void*)&ScriptProjectLibrary::internal_GetImportProgress);
		metaData.scriptClass->addInternalCall("Internal_CancelImport", (void*)&ScriptProjectLibrary::internal_CancelImport);
		metaData.scriptClass->addInternalCall("Internal_GetEntry", (void*)&ScriptProjectLibrary::internal_GetEntry);
		metaData.scriptClass->addInternalCall("Internal_IsSubresource", (void*)&ScriptProjectLibrary::internal_IsSubresource);
		metaData.scriptClass->addInternalCall("Internal_GetMeta", (void*)&ScriptProjectLibrary::internal_GetMeta);
		metaData.scriptClass->addInternalCall("Internal_GetPath", (void*)&ScriptProjectLibrary::internal_GetPath);
		metaData.scriptClass->addInternalCall("Internal_GetPathFromUUID", (void*)&ScriptProjectLibrary::internal_GetPathFromUUID);
		metaData.scriptClass->addInternalCall("Internal_Search", (void*)&ScriptProjectLibrary::internal_Search);
		metaData.scriptClass->addInternalCall("Internal_Delete", (void*)&ScriptProjectLibrary::internal_Delete);
		metaData.scriptClass->addInternalCall("Internal_CreateFolder", (void*)&ScriptProjectLibrary::internal_CreateFolder);
		metaData.scriptClass->addInternalCall("Internal_Rename", (void*)&ScriptProjectLibrary::internal_Rename);
		metaData.scriptClass->addInternalCall("Internal_Move", (void*)&ScriptProjectLibrary::internal_Move);
		metaData.scriptClass->addInternalCall("Internal_Copy", (void*)&ScriptProjectLibrary::internal_Copy);
		metaData.scriptClass->addInternalCall("Internal_GetResourceFolder", (void*)&ScriptProjectLibrary::internal_GetResourceFolder);
		metaData.scriptClass->addInternalCall("Internal_SetIncludeInBuild", (void*)&ScriptProjectLibrary::internal_SetIncludeInBuild);
		metaData.scriptClass->addInternalCall("Internal_SetEditorData", (void*)&ScriptProjectLibrary::internal_SetEditorData);
		metaData.scriptClass->addInternalCall("Internal_GetInProgressImportCount", (void*)&ScriptProjectLibrary::internal_GetInProgressImportCount);

		OnEntryAddedThunk = (OnEntryChangedThunkDef)metaData.scriptClass->getMethod("Internal_DoOnEntryAdded", 1)->getThunk();
		OnEntryRemovedThunk = (OnEntryChangedThunkDef)metaData.scriptClass->getMethod("Internal_DoOnEntryRemoved", 1)->getThunk();
		OnEntryImportedThunk = (OnEntryChangedThunkDef)metaData.scriptClass->getMethod("Internal_DoOnEntryImported", 1)->getThunk();
	}

	UINT32 ScriptProjectLibrary::internal_Refresh(MonoString* path, bool synchronous)
	{
		Path nativePath = MonoUtil::monoToString(path);

		if (!nativePath.isAbsolute())
			nativePath.makeAbsolute(gProjectLibrary().getResourcesFolder());

		const UINT32 importCount = gProjectLibrary().checkForModifications(nativePath);

		if(synchronous)
			gProjectLibrary()._finishQueuedImports(true);

		return importCount;
	}

	void ScriptProjectLibrary::internal_FinalizeImports()
	{
		gProjectLibrary()._finishQueuedImports();
	}
		
	UINT32 ScriptProjectLibrary::internal_GetInProgressImportCount()
	{
		return gProjectLibrary().getInProgressImportCount();		
	}

	void ScriptProjectLibrary::internal_Create(MonoObject* resource, MonoString* path)
	{
		ScriptResource* scrResource = ScriptResource::toNative(resource);
		Path resourcePath = MonoUtil::monoToString(path);

		gProjectLibrary().createEntry(scrResource->getGenericHandle(), resourcePath);
	}

	MonoObject* ScriptProjectLibrary::internal_Load(MonoString* path)
	{
		Path resourcePath = MonoUtil::monoToString(path);
		HResource resource = gProjectLibrary().load(resourcePath);

		if (!resource)
			return nullptr;

		ScriptResourceBase* scriptResource = ScriptResourceManager::instance().getScriptResource(resource, true);
		return scriptResource->getManagedInstance();
	}

	void ScriptProjectLibrary::internal_Save(MonoObject* resource)
	{
		ScriptResource* srcResource = ScriptResource::toNative(resource);

		if (srcResource != nullptr)
			gProjectLibrary().saveEntry(srcResource->getGenericHandle());
	}

	MonoObject* ScriptProjectLibrary::internal_GetRoot()
	{
		return ScriptDirectoryEntry::create(gProjectLibrary().getRootEntry());
	}

	void ScriptProjectLibrary::internal_Reimport(MonoString* path, MonoObject* options, bool force, bool synchronous)
	{
		Path assetPath = MonoUtil::monoToString(path);

		SPtr<ImportOptions> nativeOptions;
		if (options != nullptr)
		{
			ScriptImportOptions* scriptOptions = ScriptImportOptions::toNative(options);
			nativeOptions = scriptOptions->getInternal();
		}

		gProjectLibrary().reimport(assetPath, nativeOptions, force, synchronous);
	}

	float ScriptProjectLibrary::internal_GetImportProgress(MonoString* path)
	{
		Path assetPath = MonoUtil::monoToString(path);

		return gProjectLibrary().getImportProgress(assetPath);
	}

	void ScriptProjectLibrary::internal_CancelImport()
	{
		gProjectLibrary().cancelImport();
	}

	MonoObject* ScriptProjectLibrary::internal_GetEntry(MonoString* path)
	{
		Path assetPath = MonoUtil::monoToString(path);

		USPtr<ProjectLibrary::LibraryEntry> entry = gProjectLibrary().findEntry(assetPath);
		if (entry == nullptr)
			return nullptr;

		if (entry->type == ProjectLibrary::LibraryEntryType::File)
			return ScriptFileEntry::create(static_pointer_cast<ProjectLibrary::FileEntry>(entry));
		else
			return ScriptDirectoryEntry::create(static_pointer_cast<ProjectLibrary::DirectoryEntry>(entry));
	}

	bool ScriptProjectLibrary::internal_IsSubresource(MonoString* path)
	{
		Path assetPath = MonoUtil::monoToString(path);

		return gProjectLibrary().isSubresource(assetPath);
	}

	MonoObject* ScriptProjectLibrary::internal_GetMeta(MonoString* path)
	{
		Path assetPath = MonoUtil::monoToString(path);

		SPtr<ProjectResourceMeta> meta = gProjectLibrary().findResourceMeta(assetPath);
		if (meta == nullptr)
			return nullptr;

		return ScriptResourceMeta::create(meta);
	}

	MonoString* ScriptProjectLibrary::internal_GetPathFromUUID(UUID* uuid)
	{
		Path nativePath = gProjectLibrary().uuidToPath(*uuid);

		return MonoUtil::stringToMono(nativePath.toString());
	}

	MonoString* ScriptProjectLibrary::internal_GetPath(MonoObject* resource)
	{
		ScriptResource* srcResource = ScriptResource::toNative(resource);

		if (srcResource != nullptr)
		{
			Path nativePath = gProjectLibrary().uuidToPath(srcResource->getGenericHandle().getUUID());
			nativePath.getRelative(gProjectLibrary().getResourcesFolder());

			return MonoUtil::stringToMono(nativePath.toString());
		}

		return nullptr;
	}

	MonoArray* ScriptProjectLibrary::internal_Search(MonoString* pattern, MonoArray* types)
	{
		String strPattern = MonoUtil::monoToString(pattern);

		Vector<UINT32> typeIds;
		if (types != nullptr)
		{
			ScriptArray typeArray(types);
			for (UINT32 i = 0; i < typeArray.size(); i++)
			{
				BuiltinResourceInfo* resInfo = ScriptAssemblyManager::instance().getBuiltinResourceInfo((ScriptResourceType)typeArray.get<UINT32>(i));
				if (resInfo == nullptr)
					continue;

				typeIds.push_back(resInfo->typeId);
			}
		}

		Vector<USPtr<ProjectLibrary::LibraryEntry>> foundEntries = gProjectLibrary().search(strPattern, typeIds);

		UINT32 idx = 0;
		ScriptArray outArray = ScriptArray::create<ScriptLibraryEntry>((UINT32)foundEntries.size());
		for (auto& entry : foundEntries)
		{
			MonoObject* managedEntry = nullptr;

			if (entry->type == ProjectLibrary::LibraryEntryType::File)
				managedEntry = ScriptFileEntry::create(static_pointer_cast<ProjectLibrary::FileEntry>(entry));
			else
				managedEntry = ScriptDirectoryEntry::create(static_pointer_cast<ProjectLibrary::DirectoryEntry>(entry));

			outArray.set(idx, managedEntry);
			idx++;
		}

		return outArray.getInternal();
	}

	void ScriptProjectLibrary::internal_Delete(MonoString* path)
	{
		Path pathToDelete = MonoUtil::monoToString(path);
		gProjectLibrary().deleteEntry(pathToDelete);
	}

	void ScriptProjectLibrary::internal_CreateFolder(MonoString* path)
	{
		Path folderToCreate = MonoUtil::monoToString(path);
		gProjectLibrary().createFolderEntry(folderToCreate);
	}

	void ScriptProjectLibrary::internal_Rename(MonoString* path, MonoString* name, bool overwrite)
	{
		Path oldPath = MonoUtil::monoToString(path);
		Path newPath = oldPath.getParent().append(MonoUtil::monoToString(name));

		gProjectLibrary().moveEntry(oldPath, newPath, overwrite);
	}

	void ScriptProjectLibrary::internal_Move(MonoString* oldPath, MonoString* newPath, bool overwrite)
	{
		Path oldPathNative = MonoUtil::monoToString(oldPath);
		Path newPathNative = MonoUtil::monoToString(newPath);

		gProjectLibrary().moveEntry(oldPathNative, newPathNative, overwrite);
	}

	void ScriptProjectLibrary::internal_Copy(MonoString* source, MonoString* destination, bool overwrite)
	{
		Path oldPathNative = MonoUtil::monoToString(source);
		Path newPathNative = MonoUtil::monoToString(destination);

		gProjectLibrary().copyEntry(oldPathNative, newPathNative, overwrite);
	}

	MonoString* ScriptProjectLibrary::internal_GetResourceFolder()
	{
		String resFolder = gProjectLibrary().getResourcesFolder().toString();

		return MonoUtil::stringToMono(resFolder);
	}

	void ScriptProjectLibrary::internal_SetIncludeInBuild(MonoString* path, bool include)
	{
		Path pathNative = MonoUtil::monoToString(path);

		gProjectLibrary().setIncludeInBuild(pathNative, include);
	}

	void ScriptProjectLibrary::internal_SetEditorData(MonoString* path, MonoObject* userData)
	{
		Path pathNative = MonoUtil::monoToString(path);

		if(userData == nullptr)
		{
			gProjectLibrary().setUserData(pathNative, nullptr);
			return;
		}

		SPtr<ManagedSerializableObject> serializedUserData = ManagedSerializableObject::createFromExisting(userData);
		serializedUserData->serialize();

		gProjectLibrary().setUserData(pathNative, serializedUserData);
	}

	void ScriptProjectLibrary::startUp()
	{
		mOnEntryAddedConn = gProjectLibrary().onEntryAdded.connect(std::bind(&ScriptProjectLibrary::onEntryAdded, _1));
		mOnEntryRemovedConn = gProjectLibrary().onEntryRemoved.connect(std::bind(&ScriptProjectLibrary::onEntryRemoved, _1));
		mOnEntryImportedConn = gProjectLibrary().onEntryImported.connect(std::bind(&ScriptProjectLibrary::onEntryImported, _1));
	}

	void ScriptProjectLibrary::shutDown()
	{
		mOnEntryAddedConn.disconnect();
		mOnEntryRemovedConn.disconnect();
		mOnEntryImportedConn.disconnect();
	}

	void ScriptProjectLibrary::onEntryAdded(const Path& path)
	{
		Path relativePath = path;
		if (relativePath.isAbsolute())
			relativePath.makeRelative(gProjectLibrary().getResourcesFolder());

		MonoString* pathStr = MonoUtil::stringToMono(relativePath.toString());
		MonoUtil::invokeThunk(OnEntryAddedThunk, pathStr);
	}

	void ScriptProjectLibrary::onEntryRemoved(const Path& path)
	{
		Path relativePath = path;
		if (relativePath.isAbsolute())
			relativePath.makeRelative(gProjectLibrary().getResourcesFolder());

		MonoString* pathStr = MonoUtil::stringToMono(relativePath.toString());
		MonoUtil::invokeThunk(OnEntryRemovedThunk, pathStr);
	}

	void ScriptProjectLibrary::onEntryImported(const Path& path)
	{
		Path relativePath = path;
		if (relativePath.isAbsolute())
			relativePath.makeRelative(gProjectLibrary().getResourcesFolder());

		MonoString* pathStr = MonoUtil::stringToMono(relativePath.toString());
		MonoUtil::invokeThunk(OnEntryImportedThunk, pathStr);
	}

	ScriptLibraryEntryBase::ScriptLibraryEntryBase(MonoObject* instance)
		:ScriptObjectBase(instance)
	{ }

	ScriptLibraryEntry::ScriptLibraryEntry(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptLibraryEntry::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GetPath", (void*)&ScriptLibraryEntry::internal_GetPath);
		metaData.scriptClass->addInternalCall("Internal_GetName", (void*)&ScriptLibraryEntry::internal_GetName);
		metaData.scriptClass->addInternalCall("Internal_GetType", (void*)&ScriptLibraryEntry::internal_GetType);
		metaData.scriptClass->addInternalCall("Internal_GetParent", (void*)&ScriptLibraryEntry::internal_GetParent);
	}

	MonoString* ScriptLibraryEntry::internal_GetPath(ScriptLibraryEntryBase* thisPtr)
	{
		Path relativePath = thisPtr->getInternal()->path;
		relativePath.makeRelative(gProjectLibrary().getResourcesFolder());

		return MonoUtil::stringToMono(relativePath.toString());
	}

	MonoString* ScriptLibraryEntry::internal_GetName(ScriptLibraryEntryBase* thisPtr)
	{
		return MonoUtil::stringToMono(thisPtr->getInternal()->elementName);
	}

	ProjectLibrary::LibraryEntryType ScriptLibraryEntry::internal_GetType(ScriptLibraryEntryBase* thisPtr)
	{
		return thisPtr->getInternal()->type;
	}

	MonoObject* ScriptLibraryEntry::internal_GetParent(ScriptLibraryEntryBase* thisPtr)
	{
		ProjectLibrary::DirectoryEntry* parent = thisPtr->getInternal()->parent;
		if(parent == nullptr)
			return nullptr;

		USPtr<ProjectLibrary::LibraryEntry> entry = gProjectLibrary().findEntry(parent->path);
		if (entry == nullptr)
			return nullptr;

		return ScriptDirectoryEntry::create(static_pointer_cast<ProjectLibrary::DirectoryEntry>(entry));
	}

	ScriptDirectoryEntry::ScriptDirectoryEntry(MonoObject* instance, const USPtr<ProjectLibrary::DirectoryEntry>& entry)
		:ScriptObject(instance)
	{
		mEntry = entry;
	}

	MonoObject* ScriptDirectoryEntry::create(const USPtr<ProjectLibrary::DirectoryEntry>& entry)
	{
		MonoObject* managedInstance = metaData.scriptClass->createInstance();
		bs_new<ScriptDirectoryEntry>(managedInstance, entry);

		return managedInstance;
	}

	void ScriptDirectoryEntry::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GetChildren", (void*)&ScriptDirectoryEntry::internal_GetChildren);
	}

	MonoArray* ScriptDirectoryEntry::internal_GetChildren(ScriptDirectoryEntry* thisPtr)
	{
		ProjectLibrary::DirectoryEntry* dirEntry = static_cast<ProjectLibrary::DirectoryEntry*>(thisPtr->getInternal().get());
		ScriptArray outArray = ScriptArray::create<ScriptLibraryEntry>((UINT32)dirEntry->mChildren.size());
		for (UINT32 i = 0; i < (UINT32)dirEntry->mChildren.size(); i++)
		{
			USPtr<ProjectLibrary::LibraryEntry> childEntry = dirEntry->mChildren[i];
			MonoObject* managedChildEntry = nullptr;

			if (childEntry->type == ProjectLibrary::LibraryEntryType::File)
				managedChildEntry = ScriptFileEntry::create(static_pointer_cast<ProjectLibrary::FileEntry>(childEntry));
			else
				managedChildEntry = ScriptDirectoryEntry::create(static_pointer_cast<ProjectLibrary::DirectoryEntry>(childEntry));

			outArray.set(i, managedChildEntry);
		}

		return outArray.getInternal();
	}

	ScriptFileEntry::ScriptFileEntry(MonoObject* instance, const USPtr<ProjectLibrary::FileEntry>& entry)
		:ScriptObject(instance)
	{
		mEntry = entry;
	}

	MonoObject* ScriptFileEntry::create(const USPtr<ProjectLibrary::FileEntry>& entry)
	{
		MonoObject* managedInstance = metaData.scriptClass->createInstance();
		bs_new<ScriptFileEntry>(managedInstance, entry);

		return managedInstance;
	}

	void ScriptFileEntry::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GetImportOptions", (void*)&ScriptFileEntry::internal_GetImportOptions);
		metaData.scriptClass->addInternalCall("Internal_GetResourceMetas", (void*)&ScriptFileEntry::internal_GetResourceMetas);
		metaData.scriptClass->addInternalCall("Internal_GetIncludeInBuild", (void*)&ScriptFileEntry::internal_GetIncludeInBuild);
	}

	MonoObject* ScriptFileEntry::internal_GetImportOptions(ScriptFileEntry* thisPtr)
	{
		auto* fileEntry = static_cast<ProjectLibrary::FileEntry*>(thisPtr->getInternal().get());

		if (fileEntry->meta != nullptr)
		{
			const SPtr<ImportOptions>& io = fileEntry->meta->getImportOptions();
			if(!io)
				return nullptr;

			UINT32 rttiId = io->getRTTI()->getRTTIId(); 
			const ReflectableTypeInfo* reflTypeInfo = ScriptAssemblyManager::instance().getReflectableTypeInfo(rttiId);
			if(reflTypeInfo == nullptr)
			{
				BS_LOG(Error, Script,"Mapping between a reflectable object and a managed type is missing "
					"for type \"{0}\"", rttiId);
				return nullptr;
			}

			return reflTypeInfo->createCallback(io);
		}
		else
			return nullptr;
	}

	MonoArray* ScriptFileEntry::internal_GetResourceMetas(ScriptFileEntry* thisPtr)
	{
		auto* fileEntry = static_cast<ProjectLibrary::FileEntry*>(thisPtr->getInternal().get());
		if (fileEntry->meta != nullptr)
		{
			auto& resourceMetas = fileEntry->meta->getResourceMetaData();
			UINT32 numElements = (UINT32)resourceMetas.size();

			ScriptArray output = ScriptArray::create<ScriptResourceMeta>(numElements);
			for (UINT32 i = 0; i < numElements; i++)
				output.set(i, ScriptResourceMeta::create(resourceMetas[i]));

			return output.getInternal();
		}
			
		return ScriptArray::create<ScriptResourceMeta>(0).getInternal();
	}

	bool ScriptFileEntry::internal_GetIncludeInBuild(ScriptFileEntry* thisPtr)
	{
		auto* fileEntry = static_cast<ProjectLibrary::FileEntry*>(thisPtr->getInternal().get());

		if (fileEntry->meta != nullptr)
			return fileEntry->meta->getIncludeInBuild();

		return false;
	}

	ScriptResourceMeta::ScriptResourceMeta(MonoObject* instance, const SPtr<ProjectResourceMeta>& meta)
		:ScriptObject(instance), mMeta(meta)
	{ }

	MonoObject* ScriptResourceMeta::create(const SPtr<ProjectResourceMeta>& meta)
	{
		MonoObject* managedInstance = metaData.scriptClass->createInstance();
		bs_new<ScriptResourceMeta>(managedInstance, meta);

		return managedInstance;
	}

	void ScriptResourceMeta::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GetUUID", (void*)&ScriptResourceMeta::internal_GetUUID);
		metaData.scriptClass->addInternalCall("Internal_GetSubresourceName", (void*)&ScriptResourceMeta::internal_GetSubresourceName);
		metaData.scriptClass->addInternalCall("Internal_GetPreviewIcons", (void*)&ScriptResourceMeta::internal_GetPreviewIcons);
		metaData.scriptClass->addInternalCall("Internal_GetResourceType", (void*)&ScriptResourceMeta::internal_GetResourceType);
		metaData.scriptClass->addInternalCall("Internal_GetType", (void*)&ScriptResourceMeta::internal_GetType);
		metaData.scriptClass->addInternalCall("Internal_GetEditorData", (void*)&ScriptResourceMeta::internal_GetEditorData);
	}

	void ScriptResourceMeta::internal_GetUUID(ScriptResourceMeta* thisPtr, UUID* uuid)
	{
		*uuid = thisPtr->mMeta->getUUID();
	}

	MonoString* ScriptResourceMeta::internal_GetSubresourceName(ScriptResourceMeta* thisPtr)
	{
		return MonoUtil::stringToMono(thisPtr->mMeta->getUniqueName());
	}

	void ScriptResourceMeta::internal_GetPreviewIcons(ScriptResourceMeta* thisPtr, __ProjectResourceIconsInterop* output)
	{
		*output = ScriptProjectResourceIcons::toInterop(thisPtr->mMeta->getPreviewIcons());
	}

	ScriptResourceType ScriptResourceMeta::internal_GetResourceType(ScriptResourceMeta* thisPtr)
	{
		BuiltinResourceInfo* resInfo = ScriptAssemblyManager::instance().getBuiltinResourceInfo(thisPtr->mMeta->getTypeID());
		if (resInfo == nullptr)
			return ScriptResourceType::Undefined;

		return resInfo->resType;
	}

	MonoReflectionType* ScriptResourceMeta::internal_GetType(ScriptResourceMeta* thisPtr)
	{
		const UINT32 typeId = thisPtr->mMeta->getTypeID();
		if(typeId == TID_ManagedResource)
		{
			const auto metaData = std::static_pointer_cast<ManagedResourceMetaData>(thisPtr->mMeta->getResourceMetaData());

			if(metaData)
			{
				MonoClass* providedClass = MonoManager::instance().findClass(metaData->typeNamespace, metaData->typeName);

				if(providedClass)
					return MonoUtil::getType(providedClass->_getInternalClass());
			}
		}
		else
		{
			BuiltinResourceInfo* resInfo = ScriptAssemblyManager::instance().getBuiltinResourceInfo(thisPtr->mMeta->getTypeID());

			if (resInfo)
				return MonoUtil::getType(resInfo->metaData->scriptClass->_getInternalClass());
		}

		return nullptr;
	}

	MonoObject* ScriptResourceMeta::internal_GetEditorData(ScriptResourceMeta* thisPtr)
	{
		SPtr<IReflectable> userData = thisPtr->mMeta->getUserData();
		if (userData == nullptr)
			return nullptr;

		if (!rtti_is_of_type<ManagedSerializableObject>(userData.get()))
			return nullptr;

		SPtr<ManagedSerializableObject> userDataObj = std::static_pointer_cast<ManagedSerializableObject>(userData);
		return userDataObj->deserialize();
	}
}