//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptCurveDrawInfo.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "Image/BsColor.h"
#include "Wrappers/BsScriptColor.h"
#include "../../bsf/Source/Foundation/bsfCore/Animation/BsAnimationCurve.h"
#include "BsScriptTAnimationCurve.generated.h"

namespace bs
{
	ScriptCurveDrawInfo::ScriptCurveDrawInfo(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptCurveDrawInfo::initRuntimeData()
	{ }

	MonoObject*ScriptCurveDrawInfo::box(const __CurveDrawInfoInterop& value)
	{
		return MonoUtil::box(metaData.scriptClass->_getInternalClass(), (void*)&value);
	}

	__CurveDrawInfoInterop ScriptCurveDrawInfo::unbox(MonoObject* value)
	{
		return *(__CurveDrawInfoInterop*)MonoUtil::unbox(value);
	}

	CurveDrawInfo ScriptCurveDrawInfo::fromInterop(const __CurveDrawInfoInterop& value)
	{
		CurveDrawInfo output;
		SPtr<TAnimationCurve<float>> tmpcurve;
		ScriptTAnimationCurvefloat* scriptcurve;
		scriptcurve = ScriptTAnimationCurvefloat::toNative(value.curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->getInternal();
		if(tmpcurve != nullptr)
		output.curve = *tmpcurve;
		output.color = value.color;

		return output;
	}

	__CurveDrawInfoInterop ScriptCurveDrawInfo::toInterop(const CurveDrawInfo& value)
	{
		__CurveDrawInfoInterop output;
		MonoObject* tmpcurve;
		SPtr<TAnimationCurve<float>> tmpcurvecopy;
		tmpcurvecopy = bs_shared_ptr_new<TAnimationCurve<float>>(value.curve);
		tmpcurve = ScriptTAnimationCurvefloat::create(tmpcurvecopy);
		output.curve = tmpcurve;
		output.color = value.color;

		return output;
	}

}
