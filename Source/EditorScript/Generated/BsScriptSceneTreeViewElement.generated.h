//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "../../EditorCore/GUI/BsGUISceneTreeView.h"

namespace bs
{
	struct __SceneTreeViewElementInterop
	{
		MonoObject* sceneObject;
		bool isExpanded;
	};

	class BS_SCR_BED_EXPORT ScriptSceneTreeViewElement : public ScriptObject<ScriptSceneTreeViewElement>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "SceneTreeViewElement")

		static MonoObject* box(const __SceneTreeViewElementInterop& value);
		static __SceneTreeViewElementInterop unbox(MonoObject* value);
		static SceneTreeViewElement fromInterop(const __SceneTreeViewElementInterop& value);
		static __SceneTreeViewElementInterop toInterop(const SceneTreeViewElement& value);

	private:
		ScriptSceneTreeViewElement(MonoObject* managedInstance);

	};
}
