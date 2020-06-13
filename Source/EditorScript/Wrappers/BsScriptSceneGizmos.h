//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "Scene/BsGizmoManager.h"

namespace bs
{
	class ScriptCCamera;

	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/** Interop class between C++ & CLR for SceneGizmos. */
	class BS_SCR_BED_EXPORT ScriptSceneGizmos : public ScriptObject <ScriptSceneGizmos>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "SceneGizmos")

	private:
		ScriptSceneGizmos(MonoObject* object, const HCamera& camera, const GizmoDrawSettings& drawSettings);
		~ScriptSceneGizmos();

		HCamera mCamera;
		GizmoDrawSettings mDrawSettings;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_Create(MonoObject* managedInstance, ScriptCCamera* camera, GizmoDrawSettings* drawSettings);
		static void internal_Draw(ScriptSceneGizmos* thisPtr);
		static void internal_SetDrawSettings(ScriptSceneGizmos* thisPtr, GizmoDrawSettings* settings);
		static void internal_GetDrawSettings(ScriptSceneGizmos* thisPtr, GizmoDrawSettings* settings);
	};

	/** @} */
}