//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfEngine/Resources/BsScriptCodeImportOptions.h"

namespace bs { class ScriptCodeImportOptions; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptScriptCodeImportOptions : public TScriptReflectable<ScriptScriptCodeImportOptions, ScriptCodeImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "ScriptCodeImportOptions")

		ScriptScriptCodeImportOptions(MonoObject* managedInstance, const SPtr<ScriptCodeImportOptions>& value);

		static MonoObject* create(const SPtr<ScriptCodeImportOptions>& value);

	private:
		static bool Internal_geteditorScript(ScriptScriptCodeImportOptions* thisPtr);
		static void Internal_seteditorScript(ScriptScriptCodeImportOptions* thisPtr, bool value);
		static void Internal_create(MonoObject* managedInstance);
	};
}
