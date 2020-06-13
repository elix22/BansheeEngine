//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGizmoDrawSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptGizmoDrawSettings::ScriptGizmoDrawSettings(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptGizmoDrawSettings::initRuntimeData()
	{ }

	MonoObject*ScriptGizmoDrawSettings::box(const GizmoDrawSettings& value)
	{
		return MonoUtil::box(metaData.scriptClass->_getInternalClass(), (void*)&value);
	}

	GizmoDrawSettings ScriptGizmoDrawSettings::unbox(MonoObject* value)
	{
		return *(GizmoDrawSettings*)MonoUtil::unbox(value);
	}

}
