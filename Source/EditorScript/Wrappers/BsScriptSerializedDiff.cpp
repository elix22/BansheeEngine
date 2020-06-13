//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptSerializedDiff.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "Serialization/BsManagedSerializableObject.h"
#include "Wrappers/BsScriptSerializedObject.h"
#include "Serialization/BsManagedSerializableDiff.h"
#include "Serialization/BsBinaryDiff.h"
#include "Reflection/BsRTTIType.h"
#include "Serialization/BsSerializedObject.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Utility/BsUtility.h"
#include "Serialization/BsBinaryDiff.h"

namespace bs
{
	ScriptSerializedDiff::ScriptSerializedDiff(MonoObject* instance, const SPtr<IReflectable>& obj)
		: ScriptObject(instance), mSerializedDiff(obj)
	{

	}

	void ScriptSerializedDiff::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateDiff", (void*)&ScriptSerializedDiff::internal_CreateDiff);
		metaData.scriptClass->addInternalCall("Internal_ApplyDiff", (void*)&ScriptSerializedDiff::internal_ApplyDiff);
		metaData.scriptClass->addInternalCall("Internal_IsEmpty", (void*)&ScriptSerializedDiff::internal_IsEmpty);
	}

	MonoObject* ScriptSerializedDiff::internal_CreateDiff(ScriptSerializedObject* oldObj, ScriptSerializedObject* newObj)
	{
		SPtr<IReflectable> oldSerializedObject = oldObj->getInternal();
		SPtr<IReflectable> newSerializedObject = newObj->getInternal();

		if (oldSerializedObject == nullptr || newSerializedObject == nullptr)
			return nullptr;

		auto oldManagedSerializedObject = rtti_cast<ManagedSerializableObject>(oldSerializedObject);
		auto newManagedSerializedObject = rtti_cast<ManagedSerializableObject>(newSerializedObject);

		auto oldNativeSerializedObj = rtti_cast<SerializedObject>(oldSerializedObject);
		auto newNativeSerializedObj = rtti_cast<SerializedObject>(newSerializedObject);

		SPtr<IReflectable> diff;
		if(oldManagedSerializedObject != nullptr && newManagedSerializedObject != nullptr)
			diff = ManagedSerializableDiff::create(oldManagedSerializedObject.get(), newManagedSerializedObject.get());
		else if(oldNativeSerializedObj != nullptr && newNativeSerializedObj != nullptr)
		{
			BinaryDiff diffHandler;
			diff = diffHandler.generateDiff(oldNativeSerializedObj, newNativeSerializedObj);
		}

		if(diff)
		{
			MonoObject* instance = metaData.scriptClass->createInstance();
			new (bs_alloc<ScriptSerializedDiff>()) ScriptSerializedDiff(instance, diff);

			return instance;
		}

		return nullptr;
	}

	void ScriptSerializedDiff::internal_ApplyDiff(ScriptSerializedDiff* thisPtr, MonoObject* obj)
	{
		if(thisPtr->mSerializedDiff == nullptr)
			return;

		SPtr<ManagedSerializableDiff> managedDiff = rtti_cast<ManagedSerializableDiff>(thisPtr->mSerializedDiff);
		if(managedDiff != nullptr)
		{
			SPtr<ManagedSerializableObject> serializedObject = ManagedSerializableObject::createFromExisting(obj);
			if (!serializedObject)
				return;

			managedDiff->apply(serializedObject);
		}
		else
		{
			SPtr<IReflectable> nativeValue = ScriptAssemblyManager::instance().getReflectableFromManagedObject(obj);
			if (!nativeValue)
				return;

			IDiff& diffHandler = nativeValue->getRTTI()->getDiffHandler();
			CoreSerializationContext serzContext;
			diffHandler.applyDiff(nativeValue, rtti_cast<SerializedObject>(thisPtr->mSerializedDiff), &serzContext);
		}
	}

	bool ScriptSerializedDiff::internal_IsEmpty(ScriptSerializedDiff* thisPtr)
	{
		return thisPtr->mSerializedDiff == nullptr;
	}
}
