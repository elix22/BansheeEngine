//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptUndoRedo.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "Wrappers/BsScriptSceneObject.h"
#include "BsMonoUtil.h"
#include "BsScriptGameObjectManager.h"
#include "UndoRedo/BsUndoRedo.h"
#include "UndoRedo/BsCmdCloneSO.h"
#include "UndoRedo/BsCmdCreateSO.h"
#include "UndoRedo/BsCmdDeleteSO.h"
#include "UndoRedo/BsCmdInstantiateSO.h"
#include "UndoRedo/BsCmdReparentSO.h"
#include "UndoRedo/BsCmdBreakPrefab.h"
#include "Wrappers/BsScriptPrefab.h"
#include "BsManagedEditorCommand.h"
#include "Scene/BsPrefab.h"
#include "BsScriptObjectManager.h"

namespace bs
{
	ScriptUndoRedo* ScriptUndoRedo::sGlobalUndoRedo = nullptr;
	HEvent ScriptUndoRedo::sDomainLoadConn;

	ScriptUndoRedo::ScriptUndoRedo(MonoObject* instance, const SPtr<UndoRedo>& undoRedo)
		:ScriptObject(instance), mUndoRedo(undoRedo)
	{ }

	void ScriptUndoRedo::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptUndoRedo::internal_CreateInstance);
		metaData.scriptClass->addInternalCall("Internal_Undo", (void*)&ScriptUndoRedo::internal_Undo);
		metaData.scriptClass->addInternalCall("Internal_Redo", (void*)&ScriptUndoRedo::internal_Redo);
		metaData.scriptClass->addInternalCall("Internal_RegisterCommand", (void*)&ScriptUndoRedo::internal_RegisterCommand);
		metaData.scriptClass->addInternalCall("Internal_PushGroup", (void*)&ScriptUndoRedo::internal_PushGroup);
		metaData.scriptClass->addInternalCall("Internal_PopGroup", (void*)&ScriptUndoRedo::internal_PopGroup);
		metaData.scriptClass->addInternalCall("Internal_Clear", (void*)&ScriptUndoRedo::internal_Clear);
		metaData.scriptClass->addInternalCall("Internal_GetTopCommandId", (void*)&ScriptUndoRedo::internal_GetTopCommandId);
		metaData.scriptClass->addInternalCall("Internal_PopCommand", (void*)&ScriptUndoRedo::internal_PopCommand);
		metaData.scriptClass->addInternalCall("Internal_CloneSO", (void*)&ScriptUndoRedo::internal_CloneSO);
		metaData.scriptClass->addInternalCall("Internal_CloneSOMulti", (void*)&ScriptUndoRedo::internal_CloneSOMulti);
		metaData.scriptClass->addInternalCall("Internal_Instantiate", (void*)&ScriptUndoRedo::internal_Instantiate);
		metaData.scriptClass->addInternalCall("Internal_CreateSO", (void*)&ScriptUndoRedo::internal_CreateSO);
		metaData.scriptClass->addInternalCall("Internal_CreateSO2", (void*)&ScriptUndoRedo::internal_CreateSO2);
		metaData.scriptClass->addInternalCall("Internal_DeleteSO", (void*)&ScriptUndoRedo::internal_DeleteSO);
		metaData.scriptClass->addInternalCall("Internal_ReparentSO", (void*)&ScriptUndoRedo::internal_ReparentSO);
		metaData.scriptClass->addInternalCall("Internal_ReparentSOMulti", (void*)&ScriptUndoRedo::internal_ReparentSOMulti);
		metaData.scriptClass->addInternalCall("Internal_BreakPrefab", (void*)&ScriptUndoRedo::internal_BreakPrefab);
	}

	void ScriptUndoRedo::startUp()
	{
		auto createPanel = []()
		{
			assert(sGlobalUndoRedo == nullptr);

			bool dummy = false;
			void* ctorParams[1] = { &dummy };

			MonoObject* instance = metaData.scriptClass->createInstance("bool", ctorParams);
			new (bs_alloc<ScriptUndoRedo>()) ScriptUndoRedo(instance, nullptr);

			MonoMethod* setGlobal = metaData.scriptClass->getMethod("Internal_SetGlobal", 1);
			void* setGlobalParams[1] = { instance };
			setGlobal->invoke(nullptr, setGlobalParams);
		};

		sDomainLoadConn = ScriptObjectManager::instance().onRefreshDomainLoaded.connect(createPanel);

		createPanel();
	}

	void ScriptUndoRedo::shutDown()
	{
		sDomainLoadConn.disconnect();
	}

	MonoObject* ScriptUndoRedo::create()
	{
		bool dummy = false;
		void* params[1] = { &dummy };

		MonoObject* instance = metaData.scriptClass->createInstance("bool", params);

		SPtr<UndoRedo> undoRedo = bs_shared_ptr_new<UndoRedo>();
		new (bs_alloc<ScriptUndoRedo>()) ScriptUndoRedo(instance, undoRedo);

		return instance;
	}

	void ScriptUndoRedo::internal_CreateInstance(MonoObject* instance)
	{
		SPtr<UndoRedo> undoRedo = bs_shared_ptr_new<UndoRedo>();
		new (bs_alloc<ScriptUndoRedo>()) ScriptUndoRedo(instance, undoRedo);
	}

	void ScriptUndoRedo::internal_Undo(ScriptUndoRedo* thisPtr)
	{
		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		undoRedo->undo();
	}

	void ScriptUndoRedo::internal_Redo(ScriptUndoRedo* thisPtr)
	{
		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		undoRedo->redo();
	}

	void ScriptUndoRedo::internal_RegisterCommand(ScriptUndoRedo* thisPtr, ScriptCmdManaged* command)
	{
		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		undoRedo->registerCommand(command->getInternal());
	}

	void ScriptUndoRedo::internal_PushGroup(ScriptUndoRedo* thisPtr, MonoString* name)
	{
		String nativeName = MonoUtil::monoToString(name);

		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		undoRedo->pushGroup(nativeName);
	}

	void ScriptUndoRedo::internal_PopGroup(ScriptUndoRedo* thisPtr, MonoString* name)
	{
		String nativeName = MonoUtil::monoToString(name);

		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		undoRedo->popGroup(nativeName);
	}

	void ScriptUndoRedo::internal_Clear(ScriptUndoRedo* thisPtr)
	{
		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		return undoRedo->clear();
	}

	UINT32 ScriptUndoRedo::internal_GetTopCommandId(ScriptUndoRedo* thisPtr)
	{
		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		return undoRedo->getTopCommandId();
	}

	void ScriptUndoRedo::internal_PopCommand(ScriptUndoRedo* thisPtr, UINT32 id)
	{
		UndoRedo* undoRedo = thisPtr->mUndoRedo != nullptr ? thisPtr->mUndoRedo.get() : UndoRedo::instancePtr();
		undoRedo->popCommand(id);
	}

	MonoObject* ScriptUndoRedo::internal_CloneSO(ScriptSceneObject* soPtr, MonoString* description)
	{
		String nativeDescription = MonoUtil::monoToString(description);
		HSceneObject clone = CmdCloneSO::execute(soPtr->getHandle(), nativeDescription);

		ScriptSceneObject* cloneSoPtr = ScriptGameObjectManager::instance().getOrCreateScriptSceneObject(clone);
		return cloneSoPtr->getManagedInstance();
	}

	MonoArray* ScriptUndoRedo::internal_CloneSOMulti(MonoArray* soPtrs, MonoString* description)
	{
		String nativeDescription = MonoUtil::monoToString(description);

		ScriptArray input(soPtrs);
		ScriptArray output = ScriptArray::create<ScriptSceneObject>(input.size());

		for (UINT32 i = 0; i < input.size(); i++)
		{
			ScriptSceneObject* soPtr = input.get<ScriptSceneObject*>(i);
			HSceneObject clone = CmdCloneSO::execute(soPtr->getHandle(), nativeDescription);

			ScriptSceneObject* cloneSoPtr = ScriptGameObjectManager::instance().getOrCreateScriptSceneObject(clone);
			output.set(i, cloneSoPtr->getManagedInstance());
		}

		return output.getInternal();
	}

	MonoObject* ScriptUndoRedo::internal_Instantiate(ScriptPrefab* prefabPtr, MonoString* description)
	{
		HPrefab prefab = prefabPtr->getHandle();
		if (!prefab.isLoaded())
			return nullptr;

		String nativeDescription = MonoUtil::monoToString(description);
		HSceneObject clone = CmdInstantiateSO::execute(prefab, nativeDescription);

		ScriptSceneObject* cloneSoPtr = ScriptGameObjectManager::instance().getOrCreateScriptSceneObject(clone);
		return cloneSoPtr->getManagedInstance();
	}

	MonoObject* ScriptUndoRedo::internal_CreateSO(MonoString* name, MonoString* description)
	{
		String nativeName = MonoUtil::monoToString(name);
		String nativeDescription = MonoUtil::monoToString(description);
		HSceneObject newObj = CmdCreateSO::execute(nativeName, 0, nativeDescription);

		return ScriptGameObjectManager::instance().getOrCreateScriptSceneObject(newObj)->getManagedInstance();
	}

	MonoObject* ScriptUndoRedo::internal_CreateSO2(MonoString* name, MonoArray* types, MonoString* description)
	{
		String nativeName = MonoUtil::monoToString(name);
		String nativeDescription = MonoUtil::monoToString(description);

		ScriptAssemblyManager& sam = ScriptAssemblyManager::instance();
		MonoClass* managedComponent = sam.getBuiltinClasses().managedComponentClass;

		ScriptArray scriptArray(types);
		Vector<UINT32> typeIds;
		for (UINT32 i = 0; i < scriptArray.size(); i++)
		{
			MonoReflectionType* paramType = scriptArray.get<MonoReflectionType*>(i);
			if(!paramType)
				continue;

			::MonoClass* requestedClass = MonoUtil::getClass(paramType);

			bool isManagedComponent = MonoUtil::isSubClassOf(requestedClass, managedComponent->_getInternalClass());
			if (isManagedComponent)
			{
				BS_LOG(Warning, Editor, "Only built-in components can be added though this method.");
				continue;
			}

			BuiltinComponentInfo* info = sam.getBuiltinComponentInfo(paramType);
			if (info == nullptr)
			{
				BS_LOG(Warning, Editor, "Provided type is not a valid component");
				continue;
			}

			typeIds.push_back(info->typeId);
		}

		HSceneObject newObj = CmdCreateSO::execute(nativeName, 0, typeIds, nativeDescription);

		return ScriptGameObjectManager::instance().getOrCreateScriptSceneObject(newObj)->getManagedInstance();
	}

	void ScriptUndoRedo::internal_DeleteSO(ScriptSceneObject* soPtr, MonoString* description)
	{
		String nativeDescription = MonoUtil::monoToString(description);
		CmdDeleteSO::execute(soPtr->getHandle(), nativeDescription);
	}

	void ScriptUndoRedo::internal_ReparentSO(ScriptSceneObject* soPtr, ScriptSceneObject* parentSOPtr, MonoString* description)
	{
		HSceneObject parent;
		if (parentSOPtr != nullptr)
			parent = parentSOPtr->getHandle();

		String nativeDescription = MonoUtil::monoToString(description);

		HSceneObject so = soPtr->getHandle();
		CmdReparentSO::execute(so, parent, nativeDescription);
	}

	void ScriptUndoRedo::internal_ReparentSOMulti(MonoArray* soPtrs, ScriptSceneObject* parentSOPtr, MonoString* description)
	{
		HSceneObject parent;
		if (parentSOPtr != nullptr)
			parent = parentSOPtr->getHandle();

		String nativeDescription = MonoUtil::monoToString(description);

		ScriptArray input(soPtrs);
		for (UINT32 i = 0; i < input.size(); i++)
		{
			ScriptSceneObject* soPtr = input.get<ScriptSceneObject*>(i);
			HSceneObject so = soPtr->getHandle();
			CmdReparentSO::execute(so, parent, nativeDescription);
		}
	}

	void ScriptUndoRedo::internal_BreakPrefab(ScriptSceneObject* soPtr, MonoString* description)
	{
		String nativeDescription = MonoUtil::monoToString(description);

		HSceneObject so = soPtr->getHandle();
		CmdBreakPrefab::execute(so, nativeDescription);
	}
}