//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "Serialization/BsBuiltinReflectableTypesLookup.h"
#include "Reflection/BsRTTIType.h"
#include "../../bsf/Source/Foundation/bsfCore/Audio/BsAudioClipImportOptions.h"
#include "BsScriptAudioClipImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "BsScriptImportedAnimationEvents.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsImportOptions.h"
#include "BsScriptImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsTextureImportOptions.h"
#include "BsScriptTextureImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "BsScriptAnimationSplitInfo.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "BsScriptMeshImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsShaderImportOptions.h"
#include "BsScriptShaderImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Text/BsFontImportOptions.h"
#include "BsScriptFontImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfEngine/Resources/BsScriptCodeImportOptions.h"
#include "BsScriptScriptCodeImportOptions.editor.generated.h"

namespace bs
{
	LOOKUP_BEGIN(EditorBuiltinReflectableTypes)
		ADD_ENTRY(AudioClipImportOptions, ScriptAudioClipImportOptions)
		ADD_ENTRY(ImportedAnimationEvents, ScriptImportedAnimationEvents)
		ADD_ENTRY(ImportOptions, ScriptImportOptions)
		ADD_ENTRY(TextureImportOptions, ScriptTextureImportOptions)
		ADD_ENTRY(AnimationSplitInfo, ScriptAnimationSplitInfo)
		ADD_ENTRY(MeshImportOptions, ScriptMeshImportOptions)
		ADD_ENTRY(ShaderImportOptions, ScriptShaderImportOptions)
		ADD_ENTRY(FontImportOptions, ScriptFontImportOptions)
		ADD_ENTRY(ScriptCodeImportOptions, ScriptScriptCodeImportOptions)
	LOOKUP_END
}
#undef LOOKUP_BEGIN
#undef ADD_ENTRY
#undef LOOKUP_END
