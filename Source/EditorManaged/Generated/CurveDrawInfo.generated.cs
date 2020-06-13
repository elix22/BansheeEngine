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

	/// <summary>Curve and a color to draw it in.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct CurveDrawInfo
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static CurveDrawInfo Default()
		{
			CurveDrawInfo value = new CurveDrawInfo();
			value.curve = null;
			value.color = Color.Default();

			return value;
		}

		public CurveDrawInfo(AnimationCurve curve, Color color)
		{
			this.curve = curve;
			this.color = color;
		}

		public AnimationCurve curve;
		public Color color;
	}

	/** @} */
}
