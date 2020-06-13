//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="SphericalJoint"/> component.
    /// </summary>
    [CustomInspector(typeof(SphericalJoint))]
    internal class SphericalJointInspector : JointInspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            SphericalJoint joint = (SphericalJoint) InspectedObject;
            BuildGUI(joint, true);

            drawer.AddDefault(joint, typeof(SphericalJoint));
            drawer.AddField("Enable limit", 
                () => joint.HasFlag(SphericalJointFlag.Limit), 
                x => joint.SetFlag(SphericalJointFlag.Limit, x));
            drawer.AddConditional("Limit", () => joint.HasFlag(SphericalJointFlag.Limit));
        }
    }

    /** @} */
}