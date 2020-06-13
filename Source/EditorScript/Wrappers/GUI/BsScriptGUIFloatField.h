//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"

namespace bs
{
	struct __GUIContentInterop;

	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/**	Interop class between C++ & CLR for GUIFloatField. */
	class BS_SCR_BED_EXPORT ScriptGUIFloatField : public TScriptGUIElement<ScriptGUIFloatField>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIFloatField")

	private:
		ScriptGUIFloatField(MonoObject* instance, GUIFloatField* floatField);

		/**
		 * Triggered when the value in the native float field changes.
		 *
		 * @param[in]	newValue	New field value.
		 */
		void onChanged(float newValue);

		/**
		 * Triggered when the user confirms input in the native float field.
		 */
		void onConfirmed();

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_createInstance(MonoObject* instance, __GUIContentInterop* title, UINT32 titleWidth,
			MonoString* style, MonoArray* guiOptions, bool withTitle);

		static float internal_getValue(ScriptGUIFloatField* nativeInstance);
		static float internal_setValue(ScriptGUIFloatField* nativeInstance, float value);
		static bool internal_hasInputFocus(ScriptGUIFloatField* nativeInstance);
		static void internal_setTint(ScriptGUIFloatField* nativeInstance, Color* color);
		static void internal_setRange(ScriptGUIFloatField* nativeInstance, float min, float max);
		static void internal_setStep(ScriptGUIFloatField* nativeInstance, float step);
		static float internal_getStep(ScriptGUIFloatField* nativeInstance);

		typedef void(BS_THUNKCALL *OnChangedThunkDef) (MonoObject*, float, MonoException**);
		typedef void(BS_THUNKCALL *OnConfirmedThunkDef) (MonoObject*, MonoException**);

		static OnChangedThunkDef onChangedThunk;
		static OnConfirmedThunkDef onConfirmedThunk;
	};

	/** @} */
}