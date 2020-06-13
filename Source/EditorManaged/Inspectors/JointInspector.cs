//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders common inspector elements for all <see cref="Joint"/> components.
    /// </summary>
    internal abstract class JointInspector : Inspector
    {
        /// <summary>
        /// Creates GUI elements for fields common to all joints.
        /// </summary>
        protected virtual void BuildGUI(Joint joint, bool showOffsets)
        {
            drawer.AddField("Target", () => joint.GetBody(JointBody.Target), x => joint.SetBody(JointBody.Target, x));
            drawer.AddField("Anchor", () => joint.GetBody(JointBody.Anchor), x => joint.SetBody(JointBody.Anchor, x));

            if (showOffsets)
            {
                drawer.AddField("Target offset", 
                    () => joint.GetPosition(JointBody.Target), 
                    x => joint.SetTransform(JointBody.Target, x, joint.GetRotation(JointBody.Target)));
                drawer.AddField("Anchor offset", 
                    () => joint.GetPosition(JointBody.Anchor), 
                    x => joint.SetTransform(JointBody.Anchor, x, joint.GetRotation(JointBody.Anchor)));
            }

            drawer.AddDefault(joint, typeof(Joint));
        }
    }

    /** @} */
}
