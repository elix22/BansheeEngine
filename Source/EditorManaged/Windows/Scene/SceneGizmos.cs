//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Runtime.CompilerServices;
using bs;

namespace bs.Editor
{
    /** @addtogroup Scene-Editor
     *  @{
     */

    /// <summary>
    /// Handles rendering of the scene gizmos for the specified camera. 
    /// </summary>
    internal sealed class SceneGizmos : ScriptObject
    {
        /// <summary>
        /// Settings that control how are gizmos drawn.
        /// </summary>
        internal GizmoDrawSettings DrawSettings
        {
            get
            {
                GizmoDrawSettings value;
                Internal_GetDrawSettings(mCachedPtr, out value);
                return value;
            }

            set
            {
                Internal_SetDrawSettings(mCachedPtr, ref value);
            }
        }

        /// <summary>
        /// Creates a new scene gizmo renderer.
        /// </summary>
        /// <param name="sceneCamera">Camera into which the gizmos will be rendered.</param>
        /// <param name="drawSettings">Settings that control how are gizmos drawn.</param>
        internal SceneGizmos(Camera sceneCamera, GizmoDrawSettings drawSettings)
        {
            Internal_Create(this, sceneCamera.GetCachedPtr(), ref drawSettings);
        }

        /// <summary>
        /// Queues gizmo drawing for this frame.
        /// </summary>
        internal void Draw()
        {
            Internal_Draw(mCachedPtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_Create(SceneGizmos managedInstance, IntPtr camera, 
            ref GizmoDrawSettings drawSettings);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_Draw(IntPtr thisPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetDrawSettings(IntPtr thisPtr, ref GizmoDrawSettings settings);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetDrawSettings(IntPtr thisPtr, out GizmoDrawSettings settings);
    }

    /** @} */
}
