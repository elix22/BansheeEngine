//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsEditorResourceLoader.h"
#include "Library/BsProjectLibrary.h"
#include "Resources/BsResources.h"
#include "Library/BsProjectResourceMeta.h"
#include "Debug/BsDebug.h"

namespace bs
{
	HResource EditorResourceLoader::load(const Path& path, ResourceLoadFlags flags, bool async) const
	{
		ProjectLibrary::LibraryEntry* entry = gProjectLibrary().findEntry(path).get();

		if (entry == nullptr || entry->type == ProjectLibrary::LibraryEntryType::Directory)
			return HResource();

		ProjectLibrary::FileEntry* resEntry = static_cast<ProjectLibrary::FileEntry*>(entry);
		if (resEntry->meta == nullptr)
		{
			BS_LOG(Warning, Editor, "Missing .meta file for resource at path: \"{0}\".", path);
			return HResource();
		}

		// Note: Calling both findEntry and findResourceMeta is a bit redundant since they do a lot of the same work, and 
		// this could be optimized so only one of them is called.
		SPtr<ProjectResourceMeta> meta = gProjectLibrary().findResourceMeta(path);
		if (meta == nullptr)
		{
			BS_LOG(Warning, Editor, "Unable to load resource at path: \"{0}\". File not found. ", path);
			return HResource();
		}

		UUID resUUID = meta->getUUID();

		if (resEntry->meta->getIncludeInBuild())
		{
			BS_LOG(Warning, Editor, "Dynamically loading a resource at path: \"{0}\" but the resource isn't flagged to "
				"be included in the build. It may not be available outside of the editor.", path);
		}

		return gResources().loadFromUUID(resUUID, async, flags);
	}
}