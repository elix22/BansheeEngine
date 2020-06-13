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

	/** A command used for undo/redo purposes. Creates a scene object and removes it as an undo operation. */
	class BS_ED_EXPORT CmdCreateSO final : public EditorCommand
	{
	public:
		/**
		 * Creates a new scene object. Automatically registers the command with undo/redo system.
		 *
		 * @param[in]	name		Name of the scene object.
		 * @param[in]	flags		Optional creation flags for the scene object.
		 * @param[in]	description	Optional description of what exactly the command does.
		 * @return					Newly created scene object.
		 */
		static HSceneObject execute(const String& name, UINT32 flags, const String& description = StringUtil::BLANK);

		/**
		 * Creates a new scene object with an initial set of components. Automatically registers the command with undo/redo 
		 * system.
		 *
		 * @param[in]	name				Name of the scene object.
		 * @param[in]	flags				Optional creation flags for the scene object.
		 * @param[in]	componentTypeIds	A list of type ID's of the components to add to the scene object.
		 * @param[in]	description			Optional description of what exactly the command does.
		 * @return							Newly created scene object.
		 */
		static HSceneObject execute(const String& name, UINT32 flags, const Vector<UINT32>& componentTypeIds, 
			const String& description = StringUtil::BLANK);

		/** @copydoc EditorCommand::commit */
		void commit() override;

		/** @copydoc EditorCommand::revert */
		void revert() override;

	private:
		friend class UndoRedo;

		CmdCreateSO(const String& description, const String& name, UINT32 flags);
		CmdCreateSO(const String& description, const String& name, const Vector<UINT32>& componentTypeIds, UINT32 flags);

		String mName;
		UINT32 mFlags;

		HSceneObject mSceneObject;
		Vector<UINT32> mComponentTypeIds;
	};

	/** @} */
}