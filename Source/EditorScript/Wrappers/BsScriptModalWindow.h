//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "EditorWindow/BsModalWindow.h"
#include "Math/BsVector2I.h"

namespace bs
{
	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	class ManagedModalWindow;

	/**	Interop class between C++ & CLR for ManagedModalWindow. */
	class BS_SCR_BED_EXPORT ScriptModalWindow : public ScriptObject <ScriptModalWindow>
	{
		/**	Contains data about the managed handle to a modal window. */
		struct ModalWindowHandle
		{
			uint32_t gcHandle;
			ManagedModalWindow* nativeObj;
		};
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "ModalWindow")

		~ScriptModalWindow();

	private:
		friend class ManagedModalWindow;

		ScriptModalWindow(ManagedModalWindow* editorWidget);

		/**	Triggered when assembly refresh has started. */
		void onAssemblyRefreshStarted();

		/**	Triggered when the native modal window is closed. */
		void notifyWindowDestroyed();

		ManagedModalWindow* mModalWindow;
		HEvent mOnAssemblyRefreshStartedConn;

		static MonoField* guiPanelField;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_createInstance(MonoObject* instance, bool allowCloseButton);
		static void internal_close(ScriptModalWindow* thisPtr);
		static UINT32 internal_getWidth(ScriptModalWindow* thisPtr);
		static UINT32 internal_getHeight(ScriptModalWindow* thisPtr);
		static void internal_setWidth(ScriptModalWindow* thisPtr, UINT32 value);
		static void internal_setHeight(ScriptModalWindow* thisPtr, UINT32 value);
		static UINT32 internal_getContentWidth(ScriptModalWindow* thisPtr);
		static UINT32 internal_getContentHeight(ScriptModalWindow* thisPtr);
		static void internal_setContentWidth(ScriptModalWindow* thisPtr, UINT32 value);
		static void internal_setContentHeight(ScriptModalWindow* thisPtr, UINT32 value);
		static void internal_setTitle(ScriptModalWindow* thisPtr, MonoObject* title);
		static void internal_screenToWindowPos(ScriptModalWindow* thisPtr, Vector2I* screenPos, Vector2I* windowPos);
		static void internal_windowToScreenPos(ScriptModalWindow* thisPtr, Vector2I* windowPos, Vector2I* screenPos);
	};

	/** @} */
	/** @addtogroup EditorScript
	 *  @{
	 */

	/**
	 * Modal window implementation that handles managed modal window implementations. Each implementation is wrapped in this
	 * object and then managed by its parent interop object of ScriptModalWindow type.
	 */
	class BS_SCR_BED_EXPORT ManagedModalWindow : public ModalWindow
	{
	public:
		ManagedModalWindow(bool allowCloseButton, UINT32 width, UINT32 height, MonoObject* managedInstance);
		~ManagedModalWindow();

		/**
		 * Attempts to create a managed instance for the modal window described by the type provided upon construction.
		 *
		 * @return	True if the managed instance was created.
		 */
		bool createManagedInstance();

		/**
		 * Releases the internally held handle to the managed instance. This will cause managed instance to be destroyed if
		 * no other references are being held.
		 */
		void releaseManagedInstance();

		/**
		 * Sets the parent interop object that handles part of the communication between this object and the managed
		 * instance.
		 */
		void setParent(ScriptModalWindow* parent);

		/** @copydoc ModalWindow::update */
		void update() override;
		
		/**
		 * Loads all required mono methods, fields and types required for operation of this object. Must be called after
		 * construction and after assembly refresh.
		 *
		 * @param[in]	windowClass	Mono class to load the types from.
		 */
		void reloadMonoTypes(MonoClass* windowClass);

		/**	Triggers OnInitialize callbacks on the managed instance. */
		void triggerOnInitialize();

		/**	Triggers OnDestroy callbacks on the managed instance. */
		void triggerOnDestroy();

		/**	Returns the managed instance for the modal window represented by this object. */
		MonoObject* getManagedInstance() const { return mManagedInstance; }
	protected:
		/** @copydoc ModalWindow::resized */
		void resized() override;

		/** @copydoc ModalWindow::close */
		void close() override;

	private:
		friend class ScriptModalWindow;

		typedef void(BS_THUNKCALL *OnInitializeThunkDef) (MonoObject*, MonoException**);
		typedef void(BS_THUNKCALL *OnDestroyThunkDef) (MonoObject*, MonoException**);
		typedef void(BS_THUNKCALL *UpdateThunkDef) (MonoObject*, MonoException**);

		String mNamespace;
		String mTypename;

		OnInitializeThunkDef mOnInitializeThunk = nullptr;
		OnDestroyThunkDef mOnDestroyThunk = nullptr;
		UpdateThunkDef mUpdateThunk = nullptr;
		MonoMethod* mOnWindowResizedMethod = nullptr;

		bool mIsInitialized = false;
		MonoObject* mManagedInstance;
		uint32_t mGCHandle = 0;

		ScriptModalWindow* mScriptParent = nullptr;
		ScriptGUILayout* mContentsPanel = nullptr;
	};

	/** @} */
}