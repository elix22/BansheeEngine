//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "Scene/BsScenePicking.h"
#include "Scene/BsGizmoManager.h"

namespace bs
{
	class ScriptCCamera;

	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/** Interop class between C++ & CLR for SceneSelection. */
	class BS_SCR_BED_EXPORT ScriptSceneSelection : public ScriptObject <ScriptSceneSelection>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "SceneSelection")

	private:
		ScriptSceneSelection(MonoObject* object, const SPtr<Camera>& camera, const GizmoDrawSettings& gizmoDrawSettings);
		~ScriptSceneSelection();

		SPtr<Camera> mCamera;
		SelectionRenderer* mSelectionRenderer = nullptr;
		GizmoDrawSettings mGizmoDrawSettings;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_Create(MonoObject* managedInstance, ScriptCCamera* camera, GizmoDrawSettings* gizmoDrawSettings);
		static void internal_Draw(ScriptSceneSelection* thisPtr);
		static void internal_PickObject(ScriptSceneSelection* thisPtr, Vector2I* inputPos, bool additive, MonoArray* ignoreRenderables);
		static void internal_PickObjects(ScriptSceneSelection* thisPtr, Vector2I* inputPos, Vector2I* area, bool additive, MonoArray* ignoreRenderables);
		static MonoObject* internal_Snap(ScriptSceneSelection* thisPtr, Vector2I* inputPos, SnapData* data, MonoArray* ignoreRenderables);
		static void internal_SetGizmoDrawSettings(ScriptSceneSelection* thisPtr, GizmoDrawSettings* gizmoDrawSettings);
		static void internal_GetGizmoDrawSettings(ScriptSceneSelection* thisPtr, GizmoDrawSettings* gizmoDrawSettings);
	};

	/** @} */
}