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

	/**	Interop class between C++ & CLR for GUIToggleField. */
	class BS_SCR_BED_EXPORT ScriptGUIToggleField : public TScriptGUIElement<ScriptGUIToggleField>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GUIToggleField")

	private:
		/**
		 * Triggered when the value in the native toggle field changes.
		 *
		 * @param[in]	newValue	Is the toggle active.
		 */
		void onChanged(bool newValue);

		ScriptGUIToggleField(MonoObject* instance, GUIToggleField* toggleField);

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_createInstance(MonoObject* instance, __GUIContentInterop* title, UINT32 titleWidth,
			MonoString* style, MonoArray* guiOptions, bool withTitle);

		static void internal_getValue(ScriptGUIToggleField* nativeInstance, bool* output);
		static void internal_setValue(ScriptGUIToggleField* nativeInstance, bool value);
		static void internal_setTint(ScriptGUIToggleField* nativeInstance, Color* color);

		typedef void(BS_THUNKCALL *OnChangedThunkDef) (MonoObject*, bool, MonoException**);

		static OnChangedThunkDef onChangedThunk;
	};

	/** @} */
}