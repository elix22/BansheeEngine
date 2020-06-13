//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "GUI/BsGUIWindowFrameWidget.h"
#include "Private/RTTI/BsGameObjectRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Editor
	 *  @{
	 */

	class BS_ED_EXPORT WindowFrameWidgetRTTI : public RTTIType <WindowFrameWidget, CGUIWidget, WindowFrameWidgetRTTI>
	{
	private:

	public:
		WindowFrameWidgetRTTI()
		{ }

		const String& getRTTIName() override
		{
			static String name = "WindowFrameWidget";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_WindowFrameWidget;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return SceneObject::createEmptyComponent<WindowFrameWidget>();
		}
	};

	/** @} */
	/** @endcond */
}