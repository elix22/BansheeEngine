//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIElement.h"
#include "GUI/BsGUITimeline.h"
#include "GUI/BsGUIGraphTicks.h"
#include "2D/BsImageSprite.h"
#include "Animation/BsAnimationUtility.h"

namespace bs
{
	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/** Controls which elements should a GUICurves object draw. */
	enum class BS_SCRIPT_EXPORT(m:GUIEditor,n:CurveDrawOptions,api:bed) CurveDrawOption
	{
		/** Draws markers at specific time intervals. */
		DrawMarkers = 1 << 0,

		/** Draws elements representing individual keyframes. */
		DrawKeyframes = 1 << 1,

		/** Draws curves and the area between them. Only relevant if only two curves are provided for drawing. */
		DrawRange = 1 << 2
	};

	typedef Flags<CurveDrawOption> CurveDrawOptions;
	BS_FLAGS_OPERATORS(CurveDrawOption)

	/** Curve and a color to draw it in. */
	struct BS_SCRIPT_EXPORT(pl:true,m:GUIEditor,api:bed) CurveDrawInfo
	{
		CurveDrawInfo() = default;
		CurveDrawInfo(const TAnimationCurve<float>& curve, Color color)
			:curve(curve), color(color)
		{ }

		TAnimationCurve<float> curve;
		Color color;

		bool operator== (const CurveDrawInfo& rhs) const { return curve == rhs.curve && color == rhs.color; }
		bool operator!= (const CurveDrawInfo& rhs) const { return !operator==(rhs); }
	};

	/** GUI element that displays one or multiple curves. */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) GUICurves : public GUITimeline
	{
	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/**
		 * Creates a new GUI element.
		 *
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		BS_SCRIPT_EXPORT(ec:GUICurves)
		static GUICurves* create(const String& styleName = StringUtil::BLANK);

		/**
		 * Creates a new GUI element.
		 *
		 * @param[in]	drawOptions		Options that control which additional elements to draw.
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		BS_SCRIPT_EXPORT(ec:GUICurves)
		static GUICurves* create(CurveDrawOptions drawOptions, const String& styleName = StringUtil::BLANK);

		/**
		 * Creates a new GUI element.
		 *
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		static GUICurves* create(const GUIOptions& options, const String& styleName = StringUtil::BLANK);

		/**
		 * Creates a new GUI element.
		 * 
		 * @param[in]	drawOptions		Options that control which additional elements to draw.
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		static GUICurves* create(CurveDrawOptions drawOptions, const GUIOptions& options, 
			const String& styleName = StringUtil::BLANK);

		/**	Animation curves to display. */
		BS_SCRIPT_EXPORT(pr:setter,n:Curves)
		void setCurves(const Vector<CurveDrawInfo>& curves);

		/** @copydoc setCurves */
		BS_SCRIPT_EXPORT(pr:getter,n:Curves)
		const Vector<CurveDrawInfo>& getCurves() const { return mCurves; }

		/**
		 * Changes the visible range that the GUI element displays.
		 *
		 * @param[in]	xRange		Range of the horizontal area. Displayed area will range from [0, xRange].
		 * @param[in]	yRange		Range of the vertical area. Displayed area will range from [-yRange * 0.5, yRange * 0.5]
		 */
		BS_SCRIPT_EXPORT()
		void setRange(float xRange, float yRange);

		/**
		 * Returns the offset at which the displayed timeline values start at.
		 *
		 * @param[in]	offset		Value to start the timeline values at, where x = time, y = value.
		 */
		BS_SCRIPT_EXPORT()
		void setOffset(const Vector2& offset);

		/** Centers and zooms the view to fully display the provided set of curves. */
		BS_SCRIPT_EXPORT()
		void centerAndZoom();

		/**
		 * Converts pixel coordinates into coordinates in curve space.
		 *
		 * @param[in]	pixelCoords		Coordinates relative to this GUI element, in pixels.
		 * @param[out]	curveCoords		Curve coordinates within the range as specified by setRange(). Only valid when
		 *								function returns true.
		 * @param[in]	padding			Determines should coordinates over the area reserved for padding be registered.
		 * @return						True if the coordinates are within the curve area, false otherwise.
		 */
		BS_SCRIPT_EXPORT()
		bool pixelToCurveSpace(const Vector2I& pixelCoords, Vector2& curveCoords, bool padding = false) const;

		/**
		 * Converts coordinate in curve space (time, value) into pixel coordinates relative to this element's origin.
		 *
		 * @param[in]	curveCoords		Time and value of the location to convert.
		 * @return						Coordinates relative to this element's origin, in pixels.
		 */
		BS_SCRIPT_EXPORT()
		Vector2I curveToPixelSpace(const Vector2& curveCoords) const;

		/** 
		 * Attempts to find a curve under the provided coordinates.
		 *
		 * @param[in]	pixelCoords		Coordinates relative to this GUI element in pixels.
		 * @return						Index of the curve, or -1 if none found.
		 */
		BS_SCRIPT_EXPORT()
		UINT32 findCurve(const Vector2I& pixelCoords);

		/**
		 * Attempts to find a keyframe under the provided coordinates.
		 *
		 * @param[in]	pixelCoords		Coordinates relative to this GUI element in pixels.
		 * @param[out]	keyframe		Output object containing keyframe index and index of the curve it belongs to. Only 
		 *								valid if method returns true.
		 * @return						True if there is a keyframe under the coordinates, false otherwise.
		 */
		BS_SCRIPT_EXPORT()
		bool findKeyFrame(const Vector2I& pixelCoords, KeyframeRef& keyframe);

		/**
		 * Attempts to find a a tangent handle under the provided coordinates.
		 *
		 * @param[in]	pixelCoords		Coordinates relative to this GUI element in pixels.
		 * @param[in]	tangent			Output object containing keyframe information and tangent type. Only valid if method
		 *								returns true.
		 * @return						True if there is a tangent handle under the coordinates, false otherwise.
		 */
		BS_SCRIPT_EXPORT()
		bool findTangent(const Vector2I& pixelCoords, TangentRef& tangent);

		/** 
		 * Marks the specified key-frame as selected, changing the way it is displayed.
		 *
		 * @param[in]	keyframeRef		Keyframe reference containing the curve and keyframe index.
		 * @param[in]	tangentMode		Type of tangent to display on the selected keyframe.
		 * @param[in]	selected		True to select it, false to deselect it.
		 */
		BS_SCRIPT_EXPORT()
		void selectKeyframe(const KeyframeRef& keyframeRef, const TangentMode& tangentMode, bool selected);

		/** Clears any key-frames that were marked as selected. */
		BS_SCRIPT_EXPORT()
		void clearSelectedKeyframes();

		BS_SCRIPT_EXPORT(in:true)
		Event<void()> onClicked; /**< Triggered when the user clicks on the GUI element. */

	protected:
		GUICurves(CurveDrawOptions drawOptions, const String& styleName, const GUIDimensions& dimensions);
		~GUICurves() override = default;

		/**
		 * Converts a keyframe tangent (slope) value into a 2D normal vector.
		 *
		 * @param[in]	tangent		Keyframe tangent (slope).
		 * @return					Normalized 2D vector pointing in the direction of the tangent.
		 */
		Vector2 tangentToNormal(float tangent) const;

		/**
		 * Returns the position of the tangent, in element's pixel space.
		 *
		 * @param[in]	keyFrame	Keyframe that the tangent belongs to.
		 * @param[in]	type		Which tangent to retrieve the position for.
		 * @return					Position of the tangent, relative to the this GUI element's origin, in pixels.
		 */
		Vector2I getTangentPosition(const TKeyframe<float>& keyFrame, TangentType type) const;

		/** 
		 * Checks if the tangent should be displayed, depending on the active tangent mode. 
		 *
		 * @param[in]	mode	Tangent mode for the keyframe.
		 * @param[in]	type	Which tangent to check for.
		 * @return				True if the tangent should be displayed.
		 */
		bool isTangentDisplayed(TangentMode mode, TangentType type) const;

		/**
		 * Checks is the provided key-frame currently marked as selected.
		 *
		 * @param[in]	curveIdx		Index of the curve the keyframe is on.
		 * @param[in]	keyIdx			Index of the keyframe.
		 * @return						True if selected, false otherwise.
		 */
		bool isSelected(int curveIdx, int keyIdx) const;

		/**
		 * Draws a vertical frame marker on the curve area.
		 *
		 * @param[in]	t		Time at which to draw the marker.
		 * @param[in]	color	Color with which to draw the marker.
		 * @param[in]	onTop	Determines should the marker be drawn above or below the curve.
		 */
		void drawFrameMarker(float t, Color color, bool onTop);

		/** Draws a horizontal line representing the line at y = 0. */
		void drawCenterLine();

		/**
		 * Draws a diamond shape of the specified size at the coordinates.
		 *
		 * @param[in]	center		Position at which to place the diamond's center, in pixel coordinates.
		 * @param[in]	size		Determines number of pixels to extend the diamond in each direction.
		 * @param[in]	innerColor	Color of the diamond's background.
		 * @param[in]	outerColor	Color of the diamond's outline.
		 */
		void drawDiamond(Vector2I center, int size, Color innerColor, Color outerColor);

		/**
		 * Draws a keyframe a the specified time and value.
		 *
		 * @param[in]	t			Time to draw the keyframe at.
		 * @param[in]	y			Y value to draw the keyframe at.
		 * @param[in]	selected	Determines should the keyframe be drawing using the selected color scheme, or normally.
		 */
		void drawKeyframe(float t, float y, bool selected);

		/**
		 * Draws zero, one or two tangents for the specified keyframe. Whether tangents are drawn depends on the provided
		 * mode.
		 *
		 * @param[in]	keyFrame	Keyframe to draw the tangents for.
		 * @param[in]	tangentMode	Type of tangents in the keyframe.
		 */
		void drawTangents(const TKeyframe<float>& keyFrame, TangentMode tangentMode);

		/**
		 * Draws the curve using the provided color.
		 *
		 * @param[in]	curve		Curve to draw within the currently set range.
		 * @param[in]	color		Color to draw the curve with.	
		 */
		void drawCurve(const TAnimationCurve<float>& curve, const Color& color);

		/*
		 * Draws the area between two curves using the provided color.
		 *
		 * @param[in]	curves		Curves to draw within the currently set range.
		 * @param[in]	color		Color to draw the area with.
		 */
		void drawCurveRange(const Vector<TAnimationCurve<float>>& curves, Color color);

		/** @copydoc GUIElement::updateRenderElementsInternal */
		void updateRenderElementsInternal() override;

		/** @copydoc GUIElement::_mouseEvent() */
		bool _mouseEvent(const GUIMouseEvent& ev) override;

	private:
		/** Information about a currently selected keyframe. */
		struct SelectedKeyframe
		{
			SelectedKeyframe(KeyframeRef keyframeRef, TangentMode tangentMode)
				:keyframeRef(keyframeRef), tangentMode(tangentMode)
			{ }

			KeyframeRef keyframeRef;
			TangentMode tangentMode;
		};

		Vector<CurveDrawInfo> mCurves;
		Vector<SelectedKeyframe> mSelectedKeyframes;

		float mYRange = 20.0f;
		float mYOffset = 0.0f;
		CurveDrawOptions mDrawOptions;
		GUIGraphTicks mTickHandler;
	};

	/** @} */
}
