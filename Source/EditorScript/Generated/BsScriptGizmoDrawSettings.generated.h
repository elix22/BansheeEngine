//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "../../EditorCore/Scene/BsGizmoManager.h"

namespace bs
{
	class BS_SCR_BED_EXPORT ScriptGizmoDrawSettings : public ScriptObject<ScriptGizmoDrawSettings>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "GizmoDrawSettings")

		static MonoObject* box(const GizmoDrawSettings& value);
		static GizmoDrawSettings unbox(MonoObject* value);

	private:
		ScriptGizmoDrawSettings(MonoObject* managedInstance);

	};
}
