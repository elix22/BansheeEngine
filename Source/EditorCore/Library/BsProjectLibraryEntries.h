//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Library/BsProjectLibrary.h"
#include "Reflection/BsIReflectable.h"

namespace bs
{
	/** @addtogroup Library-Internal
	 *  @{
	 */

	/**
	 * Contains a list of entries used by the ProjectLibrary. Used primarily for serialization purposes (persisting
	 * ProjectLibrary state between application runs).
	 */
	class ProjectLibraryEntries : public IReflectable
	{
		struct ConstructPrivately { };

	public:
		explicit ProjectLibraryEntries(const ConstructPrivately& dummy);
		ProjectLibraryEntries(const USPtr<ProjectLibrary::DirectoryEntry>& rootEntry);

		/**
		 * Creates new project library entries based on a ProjectLibrary root directory entry.
		 *
		 * @param[in]	rootEntry	Root directory entry in ProjectLibrary.
		 */
		static SPtr<ProjectLibraryEntries> create(const USPtr<ProjectLibrary::DirectoryEntry>& rootEntry);
		
		/**	Returns the root directory entry that references the entire entry hierarchy. */
		const USPtr<ProjectLibrary::DirectoryEntry>& getRootEntry() const { return mRootEntry; }

	private:
		USPtr<ProjectLibrary::DirectoryEntry> mRootEntry;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/

		/** Creates new empty project library entries object. Used for serialization purposes. */
		static SPtr<ProjectLibraryEntries> createEmpty();

	public:
		friend class ProjectLibraryEntriesRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;
	};

	/** @} */
}