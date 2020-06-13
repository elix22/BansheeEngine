//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "UndoRedo/BsEditorCommand.h"
#include "UndoRedo/BsUndoRedo.h"

namespace bs
{
	/** @addtogroup UndoRedo
	 *  @{
	 */

	/** A command used for undo/redo purposes. Clones scene object(s) and removes them as an undo operation. */
	class BS_ED_EXPORT CmdCloneSO final : public EditorCommand
	{
	public:
		/**
		 * Creates a new scene object by cloning an existing object. Automatically registers the command with undo/redo 
		 * system.
		 *
		 * @param[in]	sceneObject		Scene object to clone.
		 * @param[in]	description		Optional description of what exactly the command does.
		 * @return						Cloned object.
		 */
		static HSceneObject execute(const HSceneObject& sceneObject, const String& description = StringUtil::BLANK);

		/**
		 * Creates new scene object(s) by cloning existing objects. Automatically registers the command with undo/redo 
		 * system.
		 *
		 * @param[in]	sceneObjects	Scene object(s) to clone.
		 * @param[in]	description		Optional description of what exactly the command does.
		 * @return						Cloned objects.
		 */
		static Vector<HSceneObject> execute(const Vector<HSceneObject>& sceneObjects, const String& description = StringUtil::BLANK);

		/** @copydoc EditorCommand::commit */
		void commit() override;

		/** @copydoc EditorCommand::revert */
		void revert() override;

	private:
		friend class UndoRedo;

		CmdCloneSO(const String& description, const Vector<HSceneObject>& originals);

		Vector<HSceneObject> mOriginals;
		Vector<HSceneObject> mClones;
	};

	/** @} */
}