//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "UndoRedo/BsEditorCommand.h"
#include "Utility/BsEditorUtility.h"

namespace bs
{
	class SerializedSceneObject;

	/** @addtogroup UndoRedo
	 *  @{
	 */

	/** A command used for undo/redo purposes. Deletes a scene object and restores it as an undo operation. */
	class BS_ED_EXPORT CmdDeleteSO final : public EditorCommand
	{
	public:
		/**
		 * Creates and executes the command on the provided scene object. Automatically registers the command with 
		 * undo/redo system.
		 *
		 * @param[in]	sceneObject	Scene object to delete.
		 * @param[in]	description	Optional description of what exactly the command does.
		 */
		static void execute(const HSceneObject& sceneObject, const String& description = StringUtil::BLANK);

		/** @copydoc EditorCommand::commit */
		void commit() override;

		/** @copydoc EditorCommand::revert */
		void revert() override;

	private:
		friend class UndoRedo;

		CmdDeleteSO(const String& description, const HSceneObject& sceneObject);

		HSceneObject mSceneObject;
		SPtr<SerializedSceneObject> mSerialized;
	};

	/** @} */
}
