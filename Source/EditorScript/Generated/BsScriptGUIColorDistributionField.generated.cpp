//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUIColorDistributionField.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUIColorDistributionField.h"
#include "../../EditorCore/GUI/BsGUIColorDistributionField.h"
#include "BsScriptTColorDistribution.generated.h"
#include "BsScriptHString.generated.h"
#include "BsScriptGUIContent.generated.h"

namespace bs
{
	ScriptGUIColorDistributionField::onMinClickedThunkDef ScriptGUIColorDistributionField::onMinClickedThunk; 
	ScriptGUIColorDistributionField::onMaxClickedThunkDef ScriptGUIColorDistributionField::onMaxClickedThunk; 

	ScriptGUIColorDistributionField::ScriptGUIColorDistributionField(MonoObject* managedInstance, GUIColorDistributionField* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onMinClicked.connect(std::bind(&ScriptGUIColorDistributionField::onMinClicked, this));
		value->onMaxClicked.connect(std::bind(&ScriptGUIColorDistributionField::onMaxClicked, this));
	}

	void ScriptGUIColorDistributionField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getValue", (void*)&ScriptGUIColorDistributionField::Internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_setValue", (void*)&ScriptGUIColorDistributionField::Internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptGUIColorDistributionField::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUIColorDistributionField::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUIColorDistributionField::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUIColorDistributionField::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUIColorDistributionField::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUIColorDistributionField::Internal_create3);

		onMinClickedThunk = (onMinClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onMinClicked", "")->getThunk();
		onMaxClickedThunk = (onMaxClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onMaxClicked", "")->getThunk();
	}

	void ScriptGUIColorDistributionField::onMinClicked()
	{
		MonoUtil::invokeThunk(onMinClickedThunk, getManagedInstance());
	}

	void ScriptGUIColorDistributionField::onMaxClicked()
	{
		MonoUtil::invokeThunk(onMaxClickedThunk, getManagedInstance());
	}
	MonoObject* ScriptGUIColorDistributionField::Internal_getValue(ScriptGUIColorDistributionField* thisPtr)
	{
		SPtr<TColorDistribution<ColorGradient>> tmp__output = bs_shared_ptr_new<TColorDistribution<ColorGradient>>();
		*tmp__output = static_cast<GUIColorDistributionField*>(thisPtr->getGUIElement())->getValue();

		MonoObject* __output;
		__output = ScriptTColorDistributionColorGradient::create(tmp__output);

		return __output;
	}

	void ScriptGUIColorDistributionField::Internal_setValue(ScriptGUIColorDistributionField* thisPtr, MonoObject* value)
	{
		SPtr<TColorDistribution<ColorGradient>> tmpvalue;
		ScriptTColorDistributionColorGradient* scriptvalue;
		scriptvalue = ScriptTColorDistributionColorGradient::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		static_cast<GUIColorDistributionField*>(thisPtr->getGUIElement())->setValue(*tmpvalue);
	}

	PropertyDistributionType ScriptGUIColorDistributionField::Internal_getType(ScriptGUIColorDistributionField* thisPtr)
	{
		PropertyDistributionType tmp__output;
		tmp__output = static_cast<GUIColorDistributionField*>(thisPtr->getGUIElement())->getType();

		PropertyDistributionType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIColorDistributionField::Internal_create(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorDistributionField* instance = GUIColorDistributionField::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorDistributionField>())ScriptGUIColorDistributionField(managedInstance, instance);
	}

	void ScriptGUIColorDistributionField::Internal_create0(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorDistributionField* instance = GUIColorDistributionField::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUIColorDistributionField>())ScriptGUIColorDistributionField(managedInstance, instance);
	}

	void ScriptGUIColorDistributionField::Internal_create1(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorDistributionField* instance = GUIColorDistributionField::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUIColorDistributionField>())ScriptGUIColorDistributionField(managedInstance, instance);
	}

	void ScriptGUIColorDistributionField::Internal_create2(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorDistributionField* instance = GUIColorDistributionField::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUIColorDistributionField>())ScriptGUIColorDistributionField(managedInstance, instance);
	}

	void ScriptGUIColorDistributionField::Internal_create3(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUIColorDistributionField* instance = GUIColorDistributionField::create(tmpstyle);
		new (bs_alloc<ScriptGUIColorDistributionField>())ScriptGUIColorDistributionField(managedInstance, instance);
	}
}
