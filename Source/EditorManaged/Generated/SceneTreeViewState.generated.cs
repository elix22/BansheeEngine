//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/// <summary>Information about all elements displayed in a GUISceneTreeView.</summary>
	[ShowInInspector]
	public partial class SceneTreeViewState : ScriptObject
	{
		private SceneTreeViewState(bool __dummy0) { }
		protected SceneTreeViewState() { }

		[ShowInInspector]
		[NativeWrapper]
		public SceneTreeViewElement[] Elements
		{
			get { return Internal_getelements(mCachedPtr); }
			set { Internal_setelements(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SceneTreeViewElement[] Internal_getelements(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setelements(IntPtr thisPtr, SceneTreeViewElement[] value);
	}
}
