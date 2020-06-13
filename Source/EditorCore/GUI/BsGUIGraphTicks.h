//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Math/BsMath.h"

namespace bs
{
	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/** Determines how should ticks reported by <see cref="GUIGraphTicks"/> be distributed. */
	enum class BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) GUITickStepType
	{
		/** Ticks represent time values (Multiples of 60). */
		Time,
		/** Ticks represent generic values (Multiples of 10). */
		Generic
	};

	/**
	 * Generates a set of locations that can be used for rendering ticks on a graph. As input the class takes valid range,
	 * size of the area the ticks will be displayed on, type of ticks and minimum/maximum spacing and outputs a set of
	 * coordinates along which ticks should be positioned. Ticks are reported as multiple separate levels with different
	 * strengths, depending on how close their distribution is to the upper valid range.
	 */
	class BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) GUIGraphTicks
	{
	public:
		/**
		 * Contructs a new tick generating object.
		 *
		 * @param[in]	stepType	Determines how will ticks be distributed.
		 */
		BS_SCRIPT_EXPORT()
		GUIGraphTicks(GUITickStepType stepType = GUITickStepType::Generic)
		{
			if(stepType == GUITickStepType::Generic)
				setGenericSteps();
			else
				setTimeSteps();

			rebuild();
		}

		/** Number of tick levels that will be generated. */
		BS_SCRIPT_EXPORT(pr:getter,n:NumLevels)
		UINT32 getNumLevels() const
		{
			return mNumLevels;
		}

		/**
		 * Sets the range which ticks are to be displayed for, and the range along which the ticks will be displayed.
		 *
		 * @param[in]	valueRangeStart		Start of the range the ticks are to display.
		 * @param[in]	valueRangeEnd		End of the range the ticks are to display.
		 * @param[in]	pixelRange			Width or height on which the ticks will be rendered. In pixels.
		 */
		BS_SCRIPT_EXPORT()
		void setRange(float valueRangeStart, float valueRangeEnd, UINT32 pixelRange)
		{
			this->mValueRangeStart = valueRangeStart;
			this->mValueRangeEnd = valueRangeEnd;
			this->mPixelRange = pixelRange;

			rebuild();
		}

		/**
		 * Sets valid spacing between two ticks. Tick strength will be determined by how far away are they from either
		 * end of this range.
		 *
		 * @param[in]	minPx		Minimum spacing between two ticks, in pixels.
		 * @param[in]	maxPx		Maximum spacing between two ticks, in pixels.
		 */
		BS_SCRIPT_EXPORT()
		void setTickSpacing(int minPx, int maxPx)
		{
			mMinTickSpacingPx = minPx;
			mMaxTickSpacingPx = maxPx;

			rebuild();
		}

		/**
		 * Returns the strength of a particular tick level. Levels are ordered in descending order of strength (level 0 is
		 * the strongest).
		 *
		 * @param[in]	level	Level for which to retrieve the strength. Must not be larger than getNumLevels() - 1.
		 * @return				Strength of the ticks at this level, in range [0, 1].
		 */
		BS_SCRIPT_EXPORT()
		float getLevelStrength(UINT32 level)
		{
			if (level >= mNumLevels)
				return 0.0f;

			return Math::clamp01(mLevelStrengths[mMaxLevel + level]);
		}

		/**
		 * Returns positions of all ticks of the provided level. The ticks will be within the range provided to setRange().
		 * 
		 * @param[in]	level	Level for which to retrieve the positions. Must not be larger than getNumLevels() - 1.
		 * @return				Positions of all ticks of the provided level.
		 */
		BS_SCRIPT_EXPORT()
		Vector<float> getTicks(UINT32 level)
		{
			if (level < 0 || level >= mNumLevels)
				return { };

			const float step = mValidSteps[mMaxLevel + level];

			// Round up and down so we get one extra tick on either side (outside of value range)
			// (Useful when rendering text above the ticks, so the text doesn't just pop-in when the tick appears, instead
			// it is slowly clipped-in.)
			const INT32 startTick = Math::ceilToInt(mValueRangeStart / step);
			const INT32 endTick = Math::floorToInt(mValueRangeEnd / step);

			const INT32 numTicks = endTick - startTick + 1;

			Vector<float> ticks(numTicks);
			for (INT32 i = startTick; i <= endTick; i++)
				ticks[i - startTick] = i*step;

			return ticks;
		}

	protected:
		/** Rebuilds the tick positions and strengths after some relevant parameter changes. */
		void rebuild()
		{
			mLevelStrengths = Vector<float>(mValidSteps.size());
			mMaxLevel = 0;

			const float valueRange = mValueRangeEnd - mValueRangeStart;
			const INT32 tickSpacing = (INT32)mMaxTickSpacingPx - (INT32)mMinTickSpacingPx;
			UINT32 i = 0;
			for (; i < (UINT32)mValidSteps.size(); i++)
			{
				const float tickSpacingPx = (mValidSteps[i]/valueRange) * mPixelRange;
				mLevelStrengths[i] = (tickSpacingPx - mMinTickSpacingPx)/tickSpacing;

				if (mLevelStrengths[i] > 1.0f)
					mMaxLevel = i;
				else if (mLevelStrengths[i] < 0.0f)
					break;
			}

			if (i > 0)
				mNumLevels = i - mMaxLevel;
			else
				mNumLevels = 0;
		}

		/** 
		 * Sets tick steps corresponding to time values. This will split the ticks into intervals relevant for displaying
		 * times.
		 */
		void setTimeSteps()
		{
			mValidSteps = 
			{
				3600.0f, 1800.0f, 600.0f, 300.0f,
				60.0f, 30.0f, 10.0f, 5.0f,
				1.0f, 0.5f, 0.25f, 0.1f, 0.05f, 0.01f
			};
		}

		/** Sets tick steps corresponding to generic values (as opposed to displaying time values). */
		void setGenericSteps()
		{
			constexpr UINT32 numSteps = 15;
			float minStep = 0.0000001f;

			mValidSteps = Vector<float>(numSteps * 2);
			for (int i = numSteps - 1; i >= 0; i--)
			{
				mValidSteps[i * 2 + 1] = minStep;
				mValidSteps[i * 2 + 0] = minStep * 5;

				minStep *= 10.0f;
			}
		}

		UINT32 mPixelRange = 100;
		float mValueRangeStart = 0.0f;
		float mValueRangeEnd = 1.0f;

		UINT32 mMinTickSpacingPx = 5;
		UINT32 mMaxTickSpacingPx = 30;

		Vector<float> mValidSteps = { 1.0f };
		Vector<float> mLevelStrengths = { 1.0f };
		UINT32 mNumLevels = 1;
		UINT32 mMaxLevel = 0;
	};

	/** @} */
}
