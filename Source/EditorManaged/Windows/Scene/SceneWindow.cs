//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;
using System;
using System.Collections.Generic;
using System.IO;

namespace bs.Editor
{
    /** @addtogroup Scene-Editor
     *  @{
     */

    /// <summary>
    /// Displays the scene view camera and various scene controls.
    /// </summary>
    internal sealed class SceneWindow : EditorWindow, IGlobalShortcuts
    {
        internal const string GizmoDrawSettingsKey = "SceneCamera0_GizmoDrawSettings";
        internal const string ToggleProfilerOverlayBinding = "ToggleProfilerOverlay";
        internal const string ViewToolBinding = "ViewTool";
        internal const string MoveToolBinding = "MoveTool";
        internal const string RotateToolBinding = "RotateTool";
        internal const string ScaleToolBinding = "ScaleTool";
        internal const string FrameBinding = "SceneFrame";

        private const string CameraPositionKey = "SceneCamera0_Position";
        private const string CameraRotationKey = "SceneCamera0_Rotation";

        private const int HeaderHeight = 20;
        private const float DefaultPlacementDepth = 5.0f;
        private static readonly Color ClearColor = new Color(0.0f, 0.3685f, 0.7969f);
        private const int HandleAxesGUISize = 50;
        private const int HandleAxesGUIPaddingX = 10;
        private const int HandleAxesGUIPaddingY = 5;

        private Camera camera;
        private SceneCamera sceneCamera;
        private ulong cameraUpdateCount;
        private RenderTexture renderTexture;
        private GUILayoutY mainLayout;
        private GUIPanel rtPanel;
        private GUIPanel sceneAxesPanel;
        private GUIButton focusCatcher;

        private GUIRenderTexture renderTextureGUI;
        private SceneGrid sceneGrid;
        private SceneSelection sceneSelection;
        private SceneGizmos sceneGizmos;
        private SceneHandles sceneHandles;

        private GUIToggle viewButton;
        private GUIToggle moveButton;
        private GUIToggle rotateButton;
        private GUIToggle scaleButton;

        private GUIToggle localCoordButton;
        private GUIToggle worldCoordButton;

        private GUIToggle pivotButton;
        private GUIToggle centerButton;

        private GUIToggle moveSnapButton;
        private GUIFloatField moveSnapInput;

        private GUIToggle rotateSnapButton;
        private GUIFloatField rotateSnapInput;

        private GUIButton cameraOptionsButton;

        private GUILayout progressLayout;
        private GUIProgressBar loadProgressBar;
        private GUILabel loadLabel;

        private SceneAxesGUI sceneAxesGUI;
        private ProjectionType sceneAxesLastProjectionType;

        private bool hasContentFocus = false;
        private bool HasContentFocus { get { return HasFocus && hasContentFocus; } }

        private bool loadingProgressShown = false;
        private bool AllowViewportInput { get { return !loadingProgressShown; } }

        private int editorSettingsHash = int.MaxValue;
        private GizmoDrawSettings gizmoDrawSettings = GizmoDrawSettings.Default();

        private VirtualButton frameKey;

        // Tool shortcuts
        private VirtualButton viewToolKey;
        private VirtualButton moveToolKey;
        private VirtualButton rotateToolKey;
        private VirtualButton scaleToolKey;

        // Drag & drop
        private bool dragActive;
        private SceneObject draggedSO;
        private Vector3 draggedSOOffset;
        private GUITexture dragSelection;
        private bool isDraggingSelection;
        private Vector2I dragSelectionStart;
        private Vector2I dragSelectionEnd;
        private Vector2I mouseDownPosition;
        private GUIPanel selectionPanel;

        // Camera previews
        private const int MaxCameraPreviews = 0; // Disabled until we resolve issues with adding temporary camera copies
        private List<CameraPreview> cameraPreviews = new List<CameraPreview>();
        private GUIPanel cameraPreviewsPanel;

        /// <summary>
        /// Returns the scene camera.
        /// </summary>
        public SceneCamera Camera => sceneCamera;

        /// <summary>
        /// Settings that control gizmo drawing.
        /// </summary>
        public GizmoDrawSettings GizmoDrawSettings
        {
            get => gizmoDrawSettings;
            set
            {
                gizmoDrawSettings = value;

                if(sceneGizmos != null)
                    sceneGizmos.DrawSettings = gizmoDrawSettings;

                if(sceneSelection != null)
                    sceneSelection.GizmoDrawSettings = gizmoDrawSettings;
            }
        }

        /// <summary>
        /// Constructs a new scene window.
        /// </summary>
        internal SceneWindow()
        { }

        /// <summary>
        /// Opens a scene window if its not open already.
        /// </summary>
        [MenuItem("Windows/Scene", ButtonModifier.CtrlAlt, ButtonCode.S, 6000)]
        private static void OpenSceneWindow()
        {
            OpenWindow<SceneWindow>();
        }

        /// <summary>
        /// Focuses on the currently selected object.
        /// </summary>
        [MenuItem("Tools/Frame Selected", ButtonModifier.None, ButtonCode.F, 9275, true)]
        private static void OpenSettingsWindow()
        {
            SceneWindow window = GetWindow<SceneWindow>();
            if (window != null)
                window.sceneCamera.FrameSelected();
        }

        /// <summary>
        /// Switches the active tool to the view tool.
        /// </summary>
        [MenuItem("Tools/View", ButtonModifier.Ctrl, ButtonCode.Q, 9274, true)]
        private static void SetViewTool()
        {
            SceneWindow window = GetWindow<SceneWindow>();
            if (window != null)
                window.OnSceneToolButtonClicked(SceneViewTool.View);
        }

        /// <summary>
        /// Switches the active tool to the move tool.
        /// </summary>
        [MenuItem("Tools/Move", ButtonModifier.Ctrl, ButtonCode.W, 9273)]
        private static void SetMoveTool()
        {
            SceneWindow window = GetWindow<SceneWindow>();
            if (window != null)
                window.OnSceneToolButtonClicked(SceneViewTool.Move);
        }

        /// <summary>
        /// Switches the active tool to the rotate tool.
        /// </summary>
        [MenuItem("Tools/Rotate", ButtonModifier.Ctrl, ButtonCode.E, 9272)]
        private static void SetRotateTool()
        {
            SceneWindow window = GetWindow<SceneWindow>();
            if (window != null)
                window.OnSceneToolButtonClicked(SceneViewTool.Rotate);
        }

        /// <summary>
        /// Switches the active tool to the scale tool.
        /// </summary>
        [MenuItem("Tools/Scale", ButtonModifier.Ctrl, ButtonCode.R, 9271)]
        private static void SetScaleTool()
        {
            SceneWindow window = GetWindow<SceneWindow>();
            if (window != null)
                window.OnSceneToolButtonClicked(SceneViewTool.Scale);
        }

        /// <inheritdoc/>
        protected override LocString GetDisplayName()
        {
            return new LocEdString("Scene");
        }

        private void OnInitialize()
        {
            if (ProjectSettings.HasKey(GizmoDrawSettingsKey))
                gizmoDrawSettings = ProjectSettings.GetObject<GizmoDrawSettings>(SceneWindow.GizmoDrawSettingsKey);
            else
                gizmoDrawSettings = GizmoDrawSettings.Default();

            mainLayout = GUI.AddLayoutY();

            GUIContent viewIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.View),
                new LocEdString("View"));
            GUIContent moveIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.Move),
                new LocEdString("Move"));
            GUIContent rotateIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.Rotate),
                new LocEdString("Rotate"));
            GUIContent scaleIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.Scale),
                new LocEdString("Scale"));

            GUIContent localIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.Local),
                new LocEdString("Local"));
            GUIContent worldIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.World),
                new LocEdString("World"));

            GUIContent pivotIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.Pivot),
                new LocEdString("Pivot"));
            GUIContent centerIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.Center),
                new LocEdString("Center"));

            GUIContent moveSnapIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.MoveSnap),
                new LocEdString("Move snap"));
            GUIContent rotateSnapIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.RotateSnap),
                new LocEdString("Rotate snap"));

            GUIToggleGroup handlesTG = new GUIToggleGroup();
            viewButton = new GUIToggle(viewIcon, handlesTG, EditorStyles.Button, GUIOption.FlexibleWidth(35));
            moveButton = new GUIToggle(moveIcon, handlesTG, EditorStyles.Button, GUIOption.FlexibleWidth(35));
            rotateButton = new GUIToggle(rotateIcon, handlesTG, EditorStyles.Button, GUIOption.FlexibleWidth(35));
            scaleButton = new GUIToggle(scaleIcon, handlesTG, EditorStyles.Button, GUIOption.FlexibleWidth(35));

            GUIToggleGroup coordModeTG = new GUIToggleGroup();
            localCoordButton = new GUIToggle(localIcon, coordModeTG, EditorStyles.Button, GUIOption.FlexibleWidth(75));
            worldCoordButton = new GUIToggle(worldIcon, coordModeTG, EditorStyles.Button, GUIOption.FlexibleWidth(75));

            GUIToggleGroup pivotModeTG = new GUIToggleGroup();
            pivotButton = new GUIToggle(pivotIcon, pivotModeTG, EditorStyles.Button, GUIOption.FlexibleWidth(35));
            centerButton = new GUIToggle(centerIcon, pivotModeTG, EditorStyles.Button, GUIOption.FlexibleWidth(35));

            moveSnapButton = new GUIToggle(moveSnapIcon, EditorStyles.Button, GUIOption.FlexibleWidth(35));
            moveSnapInput = new GUIFloatField("", GUIOption.FlexibleWidth(35));

            rotateSnapButton = new GUIToggle(rotateSnapIcon, EditorStyles.Button, GUIOption.FlexibleWidth(35));
            rotateSnapInput = new GUIFloatField("", GUIOption.FlexibleWidth(35));

            GUIContent cameraOptionsIcon = new GUIContent(EditorBuiltin.GetSceneWindowIcon(SceneWindowIcon.SceneCameraOptions), new LocEdString("Camera options"));
            cameraOptionsButton = new GUIButton(cameraOptionsIcon);

            viewButton.OnClick += () => OnSceneToolButtonClicked(SceneViewTool.View);
            moveButton.OnClick += () => OnSceneToolButtonClicked(SceneViewTool.Move);
            rotateButton.OnClick += () => OnSceneToolButtonClicked(SceneViewTool.Rotate);
            scaleButton.OnClick += () => OnSceneToolButtonClicked(SceneViewTool.Scale);

            localCoordButton.OnClick += () => OnCoordinateModeButtonClicked(HandleCoordinateMode.Local);
            worldCoordButton.OnClick += () => OnCoordinateModeButtonClicked(HandleCoordinateMode.World);

            pivotButton.OnClick += () => OnPivotModeButtonClicked(HandlePivotMode.Pivot);
            centerButton.OnClick += () => OnPivotModeButtonClicked(HandlePivotMode.Center);

            moveSnapButton.OnToggled += (bool active) => OnMoveSnapToggled(active);
            moveSnapInput.OnChanged += (float value) => OnMoveSnapValueChanged(value);

            rotateSnapButton.OnToggled += (bool active) => OnRotateSnapToggled(active);
            rotateSnapInput.OnChanged += (float value) => OnRotateSnapValueChanged(value);

            cameraOptionsButton.OnClick += () => OnCameraOptionsClicked();

            GUILayout handlesLayout = mainLayout.AddLayoutX();
            handlesLayout.AddElement(viewButton);
            handlesLayout.AddElement(moveButton);
            handlesLayout.AddElement(rotateButton);
            handlesLayout.AddElement(scaleButton);
            handlesLayout.AddSpace(10);
            handlesLayout.AddElement(localCoordButton);
            handlesLayout.AddElement(worldCoordButton);
            handlesLayout.AddSpace(10);
            handlesLayout.AddElement(pivotButton);
            handlesLayout.AddElement(centerButton);
            handlesLayout.AddFlexibleSpace();
            handlesLayout.AddElement(moveSnapButton);
            handlesLayout.AddElement(moveSnapInput);
            handlesLayout.AddSpace(10);
            handlesLayout.AddElement(rotateSnapButton);
            handlesLayout.AddElement(rotateSnapInput);
            handlesLayout.AddSpace(10);
            handlesLayout.AddElement(cameraOptionsButton);
            handlesLayout.SetHeight(viewButton.Bounds.height);

            GUIPanel mainPanel = mainLayout.AddPanel();
            rtPanel = mainPanel.AddPanel();

            // Loading progress
            loadLabel = new GUILabel(new LocEdString("Loading scene..."));
            loadProgressBar = new GUIProgressBar("", GUIOption.FixedWidth(200));

            progressLayout = mainPanel.AddLayoutY();
            progressLayout.AddFlexibleSpace();
            GUILayout loadLabelLayout = progressLayout.AddLayoutX();
            loadLabelLayout.AddFlexibleSpace();
            loadLabelLayout.AddElement(loadLabel);
            loadLabelLayout.AddFlexibleSpace();

            GUILayout progressBarLayout = progressLayout.AddLayoutX();
            progressBarLayout.AddFlexibleSpace();
            progressBarLayout.AddElement(loadProgressBar);
            progressBarLayout.AddFlexibleSpace();
            progressLayout.AddFlexibleSpace();

            progressLayout.Active = false;

            selectionPanel = mainPanel.AddPanel(-1);

            sceneAxesPanel = mainPanel.AddPanel(-1);
            sceneAxesGUI = new SceneAxesGUI(this, sceneAxesPanel, HandleAxesGUISize, HandleAxesGUISize, ProjectionType.Perspective);
            sceneAxesLastProjectionType = ProjectionType.Perspective;

            focusCatcher = new GUIButton("", EditorStyles.Blank);
            focusCatcher.OnFocusGained += () => hasContentFocus = true;
            focusCatcher.OnFocusLost += () => hasContentFocus = false;

            GUIPanel focusPanel = GUI.AddPanel(-2);
            focusPanel.AddElement(focusCatcher);

            cameraPreviewsPanel = GUI.AddPanel(-3);

            viewToolKey = new VirtualButton(ViewToolBinding);
            moveToolKey = new VirtualButton(MoveToolBinding);
            rotateToolKey = new VirtualButton(RotateToolBinding);
            scaleToolKey = new VirtualButton(ScaleToolBinding);
            frameKey = new VirtualButton(FrameBinding);

            UpdateRenderTexture(Width, Height - HeaderHeight);
            UpdateLoadingProgress();
            UpdateCameraPreviews();

            Selection.OnSelectionChanged += OnSelectionChanged;
            ToggleOnDemandDrawing(EditorApplication.IsOnDemandDrawingEnabled());
        }

        private void OnCameraOptionsClicked()
        {
            SceneCameraSettingsWindow.Open();
        }

        private void OnDestroy()
        {
            if (camera != null)
            {
                Vector3 pos = camera.SceneObject.Position;
                Quaternion rot = camera.SceneObject.Rotation;

                ProjectSettings.SetObject(CameraPositionKey, pos);
                ProjectSettings.SetObject(CameraRotationKey, rot);

                camera.SceneObject.Destroy(true);
                camera = null;
            }

            sceneAxesGUI.Destroy();
            sceneAxesGUI = null;

            Selection.OnSelectionChanged -= OnSelectionChanged;
        }

        /// <summary>
        /// Deletes all currently selected objects.
        /// </summary>
        private void DeleteSelection()
        {
            SceneObject[] selectedObjects = Selection.SceneObjects;
            CleanDuplicates(ref selectedObjects);

            if (selectedObjects.Length > 0)
            {
                foreach (var so in selectedObjects)
                {
                    string message = "Deleted " + so.Name;
                    UndoRedo.DeleteSO(so, message);
                }

                EditorApplication.SetSceneDirty();
            }
        }

        /// <summary>
        /// Duplicates all currently selected objects.
        /// </summary>
        private void DuplicateSelection()
        {
            SceneObject[] selectedObjects = Selection.SceneObjects;
            CleanDuplicates(ref selectedObjects);

            if (selectedObjects.Length > 0)
            {
                string message;
                if (selectedObjects.Length == 1)
                    message = "Duplicated " + selectedObjects[0].Name;
                else
                    message = "Duplicated " + selectedObjects.Length + " elements";

                Transform[] savedTransforms = new Transform[selectedObjects.Length];
                for (int i = 0; i < savedTransforms.Length; i++)
                    savedTransforms[i] = new Transform(selectedObjects[i]);

                SceneObject[] clonedObjects = UndoRedo.CloneSO(selectedObjects, message);

                // Restore world positions
                for (int i = 0; i < savedTransforms.Length; i++)
                    savedTransforms[i].Apply(clonedObjects[i]);

                EditorApplication.SetSceneDirty();
            }
        }

        /// <summary>
        /// Callback that triggers when a new set of scene objects or resources has been selected.
        /// </summary>
        /// <param name="objects">Selected scene objects.</param>
        /// <param name="resources">Selected resources.</param>
        private void OnSelectionChanged(SceneObject[] objects, string[] resources)
        {
            UpdateCameraPreviews();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnRenamePressed()
        {
            // Do nothing
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnDuplicatePressed()
        {
            DuplicateSelection();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnDeletePressed()
        {
            DeleteSelection();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnCopyPressed()
        {
            // Do nothing
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnCutPressed()
        {
            // Do nothing
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnPastePressed()
        {
            // Do nothing
        }

        /// <summary>
        /// Orients the camera so it looks along the provided axis.
        /// </summary>
        /// <param name="axis">Axis to look along.</param>
        internal void LookAlong(Vector3 axis)
        {
            axis.Normalize();

            sceneCamera.LookAlong(axis);
            UpdateGridMode();
        }

        private void UpdateGridMode()
        {
            Vector3 forward = camera.SceneObject.Forward;

            if (camera.ProjectionType == ProjectionType.Perspective)
                sceneGrid.SetMode(GridMode.Perspective);
            else
            {
                float dotX = Vector3.Dot(forward, Vector3.XAxis);
                if (dotX >= 0.95f)
                    sceneGrid.SetMode(GridMode.OrthoX);
                else if (dotX <= -0.95f)
                    sceneGrid.SetMode(GridMode.OrthoNegX);
                else
                {
                    float dotY = Vector3.Dot(forward, Vector3.YAxis);
                    if (dotY >= 0.95f)
                        sceneGrid.SetMode(GridMode.OrthoY);
                    else if (dotY <= -0.95f)
                        sceneGrid.SetMode(GridMode.OrthoNegY);
                    else
                    {
                        float dotZ = Vector3.Dot(forward, Vector3.ZAxis);
                        if (dotZ >= 0.95f)
                            sceneGrid.SetMode(GridMode.OrthoZ);
                        else if (dotZ <= -0.95f)
                            sceneGrid.SetMode(GridMode.OrthoNegZ);
                        else
                            sceneGrid.SetMode(GridMode.Perspective);
                    }
                }
            }
        }

        /// <summary>
        /// Converts screen coordinates into coordinates relative to the scene view render texture.
        /// </summary>
        /// <param name="screenPos">Coordinates relative to the screen.</param>
        /// <param name="scenePos">Output coordinates relative to the scene view texture.</param>
        /// <returns>True if the coordinates are within the scene view texture, false otherwise.</returns>
        private bool ScreenToScenePos(Vector2I screenPos, out Vector2I scenePos)
        {
            scenePos = screenPos;
            Vector2I windowPos = ScreenToWindowPos(screenPos);

            Rect2I bounds = GUIUtility.CalculateBounds(renderTextureGUI, GUI);
            if (bounds.Contains(windowPos))
            {
                scenePos.x = windowPos.x - bounds.x;
                scenePos.y = windowPos.y - bounds.y;

                return true;
            }

            return false;
        }

        private void OnEditorUpdate()
        {
            UpdateLoadingProgress();

            if (HasFocus)
            {
                if (!Input.IsPointerButtonHeld(PointerButton.Right))
                {
                    if (VirtualInput.IsButtonDown(EditorApplication.DuplicateKey))
                        DuplicateSelection();
                    else if (VirtualInput.IsButtonDown(EditorApplication.DeleteKey))
                        DeleteSelection();
                    else if (VirtualInput.IsButtonDown(viewToolKey))
                        EditorApplication.ActiveSceneTool = SceneViewTool.View;
                    else if (VirtualInput.IsButtonDown(moveToolKey))
                        EditorApplication.ActiveSceneTool = SceneViewTool.Move;
                    else if (VirtualInput.IsButtonDown(rotateToolKey))
                        EditorApplication.ActiveSceneTool = SceneViewTool.Rotate;
                    else if (VirtualInput.IsButtonDown(scaleToolKey))
                        EditorApplication.ActiveSceneTool = SceneViewTool.Scale;
                }
            }

            // Refresh GUI buttons if needed (in case someones changes the values from script)
            if (editorSettingsHash != EditorSettings.Hash)
            {
                UpdateButtonStates();
                editorSettingsHash = EditorSettings.Hash;
            }

            // Update scene view handles and selection
            sceneGrid.Draw();

            ProjectionType currentProjType = camera.ProjectionType;
            if (sceneAxesLastProjectionType != currentProjType)
            {
                sceneAxesGUI.ProjectionType = currentProjType;
                sceneAxesLastProjectionType = currentProjType;
            }

            bool handleActive = sceneHandles.IsActive() || sceneAxesGUI.IsActive();

            Vector2I scenePos;
            bool inBounds = ScreenToScenePos(Input.PointerPosition, out scenePos);

            bool clearSelection = false;
            if (AllowViewportInput)
            {
                if (Input.IsPointerButtonUp(PointerButton.Left))
                    clearSelection = true;
                else if (Input.IsPointerButtonDown(PointerButton.Left))
                    mouseDownPosition = scenePos;
            }
            else
            {
                clearSelection = true;
                inBounds = false;
            }

            bool dragResult = false;
            if (clearSelection)
            {
                dragResult = EndDragSelection();
                if (sceneHandles.IsActive())
                {
                    sceneHandles.ClearSelection();
                    NotifyNeedsRedraw();
                }

                if (sceneAxesGUI.IsActive())
                    sceneAxesGUI.ClearSelection();
            }

            bool draggedOver = DragDrop.DragInProgress || DragDrop.DropInProgress;
            draggedOver &= IsPointerHovering && inBounds && DragDrop.Type == DragDropType.Resource;

            if (draggedOver)
            {
                if (DragDrop.DropInProgress)
                {
                    dragActive = false;
                    if (draggedSO != null)
                    {
                        Selection.SceneObject = draggedSO;
                        EditorApplication.SetSceneDirty();
                    }

                    draggedSO = null;
                }
                else
                {
                    if (!dragActive)
                    {
                        dragActive = true;

                        ResourceDragDropData dragData = (ResourceDragDropData)DragDrop.Data;

                        string[] draggedPaths = dragData.Paths;

                        for (int i = 0; i < draggedPaths.Length; i++)
                        {
                            ResourceMeta meta = ProjectLibrary.GetMeta(draggedPaths[i]);
                            if (meta != null)
                            {
                                if (meta.ResType == ResourceType.Mesh)
                                {
                                    if (!string.IsNullOrEmpty(draggedPaths[i]))
                                    {
                                        Mesh mesh = ProjectLibrary.Load<Mesh>(draggedPaths[i]);

                                        string meshName = Path.GetFileNameWithoutExtension(draggedPaths[i]);
                                        draggedSO = new SceneObject(meshName);

                                        GameObjectUndo.RecordNewSceneObject(draggedSO);
                                        Renderable renderable = draggedSO.AddComponent<Renderable>();
                                        renderable.Mesh = mesh;
                                        GameObjectUndo.ResolveDiffs();

                                        if (mesh != null)
                                            draggedSOOffset = mesh.Bounds.Box.Center;
                                        else
                                            draggedSOOffset = Vector3.Zero;
                                    }

                                    break;
                                }
                                else if (meta.ResType == ResourceType.Prefab)
                                {
                                    if (!string.IsNullOrEmpty(draggedPaths[i]))
                                    {
                                        Prefab prefab = ProjectLibrary.Load<Prefab>(draggedPaths[i]);
                                        draggedSO = UndoRedo.Instantiate(prefab, "Instantiating " + prefab.Name);

                                        if (draggedSO != null)
                                        {
                                            AABox draggedObjBounds = EditorUtility.CalculateBounds(draggedSO);
                                            draggedSOOffset = draggedObjBounds.Center;
                                        }
                                        else
                                            draggedSOOffset = Vector3.Zero;
                                    }

                                    break;
                                }
                            }
                        }
                    }

                    if (draggedSO != null)
                    {
                        if (Input.IsButtonHeld(ButtonCode.Space))
                        {
                            SnapData snapData;
                            sceneSelection.Snap(scenePos, out snapData, new SceneObject[] { draggedSO });

                            Quaternion q = Quaternion.FromToRotation(Vector3.YAxis, snapData.normal);
                            draggedSO.Position = snapData.position;
                            draggedSO.Rotation = q;
                        }
                        else
                        {
                            Ray worldRay = camera.ScreenPointToRay(scenePos);
                            draggedSO.Position = worldRay * DefaultPlacementDepth - draggedSOOffset;
                        }
                    }
                }

                return;
            }
            else
            {
                if (dragActive)
                {
                    dragActive = false;

                    if (draggedSO != null)
                    {
                        draggedSO.Destroy();
                        draggedSO = null;
                    }
                }
            }

            if ((HasContentFocus || IsPointerHovering) && AllowViewportInput)
            {
                sceneCamera.EnableInput(true);

                if (inBounds && HasContentFocus)
                {
                    if (Input.IsPointerButtonDown(PointerButton.Left))
                    {
                        Rect2I sceneAxesGUIBounds = new Rect2I(Width - HandleAxesGUISize - HandleAxesGUIPaddingX,
                            HandleAxesGUIPaddingY, HandleAxesGUISize, HandleAxesGUISize);

                        if (sceneAxesGUIBounds.Contains(scenePos))
                            sceneAxesGUI.TrySelect(scenePos);
                        else
                        {
                            if(sceneHandles.TrySelect(scenePos))
                                NotifyNeedsRedraw();
                        }
                    }
                    else if (Input.IsPointerButtonHeld(PointerButton.Left) && !handleActive && !dragActive &&
                             draggedSO == null && scenePos != mouseDownPosition)
                    {
                        if (isDraggingSelection)
                            UpdateDragSelection(scenePos);
                        else
                            StartDragSelection(scenePos);
                    }
                    else if (Input.IsPointerButtonUp(PointerButton.Left))
                    {
                        if (!handleActive && !dragActive && !dragResult)
                        {
                            bool ctrlHeld = Input.IsButtonHeld(ButtonCode.LeftControl) ||
                                            Input.IsButtonHeld(ButtonCode.RightControl);

                            sceneSelection.PickObject(scenePos, ctrlHeld, new SceneObject[] { draggedSO });
                        }
                    }
                }
            }
            else
                sceneCamera.EnableInput(false);

            if (AllowViewportInput)
            {
                SceneHandles.BeginInput();
                if(sceneHandles.UpdateInput(scenePos, Input.PointerDelta))
                    NotifyNeedsRedraw();
                sceneAxesGUI.UpdateInput(scenePos);
                SceneHandles.EndInput();
            }

            if(handleActive)
                NotifyNeedsRedraw();

            sceneHandles.Draw();
            sceneAxesGUI.Draw();

            // Must be done after handle input is processed, in order to reflect most recent transform
            sceneGizmos.Draw();
            sceneSelection.Draw();

            UpdateGridMode();

            if (cameraUpdateCount != sceneCamera.UpdateCount)
            {
                sceneAxesGUI.NotifyNeedsRedraw();
                cameraUpdateCount = sceneCamera.UpdateCount;
            }

            if (VirtualInput.IsButtonDown(frameKey))
                sceneCamera.FrameSelected();
        }

        /// <inheritdoc/>
        protected override void WindowResized(int width, int height)
        {
            UpdateRenderTexture(width, height - HeaderHeight);

            base.WindowResized(width, height);
        }

        /// <inheritdoc/>
        protected override void FocusChanged(bool inFocus)
        {
            if (!inFocus)
            {
                sceneHandles.ClearSelection();
                NotifyNeedsRedraw();
            }
        }

        /// <summary>
        /// Triggered when one of the scene tool buttons is clicked, changing the active scene handle.
        /// </summary>
        /// <param name="tool">Clicked scene tool to activate.</param>
        private void OnSceneToolButtonClicked(SceneViewTool tool)
        {
            EditorApplication.ActiveSceneTool = tool;
            editorSettingsHash = EditorSettings.Hash;
        }

        /// <summary>
        /// Triggered when one of the coordinate mode buttons is clicked, changing the active coordinate mode.
        /// </summary>
        /// <param name="mode">Clicked coordinate mode to activate.</param>
        private void OnCoordinateModeButtonClicked(HandleCoordinateMode mode)
        {
            EditorApplication.ActiveCoordinateMode = mode;
            editorSettingsHash = EditorSettings.Hash;
        }

        /// <summary>
        /// Triggered when one of the pivot buttons is clicked, changing the active pivot mode.
        /// </summary>
        /// <param name="mode">Clicked pivot mode to activate.</param>
        private void OnPivotModeButtonClicked(HandlePivotMode mode)
        {
            EditorApplication.ActivePivotMode = mode;
            editorSettingsHash = EditorSettings.Hash;
        }

        /// <summary>
        /// Triggered when the move snap button is toggled.
        /// </summary>
        /// <param name="active">Determins should be move snap be activated or deactivated.</param>
        private void OnMoveSnapToggled(bool active)
        {
            Handles.MoveHandleSnapActive = active;
            editorSettingsHash = EditorSettings.Hash;
        }

        /// <summary>
        /// Triggered when the move snap increment value changes.
        /// </summary>
        /// <param name="value">Value that determines in what increments to perform move snapping.</param>
        private void OnMoveSnapValueChanged(float value)
        {
            Handles.MoveSnapAmount = MathEx.Clamp(value, 0.01f, 1000.0f);
            editorSettingsHash = EditorSettings.Hash;
        }

        /// <summary>
        /// Triggered when the rotate snap button is toggled.
        /// </summary>
        /// <param name="active">Determins should be rotate snap be activated or deactivated.</param>
        private void OnRotateSnapToggled(bool active)
        {
            Handles.RotateHandleSnapActive = active;
            editorSettingsHash = EditorSettings.Hash;
        }

        /// <summary>
        /// Triggered when the rotate snap increment value changes.
        /// </summary>
        /// <param name="value">Value that determines in what increments to perform rotate snapping.</param>
        private void OnRotateSnapValueChanged(float value)
        {
            Handles.RotateSnapAmount = (Degree)MathEx.Clamp(value, 0.01f, 360.0f);
            editorSettingsHash = EditorSettings.Hash;
        }

        /// <summary>
        /// Updates toggle button states according to current editor options. This is useful if tools, coordinate mode,
        /// pivot or other scene view options have been modified externally.
        /// </summary>
        private void UpdateButtonStates()
        {
            switch (EditorApplication.ActiveSceneTool)
            {
                case SceneViewTool.View:
                    viewButton.Value = true;
                    break;
                case SceneViewTool.Move:
                    moveButton.Value = true;
                    break;
                case SceneViewTool.Rotate:
                    rotateButton.Value = true;
                    break;
                case SceneViewTool.Scale:
                    scaleButton.Value = true;
                    break;
            }

            switch (EditorApplication.ActiveCoordinateMode)
            {
                case HandleCoordinateMode.Local:
                    localCoordButton.Value = true;
                    break;
                case HandleCoordinateMode.World:
                    worldCoordButton.Value = true;
                    break;
            }

            switch (EditorApplication.ActivePivotMode)
            {
                case HandlePivotMode.Center:
                    centerButton.Value = true;
                    break;
                case HandlePivotMode.Pivot:
                    pivotButton.Value = true;
                    break;
            }

            if (Handles.MoveHandleSnapActive)
                moveSnapButton.Value = true;
            else
                moveSnapButton.Value = false;

            moveSnapInput.Value = Handles.MoveSnapAmount;

            if (Handles.RotateHandleSnapActive)
                rotateSnapButton.Value = true;
            else
                rotateSnapButton.Value = false;

            moveSnapInput.Value = Handles.RotateSnapAmount.Degrees;
        }

        /// <summary>
        /// Creates the scene camera and updates the render texture. Should be called at least once before using the
        /// scene view. Should be called whenever the window is resized.
        /// </summary>
        /// <param name="width">Width of the scene render target, in pixels.</param>
        /// <param name="height">Height of the scene render target, in pixels.</param>
        private void UpdateRenderTexture(int width, int height)
        {
            width = MathEx.Max(20, width);
            height = MathEx.Max(20, height);

            // Note: Depth buffer and readable flags are required because ScenePicking uses it
            Texture colorTex = Texture.Create2D(width, height, PixelFormat.RGBA8, TextureUsage.Render | TextureUsage.CPUReadable);
            Texture depthTex = Texture.Create2D(width, height, PixelFormat.D32_S8X24, TextureUsage.DepthStencil | TextureUsage.CPUReadable);

            renderTexture = new RenderTexture(colorTex, depthTex);
            renderTexture.Priority = 1;

            if (camera == null)
            {
                SceneObject sceneCameraSO = new SceneObject("SceneCamera", true);
                camera = sceneCameraSO.AddComponent<Camera>();
                camera.Viewport.Target = renderTexture;
                camera.Viewport.Area = new Rect2(0.0f, 0.0f, 1.0f, 1.0f);

                Vector3 camPosition = new Vector3(0.0f, 1.7f, 5.0f);
                object camPosObj = ProjectSettings.GetObject<object>(CameraPositionKey);
                if (camPosObj is Vector3)
                    camPosition = (Vector3)camPosObj;

                Quaternion camRotation = Quaternion.Identity;
                object camRotObj = ProjectSettings.GetObject<object>(CameraRotationKey);
                if (camRotObj is Quaternion)
                    camRotation = (Quaternion)camRotObj;

                sceneCameraSO.Position = camPosition;
                sceneCameraSO.Rotation = camRotation;

                camera.Priority = 2;
                camera.Viewport.ClearColor = ClearColor;
                camera.Layers = UInt64.MaxValue & ~SceneAxesHandle.LAYER; // Don't draw scene axes in this camera
                camera.Flags = CameraFlag.OnDemand;

                sceneCamera = sceneCameraSO.AddComponent<SceneCamera>();

                renderTextureGUI = new GUIRenderTexture(renderTexture);
                rtPanel.AddElement(renderTextureGUI);

                sceneGrid = new SceneGrid(camera);
                sceneSelection = new SceneSelection(camera, gizmoDrawSettings);
                sceneGizmos = new SceneGizmos(camera, gizmoDrawSettings);
                sceneHandles = new SceneHandles(this, camera);
            }
            else
            {
                camera.Viewport.Target = renderTexture;
                renderTextureGUI.RenderTexture = renderTexture;
            }

            Rect2I rtBounds = new Rect2I(0, 0, width, height);
            renderTextureGUI.Bounds = rtBounds;
            focusCatcher.Bounds = GUIUtility.CalculateBounds(rtPanel, GUI);

            sceneAxesGUI.SetPosition(width - HandleAxesGUISize - HandleAxesGUIPaddingX, HandleAxesGUIPaddingY);

            // TODO - Consider only doing the resize once user stops resizing the widget in order to reduce constant
            // render target destroy/create cycle for every single pixel.

            camera.AspectRatio = width / (float)height;
            NotifyNeedsRedraw();
        }

        /// <summary>
        /// Notifies the system that the 3D viewport should be redrawn.
        /// </summary>
        internal void NotifyNeedsRedraw()
        {
            sceneCamera.NotifyNeedsRedraw();
            sceneAxesGUI.NotifyNeedsRedraw();
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
            sceneCamera.ToggleOnDemandDrawing(enabled);
            sceneAxesGUI.ToggleOnDemandDrawing(enabled);
        }

        /// <summary>
        /// Set up preview boxes for the currently selected cameras.
        /// </summary>
        private void UpdateCameraPreviews()
        {
            SceneObject[] selectedObjects = Selection.SceneObjects;

            // Hide unpinned camera previews which are not selected
            for (int i = 0; i < cameraPreviews.Count; i++)
            {
                var cameraPreview = cameraPreviews[i];

                // Remove preview for destroyed cameras
                if (cameraPreview.Camera == null || cameraPreview.Camera.SceneObject == null
                    || /*Temporary hack until we can render preview non-main cameras properly*/ !cameraPreview.Camera.Main)
                {
                    HideCameraPreview(cameraPreview.Camera);
                    continue;
                }

                if (cameraPreview.IsPinned)
                    continue;

                bool cameraExists = Array.Exists(selectedObjects, x => x == cameraPreview.Camera.SceneObject);
                if (!cameraExists)
                    HideCameraPreview(cameraPreview.Camera);
            }

            // Create preview for selected cameras
            for (int i = 0; i < selectedObjects.Length; i++)
            {
                if (cameraPreviews.Count >= MaxCameraPreviews)
                    break;

                var selectedCamera = selectedObjects[i].GetComponent<Camera>();
                if (selectedCamera != null && /*Temporary hack until we can render preview non-main cameras properly*/ selectedCamera.Main)
                {
                    var cameraPreview = cameraPreviews.Find(x => x.Camera == selectedCamera);
                    if (cameraPreview == null)
                        cameraPreviews.Add(new CameraPreview(selectedCamera, cameraPreviewsPanel));
                }
            }

            // Update preview box placement
            int previewsCount = cameraPreviews.Count;
            if (previewsCount > 0)
            {
                const float PreviewBoxAspect = 16.0f / 9.0f;
                const float PreviewBoxMinHeight = 80;
                const float PreviewBoxMinWidth = PreviewBoxMinHeight * PreviewBoxAspect;
                const float PreviewBoxMaxHeight = 150;
                const float PreviewBoxMaxWidth = PreviewBoxMaxHeight * PreviewBoxAspect;
                const float PreviewBoxViewSpaceMaxPercentage = 0.9f;
                const float PreviewBoxMarigin = 10;

                Rect2I rtBounds = renderTextureGUI.VisibleBounds;
                Vector2 rtSize = new Vector2(rtBounds.width, rtBounds.height);
                Vector2 usableSize = rtSize * PreviewBoxViewSpaceMaxPercentage - PreviewBoxMarigin;
                Vector2 previewSize = usableSize / previewsCount - previewsCount * PreviewBoxMarigin;
                float previewWidth = MathEx.Clamp(previewSize.x, PreviewBoxMinWidth, PreviewBoxMaxWidth);
                float previewHeight = previewWidth / PreviewBoxAspect;
                previewSize = new Vector2(previewWidth, previewHeight);

                int countPerRow = MathEx.FloorToInt(usableSize.x / previewSize.x);
                int countPerColumn = MathEx.FloorToInt(usableSize.y / previewSize.y);
                int index = 0;

                for (int y = 1; y <= countPerColumn; y++)
                {
                    for (int x = 1; x <= countPerRow; x++)
                    {
                        if (index == previewsCount)
                            break;

                        var pos = rtSize - (previewSize + PreviewBoxMarigin) * new Vector2(x, y);
                        var cameraPreview = cameraPreviews[index++];
                        cameraPreview.ShowPreview(cameraPreview.Camera, 
                            new Rect2I((int)pos.x, (int)pos.y, (int)previewSize.x, (int)previewSize.y));
                    }

                    if (index == previewsCount)
                        break;
                }
            }
        }

        /// <summary>
        /// Hides the preview box for the provided camera, if one exists.
        /// </summary>
        /// <param name="camera">Camera whose preview box to hide.</param>
        private void HideCameraPreview(Camera camera)
        {
            var cameraPreview = cameraPreviews.Find(x => x.Camera == camera);
            if (cameraPreview != null)
            {
                cameraPreviews.Remove(cameraPreview);
                cameraPreview.Destroy();
            }
        }

        /// <summary>
        /// Hides all the camera preview boxes.
        /// </summary>
        private void HideAllCameraPreviews()
        {
            foreach(var entry in cameraPreviews)
                entry.Destroy();

            cameraPreviews.Clear();
        }

        /// <summary>
        /// Parses an array of scene objects and removes elements that are children of elements that are also in the array.
        /// </summary>
        /// <param name="objects">Array containing duplicate objects as input, and array without duplicate objects as
        ///                       output.</param>
        private void CleanDuplicates(ref SceneObject[] objects)
        {
            List<SceneObject> cleanList = new List<SceneObject>();
            for (int i = 0; i < objects.Length; i++)
            {
                bool foundParent = false;
                for (int j = 0; j < objects.Length; j++)
                {
                    SceneObject elem = objects[i];

                    while (elem != null && elem != objects[j])
                        elem = elem.Parent;

                    bool isChildOf = elem == objects[j];

                    if (i != j && isChildOf)
                    {
                        foundParent = true;
                        break;
                    }
                }

                if (!foundParent)
                    cleanList.Add(objects[i]);
            }

            objects = cleanList.ToArray();
        }

        /// <summary>
        /// Checks if the load progress bar needs to be shown, shows/hides it and updates the progress accordingly.
        /// </summary>
        private void UpdateLoadingProgress()
        {
            bool needsProgress = EditorApplication.IsSceneLoading;

            if (needsProgress && !loadingProgressShown)
            {
                progressLayout.Active = true;
                rtPanel.Active = false;
                sceneAxesPanel.Active = false;

                loadingProgressShown = true;
            }
            else if (!needsProgress && loadingProgressShown)
            {
                progressLayout.Active = false;
                rtPanel.Active = true;
                sceneAxesPanel.Active = true;

                loadingProgressShown = false;
            }

            if (needsProgress)
                loadProgressBar.Percent = EditorApplication.SceneLoadProgress;
        }

        /// <summary>
        /// Starts a drag operation that displays a selection outline allowing the user to select multiple entries at once.
        /// </summary>
        /// <param name="scenePos">Coordinates relative to the scene where the drag originated.</param>
        private void StartDragSelection(Vector2I scenePos)
        {
            isDraggingSelection = true;
            dragSelectionStart = scenePos;
            dragSelectionEnd = dragSelectionStart;
        }

        /// <summary>
        /// Updates a selection outline drag operation by expanding the outline to the new location. Elements in the outline
        /// are selected.
        /// </summary>
        /// <param name="scenePos">Coordinates of the pointer relative to the scene.</param>
        /// <returns>True if the selection outline drag is valid and was updated, false otherwise.</returns>
        private bool UpdateDragSelection(Vector2I scenePos)
        {
            if (!isDraggingSelection)
                return false;

            if (dragSelection == null)
            {
                dragSelection = new GUITexture(null, true, EditorStylesInternal.SelectionArea);
                selectionPanel.AddElement(dragSelection);
            }

            dragSelectionEnd = scenePos;

            Rect2I selectionArea = new Rect2I();

            Vector2I min = new Vector2I(Math.Min(dragSelectionStart.x, dragSelectionEnd.x), Math.Min(dragSelectionStart.y, dragSelectionEnd.y));
            Vector2I max = new Vector2I(Math.Max(dragSelectionStart.x, dragSelectionEnd.x), Math.Max(dragSelectionStart.y, dragSelectionEnd.y));
            selectionArea.x = min.x;
            selectionArea.y = min.y;
            selectionArea.width = Math.Max(max.x - min.x, 1);
            selectionArea.height = Math.Max(max.y - min.y, 1);

            dragSelection.Bounds = selectionArea;

            return true;
        }

        /// <summary>
        /// Ends the selection outline drag operation. Elements in the outline are selected.
        /// </summary>
        /// <returns>True if the selection outline drag is valid and was ended, false otherwise.</returns>
        private bool EndDragSelection()
        {
            if (!isDraggingSelection)
                return false;

            if (dragSelection != null)
            {
                dragSelection.Destroy();
                dragSelection = null;
            }

            if ((dragSelectionEnd - dragSelectionStart).Length < 1)
            {
                isDraggingSelection = false;
                return false;
            }

            Vector2I min = new Vector2I(Math.Min(dragSelectionStart.x, dragSelectionEnd.x),
                    Math.Min(dragSelectionStart.y, dragSelectionEnd.y));
            Vector2I max = new Vector2I(Math.Max(dragSelectionStart.x, dragSelectionEnd.x),
                Math.Max(dragSelectionStart.y, dragSelectionEnd.y));
            sceneSelection.PickObjects(min, max - min,
                Input.IsButtonHeld(ButtonCode.LeftControl) || Input.IsButtonHeld(ButtonCode.RightControl));
            isDraggingSelection = false;
            return true;
        }

        /// <summary>
        /// Contains information about world transform of a single scene object.
        /// </summary>
        struct Transform
        {
            public Transform(SceneObject so)
            {
                position = so.Position;
                rotation = so.Rotation;
                scale = so.Scale;
            }

            /// <summary>
            /// Applies the saved transform to the specified scene object. The transform is assumed to be in world space.
            /// </summary>
            /// <param name="so">Scene object to apply the transform to.</param>
            public void Apply(SceneObject so)
            {
                so.Position = position;
                so.Rotation = rotation;
                so.LocalScale = scale;
            }

            public Vector3 position;
            public Quaternion rotation;
            public Vector3 scale;
        }
    }

    /** @} */
}
