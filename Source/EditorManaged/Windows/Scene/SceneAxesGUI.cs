﻿//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Scene-Editor
     *  @{
     */

    /// <summary>
    /// Handles rendering of scene axis handles into a GUI element.
    /// </summary>
    internal class SceneAxesGUI
    {
        private RenderTexture renderTexture;
        private Camera camera;
        private SceneHandles sceneHandles;

        private GUIPanel panel;
        private GUIRenderTexture renderTextureGUI;
        private GUILabel labelGUI;
        private Rect2I bounds;

        /// <summary>
        /// Projection type to display on the GUI.
        /// </summary>
        public ProjectionType ProjectionType
        {
            set { labelGUI.SetContent(value.ToString()); }
        }

        /// <summary>
        /// Creates a new scene axes GUI.
        /// </summary>
        /// <param name="window">Window in which the GUI is located in.</param>
        /// <param name="panel">Panel onto which to place the GUI element.</param>
        /// <param name="width">Width of the GUI element.</param>
        /// <param name="height">Height of the GUI element.</param>
        /// <param name="projType">Projection type to display on the GUI.</param>
        public SceneAxesGUI(SceneWindow window, GUIPanel panel, int width, int height, ProjectionType projType)
        {
            renderTexture = new RenderTexture(PixelFormat.RGBA8, width, height);
            renderTexture.Priority = 1;

            SceneObject cameraSO = new SceneObject("SceneAxesCamera", true);
            camera = cameraSO.AddComponent<Camera>();
            camera.Viewport.Target = renderTexture;
            camera.Viewport.Area = new Rect2(0.0f, 0.0f, 1.0f, 1.0f);
            
            cameraSO.Position = new Vector3(0, 0, 5);
            cameraSO.LookAt(new Vector3(0, 0, 0));

            camera.Priority = 2;
            camera.NearClipPlane = 0.05f;
            camera.FarClipPlane = 1000.0f;
            camera.Viewport.ClearColor = new Color(0.0f, 0.0f, 0.0f, 0.0f);
            camera.ProjectionType = ProjectionType.Orthographic;
            camera.Layers = SceneAxesHandle.LAYER;
            camera.AspectRatio = 1.0f;
            camera.OrthoHeight = 2.0f;
            camera.RenderSettings.EnableHDR = false;
            camera.RenderSettings.EnableSkybox = false;
            camera.Flags |= CameraFlag.OnDemand;

            renderTextureGUI = new GUIRenderTexture(renderTexture, true);

            GUILayoutY layout = panel.AddLayoutY();
            GUILayoutX textureLayout = layout.AddLayoutX();
            textureLayout.AddElement(renderTextureGUI);
            textureLayout.AddFlexibleSpace();

            Rect2I bounds = new Rect2I(0, 0, width, height);
            sceneHandles = new SceneHandles(window, camera);
            renderTextureGUI.Bounds = bounds;

            labelGUI = new GUILabel(projType.ToString(), EditorStyles.LabelCentered);
            layout.AddElement(labelGUI);
            layout.AddFlexibleSpace();

            this.panel = panel;
            this.bounds = bounds;

            NotifyNeedsRedraw();
        }

        /// <summary>
        /// Selects a handle under the pointer position.
        /// </summary>
        /// <param name="pointerPos">Position of the pointer relative to the parent GUI panel.</param>
        public void TrySelect(Vector2I pointerPos)
        {
            if (!bounds.Contains(pointerPos))
                return;

            pointerPos.x -= bounds.x;
            pointerPos.y -= bounds.y;
            if(sceneHandles.TrySelect(pointerPos))
                NotifyNeedsRedraw();
        }

        /// <summary>
        /// Checks is any handle currently active.
        /// </summary>
        /// <returns>True if a handle is active.</returns>
        internal bool IsActive()
        {
            return sceneHandles.IsActive();
        }

        /// <summary>
        /// Deselects any currently active handles.
        /// </summary>
        public void ClearSelection()
        {
            sceneHandles.ClearSelection();
            NotifyNeedsRedraw();
        }

        /// <summary>
        /// Updates active handles by moving them as a result of any input.
        /// </summary>
        /// <param name="pointerPos">Position of the pointer relative to the parent GUI panel</param>
        public void UpdateInput(Vector2I pointerPos)
        {
            pointerPos.x -= bounds.x;
            pointerPos.y -= bounds.y;

            if(sceneHandles.UpdateInput(pointerPos, Input.PointerDelta))
                NotifyNeedsRedraw();
        }

        /// <summary>
        /// Draws the scene axes onto the underlying camera.
        /// </summary>
        public void Draw()
        {
            sceneHandles.Draw();
        }

        /// <summary>
        /// Notifies the system that the 3D viewport should be redrawn.
        /// </summary>
        internal void NotifyNeedsRedraw()
        {
            camera.NotifyNeedsRedraw();
        }

        /// <summary>
        /// Enables or disables on-demand drawing. When enabled the 3D viewport will only be redrawn when
        /// <see cref="NotifyNeedsRedraw"/> is called. If disabled the viewport will be redrawn every frame.
        /// </summary>
        /// <param name="enabled">True to enable on-demand drawing, false otherwise.</param>
        internal void ToggleOnDemandDrawing(bool enabled)
        {
            if (enabled)
                camera.Flags |= CameraFlag.OnDemand;
            else
                camera.Flags &= ~CameraFlag.OnDemand;
        }

        /// <summary>
        /// Moves the GUI element to the specified position.
        /// </summary>
        /// <param name="x">Horizontal position of the GUI element relative to the parent panel.</param>
        /// <param name="y">Vertical position of the GUI element relative to the parent panel.</param>
        public void SetPosition(int x, int y)
        {
            bounds.x = x;
            bounds.y = y;
            renderTextureGUI.Bounds = bounds;
            panel.SetPosition(x, y);
        }

        /// <summary>
        /// Call this when done with the object so internal resources can be cleaned up.
        /// </summary>
        public void Destroy()
        {
            camera.SceneObject.Destroy();
        }
    }

    /** @} */
}
