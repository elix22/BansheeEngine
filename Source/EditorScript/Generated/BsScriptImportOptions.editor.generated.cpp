//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptImportOptions.editor.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptImportOptionsBase::ScriptImportOptionsBase(MonoObject* managedInstance)
		:ScriptReflectableBase(managedInstance)
	 { }

	SPtr<ImportOptions> ScriptImportOptionsBase::getInternal() const
	{
		return std::static_pointer_cast<ImportOptions>(mInternal);
	}
	ScriptImportOptions::ScriptImportOptions(MonoObject* managedInstance, const SPtr<ImportOptions>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptImportOptions::initRuntimeData()
	{

	}

	MonoObject* ScriptImportOptions::create(const SPtr<ImportOptions>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptImportOptions>()) ScriptImportOptions(managedInstance, value);
		return managedInstance;
	}
}
