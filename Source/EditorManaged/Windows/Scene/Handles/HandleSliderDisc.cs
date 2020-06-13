﻿//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Runtime.CompilerServices;
using bs;

namespace bs.Editor
{
    /** @addtogroup Handles
     *  @{
     */

    /// <summary>
    /// Handle slider that returns a delta value as you drag the pointer along a disc. For intersection purposes the disc 
    /// is internally represented by a torus.
    /// </summary>
    public sealed class HandleSliderDisc : HandleSlider
    {
        /// <summary>
        /// Creates a new disc handle slider.
        /// </summary>
        /// <param name="parentHandle">Handle that the slider belongs to.</param>
        /// <param name="normal">Normal that determines the orientation of the disc.</param>
        /// <param name="radius">Radius of the disc.</param>
        /// <param name="fixedScale">If true the handle slider will always try to maintain the same visible area in the 
        ///                          viewport regardless of distance from camera.</param>
        /// <param name="layer">Layer that allows filtering of which sliders are interacted with from a specific camera.</param>
        public HandleSliderDisc(Handle parentHandle, Vector3 normal, float radius, bool fixedScale = true, UInt64 layer = 1)
            :base(parentHandle)
        {
            Internal_CreateInstance(this, ref normal, radius, fixedScale, layer);
        }

        /// <summary>
        /// Returns a delta value that is the result of dragging/sliding the pointer along the disc. This changes every 
        /// frame and will be zero unless the slider is active.
        /// </summary>
        public Degree Delta
        {
            get
            {
                float value;
                Internal_GetDelta(mCachedPtr, out value);
                return (Degree)value;
            }
        }

        /// <summary>
        /// Gets the initial angle at which the drag/slide operation started. This is only valid when the slider is active.
        /// </summary>
        public Degree StartAngle
        {
            get
            {
                float value;
                Internal_GetStartAngle(mCachedPtr, out value);
                return (Degree)value;
            }
        }

        /// <summary>
        /// Enables or disables a cut-off plane that can allow the disc to be intersected with only in a 180 degree arc.
        /// </summary>
        /// <param name="angle">Angle at which to start the cut-off. Points on the dist at the specified angle and the next
        ///                     180 degrees won't be interactable.</param>
        /// <param name="enabled">Should the cutoff plane be enabled or disabled.</param>
        public void SetCutoffPlane(Degree angle, bool enabled)
        {
            Internal_SetCutoffPlane(mCachedPtr, angle.Degrees, enabled);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_CreateInstance(HandleSliderDisc instance, ref Vector3 normal, float radius, 
            bool fixedScale, UInt64 layer);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetDelta(IntPtr nativeInstance, out float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetStartAngle(IntPtr nativeInstance, out float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetCutoffPlane(IntPtr nativeInstance, float angle, bool enabled);
    }

    /** @} */
}
