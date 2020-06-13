//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIElement.h"
#include "2D/BsImageSprite.h"
#include "GUI/BsGUIElementContainer.h"

namespace bs
{
	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/** Base class that can be implemented by GUI elements needing to elements along draw a horizontal timeline. */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) GUITimeline : public GUIElementContainer
	{
	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/** Style type name for the internal background image. */
		static constexpr const char* BACKGROUND_STYLE_TYPE = "Background";

		/**	Determines the range of values to display on the timeline, in seconds. */
		BS_SCRIPT_EXPORT(pr:setter,n:Range)
		void setRange(float range);

		/** @copydoc setRange */
		BS_SCRIPT_EXPORT(pr:getter,n:Range)
		float getRange() const;

		/**	Determines the offset at which the displayed timeline values start at, in seconds. */
		BS_SCRIPT_EXPORT(pr:setter,n:Offset)
		void setOffset(float offset);

		/** @copydoc setOffset */
		BS_SCRIPT_EXPORT(pr:getter,n:Offset)
		float getOffset() const { return mOffset; }

		/**	Number of frames per second, used for frame selection and marking. */
		BS_SCRIPT_EXPORT(pr:setter,n:FPS)
		void setFPS(UINT32 FPS);

		/** @copydoc setFPS */
		BS_SCRIPT_EXPORT(pr:getter,n:FPS)
		UINT32 getFPS() const { return mFPS; }

		/**	Frame to display the frame marker on. Set to -1 to clear the frame marker. */
		BS_SCRIPT_EXPORT(pr:setter,n:MarkedFrame)
		void setMarkedFrame(UINT32 index);

		/** @copydoc setFPS */
		BS_SCRIPT_EXPORT(pr:getter,n:MarkedFrame)
		UINT32 setMarkedFrame() const { return mFPS; }

		/**
		 * Uses the assigned FPS, range and physical size to calculate the frame that is under the provided coordinates.
		 *
		 * @param[in]	pixelCoords		Coordinates relative to this GUI element.
		 * @return						Frame that was clicked on, or -1 if the coordinates are outside of valid bounds.
		 */
		BS_SCRIPT_EXPORT()
		UINT32 getFrame(const Vector2I& pixelCoords) const;

		/**
		 * Returns the time at the specified pixel value along the timeline.
		 * 
		 * @param[in]	pixel	X coordinate to sample at, relative to this GUI element in pixels.
		 * @return				Time along the curve at the specified coordinate.
		 */
		BS_SCRIPT_EXPORT()
		float getTime(INT32 pixel) const;

		/**
		 * Finds the pixel offset relative to the GUI element's origin for the specified time.
		 *
		 * @param[in]	time	Time value to return the offset for.
		 * @return				Offset in pixels relative to GUI element's origin
		 */
		BS_SCRIPT_EXPORT()
		INT32 getOffset(float time) const;

		/**
		 * Returns time for a frame with the specified index. Depends on set range and FPS.
		 * 
		 * @param[in]	index	Index of the frame (not a key-frame) to get the time for.
		 * @return				Time of the frame with the provided index.	
		 */
		BS_SCRIPT_EXPORT()
		float getTimeForFrame(INT32 index) const;

		/** Sets the size of padding to apply to the left and right sides of the curve drawing, in pixels. */
		BS_SCRIPT_EXPORT(pr:setter,n:Padding)
		void setPadding(UINT32 padding);

		/** @copydoc setPadding */
		BS_SCRIPT_EXPORT(pr:getter,n:Padding)
		UINT32 getPadding() const { return mPadding; };

		/**
		 * @name Internal
		 * @{
		 */

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */
	protected:
		GUITimeline(const String& styleName, const GUIDimensions& dimensions);
		~GUITimeline() override = default;

		/** Returns the width of the GUI element that can be drawn to (width minus padding). */
		UINT32 getDrawableWidth() const;

		/** Similar to getRange() but expands the range so it's expanded to encompas the right-most padding area. */
		float getRangeWithPadding() const;

		/** Draws a vertical frame marker at the specified time. */
		void drawFrameMarker(float t);

		/** Draws a frame marker at the currently selected frame. */
		void drawFrameMarker();

		/** @copydoc GUIElement::_updateLayoutInternal */
		void _updateLayoutInternal(const GUILayoutData& data) override;

		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		GUICanvas* mCanvas = nullptr;
		GUITexture* mBackground = nullptr;

		float mRange = 20.0f;
		float mOffset = 0.0f;
		UINT32 mFPS = 1;
		UINT32 mMarkedFrame = (UINT32)-1;
		UINT32 mPadding = 30;
	};

	/** @} */
}
