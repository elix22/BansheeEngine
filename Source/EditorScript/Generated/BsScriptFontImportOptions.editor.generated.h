//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Text/BsFontImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Text/BsFontImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Text/BsFontImportOptions.h"

namespace bs { class FontImportOptions; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptFontImportOptions : public TScriptReflectable<ScriptFontImportOptions, FontImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "FontImportOptions")

		ScriptFontImportOptions(MonoObject* managedInstance, const SPtr<FontImportOptions>& value);

		static MonoObject* create(const SPtr<FontImportOptions>& value);

	private:
		static MonoArray* Internal_getfontSizes(ScriptFontImportOptions* thisPtr);
		static void Internal_setfontSizes(ScriptFontImportOptions* thisPtr, MonoArray* value);
		static MonoArray* Internal_getcharIndexRanges(ScriptFontImportOptions* thisPtr);
		static void Internal_setcharIndexRanges(ScriptFontImportOptions* thisPtr, MonoArray* value);
		static uint32_t Internal_getdpi(ScriptFontImportOptions* thisPtr);
		static void Internal_setdpi(ScriptFontImportOptions* thisPtr, uint32_t value);
		static FontRenderMode Internal_getrenderMode(ScriptFontImportOptions* thisPtr);
		static void Internal_setrenderMode(ScriptFontImportOptions* thisPtr, FontRenderMode value);
		static bool Internal_getbold(ScriptFontImportOptions* thisPtr);
		static void Internal_setbold(ScriptFontImportOptions* thisPtr, bool value);
		static bool Internal_getitalic(ScriptFontImportOptions* thisPtr);
		static void Internal_setitalic(ScriptFontImportOptions* thisPtr, bool value);
		static void Internal_create(MonoObject* managedInstance);
	};
}
