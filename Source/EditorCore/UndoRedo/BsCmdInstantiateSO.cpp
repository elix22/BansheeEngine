//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "UndoRedo/BsCmdInstantiateSO.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsPrefab.h"

namespace bs
{
	CmdInstantiateSO::CmdInstantiateSO(const String& description, const HPrefab& prefab)
		:EditorCommand(description), mPrefab(prefab)
	{ }

	HSceneObject CmdInstantiateSO::execute(const HPrefab& prefab, const String& description)
	{
		// Register command and commit it
		CmdInstantiateSO* command = new (bs_alloc<CmdInstantiateSO>()) CmdInstantiateSO(description, prefab);
		SPtr<CmdInstantiateSO> commandPtr = bs_shared_ptr(command);

		UndoRedo::instance().registerCommand(commandPtr);
		commandPtr->commit();

		return commandPtr->mSceneObject;
	}

	void CmdInstantiateSO::commit()
	{
		mSceneObject = mPrefab->instantiate();
	}

	void CmdInstantiateSO::revert()
	{
		if (!mSceneObject.isDestroyed())
			mSceneObject->destroy(true);

		mSceneObject = nullptr;
	}
}