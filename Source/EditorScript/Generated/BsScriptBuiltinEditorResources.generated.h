//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../bsf/Source/Foundation/bsfEngine/GUI/BsGUIContent.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"

namespace bs { class BuiltinEditorResources; }
namespace bs { struct __GUIContentImagesInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptBuiltinEditorResources : public ScriptObject<ScriptBuiltinEditorResources>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "EditorBuiltin")

		ScriptBuiltinEditorResources(MonoObject* managedInstance);

	private:
		static MonoObject* Internal_getSkin();
		static MonoObject* Internal_getDefaultFont();
		static MonoObject* Internal_getDefaultAAFont();
		static MonoObject* Internal_getProjectLibraryIcon(ProjectLibraryIcon icon, int32_t size);
		static MonoObject* Internal_getToolbarIcon(ToolbarIcon icon);
		static void Internal_getSceneWindowIcon(SceneWindowIcon icon, __GUIContentImagesInterop* __output);
		static MonoObject* Internal_getSceneViewIcon(SceneViewIcon icon);
		static MonoObject* Internal_getLibraryWindowIcon(LibraryWindowIcon icon);
		static MonoObject* Internal_getInspectorWindowIcon(InspectorWindowIcon icon);
		static void Internal_getAnimationWindowIcon(AnimationWindowIcon icon, __GUIContentImagesInterop* __output);
		static MonoObject* Internal_getEditorIcon(EditorIcon icon);
		static void Internal_getEditorToggleIcon(EditorToggleIcon icon, __GUIContentImagesInterop* __output);
		static MonoObject* Internal_getLogMessageIcon(LogMessageIcon icon, uint32_t size, bool dark);
		static MonoObject* Internal_getSprite(EditorSprites sprite);
		static MonoString* Internal_getEmptyShaderCode();
		static MonoString* Internal_getEmptyCSScriptCode();
	};
}
