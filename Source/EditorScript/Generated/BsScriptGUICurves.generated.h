//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "BsScriptGUITimeline.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Animation/BsAnimationUtility.h"
#include "Math/BsVector2.h"
#include "../../EditorCore/GUI/BsGUICurves.h"
#include "Math/BsVector2I.h"
#include "../../bsf/Source/Foundation/bsfCore/Animation/BsAnimationUtility.h"

namespace bs { class GUICurves; }
namespace bs { struct __CurveDrawInfoInterop; }
namespace bs { struct __TangentRefInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUICurves : public TScriptGUIElement<ScriptGUICurves>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUICurves")

		ScriptGUICurves(MonoObject* managedInstance, GUICurves* value);

	private:
		void onClicked();

		typedef void(BS_THUNKCALL *onClickedThunkDef) (MonoObject*, MonoException**);
		static onClickedThunkDef onClickedThunk;

		static void Internal_setCurves(ScriptGUICurves* thisPtr, MonoArray* curves);
		static MonoArray* Internal_getCurves(ScriptGUICurves* thisPtr);
		static void Internal_setRange(ScriptGUICurves* thisPtr, float xRange, float yRange);
		static void Internal_setOffset(ScriptGUICurves* thisPtr, Vector2* offset);
		static void Internal_centerAndZoom(ScriptGUICurves* thisPtr);
		static bool Internal_pixelToCurveSpace(ScriptGUICurves* thisPtr, Vector2I* pixelCoords, Vector2* curveCoords, bool padding);
		static void Internal_curveToPixelSpace(ScriptGUICurves* thisPtr, Vector2* curveCoords, Vector2I* __output);
		static uint32_t Internal_findCurve(ScriptGUICurves* thisPtr, Vector2I* pixelCoords);
		static bool Internal_findKeyFrame(ScriptGUICurves* thisPtr, Vector2I* pixelCoords, KeyframeRef* keyframe);
		static bool Internal_findTangent(ScriptGUICurves* thisPtr, Vector2I* pixelCoords, __TangentRefInterop* tangent);
		static void Internal_selectKeyframe(ScriptGUICurves* thisPtr, KeyframeRef* keyframeRef, TangentModeBits tangentMode, bool selected);
		static void Internal_clearSelectedKeyframes(ScriptGUICurves* thisPtr);
		static void Internal_create(MonoObject* managedInstance, MonoString* styleName);
		static void Internal_create0(MonoObject* managedInstance, CurveDrawOption drawOptions, MonoString* styleName);
	};
}
