//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "EditorWindow/BsEditorWidgetLayout.h"
#include "EditorWindow/BsDockManagerLayout.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "RTTI/BsStringRTTI.h"
#include "RTTI/BsStdRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Editor
	 *  @{
	 */

	class EditorWidgetLayoutRTTI : public RTTIType<EditorWidgetLayout, IReflectable, EditorWidgetLayoutRTTI>
	{
	private:
		EditorWidgetLayout::Entry& getEntry(EditorWidgetLayout* obj, UINT32 idx) { return obj->mEntries[idx]; }
		void setEntry(EditorWidgetLayout* obj, UINT32 idx, EditorWidgetLayout::Entry& val) { obj->mEntries[idx] = val; } 
		UINT32 getEntriesArraySize(EditorWidgetLayout* obj) { return (UINT32)obj->mEntries.size(); }
		void setEntriesArraySize(EditorWidgetLayout* obj, UINT32 size) { obj->mEntries.resize(size); }

		SPtr<DockManagerLayout> getDockLayout(EditorWidgetLayout* obj) { return obj->mDockLayout; }
		void setDockLayout(EditorWidgetLayout* obj, SPtr<DockManagerLayout> val) { obj->mDockLayout = val; }

		bool& getIsMainWindowMaximized(EditorWidgetLayout* obj) { return obj->mMaximized; }
		void setIsMainWindowMaximized(EditorWidgetLayout* obj, bool& val) { obj->mMaximized = val; }

	public:
		EditorWidgetLayoutRTTI()
		{
			addPlainArrayField("mRootEntry", 0, &EditorWidgetLayoutRTTI::getEntry, &EditorWidgetLayoutRTTI::getEntriesArraySize, 
				&EditorWidgetLayoutRTTI::setEntry, &EditorWidgetLayoutRTTI::setEntriesArraySize);

			addReflectablePtrField("mDockLayout", 1, &EditorWidgetLayoutRTTI::getDockLayout, &EditorWidgetLayoutRTTI::setDockLayout);
			addPlainField("mMaximized", 2, &EditorWidgetLayoutRTTI::getIsMainWindowMaximized, &EditorWidgetLayoutRTTI::setIsMainWindowMaximized);
		}

		const String& getRTTIName() override
		{
			static String name = "EditorWidgetLayout";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_EditorWidgetLayout;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<EditorWidgetLayout>(EditorWidgetLayout::PrivatelyConstruct());
		}
	};

	template<> struct RTTIPlainType<bs::EditorWidgetLayout::Entry>
	{	
		enum { id = bs::TID_EditorWidgetLayoutEntry }; enum { hasDynamicSize = 1 };

		static BitLength toMemory(const bs::EditorWidgetLayout::Entry& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{ 
			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(data.widgetNames, stream);
				size += rtti_write(data.isDocked, stream);
				size += rtti_write(data.x, stream);
				size += rtti_write(data.y, stream);
				size += rtti_write(data.width, stream);
				size += rtti_write(data.height, stream);

				return size;
			});
		}

		static BitLength fromMemory(bs::EditorWidgetLayout::Entry& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{ 
			BitLength size;
			rtti_read_size_header(stream, compress, size);

			rtti_read(data.widgetNames, stream);
			rtti_read(data.isDocked, stream);
			rtti_read(data.x, stream);
			rtti_read(data.y, stream);
			rtti_read(data.width, stream);
			rtti_read(data.height, stream);

			return size;
		}

		static BitLength getSize(const bs::EditorWidgetLayout::Entry& data, const RTTIFieldInfo& fieldInfo, bool compress)	
		{ 
			BitLength dataSize = rtti_size(data.widgetNames) + rtti_size(data.isDocked) + rtti_size(data.x) +
				rtti_size(data.y) + rtti_size(data.width) + rtti_size(data.height);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}	
	}; 

	/** @} */
	/** @endcond */
}