//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "UndoRedo/BsEditorCommand.h"
#include "UndoRedo/BsUndoRedo.h"

namespace bs
{
	/** @addtogroup UndoRedo
	 *  @{
	 */

	/**
	 * A command used for undo/redo purposes. It records a scene object name change operations. It allows you to apply
	 * the name change or revert the object to its original name as needed.
	 */
	class BS_ED_EXPORT CmdRenameSO final : public EditorCommand
	{
	public:
		/**
		 * Creates and executes the command on the provided scene object(s). Automatically registers the command with
		 * undo/redo system.
		 *
		 * @param[in]	sceneObject		Object to rename.
		 * @param[in]	newName			New name for the provided object.
		 */
		static void execute(const HSceneObject& sceneObjects, const String& newName);

		/** @copydoc EditorCommand::commit */
		void commit() override;

		/** @copydoc EditorCommand::revert */
		void revert() override;

	private:
		friend class UndoRedo;

		CmdRenameSO(const String& description, const HSceneObject& sceneObject, const String& newName);

		HSceneObject mSceneObject;
		String mOldName;
		String mNewName;
	};

	/** @} */
}