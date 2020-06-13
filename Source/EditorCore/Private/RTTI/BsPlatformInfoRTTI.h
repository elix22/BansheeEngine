//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "Build/BsPlatformInfo.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Editor
	 *  @{
	 */

	class BS_ED_EXPORT PlatformInfoRTTI : public RTTIType <PlatformInfo, IReflectable, PlatformInfoRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(defines, 0)
			BS_RTTI_MEMBER_PLAIN(type, 1)
			//BS_RTTI_MEMBER_REFL(mainScene, 2)
			BS_RTTI_MEMBER_PLAIN(fullscreen, 3)
			BS_RTTI_MEMBER_PLAIN(windowedWidth, 4)
			BS_RTTI_MEMBER_PLAIN(windowedHeight, 5)
			BS_RTTI_MEMBER_PLAIN(debug, 6)
			BS_RTTI_MEMBER_REFL(mainScene, 7)
		BS_END_RTTI_MEMBERS

	public:
		const String& getRTTIName() override
		{
			static String name = "PlatformInfo";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_PlatformInfo;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<PlatformInfo>();
		}
	};

	class BS_ED_EXPORT WinPlatformInfoRTTI : public RTTIType <WinPlatformInfo, PlatformInfo, WinPlatformInfoRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			//BS_RTTI_MEMBER_REFL(icon, 9)
			BS_RTTI_MEMBER_PLAIN(titlebarText, 10)
			BS_RTTI_MEMBER_REFL(icon, 11)
		BS_END_RTTI_MEMBERS

	public:
		const String& getRTTIName() override
		{
			static String name = "WinPlatformInfo";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_WinPlatformInfo;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<WinPlatformInfo>();
		}
	};

	/** @} */
	/** @endcond */
}