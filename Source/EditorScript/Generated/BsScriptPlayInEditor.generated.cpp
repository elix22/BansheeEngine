//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptPlayInEditor.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../bsf/Source/Scripting/bsfScript/BsPlayInEditor.h"

namespace bs
{
	ScriptPlayInEditor::onPlayThunkDef ScriptPlayInEditor::onPlayThunk; 
	ScriptPlayInEditor::onStoppedThunkDef ScriptPlayInEditor::onStoppedThunk; 
	ScriptPlayInEditor::onPausedThunkDef ScriptPlayInEditor::onPausedThunk; 
	ScriptPlayInEditor::onUnpausedThunkDef ScriptPlayInEditor::onUnpausedThunk; 

	HEvent ScriptPlayInEditor::onPlayConn;
	HEvent ScriptPlayInEditor::onStoppedConn;
	HEvent ScriptPlayInEditor::onPausedConn;
	HEvent ScriptPlayInEditor::onUnpausedConn;

	ScriptPlayInEditor::ScriptPlayInEditor(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{
		mGCHandle = MonoUtil::newWeakGCHandle(managedInstance);
	}

	void ScriptPlayInEditor::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getState", (void*)&ScriptPlayInEditor::Internal_getState);
		metaData.scriptClass->addInternalCall("Internal_setState", (void*)&ScriptPlayInEditor::Internal_setState);
		metaData.scriptClass->addInternalCall("Internal_frameStep", (void*)&ScriptPlayInEditor::Internal_frameStep);

		onPlayThunk = (onPlayThunkDef)metaData.scriptClass->getMethodExact("Internal_onPlay", "")->getThunk();
		onStoppedThunk = (onStoppedThunkDef)metaData.scriptClass->getMethodExact("Internal_onStopped", "")->getThunk();
		onPausedThunk = (onPausedThunkDef)metaData.scriptClass->getMethodExact("Internal_onPaused", "")->getThunk();
		onUnpausedThunk = (onUnpausedThunkDef)metaData.scriptClass->getMethodExact("Internal_onUnpaused", "")->getThunk();
	}

	void ScriptPlayInEditor::startUp()
	{
		onPlayConn = PlayInEditor::instance().onPlay.connect(&ScriptPlayInEditor::onPlay);
		onStoppedConn = PlayInEditor::instance().onStopped.connect(&ScriptPlayInEditor::onStopped);
		onPausedConn = PlayInEditor::instance().onPaused.connect(&ScriptPlayInEditor::onPaused);
		onUnpausedConn = PlayInEditor::instance().onUnpaused.connect(&ScriptPlayInEditor::onUnpaused);
	}
	void ScriptPlayInEditor::shutDown()
	{
		onPlayConn.disconnect();
		onStoppedConn.disconnect();
		onPausedConn.disconnect();
		onUnpausedConn.disconnect();
	}

	void ScriptPlayInEditor::onPlay()
	{
		MonoUtil::invokeThunk(onPlayThunk);
	}

	void ScriptPlayInEditor::onStopped()
	{
		MonoUtil::invokeThunk(onStoppedThunk);
	}

	void ScriptPlayInEditor::onPaused()
	{
		MonoUtil::invokeThunk(onPausedThunk);
	}

	void ScriptPlayInEditor::onUnpaused()
	{
		MonoUtil::invokeThunk(onUnpausedThunk);
	}
	PlayInEditorState ScriptPlayInEditor::Internal_getState()
	{
		PlayInEditorState tmp__output;
		tmp__output = PlayInEditor::instance().getState();

		PlayInEditorState __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPlayInEditor::Internal_setState(PlayInEditorState state)
	{
		PlayInEditor::instance().setState(state);
	}

	void ScriptPlayInEditor::Internal_frameStep()
	{
		PlayInEditor::instance().frameStep();
	}
}
