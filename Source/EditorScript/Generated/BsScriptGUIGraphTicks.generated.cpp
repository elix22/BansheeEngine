//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIGraphTicks.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIGraphTicks.h"

namespace bs
{
	ScriptGUIGraphTicks::ScriptGUIGraphTicks(MonoObject* managedInstance, const SPtr<GUIGraphTicks>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptGUIGraphTicks::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GUIGraphTicks", (void*)&ScriptGUIGraphTicks::Internal_GUIGraphTicks);
		metaData.scriptClass->addInternalCall("Internal_getNumLevels", (void*)&ScriptGUIGraphTicks::Internal_getNumLevels);
		metaData.scriptClass->addInternalCall("Internal_setRange", (void*)&ScriptGUIGraphTicks::Internal_setRange);
		metaData.scriptClass->addInternalCall("Internal_setTickSpacing", (void*)&ScriptGUIGraphTicks::Internal_setTickSpacing);
		metaData.scriptClass->addInternalCall("Internal_getLevelStrength", (void*)&ScriptGUIGraphTicks::Internal_getLevelStrength);
		metaData.scriptClass->addInternalCall("Internal_getTicks", (void*)&ScriptGUIGraphTicks::Internal_getTicks);

	}

	MonoObject* ScriptGUIGraphTicks::create(const SPtr<GUIGraphTicks>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptGUIGraphTicks>()) ScriptGUIGraphTicks(managedInstance, value);
		return managedInstance;
	}
	void ScriptGUIGraphTicks::Internal_GUIGraphTicks(MonoObject* managedInstance, GUITickStepType stepType)
	{
		SPtr<GUIGraphTicks> instance = bs_shared_ptr_new<GUIGraphTicks>(stepType);
		new (bs_alloc<ScriptGUIGraphTicks>())ScriptGUIGraphTicks(managedInstance, instance);
	}

	uint32_t ScriptGUIGraphTicks::Internal_getNumLevels(ScriptGUIGraphTicks* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getInternal()->getNumLevels();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIGraphTicks::Internal_setRange(ScriptGUIGraphTicks* thisPtr, float valueRangeStart, float valueRangeEnd, uint32_t pixelRange)
	{
		thisPtr->getInternal()->setRange(valueRangeStart, valueRangeEnd, pixelRange);
	}

	void ScriptGUIGraphTicks::Internal_setTickSpacing(ScriptGUIGraphTicks* thisPtr, int32_t minPx, int32_t maxPx)
	{
		thisPtr->getInternal()->setTickSpacing(minPx, maxPx);
	}

	float ScriptGUIGraphTicks::Internal_getLevelStrength(ScriptGUIGraphTicks* thisPtr, uint32_t level)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->getLevelStrength(level);

		float __output;
		__output = tmp__output;

		return __output;
	}

	MonoArray* ScriptGUIGraphTicks::Internal_getTicks(ScriptGUIGraphTicks* thisPtr, uint32_t level)
	{
		Vector<float> vec__output;
		vec__output = thisPtr->getInternal()->getTicks(level);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<float>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}
}
