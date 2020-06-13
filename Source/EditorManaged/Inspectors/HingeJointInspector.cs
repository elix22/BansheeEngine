//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="HingeJoint"/> component.
    /// </summary>
    [CustomInspector(typeof(HingeJoint))]
    internal class HingeJointInspector : JointInspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            HingeJoint joint = (HingeJoint)InspectedObject;
            BuildGUI(joint, true);

            drawer.AddDefault(joint, typeof(HingeJoint));

            drawer.AddField("Enable limit",
                () => joint.HasFlag(HingeJointFlag.Limit),
                x => joint.SetFlag(HingeJointFlag.Limit, x));
            drawer.AddConditional("Limit", () => joint.HasFlag(HingeJointFlag.Limit));

            drawer.AddField("Enable drive",
                () => joint.HasFlag(HingeJointFlag.Drive),
                x => joint.SetFlag(HingeJointFlag.Drive, x));
            drawer.AddConditional("Drive", () => joint.HasFlag(HingeJointFlag.Drive));
        }
    }

    /** @} */
}