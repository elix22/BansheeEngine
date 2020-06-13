//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/// <summary>Information about a single element in GUISceneTreeView.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct SceneTreeViewElement
	{
		public SceneObject sceneObject;
		public bool isExpanded;
	}
}
