//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/// <summary>Settings that control gizmo drawing.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct GizmoDrawSettings
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static GizmoDrawSettings Default()
		{
			GizmoDrawSettings value = new GizmoDrawSettings();
			value.iconScale = 1f;
			value.iconRange = 500f;
			value.iconSizeMin = 0.0500000007f;
			value.iconSizeMax = 0.150000006f;
			value.iconSizeCull = 0.25f;

			return value;
		}

		/// <summary>Scale to apply to gizmo icons, controlling their size.</summary>
		public float iconScale;
		/// <summary>Maximum range at which gizmo icons will be rendered, in world units.</summary>
		public float iconRange;
		/// <summary>
		/// Icons smaller than this size will be faded out. The value represents the size of the icon relative to viewport  size 
		/// (e.g. 1 means the icons fully covers the viewport). In range [0, 1], should be smaller than maximum size value.
		/// </summary>
		public float iconSizeMin;
		/// <summary>
		/// Icons larger than this size will be faded out. The value represents the size of the icon relative to viewport  size 
		/// (e.g. 1 means the icons fully covers the viewport). In range [0, 1], should be larger than minimum size value.
		/// </summary>
		public float iconSizeMax;
		/// <summary>
		/// Icons larger than this size will not be shown. The value represents the size of the icon relative to viewport  size 
		/// (e.g. 1 means the icons fully covers the viewport). In range [0, 1], should be larger than maximum size value.
		/// </summary>
		public float iconSizeCull;
	}
}
