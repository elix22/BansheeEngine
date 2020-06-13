//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "Math/BsVector2I.h"

namespace bs { class GUITimeline; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUITimeline : public TScriptGUIElement<ScriptGUITimeline>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUITimeline")

		ScriptGUITimeline(MonoObject* managedInstance, GUITimeline* value);

	private:
		static void Internal_setRange(ScriptGUIElementBaseTBase* thisPtr, float range);
		static float Internal_getRange(ScriptGUIElementBaseTBase* thisPtr);
		static void Internal_setOffset(ScriptGUIElementBaseTBase* thisPtr, float offset);
		static float Internal_getOffset(ScriptGUIElementBaseTBase* thisPtr);
		static void Internal_setFPS(ScriptGUIElementBaseTBase* thisPtr, uint32_t FPS);
		static uint32_t Internal_getFPS(ScriptGUIElementBaseTBase* thisPtr);
		static void Internal_setMarkedFrame(ScriptGUIElementBaseTBase* thisPtr, uint32_t index);
		static uint32_t Internal_setMarkedFrame0(ScriptGUIElementBaseTBase* thisPtr);
		static uint32_t Internal_getFrame(ScriptGUIElementBaseTBase* thisPtr, Vector2I* pixelCoords);
		static float Internal_getTime(ScriptGUIElementBaseTBase* thisPtr, int32_t pixel);
		static int32_t Internal_getOffset0(ScriptGUIElementBaseTBase* thisPtr, float time);
		static float Internal_getTimeForFrame(ScriptGUIElementBaseTBase* thisPtr, int32_t index);
		static void Internal_setPadding(ScriptGUIElementBaseTBase* thisPtr, uint32_t padding);
		static uint32_t Internal_getPadding(ScriptGUIElementBaseTBase* thisPtr);
	};
}
