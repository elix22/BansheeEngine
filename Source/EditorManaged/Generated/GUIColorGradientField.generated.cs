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
	/// Label is optional. This specific implementation displays a color gradient input field.
	/// </summary>
	[ShowInInspector]
	public partial class GUIColorGradientField : GUIElement
	{
		private GUIColorGradientField(bool __dummy0) { }
		protected GUIColorGradientField() { }

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorGradientField(GUIContent labelContent, int labelWidth, string style = "")
		{
			Internal_create(this, ref labelContent, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorGradientField(GUIContent labelContent, string style = "")
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
		public GUIColorGradientField(LocString labelText, int labelWidth, string style = "")
		{
			Internal_create1(this, labelText, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorGradientField(LocString labelText, string style = "")
		{
			Internal_create2(this, labelText, style);
		}

		/// <summary>Creates a new GUI editor field without a label.</summary>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorGradientField(string style = "")
		{
			Internal_create3(this, style);
		}

		/// <summary>Changes the value of the field.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ColorGradient Value
		{
			get { return Internal_getValue(mCachedPtr); }
			set { Internal_setValue(mCachedPtr, value); }
		}

		/// <summary>Triggered when the user clicks on the gradient field.</summary>
		partial void Callback_OnClicked();

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ColorGradient Internal_getValue(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setValue(IntPtr thisPtr, ColorGradient value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(GUIColorGradientField managedInstance, ref GUIContent labelContent, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create0(GUIColorGradientField managedInstance, ref GUIContent labelContent, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create1(GUIColorGradientField managedInstance, LocString labelText, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create2(GUIColorGradientField managedInstance, LocString labelText, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create3(GUIColorGradientField managedInstance, string style);
		private void Internal_onClicked()
		{
			Callback_OnClicked();
		}
	}

	/** @} */
}
