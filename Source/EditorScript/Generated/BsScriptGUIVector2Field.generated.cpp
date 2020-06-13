//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIVector2Field.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIVector2Field.h"
#include "Wrappers/BsScriptVector.h"
#include "../../EditorCore/GUI/BsGUIVector2Field.h"
#include "BsScriptGUIContent.generated.h"
#include "BsScriptHString.generated.h"

namespace bs
{
	ScriptGUIVector2Field::onValueChangedThunkDef ScriptGUIVector2Field::onValueChangedThunk; 
	ScriptGUIVector2Field::onComponentChangedThunkDef ScriptGUIVector2Field::onComponentChangedThunk; 
	ScriptGUIVector2Field::onComponentFocusChangedThunkDef ScriptGUIVector2Field::onComponentFocusChangedThunk; 
	ScriptGUIVector2Field::onConfirmThunkDef ScriptGUIVector2Field::onConfirmThunk; 

	ScriptGUIVector2Field::ScriptGUIVector2Field(MonoObject* managedInstance, GUIVector2Field* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onValueChanged.connect(std::bind(&ScriptGUIVector2Field::onValueChanged, this, std::placeholders::_1));
		value->onComponentChanged.connect(std::bind(&ScriptGUIVector2Field::onComponentChanged, this, std::placeholders::_1, std::placeholders::_2));
		value->onComponentFocusChanged.connect(std::bind(&ScriptGUIVector2Field::onComponentFocusChanged, this, std::placeholders::_1, std::placeholders::_2));
		value->onConfirm.connect(std::bind(&ScriptGUIVector2Field::onConfirm, this, std::placeholders::_1));
	}

	void ScriptGUIVector2Field::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIVector2Field::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIVector2Field::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_hasInputFocus", (void*)&ScriptGUIVector2Field::Internal_hasInputFocus);
		metaData.scriptClass->addInternalCall("Internal_setInputFocus", (void*)&ScriptGUIVector2Field::Internal_setInputFocus);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIVector2Field::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIVector2Field::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIVector2Field::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIVector2Field::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIVector2Field::Internal_create3);

		onValueChangedThunk = (onValueChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onValueChanged", "Vector2&")->getThunk();
		onComponentChangedThunk = (onComponentChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onComponentChanged", "single,VectorComponent")->getThunk();
		onComponentFocusChangedThunk = (onComponentFocusChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onComponentFocusChanged", "bool,VectorComponent")->getThunk();
		onConfirmThunk = (onConfirmThunkDef)metaData.scriptClass->getMethodExact("Internal_onConfirm", "VectorComponent")->getThunk();
	}

	void ScriptGUIVector2Field::onValueChanged(const Vector2& p0)
	{
		MonoObject* tmpp0;
		tmpp0 = ScriptVector2::box(p0);
		MonoUtil::invokeThunk(onValueChangedThunk, getManagedInstance(), tmpp0);
	}

	void ScriptGUIVector2Field::onComponentChanged(float p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onComponentChangedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIVector2Field::onComponentFocusChanged(bool p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onComponentFocusChangedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIVector2Field::onConfirm(VectorComponent p0)
	{
		MonoUtil::invokeThunk(onConfirmThunk, getManagedInstance(), p0);
	}
	void ScriptGUIVector2Field::Internal_getValue(ScriptGUIVector2Field* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = static_cast<GUIVector2Field*>(thisPtr->getGUIElement())->getValue();

		*__output = tmp__output;
	}

	void ScriptGUIVector2Field::Internal_setValue(ScriptGUIVector2Field* thisPtr, Vector2* value)
	{
		static_cast<GUIVector2Field*>(thisPtr->getGUIElement())->setValue(*value);
	}

	bool ScriptGUIVector2Field::Internal_hasInputFocus(ScriptGUIVector2Field* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_cast<GUIVector2Field*>(thisPtr->getGUIElement())->hasInputFocus();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIVector2Field::Internal_setInputFocus(ScriptGUIVector2Field* thisPtr, VectorComponent component, bool focus)
	{
		static_cast<GUIVector2Field*>(thisPtr->getGUIElement())->setInputFocus(component, focus);
	}

	void ScriptGUIVector2Field::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector2Field* instance = GUIVector2Field::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIVector2Field>())ScriptGUIVector2Field(managedInstance, instance);
	}

	void ScriptGUIVector2Field::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector2Field* instance = GUIVector2Field::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIVector2Field>())ScriptGUIVector2Field(managedInstance, instance);
	}

	void ScriptGUIVector2Field::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector2Field* instance = GUIVector2Field::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIVector2Field>())ScriptGUIVector2Field(managedInstance, instance);
	}

	void ScriptGUIVector2Field::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector2Field* instance = GUIVector2Field::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIVector2Field>())ScriptGUIVector2Field(managedInstance, instance);
	}

	void ScriptGUIVector2Field::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector2Field* instance = GUIVector2Field::create(tmpstyle);
		new (bs_alloc<ScriptGUIVector2Field>())ScriptGUIVector2Field(managedInstance, instance);
	}
}
