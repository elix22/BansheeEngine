//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "Input/BsInputFwd.h"

namespace bs
{
	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/**	Interop class between C++ & CLR for Editor. */
	class BS_SCR_BED_EXPORT ScriptEditorInput : public ScriptObject<ScriptEditorInput>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "EditorInput")

		/**	Registers internal callbacks. Must be called on scripting system load. */
		static void startUp();

		/**	Unregisters internal callbacks. Must be called on scripting system shutdown. */
		static void shutDown();
	private:
		ScriptEditorInput(MonoObject* instance);

		/**	Triggered when the specified button is pressed. */
		static void onButtonDown(const ButtonEvent& ev);

		/**	Triggered when the specified button is released. */
		static void onButtonUp(const ButtonEvent& ev);

		/**	Triggered when the specified character is entered. */
		static void onCharInput(const TextInputEvent& ev);

		/**	Triggered when the pointer is moved. */
		static void onPointerMoved(const PointerEvent& ev);

		/**	Triggered when a pointer button is pressed. */
		static void onPointerPressed(const PointerEvent& ev);

		/**	Triggered when a pointer button is released. */
		static void onPointerReleased(const PointerEvent& ev);

		/**	Triggered when a pointer button is double-clicked. */
		static void onPointerDoubleClick(const PointerEvent& ev);

		static HEvent OnButtonPressedConn;
		static HEvent OnButtonReleasedConn;
		static HEvent OnCharInputConn;
		static HEvent OnPointerPressedConn;
		static HEvent OnPointerReleasedConn;
		static HEvent OnPointerMovedConn;
		static HEvent OnPointerDoubleClickConn;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/

		typedef void(BS_THUNKCALL *OnButtonEventThunkDef) (ButtonCode, UINT32, bool, MonoException**);
		typedef void(BS_THUNKCALL *OnCharInputEventThunkDef) (UINT32, bool, MonoException**);
		typedef void(BS_THUNKCALL *OnPointerEventThunkDef) (MonoObject*, MonoObject*, PointerEventButton,
			bool, bool, bool, float, bool, MonoException**);

		static OnButtonEventThunkDef OnButtonPressedThunk;
		static OnButtonEventThunkDef OnButtonReleasedThunk;
		static OnCharInputEventThunkDef OnCharInputThunk;
		static OnPointerEventThunkDef OnPointerPressedThunk;
		static OnPointerEventThunkDef OnPointerReleasedThunk;
		static OnPointerEventThunkDef OnPointerMovedThunk;
		static OnPointerEventThunkDef OnPointerDoubleClickThunk;
	};

	/** @} */
}