//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "EditorWindow/BsDockManagerLayout.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "RTTI/BsStdRTTI.h"
#include "RTTI/BsStringRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Editor
	 *  @{
	 */

	class DockManagerLayoutRTTI : public RTTIType<DockManagerLayout, IReflectable, DockManagerLayoutRTTI>
	{
	private:
		DockManagerLayout::Entry& getRootEntry(DockManagerLayout* obj) { return obj->mRootEntry; }
		void setRootEntry(DockManagerLayout* obj, DockManagerLayout::Entry& val) { obj->mRootEntry = val; } 

		bool& getIsMaximized(DockManagerLayout* obj) { return obj->mIsMaximized; }
		void setIsMaximized(DockManagerLayout* obj, bool& val) { obj->mIsMaximized = val; }

		Vector<String>& getMaximizedWidgetNames(DockManagerLayout* obj) { return obj->mMaximizedWidgetNames; }
		void setMaximizedWidgetNames(DockManagerLayout* obj, Vector<String>& val) { obj->mMaximizedWidgetNames = val; }

	public:
		DockManagerLayoutRTTI()
		{
			addPlainField("mRootEntry", 0, &DockManagerLayoutRTTI::getRootEntry, &DockManagerLayoutRTTI::setRootEntry);
			addPlainField("mIsMaximized", 1, &DockManagerLayoutRTTI::getIsMaximized, &DockManagerLayoutRTTI::setIsMaximized);
			addPlainField("mMaximizedWidgetNames", 2, &DockManagerLayoutRTTI::getMaximizedWidgetNames, &DockManagerLayoutRTTI::setMaximizedWidgetNames);
		}

		void onDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			DockManagerLayout* layout = static_cast<DockManagerLayout*>(obj);

			Stack<DockManagerLayout::Entry*> todo;
			todo.push(&layout->mRootEntry);

			while (!todo.empty())
			{
				DockManagerLayout::Entry* current = todo.top();
				todo.pop();

				if (!current->isLeaf)
				{
					current->children[0]->parent = current;
					current->children[1]->parent = current;

					todo.push(current->children[0]);
					todo.push(current->children[1]);
				}
			}
		}

		const String& getRTTIName() override
		{
			static String name = "DockManagerLayout";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_DockManagerLayout;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<DockManagerLayout>();
		}
	};

	template<> struct RTTIPlainType<bs::DockManagerLayout::Entry>
	{	
		enum { id = bs::TID_DockManagerLayoutEntry }; enum { hasDynamicSize = 1 };

		static BitLength toMemory(const bs::DockManagerLayout::Entry& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{ 
			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(data.isLeaf, stream);
				size += rtti_write(data.horizontalSplit, stream);
				size += rtti_write(data.splitPosition, stream);
				size += rtti_write(data.widgetNames, stream);

				if (!data.isLeaf)
				{
					size += rtti_write(*data.children[0], stream);
					size += rtti_write(*data.children[1], stream);
				}

				return size;
			});
		}

		static BitLength fromMemory(bs::DockManagerLayout::Entry& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{ 
			BitLength size;
			rtti_read_size_header(stream, compress, size);

			rtti_read(data.isLeaf, stream);
			rtti_read(data.horizontalSplit, stream);
			rtti_read(data.splitPosition, stream);
			rtti_read(data.widgetNames, stream);

			if(!data.isLeaf)
			{
				data.children[0] = bs_new<bs::DockManagerLayout::Entry>();
				data.children[1] = bs_new<bs::DockManagerLayout::Entry>();

				rtti_read(*data.children[0], stream);
				rtti_read(*data.children[1], stream);
				
				data.children[0]->parent = &data;
				data.children[1]->parent = &data;
			}

			return size;
		}

		static BitLength getSize(const bs::DockManagerLayout::Entry& data, const RTTIFieldInfo& fieldInfo, bool compress)	
		{ 
			BitLength dataSize = rtti_size(data.isLeaf) + rtti_size(data.horizontalSplit) +
				rtti_size(data.splitPosition) + rtti_size(data.widgetNames);

			if(!data.isLeaf)
			{
				dataSize += rtti_size(*data.children[0]);
				dataSize += rtti_size(*data.children[1]);
			}

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}	
	}; 

	/** @} */
	/** @endcond */
}