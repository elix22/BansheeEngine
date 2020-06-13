//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Build/BsPlatformInfo.h"
#include "Private/RTTI/BsPlatformInfoRTTI.h"

namespace bs
{
	PlatformInfo::PlatformInfo()
		:type(PlatformType::Windows), fullscreen(true), windowedWidth(1280), windowedHeight(720),
#ifdef DEBUG
        debug(true)
#else
        debug(false)
#endif
	{ }

	RTTITypeBase* PlatformInfo::getRTTIStatic()
	{
		return PlatformInfoRTTI::instance();
	}

	RTTITypeBase* PlatformInfo::getRTTI() const
	{
		return PlatformInfo::getRTTIStatic();
	}

	RTTITypeBase* WinPlatformInfo::getRTTIStatic()
	{
		return WinPlatformInfoRTTI::instance();
	}

	RTTITypeBase* WinPlatformInfo::getRTTI() const
	{
		return WinPlatformInfo::getRTTIStatic();
	}
}