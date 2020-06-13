//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Gizmos
     *  @{
     */

    /// <summary>
    /// Handles drawing of gizmos for <see cref="Decal"/> component.
    /// </summary>
    internal class DecalGizmos
    {
        /// <summary>
        /// Draws decal shape gizmo when a decal is selected.
        /// </summary>
        /// <param name="decal">Decal to draw the gizmos for.</param>
        [DrawGizmo(DrawGizmoFlags.Selected)]
        private static void Draw(Decal decal)
        {
            Gizmos.Color = Color.Yellow;
            Gizmos.Transform = decal.SceneObject.WorldTransform;
            Gizmos.DrawWireCube(
                new Vector3(0.0f, 0.0f, -decal.MaxDistance * 0.5f), 
                new Vector3(decal.Size.x, decal.Size.y, decal.MaxDistance) * 0.5f);
        }

        /// <summary>
        /// Draws decal icon in scene view.
        /// </summary>
        /// <param name="decal">Decal to draw the icon for.</param>
        [DrawGizmo(DrawGizmoFlags.NotSelected | DrawGizmoFlags.Pickable)]
        private static void DrawIcon(Decal decal)
        {
            Gizmos.DrawIcon(decal.SceneObject.Position,
                EditorBuiltin.GetSceneViewIcon(SceneViewIcon.Decal), false);
        }
    }

    /** @} */
}
