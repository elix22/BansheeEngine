//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "Image/BsColor.h"
#include "../../bsf/Source/Foundation/bsfCore/Localization/BsHString.h"

namespace bs { class GUIColorField; }
namespace bs { struct __GUIContentInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUIColorField : public TScriptGUIElement<ScriptGUIColorField>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIColorField")

		ScriptGUIColorField(MonoObject* managedInstance, GUIColorField* value);

	private:
		void onClicked();

		typedef void(BS_THUNKCALL *onClickedThunkDef) (MonoObject*, MonoException**);
		static onClickedThunkDef onClickedThunk;

		static void Internal_getValue(ScriptGUIColorField* thisPtr, Color* __output);
		static void Internal_setValue(ScriptGUIColorField* thisPtr, Color* value);
		static bool Internal_getAllowHDR(ScriptGUIColorField* thisPtr);
		static void Internal_setAllowHDR(ScriptGUIColorField* thisPtr, bool allow);
		static void Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style);
		static void Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style);
		static void Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style);
		static void Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style);
		static void Internal_create3(MonoObject* managedInstance, MonoString* style);
	};
}
