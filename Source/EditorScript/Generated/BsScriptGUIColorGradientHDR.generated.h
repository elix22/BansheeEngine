//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "../../bsf/Source/Foundation/bsfUtility/Image/BsColorGradient.h"

namespace bs { class GUIColorGradientHDR; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUIColorGradientHDR : public TScriptGUIElement<ScriptGUIColorGradientHDR>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIColorGradientHDR")

		ScriptGUIColorGradientHDR(MonoObject* managedInstance, GUIColorGradientHDR* value);

	private:
		void onClicked();

		typedef void(BS_THUNKCALL *onClickedThunkDef) (MonoObject*, MonoException**);
		static onClickedThunkDef onClickedThunk;

		static void Internal_setGradient(ScriptGUIColorGradientHDR* thisPtr, MonoObject* colorGradient);
		static MonoObject* Internal_getGradient(ScriptGUIColorGradientHDR* thisPtr);
		static void Internal_create(MonoObject* managedInstance, MonoString* styleName);
	};
}
