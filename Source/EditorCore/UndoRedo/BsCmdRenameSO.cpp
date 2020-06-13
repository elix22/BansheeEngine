//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "UndoRedo/BsCmdRenameSO.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
	CmdRenameSO::CmdRenameSO(const String& description, const HSceneObject& sceneObject, const String& newName)
		:EditorCommand(description), mSceneObject(sceneObject), mNewName(newName)
	{
		if(!sceneObject.isDestroyed())
			mOldName = sceneObject->getName();
	}

	void CmdRenameSO::execute(const HSceneObject& sceneObject, const String& newName)
	{
		String oldName;
		if(!sceneObject.isDestroyed())
			oldName = sceneObject->getName();

		// Register command and commit it
		CmdRenameSO* command = new (bs_alloc<CmdRenameSO>()) 
			CmdRenameSO(StringUtil::format("Rename scene object '{0}' to '{1}'", oldName, newName), sceneObject, newName);
		SPtr<CmdRenameSO> commandPtr = bs_shared_ptr(command);

		UndoRedo::instance().registerCommand(commandPtr);
		commandPtr->commit();
	}

	void CmdRenameSO::commit()
	{
		if (!mSceneObject.isDestroyed())
			mSceneObject->setName(mNewName);
	}

	void CmdRenameSO::revert()
	{
		if (!mSceneObject.isDestroyed())
			mSceneObject->setName(mOldName);
	}
}