//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2017 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Gizmos
     *  @{
     */

    /// <summary>
    /// Handles drawing of gizmos and selection callback for the <see cref="ParticleSystem"/> component.
    /// </summary>
    internal class ParticleSystemGizmos
    {
        /// <summary>
        /// Draws two concentric arcs, depending on the <paramref name="thickness"/> parameter.
        /// </summary>
        /// <param name="center">Position of the arc center</param>
        /// <param name="radius">Radius of the outer arc.</param>
        /// <param name="arc">Length of the arc in range [0, 360] degrees.</param>
        /// <param name="thickness">Determines radius of the inner arc, in range [0, 1]. 0 specifies the inner arc is
        /// the same radius as the outer arc, while 1 specifies the inner arc has zero radius.</param>
        /// <param name="color">Color to draw the arc with.</param>
        private static void DrawArcWithThickness(Vector3 center, float radius, Radian arc, float thickness, Color color)
        {
            Gizmos.Color = color;
            if (arc.Degrees > 0.0f)
            {
                Gizmos.DrawWireArc(center, -Vector3.ZAxis, radius, new Degree(0.0f), arc);

                if (thickness > 0.0f)
                {
                    float innerRadius = radius * (1.0f - MathEx.Clamp01(thickness));
                    if (thickness < 1.0f)
                    {
                        Gizmos.Color = color * new Color(0.5f, 0.5f, 0.5f);
                        Gizmos.DrawWireArc(center, -Vector3.ZAxis, innerRadius, new Degree(0.0f), arc);
                    }

                    Gizmos.Color = color * new Color(0.25f, 0.25f, 0.25f);

                    for (Radian x = new Radian(0.0f); x < arc; x += MathEx.HalfPi)
                    {
                        Vector3 dir = new Vector3(MathEx.Cos(x), MathEx.Sin(x), 0.0f);
                        Gizmos.DrawLine(center + dir * innerRadius, center + dir * radius);
                    }

                    if (!MathEx.ApproxEquals(arc.Degrees % 90.0f, 0.0f))
                    {
                        Vector3 dir = new Vector3(MathEx.Cos(arc), MathEx.Sin(arc), 0.0f);
                        Gizmos.DrawLine(center + dir * innerRadius, center + dir * radius);
                    }
                }
            }
        }

        /// <summary>
        /// Draws a border between two arcs.
        /// </summary>
        /// <param name="topRadius">Radius of the top arc.</param>
        /// <param name="baseRadius">Radius of the base arc.</param>
        /// <param name="arc">Length of the arc in range [0, 360] degrees.</param>
        /// <param name="length">Distance between the two arcs.</param>
        private static void DrawConeBorder(float topRadius, float baseRadius, Radian arc, float length)
        {
            for (Radian x = new Radian(0.0f); x < arc; x += MathEx.HalfPi)
            {
                Vector3 dir = new Vector3(MathEx.Cos(x), MathEx.Sin(x), 0.0f);
                Vector3 a = dir * baseRadius + new Vector3(0.0f, 0.0f, -length);
                Vector3 b = dir * topRadius;

                Gizmos.DrawLine(a, b);
            }

            if (!MathEx.ApproxEquals(arc.Degrees % 90.0f, 0.0f))
            {
                Vector3 dir = new Vector3(MathEx.Cos(arc), MathEx.Sin(arc), 0.0f);
                Vector3 a = dir * baseRadius + new Vector3(0.0f, 0.0f, -length);
                Vector3 b = dir * topRadius;

                Gizmos.DrawLine(a, b);
            }
        }

        /// <summary>
        /// Draws the particle system shape when the particle system is selected.
        /// </summary>
        /// <param name="component">Component to draw gizmos for.</param>
        [DrawGizmo(DrawGizmoFlags.Selected)]
        private static void Draw(ParticleSystem component)
        {
            SceneObject so = component.SceneObject;

            // Draw collision planes, if enabled
            Gizmos.Color = Color.Yellow;
            ParticleEvolver[] evolvers = component.Evolvers;
            foreach (var entry in evolvers)
            {
                if (entry is ParticleCollisions collisions)
                {
                    if (collisions.Options.mode == ParticleCollisionMode.Plane)
                    {
                        void DrawPlane(Vector3 position, Vector3 right, Vector3 up, Vector3 forward)
                        {
                            Vector3 topLeft = position - right + up;
                            Vector3 topRight = position + right + up;
                            Vector3 botLeft = position - right - up;
                            Vector3 botRight = position + right - up;

                            Gizmos.DrawLine(topLeft, topRight);
                            Gizmos.DrawLine(topRight, botRight);
                            Gizmos.DrawLine(botRight, botLeft);
                            Gizmos.DrawLine(botLeft, topLeft);

                            Gizmos.DrawLine(position, position + forward * 0.4f);
                        }

                        SceneObject[] planeObjects = collisions.PlaneObjects;
                        foreach (var planeSO in planeObjects)
                        {
                            Vector3 position = planeSO.Position;
                            Vector3 right = planeSO.Rotation.Rotate(Vector3.XAxis);
                            Vector3 up = planeSO.Rotation.Rotate(Vector3.YAxis);
                            Vector3 forward = planeSO.Forward;

                            DrawPlane(position, right, up, forward);
                        }

                        Plane[] planes = collisions.Planes;
                        foreach (var plane in planes)
                        {
                            Vector3 right, up;
                            Vector3.OrthogonalComplement(plane.normal, out right, out up);

                            DrawPlane(plane.normal * plane.d, right, up, plane.normal);
                        }
                    }
                }
            }

            // Draw emitter shapes
            Gizmos.Transform = Matrix4.TRS(so.Position, Quaternion.LookRotation(so.Rotation.Forward, so.Rotation.Up), Vector3.One);
            Gizmos.Color = Color.Green;

            ParticleEmitter[] emitters = component.Emitters;
            foreach (var entry in emitters)
            {
                ParticleEmitterShape shape = entry?.Shape;
                if (shape == null)
                    continue;

                if (shape is ParticleEmitterBoxShape boxShape)
                    Gizmos.DrawWireCube(Vector3.Zero, boxShape.Options.extents);
                else if (shape is ParticleEmitterSphereShape sphereShape)
                {
                    ParticleSphereShapeOptions options = sphereShape.Options;
                    Gizmos.DrawWireSphere(Vector3.Zero, options.radius);

                    if (options.thickness > 0.0f)
                    {
                        float innerRadius = options.radius * (1.0f - MathEx.Clamp01(options.thickness));
                        if (options.thickness < 1.0f)
                        {
                            Gizmos.Color = Color.Green * new Color(0.5f, 0.5f, 0.5f);
                            Gizmos.DrawWireSphere(Vector3.Zero, innerRadius);
                        }

                        Gizmos.Color = Color.Green * new Color(0.25f, 0.25f, 0.25f);
                        Gizmos.DrawLine(Vector3.XAxis * innerRadius, Vector3.XAxis * options.radius);
                        Gizmos.DrawLine(-Vector3.XAxis * innerRadius, -Vector3.XAxis * options.radius);
                        Gizmos.DrawLine(Vector3.YAxis * innerRadius, Vector3.YAxis * options.radius);
                        Gizmos.DrawLine(-Vector3.YAxis * innerRadius, -Vector3.YAxis * options.radius);
                        Gizmos.DrawLine(Vector3.ZAxis * innerRadius, Vector3.ZAxis * options.radius);
                        Gizmos.DrawLine(-Vector3.ZAxis * innerRadius, -Vector3.ZAxis * options.radius);

                    }
                }
                else if (shape is ParticleEmitterHemisphereShape hemisphereShape)
                {
                    ParticleHemisphereShapeOptions options = hemisphereShape.Options;
                    Gizmos.DrawWireHemisphere(Vector3.Zero, options.radius);
                    DrawArcWithThickness(Vector3.Zero, options.radius, new Degree(360.0f), options.thickness, Color.Green);

                    if (options.thickness > 0.0f)
                    {
                        float innerRadius = options.radius * (1.0f - MathEx.Clamp01(options.thickness));
                        if (options.thickness < 1.0f)
                        {
                            Gizmos.Color = Color.Green * new Color(0.5f, 0.5f, 0.5f);
                            Gizmos.DrawWireHemisphere(Vector3.Zero, innerRadius);
                        }

                        Gizmos.Color = Color.Green * new Color(0.25f, 0.25f, 0.25f);
                        Gizmos.DrawLine(Vector3.XAxis * innerRadius, Vector3.XAxis * options.radius);
                        Gizmos.DrawLine(-Vector3.XAxis * innerRadius, -Vector3.XAxis * options.radius);
                        Gizmos.DrawLine(Vector3.YAxis * innerRadius, Vector3.YAxis * options.radius);
                        Gizmos.DrawLine(-Vector3.YAxis * innerRadius, -Vector3.YAxis * options.radius);
                        Gizmos.DrawLine(Vector3.ZAxis * innerRadius, Vector3.ZAxis * options.radius);

                    }
                }
                else if (shape is ParticleEmitterCircleShape circleShape)
                {
                    ParticleCircleShapeOptions options = circleShape.Options;
                    DrawArcWithThickness(Vector3.Zero, options.radius, options.arc, options.thickness, Color.Green);
                }
                else if (shape is ParticleEmitterLineShape lineShape)
                {
                    float halfLength = lineShape.Options.length * 0.5f;
                    Gizmos.DrawLine(new Vector3(-halfLength, 0.0f, 0.0f), new Vector3(halfLength, 0.0f, 0.0f));
                }
                else if (shape is ParticleEmitterRectShape rectShape)
                {
                    Vector2 extents = rectShape.Options.extents;
                    Vector3 right = new Vector3(extents.x, 0.0f, 0.0f);
                    Vector3 up = new Vector3(0.0f, extents.y, 0.0f);
                    
                    Vector3 topLeft = -right + up;
                    Vector3 topRight = right + up;
                    Vector3 botLeft = -right - up;
                    Vector3 botRight = right - up;

                    Gizmos.DrawLine(topLeft, topRight);
                    Gizmos.DrawLine(topRight, botRight);
                    Gizmos.DrawLine(botRight, botLeft);
                    Gizmos.DrawLine(botLeft, topLeft);
                }
                else if (shape is ParticleEmitterConeShape coneShape)
                {
                    ParticleConeShapeOptions options = coneShape.Options;

                    float topRadius = options.radius;
                    float baseRadius = options.length * MathEx.Tan(options.angle);
                    Radian arc = options.arc;

                    if (topRadius > 0.0f)
                        DrawArcWithThickness(Vector3.Zero, topRadius, arc, options.thickness, Color.Green);

                    DrawArcWithThickness(new Vector3(0.0f, 0.0f, -options.length), baseRadius, arc, options.thickness, 
                        Color.Green);

                    Gizmos.Color = Color.Green;
                    DrawConeBorder(topRadius, baseRadius, arc, options.length);

                    if (options.thickness > 0.0f && options.thickness < 1.0f)
                    {
                        Gizmos.Color = Color.Green * new Color(0.5f, 0.5f, 0.5f);
                        DrawConeBorder(
                            topRadius * (1.0f - MathEx.Clamp01(options.thickness)), 
                            baseRadius * (1.0f - MathEx.Clamp01(options.thickness)), arc, options.length);
                    }
                }
                else if (shape is ParticleEmitterStaticMeshShape staticMeshShape)
                {
                    RRef<Mesh> mesh = staticMeshShape.Options.mesh;
                    if(mesh.IsLoaded)
                        Gizmos.DrawWireMesh(mesh.Value.MeshData);
                }
                else if (shape is ParticleEmitterSkinnedMeshShape skinnedMeshShape)
                {
                    Renderable renderable = skinnedMeshShape.Options.renderable;
                    if (renderable != null)
                    {
                        RRef<Mesh> mesh = renderable.Mesh;
                        if (mesh.IsLoaded)
                            Gizmos.DrawWireMesh(mesh.Value.MeshData);
                    }
                }
            }

            // Draw manual bounds
            if (!component.Settings.UseAutomaticBounds)
            {
                Gizmos.Color = Color.LightGray;

                AABox bounds = component.Settings.CustomBounds;
                Gizmos.DrawWireCube(bounds.Center, bounds.Size * 0.5f);
            }
        }

        /// <summary>
        /// Draws particle system icon in scene view.
        /// </summary>
        /// <param name="component">Component to draw the icon for.</param>
        [DrawGizmo(DrawGizmoFlags.NotSelected | DrawGizmoFlags.Pickable)]
        private static void DrawIcon(ParticleSystem component)
        {
            Gizmos.DrawIcon(component.SceneObject.Position, EditorBuiltin.GetSceneViewIcon(SceneViewIcon.ParticleSystem), false);
        }

        /// <summary>
        /// Method called by the runtime when the component is selected or deselected.
        /// </summary>
        /// <param name="component">Component that was selected or deselected.</param>
        /// <param name="selected">If true the provided component was selected, otherwise deselected.</param>
        [OnSelectionChanged]
        private static void SelectionChanged(ParticleSystem component, bool selected)
        {
            EditorApplication.ToggleOnDemandDrawing("__ParticlesPreview", !selected);
            component.TogglePreviewMode(selected);
        }
    }

    /** @} */
}
