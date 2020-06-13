//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include <iostream>
#include <stdio.h>
#include "BsEditorApplication.h"
#include "Error/BsCrashHandler.h"
#include "BsEditorScriptLibrary.h"

#if BS_PLATFORM == BS_PLATFORM_WIN32
#include <windows.h>

using namespace bs;

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	CrashHandler::startUp();

	__try
	{
		SPtr<EditorScriptLibrary> library = bs_shared_ptr_new<EditorScriptLibrary>();
		ScriptManager::_setScriptLibrary(library);

		EditorApplication::startUp();
		EditorApplication::instance().runMainLoop();
		EditorApplication::shutDown();
	}
	__except (gCrashHandler().reportCrash(GetExceptionInformation()))
	{
		PlatformUtility::terminate(true);
	}

	CrashHandler::shutDown();

	return 0;
}
#else
using namespace bs;

int main()
{
	SPtr<EditorScriptLibrary> library = bs_shared_ptr_new<EditorScriptLibrary>();
	ScriptManager::_setScriptLibrary(library);

	EditorApplication::startUp();
	EditorApplication::instance().runMainLoop();
	EditorApplication::shutDown();
}
#endif // End BS_PLATFORM