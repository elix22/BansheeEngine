//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Audio/BsAudioClipImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Audio/BsAudioClip.h"
#include "../../bsf/Source/Foundation/bsfCore/Audio/BsAudioClip.h"

namespace bs { class AudioClipImportOptions; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptAudioClipImportOptions : public TScriptReflectable<ScriptAudioClipImportOptions, AudioClipImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "AudioClipImportOptions")

		ScriptAudioClipImportOptions(MonoObject* managedInstance, const SPtr<AudioClipImportOptions>& value);

		static MonoObject* create(const SPtr<AudioClipImportOptions>& value);

	private:
		static AudioFormat Internal_getformat(ScriptAudioClipImportOptions* thisPtr);
		static void Internal_setformat(ScriptAudioClipImportOptions* thisPtr, AudioFormat value);
		static AudioReadMode Internal_getreadMode(ScriptAudioClipImportOptions* thisPtr);
		static void Internal_setreadMode(ScriptAudioClipImportOptions* thisPtr, AudioReadMode value);
		static bool Internal_getis3D(ScriptAudioClipImportOptions* thisPtr);
		static void Internal_setis3D(ScriptAudioClipImportOptions* thisPtr, bool value);
		static uint32_t Internal_getbitDepth(ScriptAudioClipImportOptions* thisPtr);
		static void Internal_setbitDepth(ScriptAudioClipImportOptions* thisPtr, uint32_t value);
		static void Internal_create(MonoObject* managedInstance);
	};
}
