//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/GUI/BsScriptGUISliderField.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUISliderField.h"
#include "GUI/BsGUIOptions.h"
#include "GUI/BsGUIContent.h"

#include "Generated/BsScriptGUIContent.generated.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUISliderField::OnChangedThunkDef ScriptGUISliderField::onChangedThunk;

	ScriptGUISliderField::ScriptGUISliderField(MonoObject* instance, GUISliderField* sliderField)
		:TScriptGUIElement(instance, sliderField)
	{

	}

	void ScriptGUISliderField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptGUISliderField::internal_createInstance);
		metaData.scriptClass->addInternalCall("Internal_GetValue", (void*)&ScriptGUISliderField::internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_GetStep", (void*)&ScriptGUISliderField::internal_getStep);
		metaData.scriptClass->addInternalCall("Internal_SetValue", (void*)&ScriptGUISliderField::internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_HasInputFocus", (void*)&ScriptGUISliderField::internal_hasInputFocus);
		metaData.scriptClass->addInternalCall("Internal_SetTint", (void*)&ScriptGUISliderField::internal_setTint);
		metaData.scriptClass->addInternalCall("Internal_SetRange", (void*)&ScriptGUISliderField::internal_setRange);
		metaData.scriptClass->addInternalCall("Internal_SetStep", (void*)&ScriptGUISliderField::internal_setStep);

		onChangedThunk = (OnChangedThunkDef)metaData.scriptClass->getMethod("DoOnChanged", 1)->getThunk();
	}

	void ScriptGUISliderField::internal_createInstance(MonoObject* instance, float min, float max, 
		__GUIContentInterop* title, UINT32 titleWidth, MonoString* style, MonoArray* guiOptions, bool withTitle)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		String styleName = MonoUtil::monoToString(style);

		GUISliderField* guiSliderField = nullptr;
		if (withTitle)
		{
			GUIContent nativeContent = ScriptGUIContent::fromInterop(*title);
			guiSliderField = GUISliderField::create(nativeContent, titleWidth, options, styleName);
		}
		else
		{
			guiSliderField = GUISliderField::create(options, styleName);
		}

		auto nativeInstance = new (bs_alloc<ScriptGUISliderField>()) ScriptGUISliderField(instance, guiSliderField);

		guiSliderField->setRange(min, max);
		guiSliderField->onValueChanged.connect(std::bind(&ScriptGUISliderField::onChanged, nativeInstance, _1));
	}

	float ScriptGUISliderField::internal_getValue(ScriptGUISliderField* nativeInstance)
	{
		GUISliderField* sliderField = static_cast<GUISliderField*>(nativeInstance->getGUIElement());
		return sliderField->getValue();
	}

	float ScriptGUISliderField::internal_getStep(ScriptGUISliderField* nativeInstance)
	{
		GUISliderField* sliderField = static_cast<GUISliderField*>(nativeInstance->getGUIElement());
		return sliderField->getStep();
	}

	float ScriptGUISliderField::internal_setValue(ScriptGUISliderField* nativeInstance, float value)
	{
		GUISliderField* sliderField = static_cast<GUISliderField*>(nativeInstance->getGUIElement());
		return sliderField->setValue(value);
	}

	void ScriptGUISliderField::internal_hasInputFocus(ScriptGUISliderField* nativeInstance, bool* output)
	{
		GUISliderField* sliderField = static_cast<GUISliderField*>(nativeInstance->getGUIElement());
		*output = sliderField->hasInputFocus();
	}

	void ScriptGUISliderField::internal_setTint(ScriptGUISliderField* nativeInstance, Color* color)
	{
		GUISliderField* sliderField = (GUISliderField*)nativeInstance->getGUIElement();
		sliderField->setTint(*color);
	}

	void ScriptGUISliderField::internal_setRange(ScriptGUISliderField* nativeInstance, float min, float max)
	{
		GUISliderField* sliderField = static_cast<GUISliderField*>(nativeInstance->getGUIElement());
		sliderField->setRange(min, max);
	}

	void ScriptGUISliderField::internal_setStep(ScriptGUISliderField* nativeInstance, float step)
	{
		GUISliderField* sliderField = static_cast<GUISliderField*>(nativeInstance->getGUIElement());
		sliderField->setStep(step);
	}

	void ScriptGUISliderField::onChanged(float newValue)
	{
		MonoUtil::invokeThunk(onChangedThunk, getManagedInstance(), newValue);
	}
}