//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include <utility>
#include "GUI/BsGUICurves.h"
#include "GUI/BsGUIDimensions.h"
#include "GUI/BsGUIMouseEvent.h"
#include "GUI/BsGUIHelper.h"
#include "GUI/BsGUICanvas.h"
#include "Math/BsLine2.h"
#include "GUI/BsGUIWidget.h"
#include "Image/BsPixelUtil.h"

namespace bs
{
	static constexpr int LINE_SPLIT_WIDTH = 2;
	static constexpr int TANGENT_LINE_DISTANCE = 30;
	static constexpr Color COLOR_MID_GRAY = Color(90.0f / 255.0f, 90.0f / 255.0f, 90.0f / 255.0f, 1.0f);
	static constexpr Color COLOR_DARK_GRAY = Color(40.0f / 255.0f, 40.0f / 255.0f, 40.0f / 255.0f, 1.0f);

	GUICurves::GUICurves(CurveDrawOptions drawOptions, const String& styleName, const GUIDimensions& dimensions)
		:GUITimeline(styleName, dimensions), mDrawOptions(drawOptions), mTickHandler(GUITickStepType::Time)
	{ }

	const String& GUICurves::getGUITypeName()
	{
		static String name = "GUICurves";
		return name;
	}

	GUICurves* GUICurves::create(const String& styleName)
	{
		return new (bs_alloc<GUICurves>()) GUICurves(CurveDrawOption::DrawMarkers | CurveDrawOption::DrawKeyframes, 
			getStyleName<GUICurves>(styleName), GUIDimensions::create());
	}

	GUICurves* GUICurves::create(CurveDrawOptions drawOptions, const String& styleName)
	{
		return new (bs_alloc<GUICurves>()) GUICurves(drawOptions, getStyleName<GUICurves>(styleName), GUIDimensions::create());
	}

	GUICurves* GUICurves::create(const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUICurves>()) GUICurves(CurveDrawOption::DrawMarkers | CurveDrawOption::DrawKeyframes,
			getStyleName<GUICurves>(styleName), GUIDimensions::create(options));
	}

	GUICurves* GUICurves::create(CurveDrawOptions drawOptions, const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUICurves>()) GUICurves(drawOptions, getStyleName<GUICurves>(styleName), 
			GUIDimensions::create(options));
	}

	void GUICurves::setCurves(const Vector<CurveDrawInfo>& curves)
	{
		if(mCurves == curves)
			return;

		mCurves = curves;
		_markContentAsDirty();
	}

	void GUICurves::setRange(float xRange, float yRange)
	{
		GUITimeline::setRange(xRange);
		mYRange = yRange;
	}

	void GUICurves::setOffset(const Vector2& offset)
	{
		GUITimeline::setOffset(offset.x);
		mYOffset = offset.y;
	}

	void GUICurves::centerAndZoom()
	{
		Vector<TAnimationCurve<float>> curves(mCurves.size());
		UINT32 idx = 0;
		for(auto& entry : mCurves)
			curves[idx++] = entry.curve;

		float xMin, xMax;
		float yMin, yMax;
		AnimationUtility::calculateRange(curves, xMin, xMax, yMin, yMax);

		float xRange = xMax - xMin;

		float yRange = (yMax - yMin);
		float yOffset = yMin + yRange * 0.5f;

		// Add padding to y range
		yRange *= 1.05f;

		// Don't allow zero range
		if (xRange == 0.0f)
			xRange = 60.0f;

		if (yRange == 0.0f)
			yRange = 10.0f;

		setOffset(Vector2(xMin, yOffset));
		setRange(xRange, yRange);
	}

	UINT32 GUICurves::findCurve(const Vector2I& pixelCoords)
	{
		Vector2 curveCoords;
		if (!pixelToCurveSpace(pixelCoords, curveCoords, true))
			return (UINT32)-1;

		const float time = curveCoords.x;

		float nearestDistance = std::numeric_limits<float>::max();
		auto curveIdx = (UINT32)-1;
		for (UINT32 i = 0; i < (UINT32)mCurves.size(); i++)
		{
			const TAnimationCurve<float>& curve = mCurves[i].curve;

			const float value = curve.evaluate(time, false);
			const Vector2I curPixelPos = curveToPixelSpace(Vector2(time, value));

			const auto distanceToKey = (float)pixelCoords.manhattanDist(curPixelPos);
			if (distanceToKey < nearestDistance)
			{
				nearestDistance = distanceToKey;
				curveIdx = i;
			}
		}

		// We're not near any curve
		if (nearestDistance > 5.0f)
			return (UINT32)-1;

		return curveIdx;
	}

	/**
	 * Attempts to find a keyframe under the provided coordinates.
	 *
	 * @param[in]	pixelCoords		Coordinates relative to this GUI element in pixels.
	 * @param[out]	keyframe		Output object containing keyframe index and index of the curve it belongs to. Only
	 *								valid if method returns true.
	 * @return						True if there is a keyframe under the coordinates, false otherwise.
	 */
	bool GUICurves::findKeyFrame(const Vector2I& pixelCoords, KeyframeRef& keyframe)
	{
		keyframe = KeyframeRef();

		float nearestDistance = std::numeric_limits<float>::max();
		for (UINT32 i = 0; i < (UINT32)mCurves.size(); i++)
		{
			const TAnimationCurve<float>& curve = mCurves[i].curve;
			const Vector<TKeyframe<float>>& keyframes = curve.getKeyFrames();

			for (UINT32 j = 0; j < (UINT32)keyframes.size(); j++)
			{
				const Vector2 keyframeCurveCoords = Vector2(keyframes[j].time, keyframes[j].value);
				const Vector2I keyframeCoords = curveToPixelSpace(keyframeCurveCoords);

				const auto distanceToKey = (float)pixelCoords.manhattanDist(keyframeCoords);
				if (distanceToKey < nearestDistance)
				{
					nearestDistance = distanceToKey;
					keyframe.keyIdx = j;
					keyframe.curveIdx = i;
				}
			}
		}

		return nearestDistance <= 5.0f;
	}

	/**
	 * Attempts to find a a tangent handle under the provided coordinates.
	 *
	 * @param[in]	pixelCoords		Coordinates relative to this GUI element in pixels.
	 * @param[in]	tangent			Output object containing keyframe information and tangent type. Only valid if method
	 *								returns true.
	 * @return						True if there is a tangent handle under the coordinates, false otherwise.
	 */
	bool GUICurves::findTangent(const Vector2I& pixelCoords, TangentRef& tangent)
	{
		tangent = TangentRef();

		float nearestDistance = std::numeric_limits<float>::max();
		for (auto& entry : mSelectedKeyframes)
		{
			KeyframeRef keyframeRef = entry.keyframeRef;
			if (keyframeRef.curveIdx < 0 || keyframeRef.curveIdx >= (INT32)mCurves.size())
				continue;

			const TAnimationCurve<float>& curve = mCurves[keyframeRef.curveIdx].curve;
			if (keyframeRef.keyIdx < 0 || keyframeRef.keyIdx >= (INT32)curve.getNumKeyFrames())
				continue;

			const Vector<TKeyframe<float>>& keyframes = curve.getKeyFrames();
			TangentMode tangentMode = entry.tangentMode;

			if (isTangentDisplayed(tangentMode, TangentType::In))
			{
				const Vector2I tangentCoords = getTangentPosition(keyframes[keyframeRef.keyIdx], TangentType::In);

				const auto distanceToHandle = (float)pixelCoords.manhattanDist(tangentCoords);
				if (distanceToHandle < nearestDistance)
				{
					nearestDistance = distanceToHandle;
					tangent.keyframeRef.keyIdx = keyframeRef.keyIdx;
					tangent.keyframeRef.curveIdx = keyframeRef.curveIdx;
					tangent.type = TangentType::In;
				}
			}

			if (isTangentDisplayed(tangentMode, TangentType::Out))
			{
				const Vector2I tangentCoords = getTangentPosition(keyframes[keyframeRef.keyIdx], TangentType::Out);

				const auto distanceToHandle = (float)pixelCoords.manhattanDist(tangentCoords);
				if (distanceToHandle < nearestDistance)
				{
					nearestDistance = distanceToHandle;
					tangent.keyframeRef.keyIdx = keyframeRef.keyIdx;
					tangent.keyframeRef.curveIdx = keyframeRef.curveIdx;
					tangent.type = TangentType::Out;
				}
			}
		}

		return nearestDistance <= 5.0f;
	}

	bool GUICurves::pixelToCurveSpace(const Vector2I& pixelCoords, Vector2& curveCoords, bool padding) const
	{
		const Rect2I& bounds = mLayoutData.area;

		bool outsideHorizontal;
		if (padding)
			outsideHorizontal = pixelCoords.x < 0 || pixelCoords.x >= (INT32)bounds.width;
		else
			outsideHorizontal = pixelCoords.x < (INT32)mPadding || pixelCoords.x >= ((INT32)bounds.width - (INT32)mPadding);

		// Check if outside of curve drawing bounds
		if (outsideHorizontal || pixelCoords.y < 0 || pixelCoords.y >= (INT32)bounds.height)
		{
			curveCoords = Vector2::ZERO;
			return false;
		}

		// Find time and value of the place under the coordinates
		const Vector2I relativeCoords = pixelCoords - Vector2I(mPadding, 0);

		const float lengthPerPixel = getRange() / getDrawableWidth();
		const float heightPerPixel = mYRange / mLayoutData.area.height;

		const float centerOffset = mYRange / 2.0f;

		const float t = mOffset + relativeCoords.x * lengthPerPixel;
		const float value = mYOffset + centerOffset - relativeCoords.y * heightPerPixel;

		curveCoords = Vector2();
		curveCoords.x = t;
		curveCoords.y = value;

		return true;
	}

	Vector2I GUICurves::curveToPixelSpace(const Vector2& curveCoords) const
	{
		const UINT32 heightOffset = mLayoutData.area.height / 2; // So that y = 0 is at center of canvas

		const Vector2 relativeCurveCoords = curveCoords - Vector2(mOffset, mYOffset);

		Vector2I pixelCoords = Vector2I();
		pixelCoords.x = (int)((relativeCurveCoords.x / getRange()) * getDrawableWidth()) + mPadding;
		pixelCoords.y = heightOffset - (int)((relativeCurveCoords.y / mYRange) * mLayoutData.area.height);

		return pixelCoords;
	}

	Vector2 GUICurves::tangentToNormal(float tangent) const
	{
		if (tangent == std::numeric_limits<float>::infinity())
			return Vector2(0, 1);

		Vector2 normal = Vector2(1, tangent);
		return Vector2::normalize(normal);
	}

	Vector2I GUICurves::getTangentPosition(const TKeyframe<float>& keyFrame, TangentType type) const
	{
		const Vector2I position = curveToPixelSpace(Vector2(keyFrame.time, keyFrame.value));

		Vector2 normal;
		if (type == TangentType::In)
			normal = -tangentToNormal(keyFrame.inTangent);
		else
			normal = tangentToNormal(keyFrame.outTangent);

		// X/Y ranges aren't scaled 1:1, adjust normal accordingly
		normal.x /= getRange();
		normal.y /= mYRange;
		normal = Vector2::normalize(normal);

		// Convert normal (in percentage) to pixel values
		Vector2I offset = Vector2I((int)(normal.x * TANGENT_LINE_DISTANCE),
			(int)(-normal.y * TANGENT_LINE_DISTANCE));

		return position + offset;
	}

	bool GUICurves::isTangentDisplayed(TangentMode mode, TangentType type) const
	{
		if (mode == TangentModeBits::Auto)
			return false;
		else if (mode == TangentModeBits::Free)
			return true;

		if (type == TangentType::In)
			return mode.isSet(TangentModeBits::InFree);
		else
			return mode.isSet(TangentModeBits::OutFree);
	}

	void GUICurves::drawFrameMarker(float t, Color color, bool onTop)
	{
		const INT32 xPos = (INT32)(((t - mOffset) / getRange()) * getDrawableWidth()) + mPadding;

		const Vector2I start = Vector2I(xPos, 0);
		const Vector2I end = Vector2I(xPos, mLayoutData.area.height);

		UINT8 depth;
		if (onTop)
			depth = 110;
		else
			depth = 130;

		mCanvas->drawLine(start, end, color, depth);
	}

	void GUICurves::drawCenterLine()
	{
		const Vector2I center = curveToPixelSpace(Vector2(0.0f, 0.0f));

		const Vector2I start = Vector2I(0, center.y);
		const Vector2I end = Vector2I(mLayoutData.area.width, center.y);

		mCanvas->drawLine(start, end, COLOR_DARK_GRAY, 130);
	}

	void GUICurves::drawDiamond(Vector2I center, int size, Color innerColor, Color outerColor)
	{
		const Vector2I a = Vector2I(center.x - size, center.y);
		const Vector2I b = Vector2I(center.x, center.y - size);
		const Vector2I c = Vector2I(center.x + size, center.y);
		const Vector2I d = Vector2I(center.x, center.y + size);

		// Draw diamond shape
		const Vector<Vector2I> linePoints = { a, b, c, d, a };
		const Vector<Vector2I> trianglePoints = { b, c, a, d };

		mCanvas->drawTriangleStrip(trianglePoints, innerColor, 101);
		mCanvas->drawPolyLine(linePoints, outerColor, 100);
	}

	void GUICurves::drawKeyframe(float t, float y, bool selected)
	{
		const Vector2I pixelCoords = curveToPixelSpace(Vector2(t, y));

		if (selected)
			drawDiamond(pixelCoords, 3, Color::White, Color::BansheeOrange);
		else
			drawDiamond(pixelCoords, 3, Color::White, Color::Black);
	}

	void GUICurves::drawTangents(const TKeyframe<float>& keyFrame, TangentMode tangentMode)
	{
		const Vector2I keyframeCoords = curveToPixelSpace(Vector2(keyFrame.time, keyFrame.value));

		if (isTangentDisplayed(tangentMode, TangentType::In))
		{
			Vector2I tangentCoords = getTangentPosition(keyFrame, TangentType::In);

			mCanvas->drawLine(keyframeCoords, tangentCoords, Color::LightGray);
			drawDiamond(tangentCoords, 2, Color::Green, Color::Black);
		}

		if (isTangentDisplayed(tangentMode, TangentType::Out))
		{
			Vector2I tangentCoords = getTangentPosition(keyFrame, TangentType::Out);

			mCanvas->drawLine(keyframeCoords, tangentCoords, Color::LightGray);
			drawDiamond(tangentCoords, 2, Color::Green, Color::Black);
		}
	}

	void GUICurves::drawCurve(const TAnimationCurve<float>& curve, const Color& color)
	{
		const float range = getRangeWithPadding();
		const float widthPerPixel = range / getDrawableWidth();
		const float heightPerPixel = mYRange / mLayoutData.area.height;

		const Vector<TKeyframe<float>>& keyframes = curve.getKeyFrames();
		if (keyframes.empty())
			return;

		// Draw start line
		{
			const float curveStart = keyframes[0].time;
			const float curveValue = curve.evaluate(curveStart, false);

			const Vector2I end = curveToPixelSpace(Vector2(curveStart, curveValue));
			const Vector2I start = Vector2I(-(INT32)mPadding, end.y);

			if (start.x < end.x)
				mCanvas->drawLine(start, end, COLOR_MID_GRAY);
		}

		Vector<Vector2I> linePoints;

		// Draw in between keyframes
		const float startVisibleTime = mOffset;
		const float endVisibleTime = startVisibleTime + range;
		for (INT32 i = 0; i < (INT32)keyframes.size() - 1; i++)
		{
			const float start = keyframes[i].time;
			const float end = keyframes[i + 1].time;

			if (end < startVisibleTime || start > endVisibleTime)
				continue;

			const bool isStep = keyframes[i].outTangent == std::numeric_limits<float>::infinity() ||
				keyframes[i + 1].inTangent == std::numeric_limits<float>::infinity();

			// If step tangent, draw the required lines without sampling, as the sampling will miss the step
			if (isStep)
			{
				const float startValue = curve.evaluate(start, false);
				const float endValue = curve.evaluate(end, false);

				linePoints.push_back(curveToPixelSpace(Vector2(start, startValue)));
				linePoints.push_back(curveToPixelSpace(Vector2(end, startValue)));
				linePoints.push_back(curveToPixelSpace(Vector2(end, endValue)));
			}
			else // Draw normally
			{
				const float startValue = keyframes[i].value;
				const float endValue = keyframes[i + 1].value;

				Vector2I startPixel = curveToPixelSpace(Vector2(start, startValue));
				Vector2I endPixel = curveToPixelSpace(Vector2(end, endValue));

				const UINT32 distance = startPixel.manhattanDist(endPixel);

				INT32 numSplits;
				float splitIncrement;
				if (distance > 0)
				{
					const float fNumSplits = distance / (float)LINE_SPLIT_WIDTH;

					numSplits = Math::ceilToInt(fNumSplits) + 1;
					splitIncrement = (end - start) / (float)numSplits;
				}
				else
				{
					numSplits = 1;
					splitIncrement = 0.0f;
				}

				for (int j = 0; j < numSplits; j++)
				{
					const float t = std::min(start + j * splitIncrement, end);
					const float value = curve.evaluate(t, false);

					linePoints.push_back(curveToPixelSpace(Vector2(t, value)));
				}
			}
		}

		mCanvas->drawPolyLine(linePoints, color);

		// Draw end line
		{
			const float curveEnd = keyframes[keyframes.size() - 1].time;
			const float curveValue = curve.evaluate(curveEnd, false);

			const Vector2I start = curveToPixelSpace(Vector2(curveEnd, curveValue));
			const Vector2I end = Vector2I(mLayoutData.area.width, start.y);

			if (start.x < end.x)
				mCanvas->drawLine(start, end, COLOR_MID_GRAY);
		}
	}

	void GUICurves::drawCurveRange(const Vector<TAnimationCurve<float>>& curves, Color color)
	{
		const float range = getRangeWithPadding();

		if (curves.size() != 2)
			return;

		const Vector<TKeyframe<float>>* keyframes[2] = { &curves[0].getKeyFrames(), &curves[1].getKeyFrames() };
		if (keyframes[0]->empty() || keyframes[1]->empty())
			return;

		const UINT32 numSamples = (getDrawableWidth() + LINE_SPLIT_WIDTH - 1) / LINE_SPLIT_WIDTH;
		const float timePerSample = range / numSamples;

		float time = mOffset;
		const float lengthPerPixel = mRange / getDrawableWidth();
		time -= lengthPerPixel * mPadding;

		INT32 keyframeIndices[] = { 0, 0 };

		// Find first valid keyframe indices
		for (UINT32 curveIdx = 0; curveIdx < 2; curveIdx++)
		{
			keyframeIndices[curveIdx] = (INT32)keyframes[curveIdx]->size();

			for (UINT32 i = 0; i < (UINT32)keyframes[curveIdx]->size(); i++)
			{
				if ((*keyframes[curveIdx])[i].time > time)
					keyframeIndices[curveIdx] = i;
			}
		}

		Vector<float> times;
		Vector<float> points[2];

		// Determine start points
		for (int curveIdx = 0; curveIdx < 2; curveIdx++)
		{
			float value = curves[curveIdx].evaluate(time, false);
			points[curveIdx].push_back(value);
		}

		times.push_back(time);

		const float rangeEnd = mOffset + range;
		while (time < rangeEnd)
		{
			float nextTime = time + timePerSample;
			bool hasStep = false;

			// Determine time to sample at. Use fixed increments unless there's a step keyframe within our increment in
			// which case we use its time so we can evaluate it directly
			for (UINT32 curveIdx = 0; curveIdx < 2; curveIdx++)
			{
				const INT32 keyframeIdx = keyframeIndices[curveIdx];
				if (keyframeIdx < (INT32)keyframes[curveIdx]->size())
				{
					const TKeyframe<float>& keyframe = (*keyframes[curveIdx])[keyframeIdx];

					const bool isStep = keyframe.inTangent == std::numeric_limits<float>::infinity() ||
						keyframe.outTangent == std::numeric_limits<float>::infinity();

					if (isStep && keyframe.time <= nextTime)
					{
						nextTime = std::min(nextTime, keyframe.time);
						hasStep = true;
					}
				}
			}

			// Evaluate
			if (hasStep)
			{
				for (UINT32 curveIdx = 0; curveIdx < 2; curveIdx++)
				{
					const INT32 keyframeIdx = keyframeIndices[curveIdx];
					if (keyframeIdx < (INT32)keyframes[curveIdx]->size())
					{
						const TKeyframe<float>& keyframe = (*keyframes[curveIdx])[keyframeIdx];

						if (Math::approxEquals(keyframe.time, nextTime))
						{
							if (keyframeIdx > 0)
							{
								const TKeyframe<float>& prevKeyframe = (*keyframes[curveIdx])[keyframeIdx - 1];
								points[curveIdx].push_back(prevKeyframe.value);
							}
							else
								points[curveIdx].push_back(keyframe.value);

							points[curveIdx].push_back(keyframe.value);
						}
						else
						{
							// The other curve has step but this one doesn't, we just insert the same value twice
							float value = curves[curveIdx].evaluate(nextTime, false);
							points[curveIdx].push_back(value);
							points[curveIdx].push_back(value);
						}

						times.push_back(nextTime);
						times.push_back(nextTime);
					}
				}
			}
			else
			{
				for (UINT32 curveIdx = 0; curveIdx < 2; curveIdx++)
					points[curveIdx].push_back(curves[curveIdx].evaluate(nextTime, false));

				times.push_back(nextTime);
			}

			// Advance keyframe indices
			for (UINT32 curveIdx = 0; curveIdx < 2; curveIdx++)
			{
				INT32 keyframeIdx = keyframeIndices[curveIdx];
				while (keyframeIdx < (INT32)keyframes[curveIdx]->size())
				{
					const TKeyframe<float>& keyframe = (*keyframes[curveIdx])[keyframeIdx];
					if (keyframe.time > nextTime)
						break;

					keyframeIdx = ++keyframeIndices[curveIdx];
				}
			}

			time = nextTime;
		}

		// End points
		for (UINT32 curveIdx = 0; curveIdx < 2; curveIdx++)
		{
			float value = curves[curveIdx].evaluate(rangeEnd, false);
			points[curveIdx].push_back(value);
		}

		times.push_back(rangeEnd);

		const INT32 numQuads = (INT32)times.size() - 1;
		Vector<Vector2I> vertices;

		for (int i = 0; i < numQuads; i++)
		{
			const INT32 idxLeft = points[0][i] < points[1][i] ? 0 : 1;
			const INT32 idxRight = points[0][i + 1] < points[1][i + 1] ? 0 : 1;

			Vector2 left[] =
			{
				Vector2(times[i], points[0][i]),
				Vector2(times[i], points[1][i])
			};

			Vector2 right[] =
			{
				Vector2(times[i + 1], points[0][i + 1]),
				Vector2(times[i + 1], points[1][i + 1])
			};

			if (idxLeft == idxRight)
			{
				const INT32 idxA = idxLeft;
				const INT32 idxB = (idxLeft + 1) % 2;

				vertices.push_back(curveToPixelSpace(left[idxB]));
				vertices.push_back(curveToPixelSpace(right[idxB]));
				vertices.push_back(curveToPixelSpace(left[idxA]));

				vertices.push_back(curveToPixelSpace(right[idxB]));
				vertices.push_back(curveToPixelSpace(right[idxA]));
				vertices.push_back(curveToPixelSpace(left[idxA]));
			}
			// Lines intersects, can't represent them with a single quad
			else if (idxLeft != idxRight)
			{
				const INT32 idxA = idxLeft;
				const INT32 idxB = (idxLeft + 1) % 2;

				Line2 lineA(left[idxB], right[idxA] - left[idxB]);
				Line2 lineB(left[idxA], right[idxB] - left[idxA]);

				const auto result = lineA.intersects(lineB);

				if (result.first)
				{
					const float t = result.second;
					const Vector2 intersection = left[idxB] + t * (right[idxA] - left[idxB]);

					vertices.push_back(curveToPixelSpace(left[idxB]));
					vertices.push_back(curveToPixelSpace(intersection));
					vertices.push_back(curveToPixelSpace(left[idxA]));

					vertices.push_back(curveToPixelSpace(intersection));
					vertices.push_back(curveToPixelSpace(right[idxB]));
					vertices.push_back(curveToPixelSpace(right[idxA]));
				}
			}
		}

		mCanvas->drawTriangleList(vertices, color, 129);
	}

	void GUICurves::selectKeyframe(const KeyframeRef& keyframeRef, const TangentMode& tangentMode, bool selected)
	{
		auto foundIdx = (UINT32)-1;
		for (UINT32 i = 0; i < (UINT32)mSelectedKeyframes.size(); i++)
		{
			const SelectedKeyframe entry = mSelectedKeyframes[i];

			if (entry.keyframeRef.keyIdx == keyframeRef.keyIdx && entry.keyframeRef.curveIdx == keyframeRef.curveIdx &&
				entry.tangentMode == tangentMode)
			{
				foundIdx = i;
				break;
			}
		}

		if (selected)
		{
			if (foundIdx == (UINT32)-1)
				mSelectedKeyframes.push_back(SelectedKeyframe(keyframeRef, tangentMode));
		}
		else
			mSelectedKeyframes.erase(mSelectedKeyframes.begin() + foundIdx);

		_markContentAsDirty();
	}

	void GUICurves::clearSelectedKeyframes()
	{
		mSelectedKeyframes.clear();

		_markContentAsDirty();
	}

	bool GUICurves::isSelected(int curveIdx, int keyIdx) const
	{
		for(auto& entry : mSelectedKeyframes)
		{
			if (entry.keyframeRef.curveIdx == curveIdx && entry.keyframeRef.keyIdx == keyIdx)
				return true;
		}

		return false;
	}

	void GUICurves::updateRenderElementsInternal()
	{
		mCanvas->clear();

		bool drawMarkers = mDrawOptions.isSet(CurveDrawOption::DrawMarkers);
		if (drawMarkers)
		{
			mTickHandler.setRange(mOffset, mOffset + getRangeWithPadding(), getDrawableWidth() + mPadding);

			// Draw vertical frame markers
			struct TickLevel
			{
				float t;
				float strength;
			};

			struct TickComparator
			{
				bool operator()(const TickLevel& a, const TickLevel& b) const
				{
					if (fabs(a.t - b.t) > 0.001f)
						return a.t < b.t;

					return false;
				}
			};

			Set<TickLevel, TickComparator> uniqueTicks;

			const INT32 numTickLevels = (INT32)mTickHandler.getNumLevels();
			for (INT32 i = 0; i < numTickLevels; i++)
			{
				Vector<float> ticks = mTickHandler.getTicks(i);
				const float strength = mTickHandler.getLevelStrength(i);

				for (UINT32 j = 0; j < (UINT32)ticks.size(); j++)
					uniqueTicks.insert({ticks[j], strength });
			}

			for(auto& entry : uniqueTicks)
			{
				Color color = COLOR_DARK_GRAY.getGamma();
				color *= entry.strength;

				drawFrameMarker(entry.t, color, false);
			}

			// Draw center line
			drawCenterLine();
		}

		// Draw range
		const bool drawRange = mDrawOptions.isSet(CurveDrawOption::DrawRange);
		auto curvesToDraw = (UINT32)mCurves.size();
		if (drawRange && mCurves.size() >= 2)
		{
			Vector<TAnimationCurve<float>> curves = { mCurves[0].curve, mCurves[1].curve };

			drawCurveRange(curves, Color(1.0f, 0.0f, 0.0f, 0.3f));
			curvesToDraw = 2;
		}

		// Draw curves
		const bool drawKeyframes = mDrawOptions.isSet(CurveDrawOption::DrawKeyframes);
		for (UINT32 i = 0; i < curvesToDraw; i++)
		{
			drawCurve(mCurves[i].curve, mCurves[i].color);

			// Draw keyframes
			if (drawKeyframes)
			{
				const Vector<TKeyframe<float>> keyframes = mCurves[i].curve.getKeyFrames();
				for (UINT32 j = 0; j < (UINT32)keyframes.size(); j++)
				{
					const bool selected = isSelected(i, j);
					drawKeyframe(keyframes[j].time, keyframes[j].value, selected);
				}
			}
		}

		// Draw tangents
		for (auto& entry : mSelectedKeyframes)
		{
			KeyframeRef keyframeRef = entry.keyframeRef;
			if (keyframeRef.curveIdx < 0 || keyframeRef.curveIdx >= (INT32)mCurves.size())
				continue;

			TAnimationCurve<float> curve = mCurves[keyframeRef.curveIdx].curve;
			if (keyframeRef.keyIdx < 0 || keyframeRef.keyIdx >= (INT32)curve.getNumKeyFrames())
				continue;

			const Vector<TKeyframe<float>> keyframes = mCurves[keyframeRef.curveIdx].curve.getKeyFrames();
			const TangentMode tangentMode = entry.tangentMode;

			drawTangents(keyframes[keyframeRef.keyIdx], tangentMode);
		}

		// Draw selected frame marker
		if (drawMarkers && mMarkedFrame != (UINT32)-1)
			drawFrameMarker(getTimeForFrame(mMarkedFrame), Color::BansheeOrange, true);

		GUIElement::updateRenderElementsInternal();
	}

	bool GUICurves::_mouseEvent(const GUIMouseEvent& ev)
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
