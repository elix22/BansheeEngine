//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptSceneTreeViewState.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUISceneTreeView.h"
#include "BsScriptSceneTreeViewElement.generated.h"

namespace bs
{
	ScriptSceneTreeViewState::ScriptSceneTreeViewState(MonoObject* managedInstance, const SPtr<SceneTreeViewState>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptSceneTreeViewState::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getelements", (void*)&ScriptSceneTreeViewState::Internal_getelements);
		metaData.scriptClass->addInternalCall("Internal_setelements", (void*)&ScriptSceneTreeViewState::Internal_setelements);

	}

	MonoObject* ScriptSceneTreeViewState::create(const SPtr<SceneTreeViewState>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptSceneTreeViewState>()) ScriptSceneTreeViewState(managedInstance, value);
		return managedInstance;
	}
	MonoArray* ScriptSceneTreeViewState::Internal_getelements(ScriptSceneTreeViewState* thisPtr)
	{
		Vector<SceneTreeViewElement> vec__output;
		vec__output = thisPtr->getInternal()->elements;

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptSceneTreeViewElement>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, ScriptSceneTreeViewElement::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptSceneTreeViewState::Internal_setelements(ScriptSceneTreeViewState* thisPtr, MonoArray* value)
	{
		Vector<SceneTreeViewElement> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = ScriptSceneTreeViewElement::fromInterop(arrayvalue.get<__SceneTreeViewElementInterop>(i));
			}

		}
		thisPtr->getInternal()->elements = vecvalue;
	}
}
