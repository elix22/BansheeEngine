//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="AnimationClip"/> resource.
    /// </summary>
    [CustomInspector(typeof(AnimationClip))]
    internal class AnimationClipInspector : Inspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            // No GUI for physics mesh resource
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            return InspectableState.NotModified;
        }
    }

    /** @} */
}
