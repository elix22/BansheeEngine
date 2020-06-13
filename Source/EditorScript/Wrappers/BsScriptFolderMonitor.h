//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "Platform/BsFolderMonitor.h"
#include "Utility/BsModule.h"

namespace bs
{
	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/**	Interop class between C++ & CLR for FolderMonitor. */
	class BS_SCR_BED_EXPORT ScriptFolderMonitor : public ScriptObject <ScriptFolderMonitor>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "FolderMonitor")

	private:
		friend class ScriptFolderMonitorManager;

		ScriptFolderMonitor(MonoObject* instance, FolderMonitor* monitor);
		~ScriptFolderMonitor();

		/**	Updates the native folder monitor. Must be called once per frame. */
		void update();

		/**	Destroys the native folder monitor. */
		void destroy();

		/**	Triggered when the native folder monitor detects a file has been modified. */
		void onMonitorFileModified(const Path& path);

		/**	Triggered when the native folder monitor detects a file has been added. */
		void onMonitorFileAdded(const Path& path);

		/**	Triggered when the native folder monitor detects a file has been removed. */
		void onMonitorFileRemoved(const Path& path);

		/**	Triggered when the native folder monitor detects a file has been renamed. */
		void onMonitorFileRenamed(const Path& from, const Path& to);

		FolderMonitor* mMonitor = nullptr;
		UINT32 mGCHandle = 0;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_CreateInstance(MonoObject* instance, MonoString* folder);
		static void internal_Destroy(ScriptFolderMonitor* thisPtr);

		typedef void(BS_THUNKCALL *OnModifiedThunkDef) (MonoObject*, MonoString*, MonoException**);
		typedef void(BS_THUNKCALL *OnRenamedThunkDef) (MonoObject*, MonoString*, MonoString*, MonoException**);

		static OnModifiedThunkDef OnModifiedThunk;
		static OnModifiedThunkDef OnAddedThunk;
		static OnModifiedThunkDef OnRemovedThunk;
		static OnRenamedThunkDef OnRenamedThunk;
	};

	/** @} */
	/** @addtogroup EditorScript
	 *  @{
	 */

	/**	Manages all active managed folder monitor objects. */
	class BS_SCR_BED_EXPORT ScriptFolderMonitorManager : public Module<ScriptFolderMonitorManager>
	{
	public:
		/**	Triggers updates on all active folder monitor objects. Should be called once per frame. */
		void update();

	private:
		friend class ScriptFolderMonitor;

		/**	Registers a new managed folder monitor. */
		void _registerMonitor(ScriptFolderMonitor* monitor);

		/**	Unregisters a destroyed managed folder monitor. */
		void _unregisterMonitor(ScriptFolderMonitor* monitor);

		UnorderedSet<ScriptFolderMonitor*> mMonitors;
	};

	/** @} */
}