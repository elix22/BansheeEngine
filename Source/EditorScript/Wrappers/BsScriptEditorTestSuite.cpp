//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptEditorTestSuite.h"
#include "Wrappers/BsScriptUnitTests.h"

namespace bs
{
	ScriptEditorTestSuite::ScriptEditorTestSuite()
	{
		BS_ADD_TEST(ScriptEditorTestSuite::runManagedTests);
	}

	void ScriptEditorTestSuite::runManagedTests()
	{
		ScriptUnitTests::runTests();
	}
}