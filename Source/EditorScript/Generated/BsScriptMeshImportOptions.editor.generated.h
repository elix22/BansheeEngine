//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.editor.generated.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "../../bsf/Source/Foundation/bsfCore/Importer/BsMeshImportOptions.h"

namespace bs { class MeshImportOptions; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptMeshImportOptions : public TScriptReflectable<ScriptMeshImportOptions, MeshImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "MeshImportOptions")

		ScriptMeshImportOptions(MonoObject* managedInstance, const SPtr<MeshImportOptions>& value);

		static MonoObject* create(const SPtr<MeshImportOptions>& value);

	private:
		static bool Internal_getcpuCached(ScriptMeshImportOptions* thisPtr);
		static void Internal_setcpuCached(ScriptMeshImportOptions* thisPtr, bool value);
		static bool Internal_getimportNormals(ScriptMeshImportOptions* thisPtr);
		static void Internal_setimportNormals(ScriptMeshImportOptions* thisPtr, bool value);
		static bool Internal_getimportTangents(ScriptMeshImportOptions* thisPtr);
		static void Internal_setimportTangents(ScriptMeshImportOptions* thisPtr, bool value);
		static bool Internal_getimportBlendShapes(ScriptMeshImportOptions* thisPtr);
		static void Internal_setimportBlendShapes(ScriptMeshImportOptions* thisPtr, bool value);
		static bool Internal_getimportSkin(ScriptMeshImportOptions* thisPtr);
		static void Internal_setimportSkin(ScriptMeshImportOptions* thisPtr, bool value);
		static bool Internal_getimportAnimation(ScriptMeshImportOptions* thisPtr);
		static void Internal_setimportAnimation(ScriptMeshImportOptions* thisPtr, bool value);
		static bool Internal_getreduceKeyFrames(ScriptMeshImportOptions* thisPtr);
		static void Internal_setreduceKeyFrames(ScriptMeshImportOptions* thisPtr, bool value);
		static bool Internal_getimportRootMotion(ScriptMeshImportOptions* thisPtr);
		static void Internal_setimportRootMotion(ScriptMeshImportOptions* thisPtr, bool value);
		static float Internal_getimportScale(ScriptMeshImportOptions* thisPtr);
		static void Internal_setimportScale(ScriptMeshImportOptions* thisPtr, float value);
		static CollisionMeshType Internal_getcollisionMeshType(ScriptMeshImportOptions* thisPtr);
		static void Internal_setcollisionMeshType(ScriptMeshImportOptions* thisPtr, CollisionMeshType value);
		static MonoArray* Internal_getanimationSplits(ScriptMeshImportOptions* thisPtr);
		static void Internal_setanimationSplits(ScriptMeshImportOptions* thisPtr, MonoArray* value);
		static MonoArray* Internal_getanimationEvents(ScriptMeshImportOptions* thisPtr);
		static void Internal_setanimationEvents(ScriptMeshImportOptions* thisPtr, MonoArray* value);
		static void Internal_create(MonoObject* managedInstance);
	};
}
