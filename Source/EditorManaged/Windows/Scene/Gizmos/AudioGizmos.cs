//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Gizmos
     *  @{
     */

    /// <summary>
    /// Handles drawing of gizmos for audio components.
    /// </summary>
    internal class AudioGizmos
    {
        /// <summary>
        /// Draws audio listener icon in scene view.
        /// </summary>
        /// <param name="listener">Audio listener to draw the icon for.</param>
        [DrawGizmo(DrawGizmoFlags.NotSelected | DrawGizmoFlags.Pickable)]
        private static void DrawIcon(AudioListener listener)
        {
            Gizmos.DrawIcon(listener.SceneObject.Position,
                EditorBuiltin.GetSceneViewIcon(SceneViewIcon.AudioListener), false);
        }

        /// <summary>
        /// Draws audio source icon in scene view.
        /// </summary>
        /// <param name="source">Audio source to draw the icon for.</param>
        [DrawGizmo(DrawGizmoFlags.NotSelected | DrawGizmoFlags.Pickable)]
        private static void DrawIcon(AudioSource source)
        {
            Gizmos.DrawIcon(source.SceneObject.Position,
                EditorBuiltin.GetSceneViewIcon(SceneViewIcon.AudioSource), false);
        }
    }

    /** @} */
}
