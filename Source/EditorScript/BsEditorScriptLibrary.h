//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsEngineScriptLibrary.h"

namespace bs
{
	/** @addtogroup EditorScript
	 *  @{
	 */

	/**
	 * Handles initialization/shutdown of the script systems and loading/refresh of engine and editor-specific assemblies.
	 */
	class BS_SCR_BED_EXPORT EditorScriptLibrary : public EngineScriptLibrary
	{
	public:
		EditorScriptLibrary();

		/** @copydoc ScriptLibrary::initialize */
		void initialize() override;

		/** @copydoc ScriptLibrary::update */
		void update() override;

		/** @copydoc ScriptLibrary::reload */
		void reload() override;

		/** @copydoc ScriptLibrary::destroy */
		void destroy() override;

		/**	Returns the absolute path to the built-in managed editor assembly file. */
		Path getEditorAssemblyPath() const;

		/**	Returns the absolute path of the managed editor script assembly file. */
		Path getEditorScriptAssemblyPath() const;

		/** @copydoc EngineScriptLibrary::getScriptAssemblyFolder */
		Path getScriptAssemblyFolder() const override;

		/** Returns the singleton instance of this library. */
		static EditorScriptLibrary& instance()
		{
			return static_cast<EditorScriptLibrary&>(*ScriptManager::_getScriptLibrary());
		}

	private:
		BuiltinTypeMappings mEditorTypeMappings;

		bool mScriptAssembliesLoaded;
		HEvent mQuitRequestedEvent;
	};

	/** @} */
}