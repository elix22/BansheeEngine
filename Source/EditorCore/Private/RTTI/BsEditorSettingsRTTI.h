//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Settings/BsEditorSettings.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "RTTI/BsPathRTTI.h"
#include "RTTI/BsMathRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Editor
	 *  @{
	 */

	class EditorSettingsRTTI : public RTTIType <EditorSettings, Settings, EditorSettingsRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(mMoveSnapActive, 0)
			BS_RTTI_MEMBER_PLAIN(mRotateSnapActive, 1)

			BS_RTTI_MEMBER_PLAIN(mMoveSnap, 2)
			BS_RTTI_MEMBER_PLAIN(mRotationSnap, 3)

			BS_RTTI_MEMBER_PLAIN(mGridSize, 4)
			BS_RTTI_MEMBER_PLAIN(mGridAxisSpacing, 5)

			BS_RTTI_MEMBER_PLAIN(mActiveSceneTool, 6)
			BS_RTTI_MEMBER_PLAIN(mActiveCoordinateMode, 7)
			BS_RTTI_MEMBER_PLAIN(mActivePivotMode, 8)

			BS_RTTI_MEMBER_PLAIN(mHandleSize, 9)

			BS_RTTI_MEMBER_PLAIN(mLastOpenProject, 10)
			BS_RTTI_MEMBER_PLAIN(mAutoLoadLastProject, 11)
			BS_RTTI_MEMBER_PLAIN(mRecentProjects, 12)

			BS_RTTI_MEMBER_PLAIN(mFPSLimit, 13)
			BS_RTTI_MEMBER_PLAIN(mMouseSensitivity, 14)
		BS_END_RTTI_MEMBERS
	public:
		EditorSettingsRTTI()
			:mInitMembers(this)
		{ }

		const String& getRTTIName() override
		{
			static String name = "EditorSettings";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_EditorSettings;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<EditorSettings>();
		}
	};

	template<> struct RTTIPlainType <RecentProject>
	{
		enum { id = TID_RecentProject }; enum { hasDynamicSize = 1 };

		static BitLength toMemory(const RecentProject& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(data.path, stream);
				size += rtti_write(data.accessTimestamp, stream);

				return size;
			});
		}

		static BitLength fromMemory(RecentProject& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			rtti_read_size_header(stream, compress, size);

			rtti_read(data.path, stream);
			rtti_read(data.accessTimestamp, stream);

			return size;
		}

		static BitLength getSize(const RecentProject& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = rtti_size(data.path) + rtti_size(data.accessTimestamp);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	/** @} */
	/** @endcond */
}