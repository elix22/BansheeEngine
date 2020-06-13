//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "../../bsf/Source/Foundation/bsfUtility/Image/BsColorGradient.h"

namespace bs { class GUIColorGradient; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUIColorGradient : public TScriptGUIElement<ScriptGUIColorGradient>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIColorGradient")

		ScriptGUIColorGradient(MonoObject* managedInstance, GUIColorGradient* value);

	private:
		void onClicked();

		typedef void(BS_THUNKCALL *onClickedThunkDef) (MonoObject*, MonoException**);
		static onClickedThunkDef onClickedThunk;

		static void Internal_setGradient(ScriptGUIColorGradient* thisPtr, MonoObject* colorGradient);
		static MonoObject* Internal_getGradient(ScriptGUIColorGradient* thisPtr);
		static void Internal_create(MonoObject* managedInstance, MonoString* styleName);
	};
}
