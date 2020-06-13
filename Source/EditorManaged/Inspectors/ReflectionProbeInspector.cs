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
    /// Renders an inspector for the <see cref="ReflectionProbe"/> component.
    /// </summary>
    [CustomInspector(typeof(ReflectionProbe))]
    internal class ReflectionProbeInspector : Inspector
    {
        private GUIButton captureButton = new GUIButton(new LocEdString("Capture"));

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            ReflectionProbe probe = (ReflectionProbe)InspectedObject;

            drawer.AddDefault(probe);
            drawer.AddConditional("Radius", () => probe.Type == ReflectionProbeType.Sphere);
            drawer.AddConditional("Extents", () => probe.Type == ReflectionProbeType.Box);

            captureButton.OnClick += () => probe.Capture();

            Layout.AddSpace(10);
            Layout.AddElement(captureButton);
        }
    }

    /** @} */
}
