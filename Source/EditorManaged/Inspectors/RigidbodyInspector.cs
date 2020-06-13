//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="Rigidbody"/> component.
    /// </summary>
    [CustomInspector(typeof(Rigidbody))]
    internal class RigidbodyInspector : Inspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            Rigidbody rigidbody = (Rigidbody)InspectedObject;
            drawer.AddDefault(rigidbody);

            drawer.AddField("Automatic mass",
                () => rigidbody.Flags.HasFlag(RigidbodyFlag.AutoMass),
                x =>
                {
                    if (x)
                        rigidbody.Flags |= RigidbodyFlag.AutoMass;
                    else
                        rigidbody.Flags &= ~RigidbodyFlag.AutoMass;
                });
                

            drawer.AddField("Continous", 
                () => rigidbody.Flags.HasFlag(RigidbodyFlag.CCD),
                x =>
                {
                    if (x)
                        rigidbody.Flags |= RigidbodyFlag.CCD;
                    else
                        rigidbody.Flags &= ~RigidbodyFlag.CCD;
                });
        }
    }

    /** @} */
}