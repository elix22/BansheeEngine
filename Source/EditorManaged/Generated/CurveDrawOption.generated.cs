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

	/// <summary>Controls which elements should a GUICurves object draw.</summary>
	public enum CurveDrawOptions
	{
		/// <summary>Draws markers at specific time intervals.</summary>
		DrawMarkers = 1,
		/// <summary>Draws elements representing individual keyframes.</summary>
		DrawKeyframes = 2,
		/// <summary>Draws curves and the area between them. Only relevant if only two curves are provided for drawing.</summary>
		DrawRange = 4
	}

	/** @} */
}
