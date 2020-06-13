//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"

namespace bs { class SerializedSceneObject; }
namespace bs
{
	class BS_SCR_BED_EXPORT ScriptSerializedSceneObject : public ScriptObject<ScriptSerializedSceneObject>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "SerializedSceneObject")

		ScriptSerializedSceneObject(MonoObject* managedInstance, const SPtr<SerializedSceneObject>& value);

		SPtr<SerializedSceneObject> getInternal() const { return mInternal; }
		static MonoObject* create(const SPtr<SerializedSceneObject>& value);

	private:
		SPtr<SerializedSceneObject> mInternal;

		static void Internal_SerializedSceneObject(MonoObject* managedInstance, MonoObject* sceneObject, bool hierarchy);
		static void Internal_restore(ScriptSerializedSceneObject* thisPtr);
	};
}
