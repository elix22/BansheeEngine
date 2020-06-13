//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"

namespace bs
{
	class ScriptSerializedObject;

	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/**	Interop class between C++ & CLR for SerializedDiff. */
	class BS_SCR_BED_EXPORT ScriptSerializedDiff : public ScriptObject <ScriptSerializedDiff>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "SerializedDiff")

	private:
		ScriptSerializedDiff(MonoObject* instance, const SPtr<IReflectable>& obj);

		SPtr<IReflectable> mSerializedDiff;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static MonoObject* internal_CreateDiff(ScriptSerializedObject* oldObj, ScriptSerializedObject* newObj);
		static void internal_ApplyDiff(ScriptSerializedDiff* thisPtr, MonoObject* obj);
		static bool internal_IsEmpty(ScriptSerializedDiff* thisPtr);
	};

	/** @} */
}