//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "RTTI/BsUUIDRTTI.h"
#include "RTTI/BsStringRTTI.h"
#include "Library/BsProjectResourceMeta.h"
#include "Resources/BsResourceMetaData.h"
#include "Importer/BsImportOptions.h"
#include "Resources/BsResources.h"
#include "Image/BsTexture.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Editor
	 *  @{
	 */

	class ProjectResourceMetaRTTI : public RTTIType<ProjectResourceMeta, IReflectable, ProjectResourceMetaRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(mName, 0)
			BS_RTTI_MEMBER_PLAIN(mUUID, 1)
			BS_RTTI_MEMBER_PLAIN(mTypeId, 2)
			BS_RTTI_MEMBER_REFLPTR(mResourceMeta, 3)
			BS_RTTI_MEMBER_REFLPTR(mUserData, 4)
		BS_END_RTTI_MEMBERS

		// We want to store textures directly in this object rather than referencing them externally, so we need to strip
		// away resource handles before saving them, and restore afterwards
#define GETTER_SETTER_ICON(icon)																						\
		SPtr<Texture> get##icon(ProjectResourceMeta* obj)																\
		{																												\
			if(obj->mPreviewIcons.icon.isLoaded(false))																	\
				return obj->mPreviewIcons.icon.getInternalPtr();														\
																														\
			return nullptr;																								\
		}																												\
																														\
			void set##icon(ProjectResourceMeta* obj, SPtr<Texture> data)												\
		{																												\
			obj->mPreviewIcons.icon = static_resource_cast<Texture>(gResources()._createResourceHandle(data));			\
		}																												\

		GETTER_SETTER_ICON(icon16)
		GETTER_SETTER_ICON(icon32)
		GETTER_SETTER_ICON(icon48)
		GETTER_SETTER_ICON(icon64)
		GETTER_SETTER_ICON(icon96)
		GETTER_SETTER_ICON(icon128)
		GETTER_SETTER_ICON(icon192)
		GETTER_SETTER_ICON(icon256)

	public:
		ProjectResourceMetaRTTI()
		{
			addReflectablePtrField("mPreviewIcon16", 5, 
				&ProjectResourceMetaRTTI::geticon16, &ProjectResourceMetaRTTI::seticon16, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
			addReflectablePtrField("mPreviewIcon32", 6, 
				&ProjectResourceMetaRTTI::geticon32, &ProjectResourceMetaRTTI::seticon32, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
			addReflectablePtrField("mPreviewIcon48", 7, 
				&ProjectResourceMetaRTTI::geticon48, &ProjectResourceMetaRTTI::seticon48, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
			addReflectablePtrField("mPreviewIcon64", 8, 
				&ProjectResourceMetaRTTI::geticon64, &ProjectResourceMetaRTTI::seticon64, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
			addReflectablePtrField("mPreviewIcon96", 9, 
				&ProjectResourceMetaRTTI::geticon96, &ProjectResourceMetaRTTI::seticon96, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
			addReflectablePtrField("mPreviewIcon128", 10, 
				&ProjectResourceMetaRTTI::geticon128, &ProjectResourceMetaRTTI::seticon128, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
			addReflectablePtrField("mPreviewIcon192", 11, 
				&ProjectResourceMetaRTTI::geticon192, &ProjectResourceMetaRTTI::seticon192, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
			addReflectablePtrField("mPreviewIcon256", 12, 
				&ProjectResourceMetaRTTI::geticon256, &ProjectResourceMetaRTTI::seticon256, 
				RTTIFieldInfo(RTTIFieldFlag::SkipInReferenceSearch));
		}

		const String& getRTTIName() override
		{
			static String name = "ProjectResourceMeta";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_ProjectResourceMeta;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return ProjectResourceMeta::createEmpty();
		}
	};

	class ProjectFileMetaRTTI : public RTTIType<ProjectFileMeta, IReflectable, ProjectFileMetaRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_REFLPTR(mImportOptions, 1)
			BS_RTTI_MEMBER_PLAIN(mIncludeInBuild, 4)
			BS_RTTI_MEMBER_REFLPTR_ARRAY(mResourceMetaData, 5)
			BS_RTTI_MEMBER_REFLPTR_ARRAY(mInactiveResourceMetaData, 6)
		BS_END_RTTI_MEMBERS

	public:
		const String& getRTTIName() override
		{
			static String name = "ProjectFileMeta";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_ProjectFileMeta;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return ProjectFileMeta::createEmpty();
		}
	};

	/** @} */
	/** @endcond */
}
