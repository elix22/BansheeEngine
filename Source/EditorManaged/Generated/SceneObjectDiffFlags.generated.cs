//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup Utility-Editor
	 *  @{
	 */

	/// <summary>Flags that mark which portion of a scene-object is modified.</summary>
	public enum SceneObjectDiffFlags
	{
		Name = 1,
		Position = 2,
		Rotation = 4,
		Scale = 8,
		Active = 16
	}

	/** @} */
}
