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

	/**	Interop class between C++ & CLR for GUISliderField. */
	class BS_SCR_BED_EXPORT ScriptGUISliderField : public TScriptGUIElement<ScriptGUISliderField>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUISliderField")

	private:
		ScriptGUISliderField(MonoObject* instance, GUISliderField* sliderField);

		/**
		 * Triggered when the value in the native slider field changes.
		 *
		 * @param[in]	newValue	New field value.
		 */
		void onChanged(float newValue);

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_createInstance(MonoObject* instance, float min, float max, __GUIContentInterop* title, 
			UINT32 titleWidth, MonoString* style, MonoArray* guiOptions, bool withTitle);

		static float internal_getValue(ScriptGUISliderField* nativeInstance);
		static float internal_getStep(ScriptGUISliderField* nativeInstance);
		static float internal_setValue(ScriptGUISliderField* nativeInstance, float value);
		static void internal_hasInputFocus(ScriptGUISliderField* nativeInstance, bool* output);
		static void internal_setTint(ScriptGUISliderField* nativeInstance, Color* color);
		static void internal_setRange(ScriptGUISliderField* nativeInstance, float min, float max);
		static void internal_setStep(ScriptGUISliderField* nativeInstance, float step);

		typedef void(BS_THUNKCALL *OnChangedThunkDef) (MonoObject*, float, MonoException**);

		static OnChangedThunkDef onChangedThunk;
	};

	/** @} */
}