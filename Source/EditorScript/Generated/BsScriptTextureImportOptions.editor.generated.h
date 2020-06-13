//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsTextureImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Image/BsPixelData.h"
#include "../../bsf/Source/Foundation/bsfCore/Utility/BsCommonTypes.h"

namespace bs { class TextureImportOptions; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptTextureImportOptions : public TScriptReflectable<ScriptTextureImportOptions, TextureImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "TextureImportOptions")

		ScriptTextureImportOptions(MonoObject* managedInstance, const SPtr<TextureImportOptions>& value);

		static MonoObject* create(const SPtr<TextureImportOptions>& value);

	private:
		static PixelFormat Internal_getformat(ScriptTextureImportOptions* thisPtr);
		static void Internal_setformat(ScriptTextureImportOptions* thisPtr, PixelFormat value);
		static bool Internal_getgenerateMips(ScriptTextureImportOptions* thisPtr);
		static void Internal_setgenerateMips(ScriptTextureImportOptions* thisPtr, bool value);
		static uint32_t Internal_getmaxMip(ScriptTextureImportOptions* thisPtr);
		static void Internal_setmaxMip(ScriptTextureImportOptions* thisPtr, uint32_t value);
		static bool Internal_getcpuCached(ScriptTextureImportOptions* thisPtr);
		static void Internal_setcpuCached(ScriptTextureImportOptions* thisPtr, bool value);
		static bool Internal_getsRGB(ScriptTextureImportOptions* thisPtr);
		static void Internal_setsRGB(ScriptTextureImportOptions* thisPtr, bool value);
		static bool Internal_getcubemap(ScriptTextureImportOptions* thisPtr);
		static void Internal_setcubemap(ScriptTextureImportOptions* thisPtr, bool value);
		static CubemapSourceType Internal_getcubemapSourceType(ScriptTextureImportOptions* thisPtr);
		static void Internal_setcubemapSourceType(ScriptTextureImportOptions* thisPtr, CubemapSourceType value);
		static void Internal_create(MonoObject* managedInstance);
	};
}
