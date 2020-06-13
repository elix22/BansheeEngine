//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "Math/BsVector3.h"
#include "../../bsf/Source/Foundation/bsfCore/Utility/BsCommonTypes.h"
#include "../../bsf/Source/Foundation/bsfCore/Localization/BsHString.h"

namespace bs { class GUIVector3Field; }
namespace bs { struct __GUIContentInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUIVector3Field : public TScriptGUIElement<ScriptGUIVector3Field>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIVector3Field")

		ScriptGUIVector3Field(MonoObject* managedInstance, GUIVector3Field* value);

	private:
		void onValueChanged(const Vector3& p0);
		void onComponentChanged(float p0, VectorComponent p1);
		void onComponentFocusChanged(bool p0, VectorComponent p1);
		void onConfirm(VectorComponent p0);

		typedef void(BS_THUNKCALL *onValueChangedThunkDef) (MonoObject*, MonoObject* p0, MonoException**);
		static onValueChangedThunkDef onValueChangedThunk;
		typedef void(BS_THUNKCALL *onComponentChangedThunkDef) (MonoObject*, float p0, VectorComponent p1, MonoException**);
		static onComponentChangedThunkDef onComponentChangedThunk;
		typedef void(BS_THUNKCALL *onComponentFocusChangedThunkDef) (MonoObject*, bool p0, VectorComponent p1, MonoException**);
		static onComponentFocusChangedThunkDef onComponentFocusChangedThunk;
		typedef void(BS_THUNKCALL *onConfirmThunkDef) (MonoObject*, VectorComponent p0, MonoException**);
		static onConfirmThunkDef onConfirmThunk;

		static void Internal_getValue(ScriptGUIVector3Field* thisPtr, Vector3* __output);
		static void Internal_setValue(ScriptGUIVector3Field* thisPtr, Vector3* value);
		static bool Internal_hasInputFocus(ScriptGUIVector3Field* thisPtr);
		static void Internal_setInputFocus(ScriptGUIVector3Field* thisPtr, VectorComponent component, bool focus);
		static void Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style);
		static void Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style);
		static void Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style);
		static void Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style);
		static void Internal_create3(MonoObject* managedInstance, MonoString* style);
	};
}
