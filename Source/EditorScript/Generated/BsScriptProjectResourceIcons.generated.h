//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "../../EditorCore/Library/BsProjectResourceMeta.h"

namespace bs
{
	struct __ProjectResourceIconsInterop
	{
		MonoObject* icon16;
		MonoObject* icon32;
		MonoObject* icon48;
		MonoObject* icon64;
		MonoObject* icon96;
		MonoObject* icon128;
		MonoObject* icon192;
		MonoObject* icon256;
	};

	class BS_SCR_BED_EXPORT ScriptProjectResourceIcons : public ScriptObject<ScriptProjectResourceIcons>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "ProjectResourceIcons")

		static MonoObject* box(const __ProjectResourceIconsInterop& value);
		static __ProjectResourceIconsInterop unbox(MonoObject* value);
		static ProjectResourceIcons fromInterop(const __ProjectResourceIconsInterop& value);
		static __ProjectResourceIconsInterop toInterop(const ProjectResourceIcons& value);

	private:
		ScriptProjectResourceIcons(MonoObject* managedInstance);

	};
}
