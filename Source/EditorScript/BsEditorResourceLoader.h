//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Resources/BsGameResourceManager.h"

namespace bs
{
	/** @addtogroup EditorScript
	 *  @{
	 */

	/**	Handles loading of game resources when the editor is running. */
	class BS_SCR_BED_EXPORT EditorResourceLoader : public IGameResourceLoader
	{
	public:
		/** @copydoc IGameResourceLoader::load */
		HResource load(const Path& path, ResourceLoadFlags flags, bool async) const override;
	};

	/** @} */
}