//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptGUICurves.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/GUI/BsGUICurves.h"
#include "BsScriptKeyframeRef.generated.h"
#include "BsScriptCurveDrawInfo.generated.h"
#include "../../EditorCore/GUI/BsGUICurves.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptTangentRef.generated.h"
#include "Wrappers/BsScriptVector2I.h"

namespace bs
{
	ScriptGUICurves::onClickedThunkDef ScriptGUICurves::onClickedThunk; 

	ScriptGUICurves::ScriptGUICurves(MonoObject* managedInstance, GUICurves* value)
		:TScriptGUIElement(managedInstance, value)
	{
		value->onClicked.connect(std::bind(&ScriptGUICurves::onClicked, this));
	}

	void ScriptGUICurves::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setCurves", (void*)&ScriptGUICurves::Internal_setCurves);
		metaData.scriptClass->addInternalCall("Internal_getCurves", (void*)&ScriptGUICurves::Internal_getCurves);
		metaData.scriptClass->addInternalCall("Internal_setRange", (void*)&ScriptGUICurves::Internal_setRange);
		metaData.scriptClass->addInternalCall("Internal_setOffset", (void*)&ScriptGUICurves::Internal_setOffset);
		metaData.scriptClass->addInternalCall("Internal_centerAndZoom", (void*)&ScriptGUICurves::Internal_centerAndZoom);
		metaData.scriptClass->addInternalCall("Internal_pixelToCurveSpace", (void*)&ScriptGUICurves::Internal_pixelToCurveSpace);
		metaData.scriptClass->addInternalCall("Internal_curveToPixelSpace", (void*)&ScriptGUICurves::Internal_curveToPixelSpace);
		metaData.scriptClass->addInternalCall("Internal_findCurve", (void*)&ScriptGUICurves::Internal_findCurve);
		metaData.scriptClass->addInternalCall("Internal_findKeyFrame", (void*)&ScriptGUICurves::Internal_findKeyFrame);
		metaData.scriptClass->addInternalCall("Internal_findTangent", (void*)&ScriptGUICurves::Internal_findTangent);
		metaData.scriptClass->addInternalCall("Internal_selectKeyframe", (void*)&ScriptGUICurves::Internal_selectKeyframe);
		metaData.scriptClass->addInternalCall("Internal_clearSelectedKeyframes", (void*)&ScriptGUICurves::Internal_clearSelectedKeyframes);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptGUICurves::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptGUICurves::Internal_create0);

		onClickedThunk = (onClickedThunkDef)metaData.scriptClass->getMethodExact("Internal_onClicked", "")->getThunk();
	}

	void ScriptGUICurves::onClicked()
	{
		MonoUtil::invokeThunk(onClickedThunk, getManagedInstance());
	}
	void ScriptGUICurves::Internal_setCurves(ScriptGUICurves* thisPtr, MonoArray* curves)
	{
		Vector<CurveDrawInfo> veccurves;
		if(curves != nullptr)
		{
			ScriptArray arraycurves(curves);
			veccurves.resize(arraycurves.size());
			for(int i = 0; i < (int)arraycurves.size(); i++)
			{
				veccurves[i] = ScriptCurveDrawInfo::fromInterop(arraycurves.get<__CurveDrawInfoInterop>(i));
			}
		}
		static_cast<GUICurves*>(thisPtr->getGUIElement())->setCurves(veccurves);
	}

	MonoArray* ScriptGUICurves::Internal_getCurves(ScriptGUICurves* thisPtr)
	{
		Vector<CurveDrawInfo> vec__output;
		vec__output = static_cast<GUICurves*>(thisPtr->getGUIElement())->getCurves();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptCurveDrawInfo>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, ScriptCurveDrawInfo::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptGUICurves::Internal_setRange(ScriptGUICurves* thisPtr, float xRange, float yRange)
	{
		static_cast<GUICurves*>(thisPtr->getGUIElement())->setRange(xRange, yRange);
	}

	void ScriptGUICurves::Internal_setOffset(ScriptGUICurves* thisPtr, Vector2* offset)
	{
		static_cast<GUICurves*>(thisPtr->getGUIElement())->setOffset(*offset);
	}

	void ScriptGUICurves::Internal_centerAndZoom(ScriptGUICurves* thisPtr)
	{
		static_cast<GUICurves*>(thisPtr->getGUIElement())->centerAndZoom();
	}

	bool ScriptGUICurves::Internal_pixelToCurveSpace(ScriptGUICurves* thisPtr, Vector2I* pixelCoords, Vector2* curveCoords, bool padding)
	{
		bool tmp__output;
		tmp__output = static_cast<GUICurves*>(thisPtr->getGUIElement())->pixelToCurveSpace(*pixelCoords, *curveCoords, padding);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUICurves::Internal_curveToPixelSpace(ScriptGUICurves* thisPtr, Vector2* curveCoords, Vector2I* __output)
	{
		Vector2I tmp__output;
		tmp__output = static_cast<GUICurves*>(thisPtr->getGUIElement())->curveToPixelSpace(*curveCoords);

		*__output = tmp__output;
	}

	uint32_t ScriptGUICurves::Internal_findCurve(ScriptGUICurves* thisPtr, Vector2I* pixelCoords)
	{
		uint32_t tmp__output;
		tmp__output = static_cast<GUICurves*>(thisPtr->getGUIElement())->findCurve(*pixelCoords);

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptGUICurves::Internal_findKeyFrame(ScriptGUICurves* thisPtr, Vector2I* pixelCoords, KeyframeRef* keyframe)
	{
		bool tmp__output;
		tmp__output = static_cast<GUICurves*>(thisPtr->getGUIElement())->findKeyFrame(*pixelCoords, *keyframe);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptGUICurves::Internal_findTangent(ScriptGUICurves* thisPtr, Vector2I* pixelCoords, __TangentRefInterop* tangent)
	{
		bool tmp__output;
		TangentRef tmptangent;
		tmp__output = static_cast<GUICurves*>(thisPtr->getGUIElement())->findTangent(*pixelCoords, tmptangent);

		bool __output;
		__output = tmp__output;
		__TangentRefInterop interoptangent;
		interoptangent = ScriptTangentRef::toInterop(tmptangent);
		MonoUtil::valueCopy(tangent, &interoptangent, ScriptTangentRef::getMetaData()->scriptClass->_getInternalClass());

		return __output;
	}

	void ScriptGUICurves::Internal_selectKeyframe(ScriptGUICurves* thisPtr, KeyframeRef* keyframeRef, TangentModeBits tangentMode, bool selected)
	{
		static_cast<GUICurves*>(thisPtr->getGUIElement())->selectKeyframe(*keyframeRef, tangentMode, selected);
	}

	void ScriptGUICurves::Internal_clearSelectedKeyframes(ScriptGUICurves* thisPtr)
	{
		static_cast<GUICurves*>(thisPtr->getGUIElement())->clearSelectedKeyframes();
	}

	void ScriptGUICurves::Internal_create(MonoObject* managedInstance, MonoString* styleName)
	{
		String tmpstyleName;
		tmpstyleName = MonoUtil::monoToString(styleName);
		GUICurves* instance = GUICurves::create(tmpstyleName);
		new (bs_alloc<ScriptGUICurves>())ScriptGUICurves(managedInstance, instance);
	}

	void ScriptGUICurves::Internal_create0(MonoObject* managedInstance, CurveDrawOption drawOptions, MonoString* styleName)
	{
		String tmpstyleName;
		tmpstyleName = MonoUtil::monoToString(styleName);
		GUICurves* instance = GUICurves::create(drawOptions, tmpstyleName);
		new (bs_alloc<ScriptGUICurves>())ScriptGUICurves(managedInstance, instance);
	}
}
