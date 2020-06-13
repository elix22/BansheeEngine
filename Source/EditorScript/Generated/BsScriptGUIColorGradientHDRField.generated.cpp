//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIColorGradientHDRField.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIColorGradientField.h"
#include "BsScriptHString.generated.h"
#include "BsScriptColorGradientHDR.generated.h"
#include "BsScriptGUIContent.generated.h"
#include "../../EditorCore/GUI/BsGUIColorGradientField.h"

namespace bs
{
	ScriptGUIColorGradientHDRField::onClickedThunkDef ScriptGUIColorGradientHDRField::onClickedThunk; 

	ScriptGUIColorGradientHDRField::ScriptGUIColorGradientHDRField(MonoObject* managedInstance, GUIColorGradientHDRField* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUIColorGradientHDRField::onClicked, this));
	}

	void ScriptGUIColorGradientHDRField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIColorGradientHDRField::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIColorGradientHDRField::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIColorGradientHDRField::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIColorGradientHDRField::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIColorGradientHDRField::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIColorGradientHDRField::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIColorGradientHDRField::Internal_create3);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "")->getThunk();
	}

	void ScriptGUIColorGradientHDRField::onClicked()
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance());
	}
	MonoObject* ScriptGUIColorGradientHDRField::Internal_getValue(ScriptGUIColorGradientHDRField* thisPtr)
	{
		SPtr<ColorGradientHDR> tmp__output = bs_shared_ptr_new<ColorGradientHDR>();
		*tmp__output = static_cast<GUIColorGradientHDRField*>(thisPtr->getGUIElement())->getValue();

		MonoObject* __output;
		__output = ScriptColorGradientHDR::create(tmp__output);

		return __output;
	}

	void ScriptGUIColorGradientHDRField::Internal_setValue(ScriptGUIColorGradientHDRField* thisPtr, MonoObject* value)
	{
		SPtr<ColorGradientHDR> tmpvalue;
		ScriptColorGradientHDR* scriptvalue;
		scriptvalue = ScriptColorGradientHDR::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		static_cast<GUIColorGradientHDRField*>(thisPtr->getGUIElement())->setValue(*tmpvalue);
	}

	void ScriptGUIColorGradientHDRField::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientHDRField* instance = GUIColorGradientHDRField::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientHDRField>())ScriptGUIColorGradientHDRField(managedInstance, instance);
	}

	void ScriptGUIColorGradientHDRField::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientHDRField* instance = GUIColorGradientHDRField::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientHDRField>())ScriptGUIColorGradientHDRField(managedInstance, instance);
	}

	void ScriptGUIColorGradientHDRField::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientHDRField* instance = GUIColorGradientHDRField::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientHDRField>())ScriptGUIColorGradientHDRField(managedInstance, instance);
	}

	void ScriptGUIColorGradientHDRField::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientHDRField* instance = GUIColorGradientHDRField::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientHDRField>())ScriptGUIColorGradientHDRField(managedInstance, instance);
	}

	void ScriptGUIColorGradientHDRField::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientHDRField* instance = GUIColorGradientHDRField::create(tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientHDRField>())ScriptGUIColorGradientHDRField(managedInstance, instance);
	}
}
