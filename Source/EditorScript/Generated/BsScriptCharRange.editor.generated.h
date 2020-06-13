//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "../../bsf/Source/Foundation/bsfCore/Text/BsFontImportOptions.h"

namespace bs
{
	class BS_SCR_BED_EXPORT ScriptCharRange : public ScriptObject<ScriptCharRange>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "CharRange")

		static MonoObject* box(const CharRange& value);
		static CharRange unbox(MonoObject* value);

	private:
		ScriptCharRange(MonoObject* managedInstance);

	};
}
