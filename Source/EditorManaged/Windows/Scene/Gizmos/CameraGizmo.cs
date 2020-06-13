//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2017 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Gizmos
     *  @{
     */

    /// <summary>
    /// Handles drawing of gizmos for the <see cref="Camera"/> component.
    /// </summary>
    internal class CameraGizmo
    {
        /// <summary>
        /// Draws camera shape gizmos when a camera is selected.
        /// </summary>
        /// <param name="camera">Camera to draw the gizmos for.</param>
        [DrawGizmo(DrawGizmoFlags.Selected)]
        private static void Draw(Camera camera)
        {
            SceneObject so = camera.SceneObject;

            Gizmos.Color = Color.Yellow;
            Gizmos.Transform = Matrix4.TRS(so.Position, Quaternion.LookRotation(so.Rotation.Forward, so.Rotation.Up), Vector3.One);

            Gizmos.DrawFrustum(Vector3.Zero, camera.AspectRatio, camera.FieldOfView, camera.NearClipPlane,
                camera.FarClipPlane);

        }

        /// <summary>
        /// Draws camera icon in scene view.
        /// </summary>
        /// <param name="camera">Camera to draw the icon for.</param>
        [DrawGizmo(DrawGizmoFlags.NotSelected | DrawGizmoFlags.Pickable)]
        private static void DrawIcon(Camera camera)
        {
            Gizmos.DrawIcon(camera.SceneObject.Position, EditorBuiltin.GetSceneViewIcon(SceneViewIcon.Camera), false);
        }
    }

    /** @} */
}
