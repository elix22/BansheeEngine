//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup Importer
	 *  @{
	 */

	/// <summary>Supported types of low-level shading languages.</summary>
	public enum ShadingLanguageFlags
	{
		/// <summary>High level shading language used by DirectX backend.</summary>
		HLSL = 1,
		/// <summary>OpenGL shading language.</summary>
		GLSL = 2,
		/// <summary>Variant of GLSL used for Vulkan.</summary>
		VKSL = 4,
		/// <summary>Metal shading language.</summary>
		MSL = 8,
		/// <summary>Helper entry that includes all languages.</summary>
		All = 15
	}

	/** @} */
}
