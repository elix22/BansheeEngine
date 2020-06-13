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

	/// <summary>
	/// Serializes the current state of a scene object and allows that state to be restored. The advantage of using this  
	/// class versus normal serialization is that the deserialization happens into the original scene object, instead of 
	/// creating a new scene object.
	/// </summary>
	[ShowInInspector]
	public partial class SerializedSceneObject : ScriptObject
	{
		private SerializedSceneObject(bool __dummy0) { }
		protected SerializedSceneObject() { }

		/// <summary>Serializes the current state of the provided scene object.</summary>
		/// <param name="sceneObject">Object whose state to serialize.</param>
		/// <param name="hierarchy">
		/// If true all children of the provided scene object will be serialized as well, otherwise just the provided object will.
		/// </param>
		public SerializedSceneObject(SceneObject sceneObject, bool hierarchy = false)
		{
			Internal_SerializedSceneObject(this, sceneObject, hierarchy);
		}

		/// <summary>
		/// Restores the scene object to the state as it was when this object was created. If the scene object was deleted since 
		/// it will be resurrected.
		/// </summary>
		public void Restore()
		{
			Internal_restore(mCachedPtr);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SerializedSceneObject(SerializedSceneObject managedInstance, SceneObject sceneObject, bool hierarchy);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_restore(IntPtr thisPtr);
	}

	/** @} */
}
