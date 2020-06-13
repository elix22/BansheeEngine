//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "../../EditorCore/GUI/BsGUISceneTreeView.h"

namespace bs { struct SceneTreeViewState; }
namespace bs { struct __SceneTreeViewElementInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptSceneTreeViewState : public ScriptObject<ScriptSceneTreeViewState>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "SceneTreeViewState")

		ScriptSceneTreeViewState(MonoObject* managedInstance, const SPtr<SceneTreeViewState>& value);

		SPtr<SceneTreeViewState> getInternal() const { return mInternal; }
		static MonoObject* create(const SPtr<SceneTreeViewState>& value);

	private:
		SPtr<SceneTreeViewState> mInternal;

		static MonoArray* Internal_getelements(ScriptSceneTreeViewState* thisPtr);
		static void Internal_setelements(ScriptSceneTreeViewState* thisPtr, MonoArray* value);
	};
}
