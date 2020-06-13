//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "UndoRedo/BsCmdCreateSO.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsSelection.h"

namespace bs
{
	CmdCreateSO::CmdCreateSO(const String& description, const String& name, UINT32 flags)
		:EditorCommand(description), mName(name), mFlags(flags)
	{ }

	CmdCreateSO::CmdCreateSO(const String& description, const String& name, const Vector<UINT32>& componentTypeIds, 
		UINT32 flags)
		:EditorCommand(description), mName(name), mFlags(flags), mComponentTypeIds(componentTypeIds)
	{ }

	HSceneObject CmdCreateSO::execute(const String& name, UINT32 flags, const String& description)
	{
		// Register command and commit it
		CmdCreateSO* command = new (bs_alloc<CmdCreateSO>()) CmdCreateSO(description, name, flags);
		SPtr<CmdCreateSO> commandPtr = bs_shared_ptr(command);

		UndoRedo::instance().registerCommand(commandPtr);
		commandPtr->commit();

		return commandPtr->mSceneObject;
	}

	HSceneObject CmdCreateSO::execute(const String& name, UINT32 flags, const Vector<UINT32>& componentTypeIds,
		const String& description)
	{
		// Register command and commit it
		CmdCreateSO* command = new (bs_alloc<CmdCreateSO>()) CmdCreateSO(description, name, componentTypeIds, flags);
		SPtr<CmdCreateSO> commandPtr = bs_shared_ptr(command);

		UndoRedo::instance().registerCommand(commandPtr);
		commandPtr->commit();

		return commandPtr->mSceneObject;
	}

	void CmdCreateSO::commit()
	{
		mSceneObject = SceneObject::create(mName, mFlags);

		for(auto entry : mComponentTypeIds)
			mSceneObject->addComponent(entry);

		Selection::instance().setSceneObjects({ mSceneObject });
	}

	void CmdCreateSO::revert()
	{
		if (mSceneObject == nullptr)
			return;

		if (!mSceneObject.isDestroyed())
			mSceneObject->destroy(true);

		mSceneObject = nullptr;
	}
}