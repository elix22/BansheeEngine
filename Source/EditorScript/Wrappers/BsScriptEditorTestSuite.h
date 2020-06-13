//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Testing/BsTestSuite.h"

namespace bs
{
	/** @addtogroup EditorScript
	 *  @{
	 */

	/**	Performs editor managed unit tests. */
	class ScriptEditorTestSuite : public TestSuite
	{
	public:
		ScriptEditorTestSuite();

	private:
		/**	Triggers execution of managed unit tests. */
		void runManagedTests();
	};

	/** @} */
}