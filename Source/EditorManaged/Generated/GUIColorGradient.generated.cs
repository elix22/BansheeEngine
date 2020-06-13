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

	/// <summary>GUI element that displays a color gradient.</summary>
	[ShowInInspector]
	public partial class GUIColorGradient : GUIElement
	{
		private GUIColorGradient(bool __dummy0) { }
		protected GUIColorGradient() { }

		/// <summary>Creates a new GUI color gradient element.</summary>
		/// <param name="styleName">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIColorGradient(string styleName = "")
		{
			Internal_create(this, styleName);
		}

		/// <summary>Color gradient to display.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ColorGradient Gradient
		{
			get { return Internal_getGradient(mCachedPtr); }
			set { Internal_setGradient(mCachedPtr, value); }
		}

		/// <summary>Triggered when the user clicks on the GUI element.</summary>
		partial void Callback_OnClicked();

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setGradient(IntPtr thisPtr, ColorGradient colorGradient);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ColorGradient Internal_getGradient(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(GUIColorGradient managedInstance, string styleName);
		private void Internal_onClicked()
		{
			Callback_OnClicked();
		}
	}

	/** @} */
}
