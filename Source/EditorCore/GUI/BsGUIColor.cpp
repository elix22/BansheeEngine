//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIColor.h"
#include "2D/BsImageSprite.h"
#include "GUI/BsGUIDimensions.h"
#include "GUI/BsGUIMouseEvent.h"
#include "GUI/BsGUIHelper.h"
#include "Resources/BsBuiltinResources.h"
#include "Image/BsSpriteTexture.h"

namespace bs
{
	const float GUIColor::ALPHA_SPLIT_POSITION = 0.75f;

	GUIColor::GUIColor(const String& styleName, const GUIDimensions& dimensions)
		:GUIElement(styleName, dimensions), mColorSprite(nullptr), mAlphaSprite(nullptr)
	{
		mColorSprite = bs_new<ImageSprite>();
		mAlphaSprite = bs_new<ImageSprite>();

		mColorImageDesc.texture = BuiltinResources::instance().getWhiteSpriteTexture();
		mAlphaImageDesc.texture = BuiltinResources::instance().getWhiteSpriteTexture();
	}

	GUIColor::~GUIColor()
	{
		bs_delete(mColorSprite);
		bs_delete(mAlphaSprite);
	}

	const String& GUIColor::getGUITypeName()
	{
		static String name = "Color";
		return name;
	}

	GUIColor* GUIColor::create(const String& styleName)
	{
		return new (bs_alloc<GUIColor>()) GUIColor(getStyleName<GUIColor>(styleName), GUIDimensions::create());
	}

	GUIColor* GUIColor::create(const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUIColor>()) GUIColor(getStyleName<GUIColor>(styleName), GUIDimensions::create(options));
	}

	void GUIColor::setColor(const Color& color)
	{
		if (color != mValue)
		{
			mValue = color;

			_markContentAsDirty();
		}
	}

	void GUIColor::updateRenderElementsInternal()
	{		
		Color color = mValue * getTint();

		mColorImageDesc.color = color;
		mColorImageDesc.color.a = 1.0f;

		mAlphaImageDesc.color = Color::White * color.a;
		mAlphaImageDesc.color.a = 1.0f;

		mColorImageDesc.width = (UINT32)(mLayoutData.area.width * ALPHA_SPLIT_POSITION);
		mColorImageDesc.height = mLayoutData.area.height;

		mAlphaImageDesc.width = mLayoutData.area.width - mColorImageDesc.width;
		mAlphaImageDesc.height = mLayoutData.area.height;

		mColorSprite->update(mColorImageDesc, (UINT64)_getParentWidget());
		mAlphaSprite->update(mAlphaImageDesc, (UINT64)_getParentWidget());

		// Populate GUI render elements from the sprites
		{
			using T = impl::GUIRenderElementHelper;
			T::populate({ T::SpriteInfo(mColorSprite), T::SpriteInfo(mAlphaSprite) }, mRenderElements);
		}

		GUIElement::updateRenderElementsInternal();
	}

	Vector2I GUIColor::_getOptimalSize() const
	{
		return GUIHelper::calcOptimalContentsSize(Vector2I(80, 10), *_getStyle(), _getDimensions()); // Arbitrary size
	}

	void GUIColor::_fillBuffer(UINT8* vertices, UINT32* indices, UINT32 vertexOffset, UINT32 indexOffset, 
		const Vector2I& offset, UINT32 maxNumVerts, UINT32 maxNumIndices, UINT32 renderElementIdx) const
	{
		UINT8* uvs = vertices + sizeof(Vector2);
		UINT32 vertexStride = sizeof(Vector2) * 2;
		UINT32 indexStride = sizeof(UINT32);
		
		UINT32 alphaSpriteIdx = mColorSprite->getNumRenderElements();
		
		Vector2I spriteOffset = Vector2I(mLayoutData.area.x, mLayoutData.area.y) + offset;
		if(renderElementIdx < alphaSpriteIdx)
		{
			mColorSprite->fillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices,
				vertexStride, indexStride, renderElementIdx, spriteOffset, mLayoutData.getLocalClipRect());
		}
		else if(renderElementIdx >= alphaSpriteIdx)
		{
			Vector2I alphaOffset = spriteOffset;
			UINT32 xOffset = (UINT32)(mLayoutData.area.width * ALPHA_SPLIT_POSITION);
			alphaOffset.x += xOffset;

			Rect2I alphaClipRect = mLayoutData.getLocalClipRect();
			alphaClipRect.x -= xOffset;

			mAlphaSprite->fillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices,
				vertexStride, indexStride, alphaSpriteIdx - renderElementIdx, alphaOffset, alphaClipRect);
		}
	}

	bool GUIColor::_mouseEvent(const GUIMouseEvent& ev)
	{
		if(ev.getType() == GUIMouseEventType::MouseUp)
		{
			if (!_isDisabled())
				onClicked();

			return true;
		}

		return false;
	}
}