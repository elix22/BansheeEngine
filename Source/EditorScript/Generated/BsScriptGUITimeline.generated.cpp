//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUITimeline.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUITimeline.h"
#include "Wrappers/BsScriptVector2I.h"

namespace bs
{
	ScriptGUITimeline::ScriptGUITimeline(MonoObject* managedInstance, GUITimeline* value)
		:TScriptGUIElement(managedInstance, value)
	{
	}

	void ScriptGUITimeline::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setRange", (void*)&ScriptGUITimeline::Internal_setRange);
		metaData.scriptClass->addInternalCall("Internal_getRange", (void*)&ScriptGUITimeline::Internal_getRange);
		metaData.scriptClass->addInternalCall("Internal_setOffset", (void*)&ScriptGUITimeline::Internal_setOffset);
		metaData.scriptClass->addInternalCall("Internal_getOffset", (void*)&ScriptGUITimeline::Internal_getOffset);
		metaData.scriptClass->addInternalCall("Internal_setFPS", (void*)&ScriptGUITimeline::Internal_setFPS);
		metaData.scriptClass->addInternalCall("Internal_getFPS", (void*)&ScriptGUITimeline::Internal_getFPS);
		metaData.scriptClass->addInternalCall("Internal_setMarkedFrame", (void*)&ScriptGUITimeline::Internal_setMarkedFrame);
		metaData.scriptClass->addInternalCall("Internal_setMarkedFrame0", (void*)&ScriptGUITimeline::Internal_setMarkedFrame0);
		metaData.scriptClass->addInternalCall("Internal_getFrame", (void*)&ScriptGUITimeline::Internal_getFrame);
		metaData.scriptClass->addInternalCall("Internal_getTime", (void*)&ScriptGUITimeline::Internal_getTime);
		metaData.scriptClass->addInternalCall("Internal_getOffset0", (void*)&ScriptGUITimeline::Internal_getOffset0);
		metaData.scriptClass->addInternalCall("Internal_getTimeForFrame", (void*)&ScriptGUITimeline::Internal_getTimeForFrame);
		metaData.scriptClass->addInternalCall("Internal_setPadding", (void*)&ScriptGUITimeline::Internal_setPadding);
		metaData.scriptClass->addInternalCall("Internal_getPadding", (void*)&ScriptGUITimeline::Internal_getPadding);

	}

	void ScriptGUITimeline::Internal_setRange(ScriptGUIElementBaseTBase* thisPtr, float range)
	{
		static_cast<GUITimeline*>(thisPtr->getGUIElement())->setRange(range);
	}

	float ScriptGUITimeline::Internal_getRange(ScriptGUIElementBaseTBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getRange();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUITimeline::Internal_setOffset(ScriptGUIElementBaseTBase* thisPtr, float offset)
	{
		static_cast<GUITimeline*>(thisPtr->getGUIElement())->setOffset(offset);
	}

	float ScriptGUITimeline::Internal_getOffset(ScriptGUIElementBaseTBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getOffset();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUITimeline::Internal_setFPS(ScriptGUIElementBaseTBase* thisPtr, uint32_t FPS)
	{
		static_cast<GUITimeline*>(thisPtr->getGUIElement())->setFPS(FPS);
	}

	uint32_t ScriptGUITimeline::Internal_getFPS(ScriptGUIElementBaseTBase* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getFPS();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUITimeline::Internal_setMarkedFrame(ScriptGUIElementBaseTBase* thisPtr, uint32_t index)
	{
		static_cast<GUITimeline*>(thisPtr->getGUIElement())->setMarkedFrame(index);
	}

	uint32_t ScriptGUITimeline::Internal_setMarkedFrame0(ScriptGUIElementBaseTBase* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->setMarkedFrame();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptGUITimeline::Internal_getFrame(ScriptGUIElementBaseTBase* thisPtr, Vector2I* pixelCoords)
	{
		uint32_t tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getFrame(*pixelCoords);

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptGUITimeline::Internal_getTime(ScriptGUIElementBaseTBase* thisPtr, int32_t pixel)
	{
		float tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getTime(pixel);

		float __output;
		__output = tmp__output;

		return __output;
	}

	int32_t ScriptGUITimeline::Internal_getOffset0(ScriptGUIElementBaseTBase* thisPtr, float time)
	{
		int32_t tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getOffset(time);

		int32_t __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptGUITimeline::Internal_getTimeForFrame(ScriptGUIElementBaseTBase* thisPtr, int32_t index)
	{
		float tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getTimeForFrame(index);

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUITimeline::Internal_setPadding(ScriptGUIElementBaseTBase* thisPtr, uint32_t padding)
	{
		static_cast<GUITimeline*>(thisPtr->getGUIElement())->setPadding(padding);
	}

	uint32_t ScriptGUITimeline::Internal_getPadding(ScriptGUIElementBaseTBase* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = static_cast<GUITimeline*>(thisPtr->getGUIElement())->getPadding();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}
}
