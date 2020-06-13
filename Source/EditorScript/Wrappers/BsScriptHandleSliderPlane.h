//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "BsScriptObject.h"
#include "Wrappers/BsScriptHandleSlider.h"
#include "Handles/BsHandleSliderPlane.h"
#include "Math/BsVector3.h"

namespace bs
{
	/** @addtogroup ScriptInteropEditor
	 *  @{
	 */

	/**	Interop class between C++ & CLR for HandleSliderPlane. */
	class BS_SCR_BED_EXPORT ScriptHandleSliderPlane : public ScriptObject <ScriptHandleSliderPlane, ScriptHandleSliderBase>
	{
	public:
		SCRIPT_OBJ(EDITOR_ASSEMBLY, EDITOR_NS, "HandleSliderPlane")

	protected:
		/** @copydoc ScriptHandleSliderBase::getSlider */
		HandleSlider* getSlider() const override { return mSlider; }

		/** @copydoc ScriptHandleSliderBase::getSlider */
		void destroyInternal() override;

	private:
		ScriptHandleSliderPlane(MonoObject* instance, const Vector3& dir1, const Vector3& dir2, float length, 
			bool fixedScale, UINT64 layer);
		~ScriptHandleSliderPlane();

		HandleSliderPlane* mSlider;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_CreateInstance(MonoObject* instance, Vector3* dir1, Vector3* dir2, float length, 
			bool fixedScale, UINT64 layer);
		static void internal_GetDelta(ScriptHandleSliderPlane* nativeInstance, Vector2* value);
	};

	/** @} */
}