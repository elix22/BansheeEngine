//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "../../bsf/Source/Foundation/bsfCore/Localization/BsHString.h"
#include "Math/BsVector4.h"
#include "../../bsf/Source/Foundation/bsfCore/Utility/BsCommonTypes.h"

namespace bs { class GUIVector4Field; }
namespace bs { struct __GUIContentInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGUIVector4Field : public TScriptGUIElement<ScriptGUIVector4Field>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIVector4Field")

		ScriptGUIVector4Field(MonoObject* managedInstance, GUIVector4Field* value);

	private:
		void onValueChanged(const Vector4& p0);
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

		static void Internal_getValue(ScriptGUIVector4Field* thisPtr, Vector4* __output);
		static void Internal_setValue(ScriptGUIVector4Field* thisPtr, Vector4* value);
		static bool Internal_hasInputFocus(ScriptGUIVector4Field* thisPtr);
		static void Internal_setInputFocus(ScriptGUIVector4Field* thisPtr, VectorComponent component, bool focus);
		static void Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style);
		static void Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style);
		static void Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style);
		static void Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style);
		static void Internal_create3(MonoObject* managedInstance, MonoString* style);
	};
}
