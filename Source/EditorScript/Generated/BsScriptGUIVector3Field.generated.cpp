//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIVector3Field.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIVector3Field.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptGUIContent.generated.h"
#include "../../EditorCore/GUI/BsGUIVector3Field.h"
#include "BsScriptHString.generated.h"

namespace bs
{
	ScriptGUIVector3Field::onValueChangedThunkDef ScriptGUIVector3Field::onValueChangedThunk; 
	ScriptGUIVector3Field::onComponentChangedThunkDef ScriptGUIVector3Field::onComponentChangedThunk; 
	ScriptGUIVector3Field::onComponentFocusChangedThunkDef ScriptGUIVector3Field::onComponentFocusChangedThunk; 
	ScriptGUIVector3Field::onConfirmThunkDef ScriptGUIVector3Field::onConfirmThunk; 

	ScriptGUIVector3Field::ScriptGUIVector3Field(MonoObject* managedInstance, GUIVector3Field* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onValueChanged.connect(std::bind(&ScriptGUIVector3Field::onValueChanged, this, std::placeholders::_1));
		value->onComponentChanged.connect(std::bind(&ScriptGUIVector3Field::onComponentChanged, this, std::placeholders::_1, std::placeholders::_2));
		value->onComponentFocusChanged.connect(std::bind(&ScriptGUIVector3Field::onComponentFocusChanged, this, std::placeholders::_1, std::placeholders::_2));
		value->onConfirm.connect(std::bind(&ScriptGUIVector3Field::onConfirm, this, std::placeholders::_1));
	}

	void ScriptGUIVector3Field::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIVector3Field::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIVector3Field::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_hasInputFocus", (void*)&ScriptGUIVector3Field::Internal_hasInputFocus);
		metaData.scriptClass->addInternalCall("Internal_setInputFocus", (void*)&ScriptGUIVector3Field::Internal_setInputFocus);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIVector3Field::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIVector3Field::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIVector3Field::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIVector3Field::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIVector3Field::Internal_create3);

		onValueChangedThunk = (onValueChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onValueChanged", "Vector3&")->getThunk();
		onComponentChangedThunk = (onComponentChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onComponentChanged", "single,VectorComponent")->getThunk();
		onComponentFocusChangedThunk = (onComponentFocusChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onComponentFocusChanged", "bool,VectorComponent")->getThunk();
		onConfirmThunk = (onConfirmThunkDef)metaData.scriptClass->getMethodExact("Internal_onConfirm", "VectorComponent")->getThunk();
	}

	void ScriptGUIVector3Field::onValueChanged(const Vector3& p0)
	{
		MonoObject* tmpp0;
		tmpp0 = ScriptVector3::box(p0);
		MonoUtil::invokeThunk(onValueChangedThunk, getManagedInstance(), tmpp0);
	}

	void ScriptGUIVector3Field::onComponentChanged(float p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onComponentChangedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIVector3Field::onComponentFocusChanged(bool p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onComponentFocusChangedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIVector3Field::onConfirm(VectorComponent p0)
	{
		MonoUtil::invokeThunk(onConfirmThunk, getManagedInstance(), p0);
	}
	void ScriptGUIVector3Field::Internal_getValue(ScriptGUIVector3Field* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = static_cast<GUIVector3Field*>(thisPtr->getGUIElement())->getValue();

		*__output = tmp__output;
	}

	void ScriptGUIVector3Field::Internal_setValue(ScriptGUIVector3Field* thisPtr, Vector3* value)
	{
		static_cast<GUIVector3Field*>(thisPtr->getGUIElement())->setValue(*value);
	}

	bool ScriptGUIVector3Field::Internal_hasInputFocus(ScriptGUIVector3Field* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_cast<GUIVector3Field*>(thisPtr->getGUIElement())->hasInputFocus();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIVector3Field::Internal_setInputFocus(ScriptGUIVector3Field* thisPtr, VectorComponent component, bool focus)
	{
		static_cast<GUIVector3Field*>(thisPtr->getGUIElement())->setInputFocus(component, focus);
	}

	void ScriptGUIVector3Field::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector3Field* instance = GUIVector3Field::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIVector3Field>())ScriptGUIVector3Field(managedInstance, instance);
	}

	void ScriptGUIVector3Field::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector3Field* instance = GUIVector3Field::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIVector3Field>())ScriptGUIVector3Field(managedInstance, instance);
	}

	void ScriptGUIVector3Field::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector3Field* instance = GUIVector3Field::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIVector3Field>())ScriptGUIVector3Field(managedInstance, instance);
	}

	void ScriptGUIVector3Field::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector3Field* instance = GUIVector3Field::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIVector3Field>())ScriptGUIVector3Field(managedInstance, instance);
	}

	void ScriptGUIVector3Field::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIVector3Field* instance = GUIVector3Field::create(tmpstyle);
		new (bs_alloc<ScriptGUIVector3Field>())ScriptGUIVector3Field(managedInstance, instance);
	}
}
