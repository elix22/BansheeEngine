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

	/** A command used for undo/redo purposes. It breaks a prefab link of a scene object and allows you to restore link. */
	class BS_ED_EXPORT CmdBreakPrefab final : public EditorCommand
	{
	public:
		~CmdBreakPrefab();

		/**
		 * Creates and executes the command on the provided scene object. Automatically registers the command with undo/redo
		 * system.
		 *
		 * @param[in]	sceneObject		Scene object whose prefab link to break.
		 * @param[in]	description		Optional description of what exactly the command does.
		 */
		static void execute(const HSceneObject& sceneObject, const String& description = StringUtil::BLANK);

		/** @copydoc EditorCommand::commit */
		void commit() override;

		/** @copydoc EditorCommand::revert */
		void revert() override;

	private:
		friend class UndoRedo;

		CmdBreakPrefab(const String& description, const HSceneObject& sceneObject);

		/**	Clears all internal cached data. Should be called whenever a change is commited. */
		void clear();

		HSceneObject mSceneObject;

		HSceneObject mPrefabRoot;
		UUID mPrefabLinkUUID;
		SPtr<PrefabDiff> mPrefabDiff;
		UnorderedMap<UINT64, UINT32> mLinkIds;
	};

	/** @} */
}