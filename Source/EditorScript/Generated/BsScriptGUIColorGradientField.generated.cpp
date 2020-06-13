//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIColorGradientField.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIColorGradientField.h"
#include "BsScriptColorGradient.generated.h"
#include "../../EditorCore/GUI/BsGUIColorGradientField.h"
#include "BsScriptGUIContent.generated.h"
#include "BsScriptHString.generated.h"

namespace bs
{
	ScriptGUIColorGradientField::onClickedThunkDef ScriptGUIColorGradientField::onClickedThunk; 

	ScriptGUIColorGradientField::ScriptGUIColorGradientField(MonoObject* managedInstance, GUIColorGradientField* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUIColorGradientField::onClicked, this));
	}

	void ScriptGUIColorGradientField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIColorGradientField::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIColorGradientField::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIColorGradientField::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIColorGradientField::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIColorGradientField::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIColorGradientField::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIColorGradientField::Internal_create3);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "")->getThunk();
	}

	void ScriptGUIColorGradientField::onClicked()
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance());
	}
	MonoObject* ScriptGUIColorGradientField::Internal_getValue(ScriptGUIColorGradientField* thisPtr)
	{
		SPtr<ColorGradient> tmp__output = bs_shared_ptr_new<ColorGradient>();
		*tmp__output = static_cast<GUIColorGradientField*>(thisPtr->getGUIElement())->getValue();

		MonoObject* __output;
		__output = ScriptColorGradient::create(tmp__output);

		return __output;
	}

	void ScriptGUIColorGradientField::Internal_setValue(ScriptGUIColorGradientField* thisPtr, MonoObject* value)
	{
		SPtr<ColorGradient> tmpvalue;
		ScriptColorGradient* scriptvalue;
		scriptvalue = ScriptColorGradient::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		static_cast<GUIColorGradientField*>(thisPtr->getGUIElement())->setValue(*tmpvalue);
	}

	void ScriptGUIColorGradientField::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientField* instance = GUIColorGradientField::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientField>())ScriptGUIColorGradientField(managedInstance, instance);
	}

	void ScriptGUIColorGradientField::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientField* instance = GUIColorGradientField::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientField>())ScriptGUIColorGradientField(managedInstance, instance);
	}

	void ScriptGUIColorGradientField::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientField* instance = GUIColorGradientField::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientField>())ScriptGUIColorGradientField(managedInstance, instance);
	}

	void ScriptGUIColorGradientField::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientField* instance = GUIColorGradientField::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientField>())ScriptGUIColorGradientField(managedInstance, instance);
	}

	void ScriptGUIColorGradientField::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorGradientField* instance = GUIColorGradientField::create(tmpstyle);
		new (bs_alloc<ScriptGUIColorGradientField>())ScriptGUIColorGradientField(managedInstance, instance);
	}
}
