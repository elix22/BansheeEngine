//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptUnitTests.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "Scene/BsSceneObject.h"
#include "Wrappers/BsScriptSceneObject.h"
#include "Serialization/BsManagedSerializableObject.h"
#include "Serialization/BsManagedSerializableDiff.h"

namespace bs
{
	MonoMethod* ScriptUnitTests::RunTestsMethod;
	SPtr<ManagedSerializableDiff> ScriptUnitTests::tempDiff;

	ScriptUnitTests::ScriptUnitTests(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptUnitTests::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_UT1_GameObjectClone", (void*)&ScriptUnitTests::internal_UT1_GameObjectClone);
		metaData.scriptClass->addInternalCall("Internal_UT3_GenerateDiff", (void*)&ScriptUnitTests::internal_UT3_GenerateDiff);
		metaData.scriptClass->addInternalCall("Internal_UT3_ApplyDiff", (void*)&ScriptUnitTests::internal_UT3_ApplyDiff);

		RunTestsMethod = metaData.scriptClass->getMethod("RunTests");
	}

	void ScriptUnitTests::runTests()
	{
		RunTestsMethod->invoke(nullptr, nullptr);
	}

	void ScriptUnitTests::internal_UT1_GameObjectClone(MonoObject* instance)
	{
		ScriptSceneObject* nativeInstance = ScriptSceneObject::toNative(instance);

		HSceneObject SO = static_object_cast<SceneObject>(nativeInstance->getNativeHandle());
		HSceneObject cloneSO = SO->clone();

		cloneSO->setParent(SO);
	}

	void ScriptUnitTests::internal_UT3_GenerateDiff(MonoObject* oldObj, MonoObject* newObj)
	{
		SPtr<ManagedSerializableObject> serializableOldObj = ManagedSerializableObject::createFromExisting(oldObj);
		SPtr<ManagedSerializableObject> serializableNewObj = ManagedSerializableObject::createFromExisting(newObj);

		tempDiff = ManagedSerializableDiff::create(serializableOldObj.get(), serializableNewObj.get());
	}

	void ScriptUnitTests::internal_UT3_ApplyDiff(MonoObject* obj)
	{
		SPtr<ManagedSerializableObject> serializableObj = ManagedSerializableObject::createFromExisting(obj);
		tempDiff->apply(serializableObj);

		tempDiff = nullptr;
	}
}