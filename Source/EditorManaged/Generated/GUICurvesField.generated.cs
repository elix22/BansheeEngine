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
	/// A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field. 
	/// Label is optional. This specific implementation displays an animation curve or a range between two animation curves.
	/// </summary>
	[ShowInInspector]
	public partial class GUICurvesField : GUIElement
	{
		private GUICurvesField(bool __dummy0) { }
		protected GUICurvesField() { }

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="drawOptions">Options that control which additional curve elements to draw.</param>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(CurveDrawOptions drawOptions, GUIContent labelContent, int labelWidth, string style = "")
		{
			Internal_create(this, drawOptions, ref labelContent, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="drawOptions">Options that control which additional curve elements to draw.</param>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(CurveDrawOptions drawOptions, LocString labelText, int labelWidth, string style = "")
		{
			Internal_create0(this, drawOptions, labelText, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field without a label.</summary>
		/// <param name="drawOptions">Options that control which additional curve elements to draw.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(CurveDrawOptions drawOptions, string style = "")
		{
			Internal_create1(this, drawOptions, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(GUIContent labelContent, int labelWidth, string style = "")
		{
			Internal_create2(this, ref labelContent, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(GUIContent labelContent, string style = "")
		{
			Internal_create3(this, ref labelContent, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(LocString labelText, int labelWidth, string style = "")
		{
			Internal_create4(this, labelText, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(LocString labelText, string style = "")
		{
			Internal_create5(this, labelText, style);
		}

		/// <summary>Creates a new GUI editor field without a label.</summary>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUICurvesField(string style = "")
		{
			Internal_create6(this, style);
		}

		/// <summary>
		/// Returns the curve represented by the field. If the field represents a curve range this returns the minimal curve of 
		/// that range.
		/// </summary>
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public AnimationCurve Curve
		{
			get { return Internal_getCurve(mCachedPtr); }
		}

		/// <summary>
		/// Returns the minimal curve represented by the field containing a curve range. Returns the only available curve if the 
		/// field doesn&apos;t represent a range.
		/// </summary>
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public AnimationCurve MinCurve
		{
			get { return Internal_getMinCurve(mCachedPtr); }
		}

		/// <summary>
		/// Returns the maximal curve represented by the field containing a curve range. Returns the only available curve if the 
		/// field doesn&apos;t represent a range.
		/// </summary>
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public AnimationCurve MaxCurve
		{
			get { return Internal_getMaxCurve(mCachedPtr); }
		}

		/// <summary>Sets the size of padding to apply to the left and right sides of the curve drawing, in pixels.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int Padding
		{
			set { Internal_setPadding(mCachedPtr, value); }
		}

		/// <summary>Triggered when the user clicks on the GUI element.</summary>
		partial void Callback_OnClicked();

		/// <summary>Sets an animation curve to display on the field.</summary>
		public void SetCurve(AnimationCurve curve)
		{
			Internal_setCurve(mCachedPtr, curve);
		}

		/// <summary>Sets a set of animation curves and displays the difference (range) between them.</summary>
		public void SetCurveRange(AnimationCurve curveA, AnimationCurve curveB)
		{
			Internal_setCurveRange(mCachedPtr, curveA, curveB);
		}

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

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setCurve(IntPtr thisPtr, AnimationCurve curve);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setCurveRange(IntPtr thisPtr, AnimationCurve curveA, AnimationCurve curveB);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern AnimationCurve Internal_getCurve(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern AnimationCurve Internal_getMinCurve(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern AnimationCurve Internal_getMaxCurve(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setRange(IntPtr thisPtr, float xRange, float yRange);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setOffset(IntPtr thisPtr, ref Vector2 offset);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_centerAndZoom(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setPadding(IntPtr thisPtr, int padding);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(GUICurvesField managedInstance, CurveDrawOptions drawOptions, ref GUIContent labelContent, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create0(GUICurvesField managedInstance, CurveDrawOptions drawOptions, LocString labelText, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create1(GUICurvesField managedInstance, CurveDrawOptions drawOptions, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create2(GUICurvesField managedInstance, ref GUIContent labelContent, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create3(GUICurvesField managedInstance, ref GUIContent labelContent, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create4(GUICurvesField managedInstance, LocString labelText, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create5(GUICurvesField managedInstance, LocString labelText, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create6(GUICurvesField managedInstance, string style);
		private void Internal_onClicked()
		{
			Callback_OnClicked();
		}
	}

	/** @} */
}
