//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "../../bsf/Source/Foundation/bsfCore/Localization/BsHString.h"
#include "../../bsf/Source/Foundation/bsfUtility/Image/BsColorGradient.h"

namespace bs { class GUIColorGradientHDRField; }
namespace bs { struct __GUIContentInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUIColorGradientHDRField : public TScriptGUIElement<ScriptGUIColorGradientHDRField>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIColorGradientHDRField")

		ScriptGUIColorGradientHDRField(MonoObject* managedInstance, GUIColorGradientHDRField* value);

	private:
		void onClicked();

		typedef void(BS_THUNKCALL *onClickedThunkDef) (MonoObject*, MonoException**);
		static onClickedThunkDef onClickedThunk;

		static MonoObject* Internal_getValue(ScriptGUIColorGradientHDRField* thisPtr);
		static void Internal_setValue(ScriptGUIColorGradientHDRField* thisPtr, MonoObject* value);
		static void Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style);
		static void Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style);
		static void Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style);
		static void Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style);
		static void Internal_create3(MonoObject* managedInstance, MonoString* style);
	};
}
