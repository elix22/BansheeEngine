//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "UndoRedo/BsCmdDeleteSO.h"
#include "UndoRedo/BsUndoRedo.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsSerializedSceneObject.h"
#include "Scene/BsSelection.h"

namespace bs
{
	CmdDeleteSO::CmdDeleteSO(const String& description, const HSceneObject& sceneObject)
		: EditorCommand(description), mSceneObject(sceneObject)
	{ }

	void CmdDeleteSO::execute(const HSceneObject& sceneObject, const String& description)
	{
		// Register command and commit it
		CmdDeleteSO* command = new (bs_alloc<CmdDeleteSO>()) CmdDeleteSO(description, sceneObject);
		SPtr<CmdDeleteSO> commandPtr = bs_shared_ptr(command);

		UndoRedo::instance().registerCommand(commandPtr);
		commandPtr->commit();
	}

	void CmdDeleteSO::commit()
	{
		if (mSceneObject == nullptr || mSceneObject.isDestroyed())
			return;

		mSerialized = bs_shared_ptr_new<SerializedSceneObject>(mSceneObject, true);
		mSceneObject->destroy();
	}

	void CmdDeleteSO::revert()
	{
		mSerialized->restore();

		if(!mSceneObject.isDestroyed(true))
			Selection::instance().setSceneObjects({ mSceneObject });
	}
}
