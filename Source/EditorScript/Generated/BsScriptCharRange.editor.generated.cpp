//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptCharRange.editor.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptCharRange::ScriptCharRange(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptCharRange::initRuntimeData()
	{ }

	MonoObject*ScriptCharRange::box(const CharRange& value)
	{
		return MonoUtil::box(metaData.scriptClass->_getInternalClass(), (void*)&value);
	}

	CharRange ScriptCharRange::unbox(MonoObject* value)
	{
		return *(CharRange*)MonoUtil::unbox(value);
	}

}
