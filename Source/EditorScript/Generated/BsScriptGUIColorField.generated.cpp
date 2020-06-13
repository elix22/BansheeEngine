//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIColorField.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIColorField.h"
#include "Wrappers/BsScriptColor.h"
#include "BsScriptGUIContent.generated.h"
#include "../../EditorCore/GUI/BsGUIColorField.h"
#include "BsScriptHString.generated.h"

namespace bs
{
	ScriptGUIColorField::onClickedThunkDef ScriptGUIColorField::onClickedThunk; 

	ScriptGUIColorField::ScriptGUIColorField(MonoObject* managedInstance, GUIColorField* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUIColorField::onClicked, this));
	}

	void ScriptGUIColorField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIColorField::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIColorField::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_getAllowHDR", (void*)&ScriptGUIColorField::Internal_getAllowHDR);
		metaData.scriptClass->addInternalCall("Internal_setAllowHDR", (void*)&ScriptGUIColorField::Internal_setAllowHDR);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIColorField::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIColorField::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIColorField::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIColorField::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIColorField::Internal_create3);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "")->getThunk();
	}

	void ScriptGUIColorField::onClicked()
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance());
	}
	void ScriptGUIColorField::Internal_getValue(ScriptGUIColorField* thisPtr, Color* __output)
	{
		Color tmp__output;
		tmp__output = static_cast<GUIColorField*>(thisPtr->getGUIElement())->getValue();

		*__output = tmp__output;
	}

	void ScriptGUIColorField::Internal_setValue(ScriptGUIColorField* thisPtr, Color* value)
	{
		static_cast<GUIColorField*>(thisPtr->getGUIElement())->setValue(*value);
	}

	bool ScriptGUIColorField::Internal_getAllowHDR(ScriptGUIColorField* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_cast<GUIColorField*>(thisPtr->getGUIElement())->getAllowHDR();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIColorField::Internal_setAllowHDR(ScriptGUIColorField* thisPtr, bool allow)
	{
		static_cast<GUIColorField*>(thisPtr->getGUIElement())->setAllowHDR(allow);
	}

	void ScriptGUIColorField::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorField* instance = GUIColorField::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorField>())ScriptGUIColorField(managedInstance, instance);
	}

	void ScriptGUIColorField::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorField* instance = GUIColorField::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIColorField>())ScriptGUIColorField(managedInstance, instance);
	}

	void ScriptGUIColorField::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorField* instance = GUIColorField::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorField>())ScriptGUIColorField(managedInstance, instance);
	}

	void ScriptGUIColorField::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorField* instance = GUIColorField::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIColorField>())ScriptGUIColorField(managedInstance, instance);
	}

	void ScriptGUIColorField::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorField* instance = GUIColorField::create(tmpstyle);
		new (bs_alloc<ScriptGUIColorField>())ScriptGUIColorField(managedInstance, instance);
	}
}
