//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/// <summary>Types of icons that may be displayed in the scene window.</summary>
	public enum SceneWindowIcon
	{
		MoveSnap = 8,
		View = 0,
		Move = 1,
		Rotate = 2,
		Scale = 3,
		Pivot = 4,
		Center = 5,
		Local = 6,
		World = 7,
		RotateSnap = 9,
		SceneCameraOptions = 10
	}
}
