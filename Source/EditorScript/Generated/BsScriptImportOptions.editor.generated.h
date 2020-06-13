//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsImportOptions.h"

namespace bs { class ImportOptions; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptImportOptionsBase : public ScriptReflectableBase
	{
	public:
		ScriptImportOptionsBase(MonoObject* instance);
		virtual ~ScriptImportOptionsBase() {}

		SPtr<ImportOptions> getInternal() const;
	};

	class BS_SCR_BED_EXPORT ScriptImportOptions : public TScriptReflectable<ScriptImportOptions, ImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "ImportOptions")

		ScriptImportOptions(MonoObject* managedInstance, const SPtr<ImportOptions>& value);

		static MonoObject* create(const SPtr<ImportOptions>& value);

	private:
	};
}
