//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsGUITextureField.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIDropButton.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUIPanel.h"
#include "Resources/BsResources.h"
#include "Image/BsTexture.h"
#include "Image/BsSpriteTexture.h"
#include "Library/BsProjectLibrary.h"
#include "Utility/BsBuiltinEditorResources.h"
#include "GUI/BsGUIResourceTreeView.h"
#include "GUI/BsGUISpace.h"
#include "Library/BsProjectResourceMeta.h"
#include "Scene/BsSelection.h"
#include "Reflection/BsRTTIType.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUITextureField::DEFAULT_LABEL_WIDTH = 100;

	GUITextureField::GUITextureField(const PrivatelyConstruct& dummy, GUITextureFieldType, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel) 
		:GUIElementContainer(dimensions, style), mLabel(nullptr), mDropButton(nullptr), mClearButton(nullptr)
	{
		mLayout = GUILayoutX::create();
		_registerChildElement(mLayout);

		if (withLabel)
		{
			mLabel = GUILabel::create(labelContent, GUIOptions(GUIOption::fixedWidth(labelWidth)), 
				getSubStyleName(BuiltinEditorResources::TextureFieldLabelStyleName));
			mLayout->addElement(mLabel);
		}

		mDropButton = GUIDropButton::create((UINT32)DragAndDropType::Resources, 
			getSubStyleName(BuiltinEditorResources::TextureFieldDropStyleName));
		mClearButton = GUIButton::create(HString(""), 
			getSubStyleName(BuiltinEditorResources::TextureFieldClearBtnStyleName));
		mClearButton->onClick.connect(std::bind(&GUITextureField::onClearButtonClicked, this));

		GUIElementOptions clearBtnOptions = mClearButton->getOptionFlags();
		clearBtnOptions.unset(GUIElementOption::AcceptsKeyFocus);
		mClearButton->setOptionFlags(clearBtnOptions);

		GUIPanel* dropTargetPanel = mLayout->addNewElement<GUIPanel>();
		dropTargetPanel->addElement(mDropButton);

		GUIPanel* closeBtnPanel = dropTargetPanel->addNewElement<GUIPanel>(-1);
		GUILayoutY* closeBtnLayoutY = closeBtnPanel->addNewElement<GUILayoutY>();
		closeBtnLayoutY->addNewElement<GUIFixedSpace>(5);
		GUILayoutX* closeBtnLayoutX = closeBtnLayoutY->addNewElement<GUILayoutX>();
		closeBtnLayoutY->addNewElement<GUIFlexibleSpace>();
		closeBtnLayoutX->addNewElement<GUIFlexibleSpace>();
		closeBtnLayoutX->addElement(mClearButton);
		closeBtnLayoutX->addNewElement<GUIFixedSpace>(5);

		if (withLabel)
			mLayout->addNewElement<GUIFlexibleSpace>();

		mDropButton->onDataDropped.connect(std::bind(&GUITextureField::dataDropped, this, _1));
		mDropButton->onClick.connect(std::bind(&GUITextureField::onDropButtonClicked, this));

		mDropButton->setContent(GUIContent(HString("(None)")));
		mClearButton->setVisible(false);
	}

	GUITextureField* GUITextureField::create(GUITextureFieldType type, const GUIFieldOptions& options)
	{
		const String* curStyle = &options.style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::TextureFieldStyleName;

		return bs_new<GUITextureField>(PrivatelyConstruct(), type, options.labelContent, 
			options.labelWidth, *curStyle, GUIDimensions::create(options.options), options.labelWidth > 0);
	}

	GUITextureField* GUITextureField::create(const GUIFieldOptions& options)
	{
		const String* curStyle = &options.style;
		if (*curStyle == StringUtil::BLANK)
			curStyle = &BuiltinEditorResources::TextureFieldStyleName;

		return bs_new<GUITextureField>(PrivatelyConstruct(), GUITextureFieldType::Texture, options.labelContent, 
			options.labelWidth, *curStyle, GUIDimensions::create(options.options), options.labelWidth > 0);
	}

	HTexture GUITextureField::getTexture() const
	{
		HResource resource = gResources()._getResourceHandle(mUUID);
		if(resource.isLoaded(false))
		{
			if (rtti_is_of_type<Texture>(resource.get()))
				return static_resource_cast<Texture>(resource);
		}

		return HTexture();
	}

	void GUITextureField::setTexture(const HTexture& value)
	{
		if (value)
			setUUID(value.getUUID(), false);
		else
			setUUID(UUID::EMPTY, false);
	}

	HSpriteTexture GUITextureField::getSpriteTexture() const
	{
		HResource resource = gResources()._getResourceHandle(mUUID);
		if(resource.isLoaded(false))
		{
			if (rtti_is_of_type<SpriteTexture>(resource.get()))
				return static_resource_cast<SpriteTexture>(resource);
		}

		return HSpriteTexture();
	}

	void GUITextureField::setSpriteTexture(const HSpriteTexture& value)
	{
		if (value)
			setUUID(value.getUUID(), false);
		else
			setUUID(UUID::EMPTY, false);
	}

	HResource GUITextureField::getValue() const
	{
		return gResources()._getResourceHandle(mUUID);
	}

	void GUITextureField::setValue(const HResource& value)
	{
		if (value)
		{
			if (rtti_is_of_type<SpriteTexture>(value.get()) || rtti_is_of_type<Texture>(value.get()))
				setUUID(value.getUUID(), false);
			else
				setUUID(UUID::EMPTY, false);
		}
		else
			setUUID(UUID::EMPTY, false);
	}

	void GUITextureField::setUUID(const UUID& uuid, bool triggerEvent)
	{
		if (mUUID == uuid)
			return;

		mUUID = uuid;

		HSpriteTexture previewIcon;

		Path filePath = gProjectLibrary().uuidToPath(mUUID);
		if (!filePath.isEmpty())
		{
			SPtr<ProjectResourceMeta> meta = gProjectLibrary().findResourceMeta(filePath);
			if(meta && meta->getPreviewIcons().icon128.isLoaded())
				previewIcon = SpriteTexture::create(meta->getPreviewIcons().icon128);
			else
			{
				// Not ideal. No cached texture so fall back on loading the original asset
				HResource resource = gProjectLibrary().load(filePath);
				if(resource.isLoaded(false))
				{
					if (rtti_is_of_type<SpriteTexture>(resource.get()))
						previewIcon = static_resource_cast<SpriteTexture>(resource);
					else if (rtti_is_of_type<Texture>(resource.get()))
						previewIcon = SpriteTexture::create(static_resource_cast<Texture>(resource));
				}
			}
		}
		
		if (previewIcon != nullptr)
		{
			mDropButton->setContent(GUIContent(previewIcon));
			mClearButton->setVisible(true);
		}
		else
		{
			mDropButton->setContent(GUIContent(HString("(None)")));
			mClearButton->setVisible(false);
		}

		if (triggerEvent)
			onValueChanged(gResources()._getResourceHandle(mUUID));
	}

	void GUITextureField::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mDropButton->setTint(color);
		mClearButton->setTint(color);
	}

	void GUITextureField::_updateLayoutInternal(const GUILayoutData& data)
	{
		mLayout->_setLayoutData(data);
		mLayout->_updateLayoutInternal(data);
	}

	Vector2I GUITextureField::_getOptimalSize() const
	{
		return mLayout->_getOptimalSize();
	}

	void GUITextureField::onDropButtonClicked()
	{
		if (mUUID.empty())
			return;

		Path resPath = gProjectLibrary().uuidToPath(mUUID);
		resPath = resPath.getRelative(gProjectLibrary().getResourcesFolder());

		Selection::instance().ping(resPath);
	}

	void GUITextureField::dataDropped(void* data)
	{
		DraggedResources* draggedResources = reinterpret_cast<DraggedResources*>(data);
		UINT32 numResources = (UINT32)draggedResources->resourcePaths.size();

		if (numResources <= 0)
			return;

		for (UINT32 i = 0; i < numResources; i++)
		{
			Path path = draggedResources->resourcePaths[i];

			SPtr<ProjectResourceMeta> meta = gProjectLibrary().findResourceMeta(draggedResources->resourcePaths[i]);
			if (meta == nullptr || (meta->getTypeID() != TID_Texture && meta->getTypeID() != TID_SpriteTexture))
				continue;

			setUUID(meta->getUUID());
			break;
		}
	}

	void GUITextureField::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(BuiltinEditorResources::TextureFieldLabelStyleName));

		mDropButton->setStyle(getSubStyleName(BuiltinEditorResources::TextureFieldDropStyleName));
		mClearButton->setStyle(getSubStyleName(BuiltinEditorResources::TextureFieldClearBtnStyleName));
	}

	void GUITextureField::onClearButtonClicked()
	{
		setUUID(UUID::EMPTY);
	}

	const String& GUITextureField::getGUITypeName()
	{
		return BuiltinEditorResources::TextureFieldStyleName;
	}
}