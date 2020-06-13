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
	/// Label is optional. This specific implementation displays an input field for a 2D vector distribution.
	/// </summary>
	[ShowInInspector]
	public partial class GUIVector2DistributionField : GUIElement
	{
		private GUIVector2DistributionField(bool __dummy0) { }
		protected GUIVector2DistributionField() { }

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(GUIContent labelContent, int labelWidth, string style = "")
		{
			Internal_create(this, ref labelContent, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(GUIContent labelContent, string style = "")
		{
			Internal_create0(this, ref labelContent, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(LocString labelText, int labelWidth, string style = "")
		{
			Internal_create1(this, labelText, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(LocString labelText, string style = "")
		{
			Internal_create2(this, labelText, style);
		}

		/// <summary>Creates a new GUI editor field without a label.</summary>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(string style = "")
		{
			Internal_create3(this, style);
		}

		/// <summary>Changes the value of the field.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public Vector2Distribution Value
		{
			get { return Internal_getValue(mCachedPtr); }
			set { Internal_setValue(mCachedPtr, value); }
		}

		/// <summary>Returns the type of the currently selected distribution.</summary>
		[NativeWrapper]
		public PropertyDistributionType DistributionType
		{
			get { return Internal_getType(mCachedPtr); }
		}

		/// <summary>
		/// Checks if any of the float input fields currently have input focus. Only relevant for non-curve distributions.
		/// </summary>
		[NativeWrapper]
		public bool HasInputFocus
		{
			get { return Internal_hasInputFocus(mCachedPtr); }
		}

		/// <summary>
		/// Triggered when the user clicks on the curve display. Only relevant if the distribution is a curve distribution.  
		/// Provides the index of the clicked curve.
		/// </summary>
		partial void Callback_OnClicked(VectorComponent p0);

		/// <summary>
		/// Triggered when the user modifies the value of the non-curve (constant) values of the distribution. Only relevant  if 
		/// the distribution is not a curve distribution.
		/// </summary>
		public event Action<RangeComponent, VectorComponent> OnConstantModified;

		/// <summary>
		/// Triggered when the user confirms inputs in the non-curve (constant) values of the distribution. Only relevant  if the 
		/// distribution is not a curve distribution.
		/// </summary>
		public event Action<RangeComponent, VectorComponent> OnConstantConfirmed;

		/// <summary>
		/// Triggered when a GUI field representing an individual component loses or gains focus. This only applies to input 
		/// fields representing the non-curve (constant) distribution types.
		/// </summary>
		public event Action<bool, RangeComponent, VectorComponent> OnConstantFocusChanged;

		/// <summary>Sets input focus to a specific component&apos;s input box.</summary>
		/// <param name="rangeComponent">
		/// Whether to focus on the minimum or the maximum part of the range. Only relevant if the distribution represents a 
		/// constant range.
		/// </param>
		/// <param name="vectorComponent">
		/// Vector component to focus on. Only relevant of the distribution constant is a vector type, and if the current 
		/// distribution type is a non-curve (constant) type.
		/// </param>
		/// <param name="focus">True to enable focus, false to disable.</param>
		public void SetInputFocus(RangeComponent rangeComponent, VectorComponent vectorComponent, bool focus)
		{
			Internal_setInputFocus(mCachedPtr, rangeComponent, vectorComponent, focus);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Vector2Distribution Internal_getValue(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setValue(IntPtr thisPtr, Vector2Distribution value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern PropertyDistributionType Internal_getType(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_hasInputFocus(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setInputFocus(IntPtr thisPtr, RangeComponent rangeComponent, VectorComponent vectorComponent, bool focus);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(GUIVector2DistributionField managedInstance, ref GUIContent labelContent, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create0(GUIVector2DistributionField managedInstance, ref GUIContent labelContent, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create1(GUIVector2DistributionField managedInstance, LocString labelText, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create2(GUIVector2DistributionField managedInstance, LocString labelText, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create3(GUIVector2DistributionField managedInstance, string style);
		private void Internal_onClicked(VectorComponent p0)
		{
			Callback_OnClicked(p0);
		}
		private void Internal_onConstantModified(RangeComponent p0, VectorComponent p1)
		{
			OnConstantModified?.Invoke(p0, p1);
		}
		private void Internal_onConstantConfirmed(RangeComponent p0, VectorComponent p1)
		{
			OnConstantConfirmed?.Invoke(p0, p1);
		}
		private void Internal_onConstantFocusChanged(bool p0, RangeComponent p1, VectorComponent p2)
		{
			OnConstantFocusChanged?.Invoke(p0, p1, p2);
		}
	}

	/** @} */
}
