//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptSerializedObject.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "Wrappers/BsScriptManagedComponent.h"
#include "Wrappers/BsScriptManagedResource.h"
#include "BsManagedComponent.h"
#include "BsManagedResource.h"
#include "Serialization/BsManagedSerializableObject.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Reflection/BsRTTIType.h"
#include "BsScriptGameObjectManager.h"
#include "Serialization/BsSerializedObject.h"

namespace bs
{
	ScriptSerializedObject::ScriptSerializedObject(MonoObject* instance, const SPtr<IReflectable>& obj)
		: ScriptObject(instance), mSerializedObject(obj)
	{

	}

	void ScriptSerializedObject::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_Create", (void*)&ScriptSerializedObject::internal_Create);
		metaData.scriptClass->addInternalCall("Internal_Deserialize", (void*)&ScriptSerializedObject::internal_Deserialize);
	}

	MonoObject* ScriptSerializedObject::internal_Create(MonoObject* obj)
	{
		if (obj == nullptr)
			return nullptr;

		SPtr<IReflectable> nativeValue = ScriptAssemblyManager::instance().getReflectableFromManagedObject(obj);
		if (!nativeValue)
			return nullptr;

		if(!rtti_is_of_type<ManagedSerializableObject>(nativeValue))
			nativeValue = SerializedObject::create(*nativeValue);

		MonoObject* instance = metaData.scriptClass->createInstance();
		new (bs_alloc<ScriptSerializedObject>()) ScriptSerializedObject(instance, nativeValue);

		return instance;
	}

	MonoObject* ScriptSerializedObject::internal_Deserialize(ScriptSerializedObject* thisPtr)
	{
		SPtr<IReflectable> serializedObject = thisPtr->mSerializedObject;
		if (serializedObject == nullptr)
			return nullptr;

		if(auto managedSerializableObject = rtti_cast<ManagedSerializableObject>(serializedObject))
		{
			if (!managedSerializableObject)
				return nullptr;

			return managedSerializableObject->deserialize();
		}
		else
		{
			if(auto nativeSerializedObject = rtti_cast<SerializedObject>(serializedObject))
			{
				SPtr<IReflectable> obj = nativeSerializedObject->decode();

				UINT32 rttiId = obj->getRTTI()->getRTTIId();
				const ReflectableTypeInfo* reflTypeInfo = ScriptAssemblyManager::instance().getReflectableTypeInfo(rttiId);
				if (reflTypeInfo == nullptr)
				{
					BS_LOG(Error, Script, "Mapping between a reflectable object and a managed type is missing "
						"for type \"{0}\"", rttiId);
					return nullptr;
				}

				return reflTypeInfo->createCallback(obj);
			}
		}

		return nullptr;
	}
}
