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
	/// Base class that can be implemented by GUI elements needing to elements along draw a horizontal timeline.
	/// </summary>
	[ShowInInspector]
	public partial class GUITimeline : GUIElement
	{
		private GUITimeline(bool __dummy0) { }
		protected GUITimeline() { }

		/// <summary>Determines the range of values to display on the timeline, in seconds.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public float Range
		{
			get { return Internal_getRange(mCachedPtr); }
			set { Internal_setRange(mCachedPtr, value); }
		}

		/// <summary>Determines the offset at which the displayed timeline values start at, in seconds.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public float Offset
		{
			get { return Internal_getOffset(mCachedPtr); }
			set { Internal_setOffset(mCachedPtr, value); }
		}

		/// <summary>Number of frames per second, used for frame selection and marking.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int FPS
		{
			get { return Internal_getFPS(mCachedPtr); }
			set { Internal_setFPS(mCachedPtr, value); }
		}

		/// <summary>Frame to display the frame marker on. Set to -1 to clear the frame marker.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int MarkedFrame
		{
			get { return Internal_setMarkedFrame0(mCachedPtr); }
			set { Internal_setMarkedFrame(mCachedPtr, value); }
		}

		/// <summary>Sets the size of padding to apply to the left and right sides of the curve drawing, in pixels.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int Padding
		{
			get { return Internal_getPadding(mCachedPtr); }
			set { Internal_setPadding(mCachedPtr, value); }
		}

		/// <summary>
		/// Uses the assigned FPS, range and physical size to calculate the frame that is under the provided coordinates.
		/// </summary>
		/// <param name="pixelCoords">Coordinates relative to this GUI element.</param>
		/// <returns>Frame that was clicked on, or -1 if the coordinates are outside of valid bounds.</returns>
		public int GetFrame(Vector2I pixelCoords)
		{
			return Internal_getFrame(mCachedPtr, ref pixelCoords);
		}

		/// <summary>Returns the time at the specified pixel value along the timeline.</summary>
		/// <param name="pixel">X coordinate to sample at, relative to this GUI element in pixels.</param>
		/// <returns>Time along the curve at the specified coordinate.</returns>
		public float GetTime(int pixel)
		{
			return Internal_getTime(mCachedPtr, pixel);
		}

		/// <summary>Finds the pixel offset relative to the GUI element&apos;s origin for the specified time.</summary>
		/// <param name="time">Time value to return the offset for.</param>
		/// <returns>Offset in pixels relative to GUI element&apos;s origin</returns>
		public int GetOffset(float time)
		{
			return Internal_getOffset0(mCachedPtr, time);
		}

		/// <summary>Returns time for a frame with the specified index. Depends on set range and FPS.</summary>
		/// <param name="index">Index of the frame (not a key-frame) to get the time for.</param>
		/// <returns>Time of the frame with the provided index.</returns>
		public float GetTimeForFrame(int index)
		{
			return Internal_getTimeForFrame(mCachedPtr, index);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setRange(IntPtr thisPtr, float range);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_getRange(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setOffset(IntPtr thisPtr, float offset);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_getOffset(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setFPS(IntPtr thisPtr, int FPS);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_getFPS(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setMarkedFrame(IntPtr thisPtr, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_setMarkedFrame0(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_getFrame(IntPtr thisPtr, ref Vector2I pixelCoords);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_getTime(IntPtr thisPtr, int pixel);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_getOffset0(IntPtr thisPtr, float time);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_getTimeForFrame(IntPtr thisPtr, int index);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setPadding(IntPtr thisPtr, int padding);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_getPadding(IntPtr thisPtr);
	}

	/** @} */
}
