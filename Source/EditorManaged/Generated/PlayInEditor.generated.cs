//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup Editor-General
	 *  @{
	 */

	/// <summary>Handles functionality specific to running the game in editor.</summary>
	[ShowInInspector]
	public partial class PlayInEditor : ScriptObject
	{
		private PlayInEditor(bool __dummy0) { }
		protected PlayInEditor() { }

		/// <summary>Triggered right after the play mode is entered.</summary>
		public static event Action OnPlay;

		/// <summary>Triggered right after the play mode is exited.</summary>
		static partial void Callback_OnStopped();

		/// <summary>Triggered right after the user pauses play mode.</summary>
		public static event Action OnPaused;

		/// <summary>Triggered right after the user unpauses play mode.</summary>
		public static event Action OnUnpaused;

		/// <summary>Runs the game for a single frame and then pauses it.</summary>
		public static void FrameStep()
		{
			Internal_frameStep();
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern PlayInEditorState Internal_getState();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setState(PlayInEditorState state);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_frameStep();
		private static void Internal_onPlay()
		{
			OnPlay?.Invoke();
		}
		private static void Internal_onStopped()
		{
			Callback_OnStopped();
		}
		private static void Internal_onPaused()
		{
			OnPaused?.Invoke();
		}
		private static void Internal_onUnpaused()
		{
			OnUnpaused?.Invoke();
		}
	}

	/** @} */
}
