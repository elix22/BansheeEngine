//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptSerializedSceneObject.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/Scene/BsSerializedSceneObject.h"
#include "BsScriptGameObjectManager.h"
#include "Wrappers/BsScriptSceneObject.h"

namespace bs
{
	ScriptSerializedSceneObject::ScriptSerializedSceneObject(MonoObject* managedInstance, const SPtr<SerializedSceneObject>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptSerializedSceneObject::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_SerializedSceneObject", (void*)&ScriptSerializedSceneObject::Internal_SerializedSceneObject);
		metaData.scriptClass->addInternalCall("Internal_restore", (void*)&ScriptSerializedSceneObject::Internal_restore);

	}

	MonoObject* ScriptSerializedSceneObject::create(const SPtr<SerializedSceneObject>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptSerializedSceneObject>()) ScriptSerializedSceneObject(managedInstance, value);
		return managedInstance;
	}
	void ScriptSerializedSceneObject::Internal_SerializedSceneObject(MonoObject* managedInstance, MonoObject* sceneObject, bool hierarchy)
	{
		GameObjectHandle<SceneObject> tmpsceneObject;
		ScriptSceneObject* scriptsceneObject;
		scriptsceneObject = ScriptSceneObject::toNative(sceneObject);
		if(scriptsceneObject != nullptr)
			tmpsceneObject = scriptsceneObject->getHandle();
		SPtr<SerializedSceneObject> instance = bs_shared_ptr_new<SerializedSceneObject>(tmpsceneObject, hierarchy);
		new (bs_alloc<ScriptSerializedSceneObject>())ScriptSerializedSceneObject(managedInstance, instance);
	}

	void ScriptSerializedSceneObject::Internal_restore(ScriptSerializedSceneObject* thisPtr)
	{
		thisPtr->getInternal()->restore();
	}
}
