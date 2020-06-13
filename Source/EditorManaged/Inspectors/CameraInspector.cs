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
    /// Renders an inspector for the <see cref="Camera"/> component.
    /// </summary>
    [CustomInspector(typeof(Camera))]
    internal class CameraInspector : Inspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            Camera camera = (Camera)InspectedObject;

            drawer.AddDefault(camera);
            drawer.BeginCategory("Viewport");
            drawer.AddField("Area", () => camera.Viewport.Area, x => camera.Viewport.Area = x);
            drawer.AddField("Clear flags", () => camera.Viewport.ClearFlags, x => camera.Viewport.ClearFlags = x);
            drawer.AddField("Clear color", () => camera.Viewport.ClearColor, x => camera.Viewport.ClearColor = x);
            drawer.AddField("Clear stencil", () => camera.Viewport.ClearStencil, x => camera.Viewport.ClearStencil = x);
            drawer.AddField("Clear depth", () => camera.Viewport.ClearDepth, x => camera.Viewport.ClearDepth = x);

            drawer.AddConditional("FieldOfView", () => camera.ProjectionType == ProjectionType.Perspective);
            drawer.AddConditional("OrthoHeight", () => camera.ProjectionType == ProjectionType.Orthographic);

            drawer.EndCategory();
        }
    }

    /** @} */
}
