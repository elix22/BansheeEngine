//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include <utility>
#include "GUI/BsGUITimeline.h"
#include "GUI/BsGUIDimensions.h"
#include "GUI/BsGUIMouseEvent.h"
#include "GUI/BsGUICanvas.h"
#include "Math/BsLine2.h"
#include "GUI/BsGUIWidget.h"
#include "GUI/BsGUITexture.h"

namespace bs
{
	GUITimeline::GUITimeline(const String& styleName, const GUIDimensions& dimensions)
		:GUIElementContainer(dimensions, styleName)
	{
		mCanvas = GUICanvas::create();
		mBackground = GUITexture::create(getSubStyleName(BACKGROUND_STYLE_TYPE));

		mBackground->_setElementDepth(255);

		_registerChildElement(mCanvas);
		_registerChildElement(mBackground);
	}

	const String& GUITimeline::getGUITypeName()
	{
		static String name = "Timeline";
		return name;
	}

	void GUITimeline::setRange(float range)
	{
		mRange = std::max(0.0f, range);
		_markContentAsDirty();
	}

	float GUITimeline::getRange() const
	{
		const float spf = 1.0f / (float)mFPS;
		return std::max(1.0f, mRange / spf) * spf;
	}

	void GUITimeline::setOffset(float offset)
	{
		mOffset = offset;
		_markContentAsDirty();
	}

	void GUITimeline::setFPS(UINT32 FPS)
	{
		mFPS = std::max(1U, FPS);
		_markContentAsDirty();
	}

	void GUITimeline::setMarkedFrame(UINT32 index)
	{
		mMarkedFrame = index;
		_markContentAsDirty();
	}

	void GUITimeline::setPadding(UINT32 padding)
	{
		mPadding = padding;
		_markContentAsDirty();
	}

	UINT32 GUITimeline::getFrame(const Vector2I& pixelCoords) const
	{
		const Rect2I& bounds = mLayoutData.area;

		if (pixelCoords.x < (INT32)mPadding || 
			pixelCoords.x >= ((INT32)bounds.width - (INT32)mPadding) ||
			pixelCoords.y < 0 || 
			pixelCoords.y >= (INT32)bounds.height)
		{
			return -1;
		}

		const Vector2I relativeCoords = pixelCoords - Vector2I(bounds.x + mPadding, bounds.y);

		const float lengthPerPixel = getRange() / getDrawableWidth();
		const float time = mOffset + relativeCoords.x * lengthPerPixel;

		return Math::roundToPosInt(time * mFPS);
	}

	float GUITimeline::getTime(INT32 pixel) const
	{
		const Rect2I& bounds = mLayoutData.area;
		const INT32 relativeCoords = pixel - (bounds.x + mPadding);

		const float lengthPerPixel = getRange() / getDrawableWidth();
		return mOffset + relativeCoords * lengthPerPixel;
	}

	INT32 GUITimeline::getOffset(float time) const
	{
		return (INT32)(((time - mOffset) / getRange()) * getDrawableWidth()) + mPadding;
	}

	float GUITimeline::getTimeForFrame(INT32 index) const
	{
		return index / (float)mFPS;
	}

	UINT32 GUITimeline::getDrawableWidth() const
	{
		return std::max(0, (INT32)mLayoutData.area.width - (INT32)mPadding * 2);
	}

	float GUITimeline::getRangeWithPadding() const
	{
		const float spf = 1.0f / (float)mFPS;

		const float lengthPerPixel = mRange / getDrawableWidth();
		const float range = mRange + lengthPerPixel * mPadding;

		return std::max(1.0f, range / spf) * spf;
	}

	void GUITimeline::drawFrameMarker(float t)
	{
		const INT32 xPos = (INT32)(((t - mOffset) / getRange()) * getDrawableWidth()) + mPadding;

		const Vector2I start(xPos, 0);
		const Vector2I end(xPos, mLayoutData.area.height);

		mCanvas->drawLine(start, end, Color::BansheeOrange);
	}

	void GUITimeline::drawFrameMarker()
	{
		if(mMarkedFrame != (UINT32)-1)
			drawFrameMarker(mMarkedFrame / (float)mFPS);
	}

	void GUITimeline::_updateLayoutInternal(const GUILayoutData& data)
	{
		mBackground->_setLayoutData(data);

		mCanvas->_setLayoutData(data);
		mCanvas->_updateLayoutInternal(data);
	}

	Vector2I GUITimeline::_getOptimalSize() const
	{
		return mCanvas->_getOptimalSize();
	}

	void GUITimeline::styleUpdated()
	{
		mCanvas->setStyle(GUICanvas::getGUITypeName());
		mBackground->setStyle(getSubStyleName(BACKGROUND_STYLE_TYPE));
	}
}
