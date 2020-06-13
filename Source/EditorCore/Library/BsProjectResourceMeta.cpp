//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Library/BsProjectResourceMeta.h"
#include "Private/RTTI/BsProjectResourceMetaRTTI.h"
#include "String/BsUnicode.h"

namespace bs
{
	ProjectResourceMeta::ProjectResourceMeta(const ConstructPrivately& dummy)
	{

	}

	String ProjectResourceMeta::getUniqueName() const
	{
		return UTF8::fromWide(mName);
	}

	SPtr<ProjectResourceMeta> ProjectResourceMeta::create(const String& name, const UUID& uuid, UINT32 typeId,
		const ProjectResourceIcons& previewIcons, const SPtr<ResourceMetaData>& resourceMetaData)
	{
		SPtr<ProjectResourceMeta> meta = bs_shared_ptr_new<ProjectResourceMeta>(ConstructPrivately());
		meta->mName = UTF8::toWide(name); // Using wide string internally to keep compatibility with older versions
		meta->mUUID = uuid;
		meta->mTypeId = typeId;
		meta->mPreviewIcons = previewIcons;
		meta->mResourceMeta = resourceMetaData;

		return meta;
	}

	SPtr<ProjectResourceMeta> ProjectResourceMeta::createEmpty()
	{
		return bs_shared_ptr_new<ProjectResourceMeta>(ConstructPrivately());
	}

	/************************************************************************/
	/* 								RTTI		                     		*/
	/************************************************************************/

	RTTITypeBase* ProjectResourceMeta::getRTTIStatic()
	{
		return ProjectResourceMetaRTTI::instance();
	}

	RTTITypeBase* ProjectResourceMeta::getRTTI() const
	{
		return ProjectResourceMeta::getRTTIStatic();
	}

	ProjectFileMeta::ProjectFileMeta(const ConstructPrivately& dummy)
		:mIncludeInBuild(false)
	{

	}

	SPtr<ProjectFileMeta> ProjectFileMeta::create(const SPtr<ImportOptions>& importOptions)
	{
		SPtr<ProjectFileMeta> meta = bs_shared_ptr_new<ProjectFileMeta>(ConstructPrivately());
		meta->mImportOptions = importOptions;
		meta->mIncludeInBuild = false;

		return meta;
	}

	void ProjectFileMeta::add(const SPtr<ProjectResourceMeta>& resourceMeta)
	{
		mResourceMetaData.push_back(resourceMeta);
	}

	void ProjectFileMeta::addInactive(const SPtr<ProjectResourceMeta>& resourceMeta)
	{
		mInactiveResourceMetaData.push_back(resourceMeta);
	}

	Vector<SPtr<ProjectResourceMeta>> ProjectFileMeta::getAllResourceMetaData() const
	{
		Vector<SPtr<ProjectResourceMeta>> output(mResourceMetaData);

		for (auto& entry : mInactiveResourceMetaData)
			output.push_back(entry);

		return output;
	}

	void ProjectFileMeta::clearResourceMetaData()
	{
		mResourceMetaData.clear();
		mInactiveResourceMetaData.clear();
	}

	bool ProjectFileMeta::hasTypeId(UINT32 typeId) const
	{
		for(auto& entry : mResourceMetaData)
		{
			if (entry->getTypeID() == typeId)
				return true;
		}

		return false;
	}

	bool ProjectFileMeta::hasUUID(const UUID& uuid) const
	{
		for (auto& entry : mResourceMetaData)
		{
			if (entry->getUUID() == uuid)
				return true;
		}

		return false;
	}

	SPtr<ProjectFileMeta> ProjectFileMeta::createEmpty()
	{
		return bs_shared_ptr_new<ProjectFileMeta>(ConstructPrivately());
	}

	/************************************************************************/
	/* 								RTTI		                     		*/
	/************************************************************************/

	RTTITypeBase* ProjectFileMeta::getRTTIStatic()
	{
		return ProjectFileMetaRTTI::instance();
	}

	RTTITypeBase* ProjectFileMeta::getRTTI() const
	{
		return ProjectFileMeta::getRTTIStatic();
	}
}
