//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="DistanceJoint"/> component.
    /// </summary>
    [CustomInspector(typeof(DistanceJoint))]
    internal class DistanceJointInspector : JointInspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            DistanceJoint joint = (DistanceJoint) InspectedObject;
            BuildGUI(joint, true);

            drawer.AddDefault(joint, typeof(DistanceJoint));

            drawer.AddField("Enable minimum limit",
                () => joint.HasFlag(DistanceJointFlag.MinDistance),
                x => joint.SetFlag(DistanceJointFlag.MinDistance, x));
            drawer.AddConditional("MinDistance", () => joint.HasFlag(DistanceJointFlag.MinDistance));

            drawer.AddField("Enable maximum limit",
                () => joint.HasFlag(DistanceJointFlag.MaxDistance),
                x => joint.SetFlag(DistanceJointFlag.MaxDistance, x));
            drawer.AddConditional("MaxDistance", () => joint.HasFlag(DistanceJointFlag.MaxDistance));

            drawer.AddField("Enable spring",
                () => joint.HasFlag(DistanceJointFlag.Spring),
                x => joint.SetFlag(DistanceJointFlag.Spring, x));
            drawer.AddConditional("Spring", () => joint.HasFlag(DistanceJointFlag.Spring));
        }
    }

    /** @} */
}