﻿//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Windows
     *  @{
     */

    /// <summary>
    /// Displays in-game viewport in the editor.
    /// </summary>
    public class GameWindow : EditorWindow
    {
        private const int HeaderHeight = 20;
        private static readonly Color BG_COLOR = Color.VeryDarkGray;
        private readonly AspectRatio[] aspectRatios =
        {
            new AspectRatio(16, 9), 
            new AspectRatio(16, 10),
            new AspectRatio(5, 4),
            new AspectRatio(4, 3),
            new AspectRatio(3, 2)
        };

        private int selectedAspectRatio = 0;
        private GUIRenderTexture renderTextureGUI;
        private GUITexture renderTextureBg;
        private GUILabel noCameraLabel;

        private Camera currentCamera;
        private bool onDemandDisabled;

        /// <summary>
        /// Opens the game window.
        /// </summary>
        [MenuItem("Windows/Game", ButtonModifier.CtrlAlt, ButtonCode.G, 6000)]
        private static void OpenGameWindow()
        {
            OpenWindow<GameWindow>();
        }

        /// <summary>
        /// Starts execution of the game in the game window.
        /// </summary>
        [MenuItem("Tools/Play", 9300)]
        [ToolbarItem("Play", ToolbarIcon.Play, "Play", 1800, true)]
        private static void Play()
        {
            GameWindow gameWindow = GetWindow<GameWindow>();
            SceneWindow sceneWindow = GetWindow<SceneWindow>();

            PlayInEditorState state = PlayInEditor.State;
            if (state != PlayInEditorState.Playing)
            {
                gameWindow.Active = true;
                gameWindow.HasFocus = true;
            }
            else
            {
                sceneWindow.Active = true;
                sceneWindow.HasFocus = true;
            }

            if (state == PlayInEditorState.Paused)
                PlayInEditor.State = PlayInEditorState.Playing;
            else
            {
                if(state == PlayInEditorState.Playing)
                    PlayInEditor.State = PlayInEditorState.Stopped;
                else
                    PlayInEditor.State = PlayInEditorState.Playing;
            }
        }

        /// <summary>
        /// Pauses the execution of the game on the current frame.
        /// </summary>
        [MenuItem("Tools/Pause", 9299)]
        [ToolbarItem("Pause", ToolbarIcon.Pause, "Pause", 1799)]
        private static void Pause()
        {
            if (PlayInEditor.State == PlayInEditorState.Paused)
                PlayInEditor.State = PlayInEditorState.Playing;
            else
                PlayInEditor.State = PlayInEditorState.Paused;
        }

        /// <summary>
        /// Moves the execution of the game by one frame forward.
        /// </summary>
        [MenuItem("Tools/Step", 9298)]
        [ToolbarItem("Step", ToolbarIcon.Step, "Frame step", 1798)]
        private static void Step()
        {
            PlayInEditor.FrameStep();
        }

        /// <inheritdoc/>
        protected override LocString GetDisplayName()
        {
            return new LocEdString("Game");
        }

        private void OnInitialize()
        {
            GUILayoutY mainLayout = GUI.AddLayoutY();

            string[] aspectRatioTitles = new string[aspectRatios.Length + 1];
            aspectRatioTitles[0] = "Free";

            for (int i = 0; i < aspectRatios.Length; i++)
                aspectRatioTitles[i + 1] = aspectRatios[i].width + ":" + aspectRatios[i].height;

            GUIListBoxField aspectField = new GUIListBoxField(aspectRatioTitles, new LocEdString("Aspect ratio"));
            aspectField.OnSelectionChanged += OnAspectRatioChanged;
            
            GUILayoutY buttonLayoutVert = mainLayout.AddLayoutY();
            GUILayoutX buttonLayout = buttonLayoutVert.AddLayoutX();
            buttonLayout.AddElement(aspectField);
            buttonLayout.AddFlexibleSpace();
            buttonLayoutVert.AddFlexibleSpace();

            renderTextureGUI = new GUIRenderTexture(null);
            renderTextureBg = new GUITexture(Builtin.WhiteTexture);
            renderTextureBg.SetTint(BG_COLOR);

            noCameraLabel = new GUILabel(new LocEdString("(No main camera in scene)"));

            GUIPanel rtPanel = mainLayout.AddPanel();
            rtPanel.AddElement(renderTextureGUI);

            GUIPanel bgPanel = rtPanel.AddPanel(1);
            bgPanel.AddElement(renderTextureBg);

            GUILayoutY alignLayoutY = rtPanel.AddLayoutY();
            alignLayoutY.AddFlexibleSpace();
            GUILayoutX alignLayoutX = alignLayoutY.AddLayoutX();
            alignLayoutX.AddFlexibleSpace();
            alignLayoutX.AddElement(noCameraLabel);
            alignLayoutX.AddFlexibleSpace();
            alignLayoutY.AddFlexibleSpace();

            UpdateRenderTexture(Width, Height);

            currentCamera = Scene.Camera;
            bool hasMainCamera = currentCamera != null;

            renderTextureGUI.Active = hasMainCamera;
            noCameraLabel.Active = !hasMainCamera;

            ToggleOnDemandDrawing(EditorApplication.IsOnDemandDrawingEnabled());
            NotifyNeedsRedraw();
        }

        private void OnEditorUpdate()
        {
            Camera camera = Scene.Camera;
            if (camera != currentCamera)
            {
                if (currentCamera != null)
                    currentCamera.Flags &= ~CameraFlag.OnDemand;

                if(!onDemandDisabled)
                    camera.Flags |= CameraFlag.OnDemand;

                currentCamera = camera;
            }

            bool hasMainCamera = camera != null;

            renderTextureGUI.Active = hasMainCamera;
            noCameraLabel.Active = !hasMainCamera;
        }

        private void OnDestroy()
        {
            if (currentCamera != null)
                currentCamera.Flags &= ~CameraFlag.OnDemand;

            EditorApplication.MainRenderTarget = null;
        }

        /// <summary>
        /// Notifies the system that the 3D viewport should be redrawn.
        /// </summary>
        internal void NotifyNeedsRedraw()
        {
            Camera camera = Scene.Camera;
            camera?.NotifyNeedsRedraw();
        }

        /// <summary>
        /// Enables or disables on-demand drawing. When enabled the 3D viewport will only be redrawn when
        /// <see cref="NotifyNeedsRedraw"/> is called. If disabled the viewport will be redrawn every frame.
        /// Normally you always want to keep this disabled unless you know the viewport will require updates
        /// every frame (e.g. when a game is running, or when previewing animations).
        /// </summary>
        /// <param name="enabled">True to enable on-demand drawing, false otherwise.</param>
        internal void ToggleOnDemandDrawing(bool enabled)
        {
            onDemandDisabled = !enabled;

            if (currentCamera == null)
                return;

            if (enabled)
                currentCamera.Flags |= CameraFlag.OnDemand;
            else
                currentCamera.Flags &= ~CameraFlag.OnDemand;
        }

        /// <summary>
        /// Creates or rebuilds the main render texture. Should be called at least once before using the
        /// game window. Should be called whenever the window is resized.
        /// </summary>
        /// <param name="width">Width of the scene render target, in pixels.</param>
        /// <param name="height">Height of the scene render target, in pixels.</param>
        private void UpdateRenderTexture(int width, int height)
        {
            height = height - HeaderHeight;

            int rtWidth = MathEx.Max(20, width);
            int rtHeight = MathEx.Max(20, height);

            if (selectedAspectRatio != 0) // 0 is free aspect
            {
                AspectRatio aspectRatio = aspectRatios[selectedAspectRatio - 1];

                int visibleAreaHeight = rtHeight;

                float aspectInv = aspectRatio.height/(float)aspectRatio.width;
                rtHeight = MathEx.RoundToInt(rtWidth*aspectInv);

                if (rtHeight > visibleAreaHeight)
                {
                    rtHeight = visibleAreaHeight;
                    float aspect = aspectRatio.width / (float)aspectRatio.height;
                    rtWidth = MathEx.RoundToInt(rtHeight * aspect);
                }
            }

            RenderTexture renderTexture = new RenderTexture(PixelFormat.RGBA8, rtWidth, rtHeight) { Priority = 1};

            EditorApplication.MainRenderTarget = renderTexture;
            renderTextureGUI.RenderTexture = renderTexture;

            int offsetX = (width - rtWidth)/2;
            int offsetY = (height - rtHeight)/2;

            Rect2I rtBounds = new Rect2I(offsetX, offsetY, rtWidth, rtHeight);
            renderTextureGUI.Bounds = rtBounds;

            Rect2I bgBounds = new Rect2I(0, 0, width, height);
            renderTextureBg.Bounds = bgBounds;

            NotifyNeedsRedraw();
        }

        /// <summary>
        /// Triggered when the user selects a new aspect ratio from the drop down box.
        /// </summary>
        /// <param name="idx">Index of the aspect ratio the user selected.</param>
        private void OnAspectRatioChanged(int idx)
        {
            selectedAspectRatio = idx;
            UpdateRenderTexture(Width, Height);
        }

        /// <inheritdoc/>
        protected override void WindowResized(int width, int height)
        {
            UpdateRenderTexture(width, height);

            base.WindowResized(width, height);
        }

        /// <summary>
        /// Camera aspect ratio as numerator and denominator.
        /// </summary>
        struct AspectRatio
        {
            /// <summary>
            /// Creates a new object that holds the aspect ratio.
            /// </summary>
            /// <param name="width">Numerator of the aspect ratio.</param>
            /// <param name="height">Denominator of the aspect ratio.</param>
            public AspectRatio(int width, int height)
            {
                this.width = width;
                this.height = height;
            }

            public int width;
            public int height;
        }
    }

    /** @} */
}
