//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIColorGradientHDR.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIColorGradient.h"
#include "BsScriptColorGradientHDR.generated.h"
#include "../../EditorCore/GUI/BsGUIColorGradient.h"

namespace bs
{
	ScriptGUIColorGradientHDR::onClickedThunkDef ScriptGUIColorGradientHDR::onClickedThunk; 

	ScriptGUIColorGradientHDR::ScriptGUIColorGradientHDR(MonoObject* managedInstance, GUIColorGradientHDR* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUIColorGradientHDR::onClicked, this));
	}

	void ScriptGUIColorGradientHDR::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setGradient", (void*)&ScriptGUIColorGradientHDR::Internal_setGradient);
		metaData.scriptClass->addInternalCall("Internal_getGradient", (void*)&ScriptGUIColorGradientHDR::Internal_getGradient);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIColorGradientHDR::Internal_create);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "")->getThunk();
	}

	void ScriptGUIColorGradientHDR::onClicked()
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance());
	}
	void ScriptGUIColorGradientHDR::Internal_setGradient(ScriptGUIColorGradientHDR* thisPtr, MonoObject* colorGradient)
	{
		SPtr<ColorGradientHDR> tmpcolorGradient;
		ScriptColorGradientHDR* scriptcolorGradient;
		scriptcolorGradient = ScriptColorGradientHDR::toNative(colorGradient);
		if(scriptcolorGradient != nullptr)
			tmpcolorGradient = scriptcolorGradient->getInternal();
		static_cast<GUIColorGradientHDR*>(thisPtr->getGUIElement())->setGradient(*tmpcolorGradient);
	}

	MonoObject* ScriptGUIColorGradientHDR::Internal_getGradient(ScriptGUIColorGradientHDR* thisPtr)
	{
		SPtr<ColorGradientHDR> tmp__output = bs_shared_ptr_new<ColorGradientHDR>();
		*tmp__output = static_cast<GUIColorGradientHDR*>(thisPtr->getGUIElement())->getGradient();

		MonoObject* __output;
		__output = ScriptColorGradientHDR::create(tmp__output);

		return __output;
	}

	void ScriptGUIColorGradientHDR::Internal_create(MonoObject* managedInstance, MonoString* styleName)
	{
		String tmpstyleName;
		tmpstyleName = MonoUtil::monoToString(styleName);
		GUIColorGradientHDR* instance = GUIColorGradientHDR::create(tmpstyleName);
		new (bs_alloc<ScriptGUIColorGradientHDR>())ScriptGUIColorGradientHDR(managedInstance, instance);
	}
}
