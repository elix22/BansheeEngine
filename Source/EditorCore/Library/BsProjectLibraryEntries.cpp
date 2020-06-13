//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Library/BsProjectLibraryEntries.h"
#include "Private/RTTI/BsProjectLibraryEntriesRTTI.h"

namespace bs
{
	ProjectLibraryEntries::ProjectLibraryEntries(const USPtr<ProjectLibrary::DirectoryEntry>& rootEntry)
		:mRootEntry(rootEntry)
	{ }

	ProjectLibraryEntries::ProjectLibraryEntries(const ConstructPrivately& dummy)
	{ }

	SPtr<ProjectLibraryEntries> ProjectLibraryEntries::create(const USPtr<ProjectLibrary::DirectoryEntry>& rootEntry)
	{
		return bs_shared_ptr_new<ProjectLibraryEntries>(rootEntry);
	}

	SPtr<ProjectLibraryEntries> ProjectLibraryEntries::createEmpty()
	{
		return bs_shared_ptr_new<ProjectLibraryEntries>(ConstructPrivately());
	}

	RTTITypeBase* ProjectLibraryEntries::getRTTIStatic()
	{
		return ProjectLibraryEntriesRTTI::instance();
	}

	RTTITypeBase* ProjectLibraryEntries::getRTTI() const
	{
		return ProjectLibraryEntries::getRTTIStatic();
	}
}