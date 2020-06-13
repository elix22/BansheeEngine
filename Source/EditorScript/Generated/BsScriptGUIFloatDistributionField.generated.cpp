//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIFloatDistributionField.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIFloatDistributionField.h"
#include "BsScriptTDistribution.generated.h"
#include "BsScriptHString.generated.h"
#include "BsScriptGUIContent.generated.h"
#include "../../EditorCore/GUI/BsGUIFloatDistributionField.h"

namespace bs
{
	ScriptGUIFloatDistributionField::onClickedThunkDef ScriptGUIFloatDistributionField::onClickedThunk; 
	ScriptGUIFloatDistributionField::onConstantModifiedThunkDef ScriptGUIFloatDistributionField::onConstantModifiedThunk; 
	ScriptGUIFloatDistributionField::onConstantConfirmedThunkDef ScriptGUIFloatDistributionField::onConstantConfirmedThunk; 
	ScriptGUIFloatDistributionField::onConstantFocusChangedThunkDef ScriptGUIFloatDistributionField::onConstantFocusChangedThunk; 

	ScriptGUIFloatDistributionField::ScriptGUIFloatDistributionField(MonoObject* managedInstance, GUIFloatDistributionField* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUIFloatDistributionField::onClicked, this, std::placeholders::_1));
		value->onConstantModified.connect(std::bind(&ScriptGUIFloatDistributionField::onConstantModified, this, std::placeholders::_1, std::placeholders::_2));
		value->onConstantConfirmed.connect(std::bind(&ScriptGUIFloatDistributionField::onConstantConfirmed, this, std::placeholders::_1, std::placeholders::_2));
		value->onConstantFocusChanged.connect(std::bind(&ScriptGUIFloatDistributionField::onConstantFocusChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	void ScriptGUIFloatDistributionField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIFloatDistributionField::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIFloatDistributionField::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptGUIFloatDistributionField::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_hasInputFocus", (void*)&ScriptGUIFloatDistributionField::Internal_hasInputFocus);
		metaData.scriptClass->addInternalCall("Internal_setInputFocus", (void*)&ScriptGUIFloatDistributionField::Internal_setInputFocus);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIFloatDistributionField::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIFloatDistributionField::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIFloatDistributionField::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIFloatDistributionField::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIFloatDistributionField::Internal_create3);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "VectorComponent")->getThunk();
		onConstantModifiedThunk = (onConstantModifiedThunkDef)metaData.scriptClass->getMethodExact("Internal_onConstantModified", "RangeComponent,VectorComponent")->getThunk();
		onConstantConfirmedThunk = (onConstantConfirmedThunkDef)metaData.scriptClass->getMethodExact("Internal_onConstantConfirmed", "RangeComponent,VectorComponent")->getThunk();
		onConstantFocusChangedThunk = (onConstantFocusChangedThunkDef)metaData.scriptClass->getMethodExact("Internal_onConstantFocusChanged", "bool,RangeComponent,VectorComponent")->getThunk();
	}

	void ScriptGUIFloatDistributionField::onClicked(VectorComponent p0)
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance(), p0);
	}

	void ScriptGUIFloatDistributionField::onConstantModified(RangeComponent p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onConstantModifiedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIFloatDistributionField::onConstantConfirmed(RangeComponent p0, VectorComponent p1)
	{
		MonoUtil::invokeThunk(onConstantConfirmedThunk, getManagedInstance(), p0, p1);
	}

	void ScriptGUIFloatDistributionField::onConstantFocusChanged(bool p0, RangeComponent p1, VectorComponent p2)
	{
		MonoUtil::invokeThunk(onConstantFocusChangedThunk, getManagedInstance(), p0, p1, p2);
	}
	MonoObject* ScriptGUIFloatDistributionField::Internal_getValue(ScriptGUIFloatDistributionField* thisPtr)
	{
		SPtr<TDistribution<float>> tmp__output = bs_shared_ptr_new<TDistribution<float>>();
		*tmp__output = static_cast<GUIFloatDistributionField*>(thisPtr->getGUIElement())->getValue();

		MonoObject* __output;
		__output = ScriptTDistributionfloat::create(tmp__output);

		return __output;
	}

	void ScriptGUIFloatDistributionField::Internal_setValue(ScriptGUIFloatDistributionField* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<float>> tmpvalue;
		ScriptTDistributionfloat* scriptvalue;
		scriptvalue = ScriptTDistributionfloat::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		static_cast<GUIFloatDistributionField*>(thisPtr->getGUIElement())->setValue(*tmpvalue);
	}

	PropertyDistributionType ScriptGUIFloatDistributionField::Internal_getType(ScriptGUIFloatDistributionField* thisPtr)
	{
		PropertyDistributionType tmp__output;
		tmp__output = static_cast<GUIFloatDistributionField*>(thisPtr->getGUIElement())->getType();

		PropertyDistributionType __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptGUIFloatDistributionField::Internal_hasInputFocus(ScriptGUIFloatDistributionField* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_cast<GUIFloatDistributionField*>(thisPtr->getGUIElement())->hasInputFocus();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIFloatDistributionField::Internal_setInputFocus(ScriptGUIFloatDistributionField* thisPtr, RangeComponent rangeComponent, VectorComponent vectorComponent, bool focus)
	{
		static_cast<GUIFloatDistributionField*>(thisPtr->getGUIElement())->setInputFocus(rangeComponent, vectorComponent, focus);
	}

	void ScriptGUIFloatDistributionField::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIFloatDistributionField* instance = GUIFloatDistributionField::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIFloatDistributionField>())ScriptGUIFloatDistributionField(managedInstance, instance);
	}

	void ScriptGUIFloatDistributionField::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIFloatDistributionField* instance = GUIFloatDistributionField::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIFloatDistributionField>())ScriptGUIFloatDistributionField(managedInstance, instance);
	}

	void ScriptGUIFloatDistributionField::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIFloatDistributionField* instance = GUIFloatDistributionField::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIFloatDistributionField>())ScriptGUIFloatDistributionField(managedInstance, instance);
	}

	void ScriptGUIFloatDistributionField::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIFloatDistributionField* instance = GUIFloatDistributionField::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIFloatDistributionField>())ScriptGUIFloatDistributionField(managedInstance, instance);
	}

	void ScriptGUIFloatDistributionField::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIFloatDistributionField* instance = GUIFloatDistributionField::create(tmpstyle);
		new (bs_alloc<ScriptGUIFloatDistributionField>())ScriptGUIFloatDistributionField(managedInstance, instance);
	}
}
