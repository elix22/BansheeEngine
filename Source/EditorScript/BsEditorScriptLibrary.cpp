//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsEditorScriptLibrary.h"
#include "BsEditorScriptManager.h"
#include "BsEditorApplication.h"
#include "BsScriptObjectManager.h"
#include "FileSystem/BsFileSystem.h"
#include "BsMonoManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsMonoAssembly.h"
#include "Scene/BsGameObjectManager.h"

#include "Generated/BsEditorBuiltinReflectableTypesLookup.generated.h"

namespace bs
{
	EditorScriptLibrary::EditorScriptLibrary()
		:mScriptAssembliesLoaded(false)
	{ }

	void EditorScriptLibrary::initialize()
	{
		EngineScriptLibrary::initialize();

		mEditorTypeMappings.reflectableObjects = EditorBuiltinReflectableTypes::getEntries();

		MonoManager::instance().loadAssembly(getEditorAssemblyPath(), EDITOR_ASSEMBLY);
		ScriptAssemblyManager::instance().loadAssemblyInfo(EDITOR_ASSEMBLY, mEditorTypeMappings);

		EditorScriptManager::startUp();
		
		mQuitRequestedEvent = gEditorApplication().onQuitRequested.connect([]()
		{
			EditorScriptManager::instance().quitRequested();
		});
	}

	void EditorScriptLibrary::update()
	{
		EngineScriptLibrary::update();

		EditorScriptManager::instance().update();
	}

	void EditorScriptLibrary::reload()
	{
		Path engineAssemblyPath = getEngineAssemblyPath();
		Path gameAssemblyPath = getGameAssemblyPath();

		Path editorAssemblyPath = getEditorAssemblyPath();
		Path editorScriptAssemblyPath = getEditorScriptAssemblyPath();

#if BS_DEBUG_MODE
		mScriptAssembliesLoaded = true; // Force assembly refresh as an ad hoc unit test in debug mode
#endif

		// Do a full refresh if we have already loaded script assemblies
		if (mScriptAssembliesLoaded)
		{
			Vector<AssemblyRefreshInfo> assemblies;

			assemblies.push_back(AssemblyRefreshInfo(ENGINE_ASSEMBLY, &engineAssemblyPath, &mEngineTypeMappings));
			if (gEditorApplication().isProjectLoaded())
			{
				if (FileSystem::exists(gameAssemblyPath))
					assemblies.push_back(AssemblyRefreshInfo(SCRIPT_GAME_ASSEMBLY, &gameAssemblyPath, &BuiltinTypeMappings::EMPTY));
			}

			assemblies.push_back(AssemblyRefreshInfo(EDITOR_ASSEMBLY, &editorAssemblyPath, &mEditorTypeMappings));
			if (gEditorApplication().isProjectLoaded())
			{
				if (FileSystem::exists(editorScriptAssemblyPath))
					assemblies.push_back(AssemblyRefreshInfo(SCRIPT_EDITOR_ASSEMBLY, &editorScriptAssemblyPath, &BuiltinTypeMappings::EMPTY));
			}

			ScriptObjectManager::instance().refreshAssemblies(assemblies);
		}
		else // Otherwise just additively load them
		{
			if (FileSystem::exists(gameAssemblyPath))
			{
				MonoManager::instance().loadAssembly(gameAssemblyPath.toString(), SCRIPT_GAME_ASSEMBLY);
				ScriptAssemblyManager::instance().loadAssemblyInfo(SCRIPT_GAME_ASSEMBLY, BuiltinTypeMappings());
			}

			if (FileSystem::exists(editorScriptAssemblyPath))
			{
				MonoManager::instance().loadAssembly(editorScriptAssemblyPath.toString(), SCRIPT_EDITOR_ASSEMBLY);
				ScriptAssemblyManager::instance().loadAssemblyInfo(SCRIPT_EDITOR_ASSEMBLY, BuiltinTypeMappings());
			}

			mScriptAssembliesLoaded = true;
		}
	}

	void EditorScriptLibrary::destroy()
	{
		mQuitRequestedEvent.disconnect();

		GameObjectManager::instance().destroyQueuedObjects();

		unloadAssemblies();
		EditorScriptManager::shutDown();
		shutdownModules();
	}

	Path EditorScriptLibrary::getEditorAssemblyPath() const
	{
		Path assemblyPath = getBuiltinAssemblyFolder();
		assemblyPath.append(String(EDITOR_ASSEMBLY) + ".dll");

		return assemblyPath;
	}

	Path EditorScriptLibrary::getEditorScriptAssemblyPath() const
	{
		Path assemblyPath = getScriptAssemblyFolder();
		assemblyPath.append(String(SCRIPT_EDITOR_ASSEMBLY) + ".dll");

		return assemblyPath;
	}

	Path EditorScriptLibrary::getScriptAssemblyFolder() const
	{
		if (!gEditorApplication().isProjectLoaded())
			return Path::BLANK;

		Path assemblyFolder = gEditorApplication().getProjectPath();
		assemblyFolder.append(INTERNAL_ASSEMBLY_PATH);

		return assemblyFolder;
	}

}
