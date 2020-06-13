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

	/// <summary>Controls what type of collision mesh should be imported during mesh import.</summary>
	public enum CollisionMeshType
	{
		/// <summary>No collision mesh will be imported.</summary>
		None = 0,
		/// <summary>Normal triangle mesh will be imported.</summary>
		Normal = 1,
		/// <summary>A convex hull will be generated from the source mesh.</summary>
		Convex = 2
	}

	/** @} */
}
