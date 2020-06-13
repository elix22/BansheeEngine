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

	/// <summary>A set of animation events that will be added to an animation clip during animation import.</summary>
	[ShowInInspector]
	public partial class ImportedAnimationEvents : ScriptObject
	{
		private ImportedAnimationEvents(bool __dummy0) { }

		public ImportedAnimationEvents()
		{
			Internal_ImportedAnimationEvents(this);
		}

		[ShowInInspector]
		[NativeWrapper]
		public string Name
		{
			get { return Internal_getname(mCachedPtr); }
			set { Internal_setname(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public AnimationEvent[] Events
		{
			get { return Internal_getevents(mCachedPtr); }
			set { Internal_setevents(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ImportedAnimationEvents(ImportedAnimationEvents managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string Internal_getname(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setname(IntPtr thisPtr, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern AnimationEvent[] Internal_getevents(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setevents(IntPtr thisPtr, AnimationEvent[] value);
	}

	/** @} */
}
