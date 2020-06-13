//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIVector4Field.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIVector4Field.h"
#include "BsScriptHString.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptGUIContent.generated.h"
#include "../../EditorCore/GUI/BsGUIVector4Field.h"

namespace bs
{
	ScriptGUIVector4Field::onValueChangedThunkDef ScriptGUIVector4Field::onValueChangedThunk; 
	ScriptGUIVector4Field::onComponentChangedThunkDef ScriptGUIVector4Field::onComponentChangedThunk; 
	ScriptGUIVector4Field::onComponentFocusChangedThunkDef ScriptGUIVector4Field::onComponentFocusChangedThunk; 
	ScriptGUIVector4Field::onConfirmThunkDef ScriptGUIVector4Field::onConfirmThunk; 

	ScriptGUIVector4Field::ScriptGUIVector4Field(MonoObject* managedInstance, GUIVector4Field* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onValueChanged.connect(std::bind(&ScriptGUIVector4Field::onValueChanged, this, std::placeholders::_1));
		value->onComponentChanged.connect(std::bind(&ScriptGUIVector4Field::onComponentChanged, this, std::placeholders::_1, std::placeholders::_2));
		value->onComponentFocusChanged.connect(std::bind(&ScriptGUIVector4Field::onComponentFocusChanged, this, std::placeholders::_1, std::placeholders::_2));
		value->onConfirm.connect(std::bind(&ScriptGUIVector4Field::onConfirm, this, std::placeholders::_1));
	}

	void ScriptGUIVector4Field::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIVector4Field::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIVector4Field::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_hasInputFocus", (void*)&ScriptGUIVector4Field::Internal_hasInputFocus);
		metaData.scriptClass->addInternalCall("Internal_setInputFocus", (void*)&ScriptGUIVector4Field::Internal_setInputFocus);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIVector4Field::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIVector4Field::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIVector4Field::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIVector4Field::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIVector4Field::Internal_create3);

		onValueChangedThunk = (onValueChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onValueChanged", "Vector4&")->getThunk();
		onComponentChangedThunk = (onComponentChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onComponentChanged", "single,VectorComponent")->getThunk();
		onComponentFocusChangedThunk = (onComponentFocusChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onComponentFocusChanged", "bool,VectorComponent")->getThunk();
		onConfirmThunk = (onConfirmThunkDef)metaData.scriptClass->getMethodExact("Internal_onConfirm", "VectorComponent")->getThunk();
	}

	void ScriptGUIVector4Field::onValueChanged(const Vector4& p0)
	{
		MonoObject* tmpp0;
		tmpp0 = ScriptVector4::box(p0);
		MonoUtil::invokeThunk(onValueChangedThunk, getManagedInstance(), tmpp0);
	}

	void ScriptGUIVector4Field::onComponentChanged(float p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onComponentChangedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIVector4Field::onComponentFocusChanged(bool p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onComponentFocusChangedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIVector4Field::onConfirm(VectorComponent p0)
	{
		MonoUtil::invokeThunk(onConfirmThunk, getManagedInstance(), p0);
	}
	void ScriptGUIVector4Field::Internal_getValue(ScriptGUIVector4Field* thisPtr, Vector4* __output)
	{
		Vector4 tmp__output;
		tmp__output = static_cast<GUIVector4Field*>(thisPtr->getGUIElement())->getValue();

		*__output = tmp__output;
	}

	void ScriptGUIVector4Field::Internal_setValue(ScriptGUIVector4Field* thisPtr, Vector4* value)
	{
		static_cast<GUIVector4Field*>(thisPtr->getGUIElement())->setValue(*value);
	}

	bool ScriptGUIVector4Field::Internal_hasInputFocus(ScriptGUIVector4Field* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_cast<GUIVector4Field*>(thisPtr->getGUIElement())->hasInputFocus();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIVector4Field::Internal_setInputFocus(ScriptGUIVector4Field* thisPtr, VectorComponent component, bool focus)
	{
		static_cast<GUIVector4Field*>(thisPtr->getGUIElement())->setInputFocus(component, focus);
	}

	void ScriptGUIVector4Field::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector4Field* instance = GUIVector4Field::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIVector4Field>())ScriptGUIVector4Field(managedInstance, instance);
	}

	void ScriptGUIVector4Field::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector4Field* instance = GUIVector4Field::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIVector4Field>())ScriptGUIVector4Field(managedInstance, instance);
	}

	void ScriptGUIVector4Field::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector4Field* instance = GUIVector4Field::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIVector4Field>())ScriptGUIVector4Field(managedInstance, instance);
	}

	void ScriptGUIVector4Field::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector4Field* instance = GUIVector4Field::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIVector4Field>())ScriptGUIVector4Field(managedInstance, instance);
	}

	void ScriptGUIVector4Field::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector4Field* instance = GUIVector4Field::create(tmpstyle);
		new (bs_alloc<ScriptGUIVector4Field>())ScriptGUIVector4Field(managedInstance, instance);
	}
}
