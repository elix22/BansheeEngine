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

	/// <summary>GUI element that displays one or multiple curves.</summary>
	[ShowInInspector]
	public partial class GUICurves : GUITimeline
	{
		private GUICurves(bool __dummy0) { }
		protected GUICurves() { }

		/// <summary>Creates a new GUI element.</summary>
		/// <param name="styleName">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurves(string styleName = "")
		{
			Internal_create(this, styleName);
		}

		/// <summary>Creates a new GUI element.</summary>
		/// <param name="drawOptions">Options that control which additional elements to draw.</param>
		/// <param name="styleName">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurves(CurveDrawOptions drawOptions, string styleName = "")
		{
			Internal_create0(this, drawOptions, styleName);
		}

		/// <summary>Animation curves to display.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public CurveDrawInfo[] Curves
		{
			get { return Internal_getCurves(mCachedPtr); }
			set { Internal_setCurves(mCachedPtr, value); }
		}

		/// <summary>Triggered when the user clicks on the GUI element.</summary>
		partial void Callback_OnClicked();

		/// <summary>Changes the visible range that the GUI element displays.</summary>
		/// <param name="xRange">Range of the horizontal area. Displayed area will range from [0, xRange].</param>
		/// <param name="yRange">Range of the vertical area. Displayed area will range from [-yRange * 0.5, yRange * 0.5]</param>
		public void SetRange(float xRange, float yRange)
		{
			Internal_setRange(mCachedPtr, xRange, yRange);
		}

		/// <summary>Returns the offset at which the displayed timeline values start at.</summary>
		/// <param name="offset">Value to start the timeline values at, where x = time, y = value.</param>
		public void SetOffset(Vector2 offset)
		{
			Internal_setOffset(mCachedPtr, ref offset);
		}

		/// <summary>Centers and zooms the view to fully display the provided set of curves.</summary>
		public void CenterAndZoom()
		{
			Internal_centerAndZoom(mCachedPtr);
		}

		/// <summary>Converts pixel coordinates into coordinates in curve space.</summary>
		/// <param name="pixelCoords">Coordinates relative to this GUI element, in pixels.</param>
		/// <param name="curveCoords">
		/// Curve coordinates within the range as specified by setRange(). Only valid when function returns true.
		/// </param>
		/// <param name="padding">Determines should coordinates over the area reserved for padding be registered.</param>
		/// <returns>True if the coordinates are within the curve area, false otherwise.</returns>
		public bool PixelToCurveSpace(Vector2I pixelCoords, out Vector2 curveCoords, bool padding = false)
		{
			return Internal_pixelToCurveSpace(mCachedPtr, ref pixelCoords, out curveCoords, padding);
		}

		/// <summary>
		/// Converts coordinate in curve space (time, value) into pixel coordinates relative to this element&apos;s origin.
		/// </summary>
		/// <param name="curveCoords">Time and value of the location to convert.</param>
		/// <returns>Coordinates relative to this element&apos;s origin, in pixels.</returns>
		public Vector2I CurveToPixelSpace(Vector2 curveCoords)
		{
			Vector2I temp;
			Internal_curveToPixelSpace(mCachedPtr, ref curveCoords, out temp);
			return temp;
		}

		/// <summary>Attempts to find a curve under the provided coordinates.</summary>
		/// <param name="pixelCoords">Coordinates relative to this GUI element in pixels.</param>
		/// <returns>Index of the curve, or -1 if none found.</returns>
		public int FindCurve(Vector2I pixelCoords)
		{
			return Internal_findCurve(mCachedPtr, ref pixelCoords);
		}

		/// <summary>Attempts to find a keyframe under the provided coordinates.</summary>
		/// <param name="pixelCoords">Coordinates relative to this GUI element in pixels.</param>
		/// <param name="keyframe">
		/// Output object containing keyframe index and index of the curve it belongs to. Only  valid if method returns true.
		/// </param>
		/// <returns>True if there is a keyframe under the coordinates, false otherwise.</returns>
		public bool FindKeyFrame(Vector2I pixelCoords, out KeyframeRef keyframe)
		{
			return Internal_findKeyFrame(mCachedPtr, ref pixelCoords, out keyframe);
		}

		/// <summary>Attempts to find a a tangent handle under the provided coordinates.</summary>
		/// <param name="pixelCoords">Coordinates relative to this GUI element in pixels.</param>
		/// <param name="tangent">
		/// Output object containing keyframe information and tangent type. Only valid if method returns true.
		/// </param>
		/// <returns>True if there is a tangent handle under the coordinates, false otherwise.</returns>
		public bool FindTangent(Vector2I pixelCoords, out TangentRef tangent)
		{
			return Internal_findTangent(mCachedPtr, ref pixelCoords, out tangent);
		}

		/// <summary>Marks the specified key-frame as selected, changing the way it is displayed.</summary>
		/// <param name="keyframeRef">Keyframe reference containing the curve and keyframe index.</param>
		/// <param name="tangentMode">Type of tangent to display on the selected keyframe.</param>
		/// <param name="selected">True to select it, false to deselect it.</param>
		public void SelectKeyframe(KeyframeRef keyframeRef, TangentMode tangentMode, bool selected)
		{
			Internal_selectKeyframe(mCachedPtr, ref keyframeRef, tangentMode, selected);
		}

		/// <summary>Clears any key-frames that were marked as selected.</summary>
		public void ClearSelectedKeyframes()
		{
			Internal_clearSelectedKeyframes(mCachedPtr);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setCurves(IntPtr thisPtr, CurveDrawInfo[] curves);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern CurveDrawInfo[] Internal_getCurves(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setRange(IntPtr thisPtr, float xRange, float yRange);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setOffset(IntPtr thisPtr, ref Vector2 offset);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_centerAndZoom(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_pixelToCurveSpace(IntPtr thisPtr, ref Vector2I pixelCoords, out Vector2 curveCoords, bool padding);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_curveToPixelSpace(IntPtr thisPtr, ref Vector2 curveCoords, out Vector2I __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_findCurve(IntPtr thisPtr, ref Vector2I pixelCoords);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_findKeyFrame(IntPtr thisPtr, ref Vector2I pixelCoords, out KeyframeRef keyframe);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_findTangent(IntPtr thisPtr, ref Vector2I pixelCoords, out TangentRef tangent);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_selectKeyframe(IntPtr thisPtr, ref KeyframeRef keyframeRef, TangentMode tangentMode, bool selected);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_clearSelectedKeyframes(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(GUICurves managedInstance, string styleName);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create0(GUICurves managedInstance, CurveDrawOptions drawOptions, string styleName);
		private void Internal_onClicked()
		{
			Callback_OnClicked();
		}
	}

	/** @} */
}
