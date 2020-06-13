//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup Utility
	 *  @{
	 */

	/// <summary>Determines the type of the source image for generating cubemaps.</summary>
	public enum CubemapSourceType
	{
		/// <summary>Source is a single image that will be replicated on all cubemap faces.</summary>
		Single = 0,
		/// <summary>
		/// Source is a list of 6 images, either sequentially next to each other or in a cross format. The system will 
		/// automatically guess the layout and orientation based on the aspect ratio.
		/// </summary>
		Faces = 1,
		/// <summary>Source is a single spherical panoramic image.</summary>
		Spherical = 2,
		/// <summary>Source is a single cylindrical panoramic image.</summary>
		Cylindrical = 3
	}

	/** @} */
}
