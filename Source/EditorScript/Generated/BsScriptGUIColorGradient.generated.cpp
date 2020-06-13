//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIColorGradient.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIColorGradient.h"
#include "BsScriptColorGradient.generated.h"
#include "../../EditorCore/GUI/BsGUIColorGradient.h"

namespace bs
{
	ScriptGUIColorGradient::onClickedThunkDef ScriptGUIColorGradient::onClickedThunk; 

	ScriptGUIColorGradient::ScriptGUIColorGradient(MonoObject* managedInstance, GUIColorGradient* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUIColorGradient::onClicked, this));
	}

	void ScriptGUIColorGradient::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setGradient", (void*)&ScriptGUIColorGradient::Internal_setGradient);
		metaData.scriptClass->addInternalCall("Internal_getGradient", (void*)&ScriptGUIColorGradient::Internal_getGradient);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIColorGradient::Internal_create);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "")->getThunk();
	}

	void ScriptGUIColorGradient::onClicked()
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance());
	}
	void ScriptGUIColorGradient::Internal_setGradient(ScriptGUIColorGradient* thisPtr, MonoObject* colorGradient)
	{
		SPtr<ColorGradient> tmpcolorGradient;
		ScriptColorGradient* scriptcolorGradient;
		scriptcolorGradient = ScriptColorGradient::toNative(colorGradient);
		if(scriptcolorGradient != nullptr)
			tmpcolorGradient = scriptcolorGradient->getInternal();
		static_cast<GUIColorGradient*>(thisPtr->getGUIElement())->setGradient(*tmpcolorGradient);
	}

	MonoObject* ScriptGUIColorGradient::Internal_getGradient(ScriptGUIColorGradient* thisPtr)
	{
		SPtr<ColorGradient> tmp__output = bs_shared_ptr_new<ColorGradient>();
		*tmp__output = static_cast<GUIColorGradient*>(thisPtr->getGUIElement())->getGradient();

		MonoObject* __output;
		__output = ScriptColorGradient::create(tmp__output);

		return __output;
	}

	void ScriptGUIColorGradient::Internal_create(MonoObject* managedInstance, MonoString* styleName)
	{
		String tmpstyleName;
		tmpstyleName = MonoUtil::monoToString(styleName);
		GUIColorGradient* instance = GUIColorGradient::create(tmpstyleName);
		new (bs_alloc<ScriptGUIColorGradient>())ScriptGUIColorGradient(managedInstance, instance);
	}
}
