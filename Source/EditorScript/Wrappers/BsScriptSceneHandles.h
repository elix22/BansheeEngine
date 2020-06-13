//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "Math/BsVector2I.h"

namespace bs
{
	class ScriptCCamera;

	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/** Interop class between C++ & CLR for SceneHandles. */
	class BS_SCR_BED_EXPORT ScriptSceneHandles : public ScriptObject <ScriptSceneHandles>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "SceneHandles")

	private:
		ScriptSceneHandles(MonoObject* object, EditorWidgetBase* parentWidget, const HCamera& camera);
		~ScriptSceneHandles();

		/**
		 * Checks is the pointer currently within the provided window, and if it is not the cursor is wrapped in such a way
		 * so that it is returned to within the window bounds.
		 *		
		 * @return	How far was the cursor moved due to wrapping. This will be (0, 0) if the cursor is within window bounds
		 *			initially.
		 */
		Vector2I wrapCursorToWindow() const;

		EditorWidgetBase* mParentWidget;
		HCamera mCamera;
		Vector2I mMouseDeltaCompensate;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_Create(MonoObject* managedInstance, ScriptEditorWindow* parentWindow, ScriptCCamera* camera);
		static void internal_Draw(ScriptSceneHandles* thisPtr);
		static void internal_BeginInput();
		static void internal_EndInput();
		static bool internal_UpdateInput(ScriptSceneHandles* thisPtr, Vector2I* inputPos, Vector2I* inputDelta);
		static bool internal_TrySelect(ScriptSceneHandles* thisPtr, Vector2I* inputPos);
		static bool internal_IsActive(ScriptSceneHandles* thisPtr);
		static void internal_ClearSelection(ScriptSceneHandles* thisPtr);
	};

	/** @} */
}