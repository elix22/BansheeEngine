//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "../../bsf/Source/Scripting/bsfScript/BsPlayInEditor.h"

namespace bs { class PlayInEditor; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptPlayInEditor : public ScriptObject<ScriptPlayInEditor>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "PlayInEditor")

		ScriptPlayInEditor(MonoObject* managedInstance);

		static void startUp();
		static void shutDown();

	private:
		uint32_t mGCHandle = 0;

		static void onPlay();
		static void onStopped();
		static void onPaused();
		static void onUnpaused();

		typedef void(BS_THUNKCALL *onPlayThunkDef) (MonoException**);
		static onPlayThunkDef onPlayThunk;
		typedef void(BS_THUNKCALL *onStoppedThunkDef) (MonoException**);
		static onStoppedThunkDef onStoppedThunk;
		typedef void(BS_THUNKCALL *onPausedThunkDef) (MonoException**);
		static onPausedThunkDef onPausedThunk;
		typedef void(BS_THUNKCALL *onUnpausedThunkDef) (MonoException**);
		static onUnpausedThunkDef onUnpausedThunk;

		static HEvent onPlayConn;
		static HEvent onStoppedConn;
		static HEvent onPausedConn;
		static HEvent onUnpausedConn;

		static PlayInEditorState Internal_getState();
		static void Internal_setState(PlayInEditorState state);
		static void Internal_frameStep();
	};
}
