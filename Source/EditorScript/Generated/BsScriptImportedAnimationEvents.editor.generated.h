//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Animation/BsAnimationClip.h"

namespace bs { struct ImportedAnimationEvents; }
namespace bs { struct __AnimationEventInterop; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptImportedAnimationEvents : public TScriptReflectable<ScriptImportedAnimationEvents, ImportedAnimationEvents>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "ImportedAnimationEvents")

		ScriptImportedAnimationEvents(MonoObject* managedInstance, const SPtr<ImportedAnimationEvents>& value);

		static MonoObject* create(const SPtr<ImportedAnimationEvents>& value);

	private:
		static void Internal_ImportedAnimationEvents(MonoObject* managedInstance);
		static MonoString* Internal_getname(ScriptImportedAnimationEvents* thisPtr);
		static void Internal_setname(ScriptImportedAnimationEvents* thisPtr, MonoString* value);
		static MonoArray* Internal_getevents(ScriptImportedAnimationEvents* thisPtr);
		static void Internal_setevents(ScriptImportedAnimationEvents* thisPtr, MonoArray* value);
	};
}
