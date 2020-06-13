//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using bs;

namespace bs.Editor
{
    /** @addtogroup Window
     *  @{
     */

    /// <summary>
    /// Contains various menu item callbacks for the main editor menu bar.
    /// </summary>
    static class MenuItems
    {
        /// <summary>
        /// Adds a camera component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Camera", 7950)]
        private static void AddCamera()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Camera component");
            Camera cam = so.AddComponent<Camera>();
            cam.Main = true;

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a renderable component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Renderable", 7949)]
        private static void AddRenderable()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Renderable component");
            so.AddComponent<Renderable>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a ParticleSystem component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Particle system", 7948)]
        private static void AddParticleSystem()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Particle System component");
            so.AddComponent<ParticleSystem>();
            
            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a Decal component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Decal", 7947)]
        private static void AddDecal()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Decal component");
            so.AddComponent<Decal>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a point light component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Radial light", 7900, true)]
        private static void AddPointLight()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Light component");
            Light light = so.AddComponent<Light>();
            light.Type = LightType.Radial;

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a spot light component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Spot light", 7899)]
        private static void AddSpotLight()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Light component");
            Light light = so.AddComponent<Light>();
            light.Type = LightType.Spot;

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a directional light component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Directional light", 7898)]
        private static void AddDirectionalLight()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Light component");
            Light light = so.AddComponent<Light>();
            light.Type = LightType.Directional;

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a Skybox component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Skybox", 7897)]
        private static void AddSkybox()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added an Skybox component");
            so.AddComponent<Skybox>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a ReflectionProbe component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Reflection probe", 7896)]
        private static void AddReflectionProbe()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added an ReflectionProbe component");
            so.AddComponent<ReflectionProbe>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a LightProbeVolume component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Light probe volume", 7895)]
        private static void AddLightProbeVolume()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a Light Probe Volume component");
            so.AddComponent<LightProbeVolume>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a GUI widget component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/GUI widget", 7850, true)]
        private static void AddGUIWidget()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added a GUIWidget component");
            so.AddComponent<GUIWidget>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a BoxCollider component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Box collider", 7800, true)]
        private static void AddBoxCollider()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            {
                so = new SceneObject("BoxCollider");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<BoxCollider>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a BoxCollider component");
                so.AddComponent<BoxCollider>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a SphereCollider component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Sphere collider", 7799)]
        private static void AddSphereCollider()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("SphereCollider");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<SphereCollider>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a SphereCollider component");
                so.AddComponent<SphereCollider>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a CapsuleCollider component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Capsule collider", 7798)]
        private static void AddCapsuleCollider()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("CapsuleCollider");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<CapsuleCollider>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a CapsuleCollider component");
                so.AddComponent<CapsuleCollider>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a MeshCollider component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Mesh collider", 7797)]
        private static void AddMeshCollider()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("MeshCollider");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<MeshCollider>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a MeshCollider component");
                so.AddComponent<MeshCollider>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a PlaneCollider component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Plane collider", 7796)]
        private static void AddPlaneCollider()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("PlaneCollider");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<PlaneCollider>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a PlaneCollider component");
                so.AddComponent<PlaneCollider>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a Rigidbody component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Rigidbody", 7750, true)]
        private static void AddRigidbody()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            {
                so = new SceneObject("Rigidbody");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<Rigidbody>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a Rigidbody component");
                so.AddComponent<Rigidbody>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a CharacterController component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Character controller", 7749)]
        private static void AddCharacterController()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("CharacterController");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<CharacterController>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a CharacterController component");
                so.AddComponent<CharacterController>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a FixedJoint component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Fixed joint", 7600, true)]
        private static void AddFixedJoint()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("FixedJoint");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<FixedJoint>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a FixedJoint component");
                so.AddComponent<FixedJoint>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a DistanceJoint component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Distance joint", 7599)]
        private static void AddDistanceJoint()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("DistanceJoint");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<DistanceJoint>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a DistanceJoint component");
                so.AddComponent<DistanceJoint>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a HingeJoint component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Hinge joint", 7598)]
        private static void AddHingeJoint()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("HingeJoint");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<HingeJoint>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a HingeJoint component");
                so.AddComponent<HingeJoint>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a SphericalJoint component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Spherical joint", 7597)]
        private static void AddSphericalJoint()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("SphericalJoint");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<SphericalJoint>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a SphericalJoint component");
                so.AddComponent<SphericalJoint>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a SliderJoint component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/Slider joint", 7596)]
        private static void AddSliderJoint()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("SliderJoint");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<SliderJoint>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a SliderJoint component");
                so.AddComponent<SliderJoint>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds a D6Joint component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Physics/D6 joint", 7595)]
        private static void AddD6Joint()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            { 
                so = new SceneObject("D6Joint");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<D6Joint>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a D6Joint component");
                so.AddComponent<D6Joint>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds an AudioListener component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Audio/Listener", 7550, true)]
        private static void AddAudioListener()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            {
                so = new SceneObject("AudioListener");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<AudioListener>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a AudioListener component");
                so.AddComponent<AudioListener>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds an AudioSource component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Audio/Source", 7549)]
        private static void AddAudioSource()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
            {
                so = new SceneObject("AudioSource");

                GameObjectUndo.RecordNewSceneObject(so);
                so.AddComponent<AudioSource>();

                FocusOnHierarchyOrScene();
            }
            else
            {
                GameObjectUndo.RecordSceneObject(so, false, "Added a AudioSource component");
                so.AddComponent<AudioSource>();
            }

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Adds an Animation component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Animation", 7500, true)]
        private static void AddAnimation()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added an Animation component");
            so.AddComponent<Animation>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }


        /// <summary>
        /// Adds a Bone component to the currently selected scene object.
        /// </summary>
        [MenuItem("Components/Bone", 7499)]
        private static void AddBone()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, false, "Added an Bone component");
            so.AddComponent<Bone>();

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }


        /// <summary>
        /// Creates a new empty scene object.
        /// </summary>
        [MenuItem("Scene Objects/Scene Object", 9000)]
        [ToolbarItem("SceneObject", ToolbarIcon.NewSceneObject, "Creates a new empty scene object", 1601, true)]
        private static void AddEmptySO()
        {
            UndoRedo.CreateSO("SceneObject", "New scene object");

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a Camera component.
        /// </summary>
        [MenuItem("Scene Objects/Camera", 8950, true)]
        [ToolbarItem("Camera", ToolbarIcon.NewCamera, "New camera", 1600, false)]
        private static void AddCameraSO()
        {
            SceneObject so = new SceneObject("Camera");
            GameObjectUndo.RecordNewSceneObject(so);

            Camera cam = so.AddComponent<Camera>();
            cam.Main = true;

            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a Renderable component.
        /// </summary>
        [MenuItem("Scene Objects/Renderable", 8949)]
        [ToolbarItem("Renderable", ToolbarIcon.NewRenderable, "New renderable", 1599)]
        private static void AddRenderableSO()
        {
            SceneObject so = new SceneObject("Renderable");

            GameObjectUndo.RecordNewSceneObject(so);
            so.AddComponent<Renderable>();
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a Particle System component.
        /// </summary>
        [MenuItem("Scene Objects/Particle system", 8948)]
        private static void AddParticleSystemSO()
        {
            SceneObject so = new SceneObject("Particle system");

            GameObjectUndo.RecordNewSceneObject(so);
            so.AddComponent<ParticleSystem>();
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a Decal component.
        /// </summary>
        [MenuItem("Scene Objects/Decal", 8947)]
        private static void AddDecalSO()
        {
            SceneObject so = new SceneObject("Decal");

            GameObjectUndo.RecordNewSceneObject(so);
            so.AddComponent<Decal>();
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a radial light component.
        /// </summary>
        [MenuItem("Scene Objects/Radial light", 8900, true)]
        [ToolbarItem("Point light", ToolbarIcon.NewPointLight, "New radial light", 1598)]
        private static void AddPointLightSO()
        {
            SceneObject so = new SceneObject("Radial light");

            GameObjectUndo.RecordNewSceneObject(so);
            Light light = so.AddComponent<Light>();
            light.Type = LightType.Radial;
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a spot light component.
        /// </summary>
        [MenuItem("Scene Objects/Spot light", 8899)]
        [ToolbarItem("Spot light", ToolbarIcon.NewSpotLight, "New spot light", 1597)]
        private static void AddSpotLightSO()
        {
            SceneObject so = new SceneObject("Spot light");

            GameObjectUndo.RecordNewSceneObject(so);
            Light light = so.AddComponent<Light>();
            light.Type = LightType.Spot;
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a directional light component.
        /// </summary>
        [MenuItem("Scene Objects/Directional light", 8898)]
        [ToolbarItem("Directional light", ToolbarIcon.NewDirLight, "New directional light", 1596)]
        private static void AddDirectionalLightSO()
        {
            SceneObject so = new SceneObject("Directional light");

            GameObjectUndo.RecordNewSceneObject(so);
            Light light = so.AddComponent<Light>();
            light.Type = LightType.Directional;
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a GUI widget component.
        /// </summary>
        [MenuItem("Scene Objects/GUI widget", 8850, true)]
        private static void AddGUIWidgetSO()
        {
            SceneObject so = new SceneObject("GUI widget");

            GameObjectUndo.RecordNewSceneObject(so);
            so.AddComponent<GUIWidget>();
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a box primitive.
        /// </summary>
        [MenuItem("Scene Objects/3D primitives/Box", 8100, true)]
        [ToolbarItem("Cube", ToolbarIcon.NewCube, "Creates a scene object with a box primitive", 1700, true)]
        private static void Add3DBox()
        {
            SceneObject so = new SceneObject("Box");

            GameObjectUndo.RecordNewSceneObject(so);
            Renderable renderable = so.AddComponent<Renderable>();
            renderable.Mesh = Builtin.Box;
            so.AddComponent<BoxCollider>();
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a sphere primitive.
        /// </summary>
        [MenuItem("Scene Objects/3D primitives/Sphere", 8099)]
        [ToolbarItem("Sphere", ToolbarIcon.NewSphere, "Creates a scene object with a sphere primitive", 1699)]
        private static void Add3DSphere()
        {
            SceneObject so = new SceneObject("Sphere");

            GameObjectUndo.RecordNewSceneObject(so);
            Renderable renderable = so.AddComponent<Renderable>();
            renderable.Mesh = Builtin.Sphere;
            so.AddComponent<SphereCollider>();
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a cone primitive.
        /// </summary>
        [MenuItem("Scene Objects/3D primitives/Cone", 8098)]
        [ToolbarItem("Cone", ToolbarIcon.NewCone, "Creates a scene object with a cone primitive", 1698)]
        private static void Add3DCone()
        {
            SceneObject so = new SceneObject("Cone");

            GameObjectUndo.RecordNewSceneObject(so);
            Renderable renderable = so.AddComponent<Renderable>();
            renderable.Mesh = Builtin.Cone;
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a quad primitive.
        /// </summary>
        [MenuItem("Scene Objects/3D primitives/Quad", 8097)]
        [ToolbarItem("Quad", ToolbarIcon.NewQuad, "Creates a scene object with a quad primitive", 1697)]
        private static void Add3DQuad()
        {
            SceneObject so = new SceneObject("Quad");

            GameObjectUndo.RecordNewSceneObject(so);
            Renderable renderable = so.AddComponent<Renderable>();
            renderable.Mesh = Builtin.Quad;
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Creates a new scene object with a disc primitive.
        /// </summary>
        [MenuItem("Scene Objects/3D primitives/Disc", 8096)]
        private static void Add3DDisc()
        {
            SceneObject so = new SceneObject("Disc");

            GameObjectUndo.RecordNewSceneObject(so);
            Renderable renderable = so.AddComponent<Renderable>();
            renderable.Mesh = Builtin.Disc;
            GameObjectUndo.ResolveDiffs();

            FocusOnHierarchyOrScene();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Applies changes from the prefab instance to the prefab resource.
        /// </summary>
        [MenuItem("Scene Objects/Apply prefab", 8025, true)]
        private static void ApplyPrefab()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            PrefabUtility.ApplyPrefab(so);
        }

        /// <summary>
        /// Reverts a prefab instance to the original state of its prefab.
        /// </summary>
        [MenuItem("Scene Objects/Revert to prefab", 8024)]
        private static void RevertToPrefab()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            GameObjectUndo.RecordSceneObject(so, true, "Reverting \"" + so.Name + "\" to prefab.");
            PrefabUtility.RevertPrefab(so);

            GameObjectUndo.ResolveDiffs();
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Breaks a link between a prefab and its instance.
        /// </summary>
        [MenuItem("Scene Objects/Break prefab link", 8023)]
        private static void BreakPrefabLink()
        {
            SceneObject so = Selection.SceneObject;
            if (so == null)
                return;

            UndoRedo.BreakPrefab(so, "Breaking prefab link for " + so.Name);
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Cuts the currently selected scene object or resource.
        /// </summary>
        [MenuItem("Edit/Cut", 9450, true)]
        public static void Cut()
        {
            EditorApplication.TriggerGlobalShortcut(EditorApplication.CutKey);
        }

        /// <summary>
        /// Copies the currently selected scene object or resource.
        /// </summary>
        [MenuItem("Edit/Copy", 9449)]
        public static void Copy()
        {
            EditorApplication.TriggerGlobalShortcut(EditorApplication.CopyKey);
        }

        /// <summary>
        /// Pastes the scene objects or resources that were previously cut or copied.
        /// </summary>
        [MenuItem("Edit/Paste", 9448)]
        public static void Paste()
        {
            EditorApplication.TriggerGlobalShortcut(EditorApplication.PasteKey);
        }

        /// <summary>
        /// Deletes currently selected scene objects or resources.
        /// </summary>
        [MenuItem("Edit/Delete", 9447)]
        public static void Delete()
        {
            EditorApplication.TriggerGlobalShortcut(EditorApplication.DeleteKey);
        }

        /// <summary>
        /// Duplicates currently selected scene objects or resources.
        /// </summary>
        [MenuItem("Edit/Duplicate", 9446)]
        public static void Duplicate()
        {
            EditorApplication.TriggerGlobalShortcut(EditorApplication.DuplicateKey);
        }

        /// <summary>
        /// Executes the last command on the undo stack, undoing its operations.
        /// </summary>
        [MenuItem("Edit/Undo", ButtonModifier.Ctrl, ButtonCode.Z, 9500, true)]
        [ToolbarItem("Undo", ToolbarIcon.Undo, "Undo (Ctrl + Z)", 1900, true)]
        public static void Undo()
        {
            EditorWindow[] allWindows = EditorWindow.AllWindows;
            foreach (var window in allWindows)
            {
                if (!window.HasFocus)
                    continue;

                UndoRedo localStack = window.UndoRedo;
                if (localStack == null)
                    continue;

                localStack.Undo();
                return;
            }

            GameObjectUndo.ResolveDiffs();
            UndoRedo.Global.Undo();
        }

        /// <summary>
        /// Executes the last command on the redo stack (last command we called undo on), re-applying its operation.
        /// </summary>
        [MenuItem("Edit/Redo", ButtonModifier.Ctrl, ButtonCode.Y, 9499)]
        [ToolbarItem("Redo", ToolbarIcon.Redo, "Redo (Ctrl + Y)", 1899)]
        public static void Redo()
        {
            EditorWindow[] allWindows = EditorWindow.AllWindows;
            foreach (var window in allWindows)
            {
                if (!window.HasFocus)
                    continue;

                UndoRedo localStack = window.UndoRedo;
                if (localStack == null)
                    continue;

                localStack.Redo();
                return;
            }

            UndoRedo.Global.Redo();
        }

        /// <summary>
        /// Sets keyboard focus to the Hierarchy or Scene windows if open.
        /// </summary>
        private static void FocusOnHierarchyOrScene()
        {
            SceneWindow sceneWindow = EditorWindow.GetWindow<SceneWindow>();
            if (sceneWindow != null)
            {
                sceneWindow.HasFocus = true;
                return;
            }

            HierarchyWindow hierarchyWindow = EditorWindow.GetWindow<HierarchyWindow>();
            if (hierarchyWindow != null)
                hierarchyWindow.HasFocus = true;
        }
    }

    /** @} */
}
