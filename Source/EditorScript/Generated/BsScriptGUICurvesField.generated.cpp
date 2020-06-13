//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUICurvesField.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUICurvesField.h"
#include "BsScriptHString.generated.h"
#include "BsScriptTAnimationCurve.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptGUIContent.generated.h"
#include "../../EditorCore/GUI/BsGUICurvesField.h"

namespace bs
{
	ScriptGUICurvesField::onClickedThunkDef ScriptGUICurvesField::onClickedThunk; 

	ScriptGUICurvesField::ScriptGUICurvesField(MonoObject* managedInstance, GUICurvesField* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUICurvesField::onClicked, this));
	}

	void ScriptGUICurvesField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setCurve", (void*)&ScriptGUICurvesField::Internal_setCurve);
		metaData.scriptClass->addInternalCall("Internal_setCurveRange", (void*)&ScriptGUICurvesField::Internal_setCurveRange);
		metaData.scriptClass->addInternalCall("Internal_getCurve", (void*)&ScriptGUICurvesField::Internal_getCurve);
		metaData.scriptClass->addInternalCall("Internal_getMinCurve", (void*)&ScriptGUICurvesField::Internal_getMinCurve);
		metaData.scriptClass->addInternalCall("Internal_getMaxCurve", (void*)&ScriptGUICurvesField::Internal_getMaxCurve);
		metaData.scriptClass->addInternalCall("Internal_setRange", (void*)&ScriptGUICurvesField::Internal_setRange);
		metaData.scriptClass->addInternalCall("Internal_setOffset", (void*)&ScriptGUICurvesField::Internal_setOffset);
		metaData.scriptClass->addInternalCall("Internal_centerAndZoom", (void*)&ScriptGUICurvesField::Internal_centerAndZoom);
		metaData.scriptClass->addInternalCall("Internal_setPadding", (void*)&ScriptGUICurvesField::Internal_setPadding);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUICurvesField::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUICurvesField::Internal_create0);
		metaData.scriptClass->addInternalCall("Internal_create1", (void*)&ScriptGUICurvesField::Internal_create1);
		metaData.scriptClass->addInternalCall("Internal_create2", (void*)&ScriptGUICurvesField::Internal_create2);
		metaData.scriptClass->addInternalCall("Internal_create3", (void*)&ScriptGUICurvesField::Internal_create3);
		metaData.scriptClass->addInternalCall("Internal_create4", (void*)&ScriptGUICurvesField::Internal_create4);
		metaData.scriptClass->addInternalCall("Internal_create5", (void*)&ScriptGUICurvesField::Internal_create5);
		metaData.scriptClass->addInternalCall("Internal_create6", (void*)&ScriptGUICurvesField::Internal_create6);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "")->getThunk();
	}

	void ScriptGUICurvesField::onClicked()
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance());
	}
	void ScriptGUICurvesField::Internal_setCurve(ScriptGUICurvesField* thisPtr, MonoObject* curve)
	{
		SPtr<TAnimationCurve<float>> tmpcurve;
		ScriptTAnimationCurvefloat* scriptcurve;
		scriptcurve = ScriptTAnimationCurvefloat::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->getInternal();
		static_cast<GUICurvesField*>(thisPtr->getGUIElement())->setCurve(*tmpcurve);
	}

	void ScriptGUICurvesField::Internal_setCurveRange(ScriptGUICurvesField* thisPtr, MonoObject* curveA, MonoObject* curveB)
	{
		SPtr<TAnimationCurve<float>> tmpcurveA;
		ScriptTAnimationCurvefloat* scriptcurveA;
		scriptcurveA = ScriptTAnimationCurvefloat::toNative(curveA);
		if(scriptcurveA != nullptr)
			tmpcurveA = scriptcurveA->getInternal();
		SPtr<TAnimationCurve<float>> tmpcurveB;
		ScriptTAnimationCurvefloat* scriptcurveB;
		scriptcurveB = ScriptTAnimationCurvefloat::toNative(curveB);
		if(scriptcurveB != nullptr)
			tmpcurveB = scriptcurveB->getInternal();
		static_cast<GUICurvesField*>(thisPtr->getGUIElement())->setCurveRange(*tmpcurveA, *tmpcurveB);
	}

	MonoObject* ScriptGUICurvesField::Internal_getCurve(ScriptGUICurvesField* thisPtr)
	{
		SPtr<TAnimationCurve<float>> tmp__output = bs_shared_ptr_new<TAnimationCurve<float>>();
		*tmp__output = static_cast<GUICurvesField*>(thisPtr->getGUIElement())->getCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurvefloat::create(tmp__output);

		return __output;
	}

	MonoObject* ScriptGUICurvesField::Internal_getMinCurve(ScriptGUICurvesField* thisPtr)
	{
		SPtr<TAnimationCurve<float>> tmp__output = bs_shared_ptr_new<TAnimationCurve<float>>();
		*tmp__output = static_cast<GUICurvesField*>(thisPtr->getGUIElement())->getMinCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurvefloat::create(tmp__output);

		return __output;
	}

	MonoObject* ScriptGUICurvesField::Internal_getMaxCurve(ScriptGUICurvesField* thisPtr)
	{
		SPtr<TAnimationCurve<float>> tmp__output = bs_shared_ptr_new<TAnimationCurve<float>>();
		*tmp__output = static_cast<GUICurvesField*>(thisPtr->getGUIElement())->getMaxCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurvefloat::create(tmp__output);

		return __output;
	}

	void ScriptGUICurvesField::Internal_setRange(ScriptGUICurvesField* thisPtr, float xRange, float yRange)
	{
		static_cast<GUICurvesField*>(thisPtr->getGUIElement())->setRange(xRange, yRange);
	}

	void ScriptGUICurvesField::Internal_setOffset(ScriptGUICurvesField* thisPtr, Vector2* offset)
	{
		static_cast<GUICurvesField*>(thisPtr->getGUIElement())->setOffset(*offset);
	}

	void ScriptGUICurvesField::Internal_centerAndZoom(ScriptGUICurvesField* thisPtr)
	{
		static_cast<GUICurvesField*>(thisPtr->getGUIElement())->centerAndZoom();
	}

	void ScriptGUICurvesField::Internal_setPadding(ScriptGUICurvesField* thisPtr, uint32_t padding)
	{
		static_cast<GUICurvesField*>(thisPtr->getGUIElement())->setPadding(padding);
	}

	void ScriptGUICurvesField::Internal_create(MonoObject* managedInstance, CurveDrawOption drawOptions, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(drawOptions, tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}

	void ScriptGUICurvesField::Internal_create0(MonoObject* managedInstance, CurveDrawOption drawOptions, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(drawOptions, *tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}

	void ScriptGUICurvesField::Internal_create1(MonoObject* managedInstance, CurveDrawOption drawOptions, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(drawOptions, tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}

	void ScriptGUICurvesField::Internal_create2(MonoObject* managedInstance, __GUIContentInterop* labelContent, uint32_t labelWidth, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(tmplabelContent, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}

	void ScriptGUICurvesField::Internal_create3(MonoObject* managedInstance, __GUIContentInterop* labelContent, MonoString* style)
	{
		GUIContent tmplabelContent;
		tmplabelContent = ScriptGUIContent::fromInterop(*labelContent);
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(tmplabelContent, tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}

	void ScriptGUICurvesField::Internal_create4(MonoObject* managedInstance, MonoObject* labelText, uint32_t labelWidth, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(*tmplabelText, labelWidth, tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}

	void ScriptGUICurvesField::Internal_create5(MonoObject* managedInstance, MonoObject* labelText, MonoString* style)
	{
		SPtr<HString> tmplabelText;
		ScriptHString* scriptlabelText;
		scriptlabelText = ScriptHString::toNative(labelText);
		if(scriptlabelText != nullptr)
			tmplabelText = scriptlabelText->getInternal();
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(*tmplabelText, tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}

	void ScriptGUICurvesField::Internal_create6(MonoObject* managedInstance, MonoString* style)
	{
		String tmpstyle;
		tmpstyle = MonoUtil::monoToString(style);
		GUICurvesField* instance = GUICurvesField::create(tmpstyle);
		new (bs_alloc<ScriptGUICurvesField>())ScriptGUICurvesField(managedInstance, instance);
	}
}
