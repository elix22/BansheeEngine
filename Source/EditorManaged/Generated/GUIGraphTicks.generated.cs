//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup GUIEditor
	 *  @{
	 */

	/// <summary>
	/// Generates a set of locations that can be used for rendering ticks on a graph. As input the class takes valid range, 
	/// size of the area the ticks will be displayed on, type of ticks and minimum/maximum spacing and outputs a set of 
	/// coordinates along which ticks should be positioned. Ticks are reported as multiple separate levels with different 
	/// strengths, depending on how close their distribution is to the upper valid range.
	/// </summary>
	[ShowInInspector]
	public partial class GUIGraphTicks : ScriptObject
	{
		private GUIGraphTicks(bool __dummy0) { }
		protected GUIGraphTicks() { }

		/// <summary>Contructs a new tick generating object.</summary>
		/// <param name="stepType">Determines how will ticks be distributed.</param>
		public GUIGraphTicks(GUITickStepType stepType = GUITickStepType.Generic)
		{
			Internal_GUIGraphTicks(this, stepType);
		}

		/// <summary>Number of tick levels that will be generated.</summary>
		[NativeWrapper]
		public int NumLevels
		{
			get { return Internal_getNumLevels(mCachedPtr); }
		}

		/// <summary>
		/// Sets the range which ticks are to be displayed for, and the range along which the ticks will be displayed.
		/// </summary>
		/// <param name="valueRangeStart">Start of the range the ticks are to display.</param>
		/// <param name="valueRangeEnd">End of the range the ticks are to display.</param>
		/// <param name="pixelRange">Width or height on which the ticks will be rendered. In pixels.</param>
		public void SetRange(float valueRangeStart, float valueRangeEnd, int pixelRange)
		{
			Internal_setRange(mCachedPtr, valueRangeStart, valueRangeEnd, pixelRange);
		}

		/// <summary>
		/// Sets valid spacing between two ticks. Tick strength will be determined by how far away are they from either end of 
		/// this range.
		/// </summary>
		/// <param name="minPx">Minimum spacing between two ticks, in pixels.</param>
		/// <param name="maxPx">Maximum spacing between two ticks, in pixels.</param>
		public void SetTickSpacing(int minPx, int maxPx)
		{
			Internal_setTickSpacing(mCachedPtr, minPx, maxPx);
		}

		/// <summary>
		/// Returns the strength of a particular tick level. Levels are ordered in descending order of strength (level 0 is the 
		/// strongest).
		/// </summary>
		/// <param name="level">Level for which to retrieve the strength. Must not be larger than getNumLevels() - 1.</param>
		/// <returns>Strength of the ticks at this level, in range [0, 1].</returns>
		public float GetLevelStrength(int level)
		{
			return Internal_getLevelStrength(mCachedPtr, level);
		}

		/// <summary>
		/// Returns positions of all ticks of the provided level. The ticks will be within the range provided to setRange().
		/// </summary>
		/// <param name="level">Level for which to retrieve the positions. Must not be larger than getNumLevels() - 1.</param>
		/// <returns>Positions of all ticks of the provided level.</returns>
		public float[] GetTicks(int level)
		{
			return Internal_getTicks(mCachedPtr, level);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GUIGraphTicks(GUIGraphTicks managedInstance, GUITickStepType stepType);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_getNumLevels(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setRange(IntPtr thisPtr, float valueRangeStart, float valueRangeEnd, int pixelRange);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setTickSpacing(IntPtr thisPtr, int minPx, int maxPx);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_getLevelStrength(IntPtr thisPtr, int level);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float[] Internal_getTicks(IntPtr thisPtr, int level);
	}

	/** @} */
}
