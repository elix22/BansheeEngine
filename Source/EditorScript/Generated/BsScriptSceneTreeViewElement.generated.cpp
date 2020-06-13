//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptSceneTreeViewElement.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptGameObjectManager.h"
#include "Scene/BsSceneObject.h"
#include "Wrappers/BsScriptSceneObject.h"

namespace bs
{
	ScriptSceneTreeViewElement::ScriptSceneTreeViewElement(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptSceneTreeViewElement::initRuntimeData()
	{ }

	MonoObject*ScriptSceneTreeViewElement::box(const __SceneTreeViewElementInterop& value)
	{
		return MonoUtil::box(metaData.scriptClass->_getInternalClass(), (void*)&value);
	}

	__SceneTreeViewElementInterop ScriptSceneTreeViewElement::unbox(MonoObject* value)
	{
		return *(__SceneTreeViewElementInterop*)MonoUtil::unbox(value);
	}

	SceneTreeViewElement ScriptSceneTreeViewElement::fromInterop(const __SceneTreeViewElementInterop& value)
	{
		SceneTreeViewElement output;
		GameObjectHandle<SceneObject> tmpsceneObject;
		ScriptSceneObject* scriptsceneObject;
		scriptsceneObject = ScriptSceneObject::toNative(value.sceneObject);
		if(scriptsceneObject != nullptr)
			tmpsceneObject = scriptsceneObject->getHandle();
		output.sceneObject = tmpsceneObject;
		output.isExpanded = value.isExpanded;

		return output;
	}

	__SceneTreeViewElementInterop ScriptSceneTreeViewElement::toInterop(const SceneTreeViewElement& value)
	{
		__SceneTreeViewElementInterop output;
		ScriptSceneObject* scriptsceneObject = nullptr;
		if(value.sceneObject)
		scriptsceneObject = ScriptGameObjectManager::instance().getOrCreateScriptSceneObject(value.sceneObject);
		MonoObject* tmpsceneObject;
		if(scriptsceneObject != nullptr)
			tmpsceneObject = scriptsceneObject->getManagedInstance();
		else
			tmpsceneObject = nullptr;
		output.sceneObject = tmpsceneObject;
		output.isExpanded = value.isExpanded;

		return output;
	}

}
