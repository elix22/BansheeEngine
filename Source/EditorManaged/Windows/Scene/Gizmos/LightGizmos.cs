//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Gizmos
     *  @{
     */

    /// <summary>
    /// Handles drawing of gizmos for the <see cref="Light"/> component.
    /// </summary>
    internal class LightGizmos
    {
        /// <summary>
        /// Draws light shape gizmos when a light is selected.
        /// </summary>
        /// <param name="light">Light to draw the gizmos for.</param>
        [DrawGizmo(DrawGizmoFlags.Selected)]
        private static void Draw(Light light)
        {
            Vector3 position = light.SceneObject.Position;

            Gizmos.Color = Color.Yellow;
            switch (light.Type)
            {
                case LightType.Directional:
                {
                    Vector3 right = light.SceneObject.Rotation.Rotate(Vector3.XAxis);
                    Vector3 up = light.SceneObject.Rotation.Rotate(Vector3.YAxis);
                    Vector3 forward = light.SceneObject.Forward;

                    Vector3 topLeft = position - right + up;
                    Vector3 topRight = position + right + up;
                    Vector3 botLeft = position - right - up;
                    Vector3 botRight = position + right - up;

                    Gizmos.DrawLine(topLeft, topRight);
                    Gizmos.DrawLine(topRight, botRight);
                    Gizmos.DrawLine(botRight, botLeft);
                    Gizmos.DrawLine(botLeft, topLeft);

                    Gizmos.DrawLine(topLeft, topLeft + forward*5.0f);
                    Gizmos.DrawLine(topRight, topRight + forward*5.0f);
                    Gizmos.DrawLine(botRight, botRight + forward*5.0f);
                    Gizmos.DrawLine(botLeft, botLeft + forward*5.0f);
                }
                    break;
                case LightType.Radial:
                    Gizmos.DrawWireSphere(position, light.AttenuationRadius);

                    if (light.SourceRadius > 0.0f)
                    {
                        Gizmos.Color = light.Color;
                        Gizmos.DrawSphere(position, light.SourceRadius);
                    }

                    break;
                case LightType.Spot:
                {
                    Vector3 right = light.SceneObject.Rotation.Rotate(Vector3.XAxis);
                    Vector3 up = light.SceneObject.Rotation.Rotate(Vector3.YAxis);
                    Vector3 forward = light.SceneObject.Forward;

                    float discRadius = light.AttenuationRadius * MathEx.Tan(light.SpotAngle * 0.5f);

                    Gizmos.DrawLine(position, position + forward * light.AttenuationRadius + up * discRadius);
                    Gizmos.DrawLine(position, position + forward * light.AttenuationRadius - up * discRadius);
                    Gizmos.DrawLine(position, position + forward * light.AttenuationRadius + right * discRadius);
                    Gizmos.DrawLine(position, position + forward * light.AttenuationRadius - right * discRadius);

                    float falloffDiscRadius = light.AttenuationRadius * MathEx.Tan(light.SpotAngleFalloff * 0.5f);

                    Gizmos.DrawWireDisc(position + forward * light.AttenuationRadius, forward, discRadius);
                    Gizmos.DrawWireDisc(position + forward * light.AttenuationRadius, forward, falloffDiscRadius);

                    if (light.SourceRadius > 0.0f)
                    {
                        Gizmos.Color = light.Color;
                        Gizmos.DrawDisc(position, forward, light.SourceRadius);
                    }
                }
                    break;
            }
        }

        /// <summary>
        /// Draws light icon in scene view.
        /// </summary>
        /// <param name="light">Light to draw the icon for.</param>
        [DrawGizmo(DrawGizmoFlags.NotSelected | DrawGizmoFlags.Pickable)]
        private static void DrawIcon(Light light)
        {
            Gizmos.DrawIcon(light.SceneObject.Position, EditorBuiltin.GetSceneViewIcon(SceneViewIcon.Light), false);
        }
    }

    /** @} */
}
