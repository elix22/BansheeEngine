//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "../../bsf/Source/Foundation/bsfCore/Localization/BsHString.h"
#include "../../bsf/Source/Foundation/bsfCore/Animation/BsAnimationCurve.h"
#include "Math/BsVector2.h"
#include "../../EditorCore/GUI/BsGUICurves.h"

namespace bs { struct __GUIContentInterop; }
namespace bs { class GUICurvesField; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUICurvesField : public TScriptGUIElement<ScriptGUICurvesField>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUICurvesField")

		ScriptGUICurvesField(MonoObject* managedInstance, GUICurvesField* value);

	private:
		void onClicked();

		typedef void(BS_THUNKCALL *onClickedThunkDef) (MonoObject*, MonoException**);
		static onClickedThunkDef onClickedThunk;

		static void Internal_setCurve(ScriptGUICurvesField* thisPtr, MonoObject* curve);
		static void Internal_setCurveRange(ScriptGUICurvesField* thisPtr, MonoObject* curveA, MonoObject* curveB);
		static MonoObject* Internal_getCurve(ScriptGUICurvesField* thisPtr);
		static MonoObject* Internal_getMinCurve(ScriptGUICurvesField* thisPtr);
		static MonoObject* Internal_getMaxCurve(ScriptGUICurvesField* thisPtr);
		static void Internal_setRange(ScriptGUICurvesField* thisPtr, float xRange, float yRange);
		static void Internal_setOffset(ScriptGUICurvesField* thisPtr, Vector2* offset);
		static void Internal_centerAndZoom(ScriptGUICurvesField* thisPtr);
		static void Internal_setPadding(ScriptGUICurvesField* thisPtr, uint32_t padding);
		static void Internal_create(MonoObject* managedInstance, CurveDrawOption drawOptions, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style);
		static void Internal_create0(MonoObject* managedInstance, CurveDrawOption drawOptions, MonoObject* labelText, uint32_t labelWidth, MonoString* style);
		static void Internal_create1(MonoObject* managedInstance, CurveDrawOption drawOptions, MonoString* style);
		static void Internal_create2(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style);
		static void Internal_create3(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style);
		static void Internal_create4(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style);
		static void Internal_create5(MonoObject* managedInstance, MonoObject* labelText, MonoString* style);
		static void Internal_create6(MonoObject* managedInstance, MonoString* style);
	};
}
