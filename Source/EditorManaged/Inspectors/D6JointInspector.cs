//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="D6Joint"/> component.
    /// </summary>
    [CustomInspector(typeof(D6Joint))]
    internal class D6JointInspector : JointInspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            D6Joint joint = (D6Joint) InspectedObject;
            BuildGUI(joint, true);

            drawer.AddDefault(joint, typeof(D6Joint));

            drawer.BeginCategory("Motion constraints");
            for (int i = 0; i < (int)D6JointAxis.Count; i++)
            {
                D6JointAxis axis = (D6JointAxis)i;
                string entryName = Enum.GetName(typeof(D6JointAxis), axis);

                drawer.AddField(entryName, () => joint.GetMotion(axis), x => joint.SetMotion(axis, x));
            }

            drawer.BeginCategory("Drive");
            drawer.AddField("Drive position", () => joint.DrivePosition, x => joint.SetDriveTransform(x, joint.DriveRotation));
            drawer.AddField("Drive rotation", () => joint.DriveRotation.ToEuler(), x => joint.SetDriveTransform(joint.DrivePosition, Quaternion.FromEuler(x)));
            drawer.AddField("Drive linear velocity", () => joint.DriveLinearVelocity, x => joint.SetDriveVelocity(x, joint.DriveAngularVelocity));
            drawer.AddField("Drive angular velocity", () => joint.DriveAngularVelocity, x => joint.SetDriveVelocity(joint.DriveLinearVelocity, x));

            for (int i = 0; i < (int)D6JointDriveType.Count; i++)
            {
                D6JointDriveType type = (D6JointDriveType)i;
                string entryName = Enum.GetName(typeof(D6JointDriveType), type);

                drawer.AddField(entryName, () => joint.GetDrive(type), x => joint.SetDrive(type, x));
            }

            drawer.EndCategory();
        }
    }

    /** @} */
}