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
	/// Label is optional. This specific implementation displays an input field for a color distribution.
	/// </summary>
	[ShowInInspector]
	public partial class GUIColorDistributionField : GUIElement
	{
		private GUIColorDistributionField(bool __dummy0) { }
		protected GUIColorDistributionField() { }

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorDistributionField(GUIContent labelContent, int labelWidth, string style = "")
		{
			Internal_create(this, ref labelContent, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorDistributionField(GUIContent labelContent, string style = "")
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
		public GUIColorDistributionField(LocString labelText, int labelWidth, string style = "")
		{
			Internal_create1(this, labelText, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorDistributionField(LocString labelText, string style = "")
		{
			Internal_create2(this, labelText, style);
		}

		/// <summary>Creates a new GUI editor field without a label.</summary>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorDistributionField(string style = "")
		{
			Internal_create3(this, style);
		}

		/// <summary>Changes the value of the field.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ColorDistribution Value
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

		/// <summary>Triggered when the user clicks on the minimum value display for the distribution.</summary>
		partial void Callback_OnMinClicked();

		/// <summary>Triggered when the user clicks on the maximum value display for the distribution.</summary>
		partial void Callback_OnMaxClicked();

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ColorDistribution Internal_getValue(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setValue(IntPtr thisPtr, ColorDistribution value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern PropertyDistributionType Internal_getType(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(GUIColorDistributionField managedInstance, ref GUIContent labelContent, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create0(GUIColorDistributionField managedInstance, ref GUIContent labelContent, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create1(GUIColorDistributionField managedInstance, LocString labelText, int labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create2(GUIColorDistributionField managedInstance, LocString labelText, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create3(GUIColorDistributionField managedInstance, string style);
		private void Internal_onMinClicked()
		{
			Callback_OnMinClicked();
		}
		private void Internal_onMaxClicked()
		{
			Callback_OnMaxClicked();
		}
	}

	/** @} */
}
