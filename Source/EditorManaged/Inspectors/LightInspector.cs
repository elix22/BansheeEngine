//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="Light"/> component.
    /// </summary>
    [CustomInspector(typeof(Light))]
    internal class LightInspector : Inspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            Light light = (Light)InspectedObject;

            drawer.AddDefault(light);

            drawer.AddConditional("SpotAngle", () => light.Type == LightType.Spot);
            drawer.AddConditional("SpotAngleFalloff", () => light.Type == LightType.Spot);
            drawer.AddConditional("AutoAttenuation", () => light.Type != LightType.Directional);
            drawer.AddConditional("AttenuationRadius", () =>
            {
                if (light.Type == LightType.Directional)
                    return false;

                return !light.UseAutoAttenuation;
            });
            drawer.AddConditional("ShadowBias", () => light.CastsShadow);
        }
    }

    /** @} */
}
