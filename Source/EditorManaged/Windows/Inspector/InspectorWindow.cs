//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using System.IO;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a <see cref="SceneObject"/> or for a <see cref="Resource"/>. Scene object's transform values
    /// are displayed, along with all their components and their fields.
    /// </summary>
    internal sealed class InspectorWindow : EditorWindow
    {
        /// <summary>
        /// Type of objects displayed in the window.
        /// </summary>
        private enum InspectorType
        {
            SceneObject,
            Resource,
            Multiple,
            None
        }

        /// <summary>
        /// Inspector GUI elements for a single <see cref="Component"/> in a <see cref="SceneObject"/>.
        /// </summary>
        private class InspectorComponent
        {
            public GUIToggle foldout;
            public GUIButton removeBtn;
            public GUILayout title;
            public GUIPanel panel;
            public Inspector inspector;
            public UUID uuid;
            public bool folded;
        }

        /// <summary>
        /// Inspector GUI elements for a <see cref="Resource"/>
        /// </summary>
        private class InspectorResource
        {
            public GUIPanel mainPanel;
            public GUIPanel previewPanel;
            public Inspector inspector;
        }

        private static readonly Color HIGHLIGHT_COLOR = new Color(1.0f, 1.0f, 1.0f, 0.5f);
        private const int RESOURCE_TITLE_HEIGHT = 30;
        private const int COMPONENT_SPACING = 10;
        private const int PADDING = 5;

        private List<InspectorComponent> inspectorComponents = new List<InspectorComponent>();
        private InspectorPersistentData persistentData;
        private InspectorResource inspectorResource;
        private GUIScrollArea inspectorScrollArea;
        private GUILayout inspectorLayout;
        private GUIPanel highlightPanel;
        private GUITexture scrollAreaHighlight;

        private SceneObject activeSO;
        private InspectableState modifyState;
        private GUITextBox soNameInput;
        private GUIToggle soActiveToggle;
        private GUIEnumField soMobility;
        private GUILayout soPrefabLayout;
        private bool soHasPrefab;
        private GUIVector3Field soPos;
        private GUIVector3Field soRot;
        private GUIVector3Field soScale;

        private Quaternion lastRotation;

        private Rect2I[] dropAreas = new Rect2I[0];

        private InspectorType currentType = InspectorType.None;
        private string activeResourcePath;
        private bool resourceInspectorInitialized;

        /// <summary>
        /// Opens the inspector window from the menu bar.
        /// </summary>
        [MenuItem("Windows/Inspector", ButtonModifier.CtrlAlt, ButtonCode.I, 6000)]
        private static void OpenInspectorWindow()
        {
            OpenWindow<InspectorWindow>();
        }

        /// <summary>
        /// Name of the inspector window to display on the window title.
        /// </summary>
        /// <returns>Name of the inspector window to display on the window title.</returns>
        protected override LocString GetDisplayName()
        {
            return new LocEdString("Inspector");
        }

        /// <summary>
        /// Sets a resource whose GUI is to be displayed in the inspector. Clears any previous contents of the window.
        /// </summary>
        /// <param name="resourcePath">Resource path relative to the project of the resource to inspect.</param>
        private void SetObjectToInspect(string resourcePath)
        {
            activeResourcePath = resourcePath;
            resourceInspectorInitialized = false;

            if (!ProjectLibrary.Exists(resourcePath))
                return;

            ResourceMeta meta = ProjectLibrary.GetMeta(resourcePath);
            if (meta == null)
                return;

            Type resourceType = meta.Type;

            currentType = InspectorType.Resource;
            resourceInspectorInitialized = true;

            inspectorScrollArea = new GUIScrollArea(ScrollBarType.ShowIfDoesntFit, ScrollBarType.NeverShow);
            GUI.AddElement(inspectorScrollArea);
            inspectorLayout = inspectorScrollArea.Layout;

            GUIPanel titlePanel = inspectorLayout.AddPanel();
            titlePanel.SetHeight(RESOURCE_TITLE_HEIGHT);

            GUILayoutY titleLayout = titlePanel.AddLayoutY();
            titleLayout.SetPosition(PADDING, PADDING);

            string name = Path.GetFileNameWithoutExtension(resourcePath);
            string type = resourceType.Name;

            LocString title = new LocEdString(name + " (" + type + ")");
            GUILabel titleLabel = new GUILabel(title);

            titleLayout.AddFlexibleSpace();
            GUILayoutX titleLabelLayout = titleLayout.AddLayoutX();
            titleLabelLayout.AddElement(titleLabel);
            titleLayout.AddFlexibleSpace();

            GUIPanel titleBgPanel = titlePanel.AddPanel(1);

            GUITexture titleBg = new GUITexture(null, EditorStylesInternal.InspectorTitleBg);
            titleBgPanel.AddElement(titleBg);

            inspectorLayout.AddSpace(COMPONENT_SPACING);

            inspectorResource = new InspectorResource();
            inspectorResource.mainPanel = inspectorLayout.AddPanel();
            inspectorLayout.AddFlexibleSpace();
            inspectorResource.previewPanel = inspectorLayout.AddPanel();

            var persistentProperties = persistentData.GetProperties(meta.UUID.ToString());

            inspectorResource.inspector = InspectorUtility.GetInspector(resourceType);
            inspectorResource.inspector.Initialize(inspectorResource.mainPanel, inspectorResource.previewPanel,
                activeResourcePath, persistentProperties);
        }

        /// <summary>
        /// Sets a scene object whose GUI is to be displayed in the inspector. Clears any previous contents of the window.
        /// </summary>
        /// <param name="so">Scene object to inspect.</param>
        private void SetObjectToInspect(SceneObject so)
        {
            if (so == null)
                return;

            currentType = InspectorType.SceneObject;
            activeSO = so;

            inspectorScrollArea = new GUIScrollArea(ScrollBarType.ShowIfDoesntFit, ScrollBarType.NeverShow);
            scrollAreaHighlight = new GUITexture(Builtin.WhiteTexture);
            scrollAreaHighlight.SetTint(HIGHLIGHT_COLOR);
            scrollAreaHighlight.Active = false;

            GUI.AddElement(inspectorScrollArea);
            GUIPanel inspectorPanel = inspectorScrollArea.Layout.AddPanel();
            inspectorLayout = inspectorPanel.AddLayoutY();
            highlightPanel = inspectorPanel.AddPanel(-1);
            highlightPanel.AddElement(scrollAreaHighlight);

            // SceneObject fields
            CreateSceneObjectFields();
            RefreshSceneObjectFields(true);

            // Components
            Component[] allComponents = so.GetComponents();
            for (int i = 0; i < allComponents.Length; i++)
            {
                inspectorLayout.AddSpace(COMPONENT_SPACING);

                InspectorComponent data = new InspectorComponent();
                data.uuid = allComponents[i].UUID;
                data.folded = false;

                data.foldout = new GUIToggle(allComponents[i].GetType().Name, EditorStyles.Foldout);
                data.foldout.AcceptsKeyFocus = false;

                SpriteTexture xBtnIcon = EditorBuiltin.GetEditorIcon(EditorIcon.X);
                data.removeBtn = new GUIButton(new GUIContent(xBtnIcon), GUIOption.FixedWidth(30));

                data.title = inspectorLayout.AddLayoutX();
                data.title.AddElement(data.foldout);
                data.title.AddElement(data.removeBtn);
                data.panel = inspectorLayout.AddPanel();

                var persistentProperties = persistentData.GetProperties(allComponents[i].InstanceId);

                data.inspector = InspectorUtility.GetInspector(allComponents[i].GetType());
                data.inspector.Initialize(data.panel, allComponents[i], persistentProperties);

                bool isExpanded = data.inspector.Persistent.GetBool(data.uuid + "_Expanded", true);
                data.foldout.Value = isExpanded;

                if (!isExpanded)
                    data.inspector.SetVisible(false);

                Type curComponentType = allComponents[i].GetType();
                data.foldout.OnToggled += (bool expanded) => OnComponentFoldoutToggled(data, expanded);
                data.removeBtn.OnClick += () => OnComponentRemoveClicked(curComponentType);

                inspectorComponents.Add(data);
            }

            inspectorLayout.AddFlexibleSpace();

            UpdateDropAreas();
        }

        /// <summary>
        /// Creates GUI elements required for displaying <see cref="SceneObject"/> fields like name, prefab data and 
        /// transform (position, rotation, scale). Assumes that necessary inspector scroll area layout has already been 
        /// created.
        /// </summary>
        private void CreateSceneObjectFields()
        {
            GUIPanel sceneObjectPanel = inspectorLayout.AddPanel();
            sceneObjectPanel.SetHeight(GetTitleBounds().height);

            GUILayoutY sceneObjectLayout = sceneObjectPanel.AddLayoutY();
            sceneObjectLayout.SetPosition(PADDING, PADDING);

            GUIPanel sceneObjectBgPanel = sceneObjectPanel.AddPanel(1);

            GUILayoutX nameLayout = sceneObjectLayout.AddLayoutX();
            soActiveToggle = new GUIToggle("");
            soActiveToggle.OnToggled += OnSceneObjectActiveStateToggled;
            GUILabel nameLbl = new GUILabel(new LocEdString("Name"), GUIOption.FixedWidth(50));
            soNameInput = new GUITextBox(false, GUIOption.FlexibleWidth(180));
            soNameInput.Text = activeSO.Name;
            soNameInput.OnChanged += OnSceneObjectRename;
            soNameInput.OnConfirmed += () =>
            {
                OnModifyConfirm();
                StartUndo("name");
            };
            soNameInput.OnFocusGained += () => StartUndo("name");
            soNameInput.OnFocusLost += OnModifyConfirm;

            nameLayout.AddElement(soActiveToggle);
            nameLayout.AddSpace(3);
            nameLayout.AddElement(nameLbl);
            nameLayout.AddElement(soNameInput);
            nameLayout.AddFlexibleSpace();

            GUILayoutX mobilityLayout = sceneObjectLayout.AddLayoutX();
            GUILabel mobilityLbl = new GUILabel(new LocEdString("Mobility"), GUIOption.FixedWidth(50));
            soMobility = new GUIEnumField(typeof(ObjectMobility), "", 0, GUIOption.FixedWidth(85));
            soMobility.Value = (ulong)activeSO.Mobility;
            soMobility.OnSelectionChanged += value => activeSO.Mobility = (ObjectMobility) value;
            mobilityLayout.AddElement(mobilityLbl);
            mobilityLayout.AddElement(soMobility);

            soPrefabLayout = sceneObjectLayout.AddLayoutX();

            soPos = new GUIVector3Field(new LocEdString("Position"), 50);
            sceneObjectLayout.AddElement(soPos);

            soPos.OnComponentChanged += OnPositionChanged;
            soPos.OnConfirm += x =>
            {
                OnModifyConfirm();
                StartUndo("position." + x.ToString());
            };
            soPos.OnComponentFocusChanged += (focus, comp) =>
            {
                if (focus)
                    StartUndo("position." + comp.ToString());
                else
                    OnModifyConfirm();
            };

            soRot = new GUIVector3Field(new LocEdString("Rotation"), 50);
            sceneObjectLayout.AddElement(soRot);

            soRot.OnComponentChanged += OnRotationChanged;
            soRot.OnConfirm += x =>
            {
                OnModifyConfirm();
                StartUndo("rotation." + x.ToString());
            };
            soRot.OnComponentFocusChanged += (focus, comp) =>
            {
                if (focus)
                    StartUndo("rotation." + comp.ToString());
                else
                    OnModifyConfirm();
            };

            soScale = new GUIVector3Field(new LocEdString("Scale"), 50);
            sceneObjectLayout.AddElement(soScale);

            soScale.OnComponentChanged += OnScaleChanged;
            soScale.OnConfirm += x =>
            {
                OnModifyConfirm();
                StartUndo("scale." + x.ToString());
            };
            soScale.OnComponentFocusChanged += (focus, comp) =>
            {
                if (focus)
                    StartUndo("scale." + comp.ToString());
                else
                    OnModifyConfirm();
            };

            sceneObjectLayout.AddFlexibleSpace();

            GUITexture titleBg = new GUITexture(null, EditorStylesInternal.InspectorTitleBg);
            sceneObjectBgPanel.AddElement(titleBg);
        }

        /// <summary>
        /// Updates contents of the scene object specific fields (name, position, rotation, etc.)
        /// </summary>
        /// <param name="forceUpdate">If true, the GUI elements will be updated regardless of whether a change was
        ///                           detected or not.</param>
        internal void RefreshSceneObjectFields(bool forceUpdate)
        {
            if (activeSO == null)
                return;

            soNameInput.Text = activeSO.Name;
            soActiveToggle.Value = activeSO.Active;
            soMobility.Value = (ulong) activeSO.Mobility;

            SceneObject prefabParent = PrefabUtility.GetPrefabParent(activeSO);

            // Ignore prefab parent if scene root, we only care for non-root prefab instances
            bool hasPrefab = prefabParent != null && prefabParent.Parent != null;
            if (soHasPrefab != hasPrefab || forceUpdate)
            {
                int numChildren = soPrefabLayout.ChildCount;
                for (int i = 0; i < numChildren; i++)
                    soPrefabLayout.GetChild(0).Destroy();

                GUILabel prefabLabel =new GUILabel(new LocEdString("Prefab"), GUIOption.FixedWidth(50));
                soPrefabLayout.AddElement(prefabLabel);

                if (hasPrefab)
                {
                    GUIButton btnApplyPrefab = new GUIButton(new LocEdString("Apply"), GUIOption.FixedWidth(60));
                    GUIButton btnRevertPrefab = new GUIButton(new LocEdString("Revert"), GUIOption.FixedWidth(60));
                    GUIButton btnBreakPrefab = new GUIButton(new LocEdString("Break"), GUIOption.FixedWidth(60));

                    btnApplyPrefab.OnClick += () =>
                    {
                        PrefabUtility.ApplyPrefab(activeSO);
                    };
                    btnRevertPrefab.OnClick += () =>
                    {
                        GameObjectUndo.RecordSceneObject(activeSO, true, "Reverting \"" + activeSO.Name + "\" to prefab.");
                        PrefabUtility.RevertPrefab(activeSO);

                        GameObjectUndo.ResolveDiffs();
                        EditorApplication.SetSceneDirty();
                    };
                    btnBreakPrefab.OnClick += () =>
                    {
                        UndoRedo.BreakPrefab(activeSO, "Breaking prefab link for " + activeSO.Name);

                        EditorApplication.SetSceneDirty();
                    };

                    soPrefabLayout.AddElement(btnApplyPrefab);
                    soPrefabLayout.AddElement(btnRevertPrefab);
                    soPrefabLayout.AddElement(btnBreakPrefab);
                }
                else
                {
                    GUILabel noPrefabLabel = new GUILabel("None");
                    soPrefabLayout.AddElement(noPrefabLabel);
                }

                soHasPrefab = hasPrefab;
            }

            Vector3 position;
            Quaternion rotation;
            if (EditorApplication.ActiveCoordinateMode == HandleCoordinateMode.World)
            {
                position = activeSO.Position;
                rotation = activeSO.Rotation;
            }
            else
            {
                position = activeSO.LocalPosition;
                rotation = activeSO.LocalRotation;
            }

            Vector3 scale = activeSO.LocalScale;

            if (!soPos.HasInputFocus || forceUpdate)
                soPos.Value = position;

            // Avoid updating the rotation unless actually changed externally, since switching back and forth between
            // quaternion and euler angles can cause weird behavior
            if ((!soRot.HasInputFocus && rotation != lastRotation) || forceUpdate)
            {
                soRot.Value = rotation.ToEuler();
                lastRotation = rotation;
            }

            if (!soScale.HasInputFocus || forceUpdate)
                soScale.Value = scale;
        }

        /// <summary>
        /// Forces all the GUI fields for the specified component to update their values from the current component state.
        /// </summary>
        /// <param name="component">Component for whose GUI elements to perform the refresh on.</param>
        internal void RefreshComponentFields(Component component)
        {
            if (component == null)
                return;

            foreach (var entry in inspectorComponents)
            {
                if (entry.uuid == component.UUID)
                    entry.inspector.Refresh(true);
            }
        }

        private void OnInitialize()
        {
            Selection.OnSelectionChanged += OnSelectionChanged;
            ProjectLibrary.OnEntryImported += OnResourceImported;

            const string soName = "InspectorPersistentData";
            SceneObject so = Scene.Root.FindChild(soName);
            if (so == null)
                so = new SceneObject(soName, true);

            persistentData = so.GetComponent<InspectorPersistentData>();
            if (persistentData == null)
                persistentData = so.AddComponent<InspectorPersistentData>();

            OnSelectionChanged(new SceneObject[0], new string[0]);
        }

        private void OnDestroy()
        {
            Selection.OnSelectionChanged -= OnSelectionChanged;
            ProjectLibrary.OnEntryImported -= OnResourceImported;
        }

        private void OnEditorUpdate()
        {
            if (currentType == InspectorType.SceneObject)
            {
                Component[] allComponents = activeSO.GetComponents();
                bool requiresRebuild = allComponents.Length != inspectorComponents.Count;

                if (!requiresRebuild)
                {
                    for (int i = 0; i < inspectorComponents.Count; i++)
                    {
                        if (inspectorComponents[i].uuid != allComponents[i].UUID)
                        {
                            requiresRebuild = true;
                            break;
                        }
                    }
                }

                if (requiresRebuild)
                {
                    SceneObject so = activeSO;
                    Clear();
                    SetObjectToInspect(so);
                }
                else
                {
                    RefreshSceneObjectFields(false);

                    InspectableState componentModifyState = InspectableState.NotModified;
                    for (int i = 0; i < inspectorComponents.Count; i++)
                        componentModifyState |= inspectorComponents[i].inspector.Refresh();

                    if (componentModifyState.HasFlag(InspectableState.ModifyInProgress))
                        EditorApplication.SetSceneDirty();

                    modifyState |= componentModifyState;
                }
            }
            else if (currentType == InspectorType.Resource)
            {
                inspectorResource.inspector.Refresh();
            }

            // Detect drag and drop
            bool isValidDrag = false;

            if (activeSO != null)
            {
                if ((DragDrop.DragInProgress || DragDrop.DropInProgress) && DragDrop.Type == DragDropType.Resource)
                {
                    Vector2I windowPos = ScreenToWindowPos(Input.PointerPosition);
                    Vector2I scrollPos = windowPos;
                    Rect2I contentBounds = inspectorLayout.Bounds;
                    scrollPos.x -= contentBounds.x;
                    scrollPos.y -= contentBounds.y;

                    bool isInBounds = false;
                    Rect2I dropArea = new Rect2I();
                    foreach (var bounds in dropAreas)
                    {
                        if (bounds.Contains(scrollPos))
                        {
                            isInBounds = true;
                            dropArea = bounds;
                            break;
                        }
                    }

                    Type draggedComponentType = null;
                    if (isInBounds)
                    {
                        ResourceDragDropData dragData = DragDrop.Data as ResourceDragDropData;
                        if (dragData != null)
                        {
                            foreach (var resPath in dragData.Paths)
                            {
                                ResourceMeta meta = ProjectLibrary.GetMeta(resPath);
                                if (meta != null)
                                {
                                    if (meta.ResType == ResourceType.ScriptCode)
                                    {
                                        ScriptCode scriptFile = ProjectLibrary.Load<ScriptCode>(resPath);

                                        if (scriptFile != null)
                                        {
                                            Type[] scriptTypes = scriptFile.Types;
                                            foreach (var type in scriptTypes)
                                            {
                                                if (type.IsSubclassOf(typeof (Component)))
                                                {
                                                    draggedComponentType = type;
                                                    isValidDrag = true;
                                                    break;
                                                }
                                            }

                                            if (draggedComponentType != null)
                                                break;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (isValidDrag)
                    {
                        scrollAreaHighlight.Bounds = dropArea;

                        if (DragDrop.DropInProgress)
                        {
                            GameObjectUndo.RecordSceneObject(activeSO, false, $"Added component \"{draggedComponentType.Name}\" to \"{activeSO.Name}\"");
                            activeSO.AddComponent(draggedComponentType);

                            modifyState = InspectableState.Modified;
                            EditorApplication.SetSceneDirty();
                        }
                    }
                }               
            }

            if (scrollAreaHighlight != null)
                scrollAreaHighlight.Active = isValidDrag;
        }

        /// <summary>
        /// Triggered when the user selects a new resource or a scene object, or deselects everything.
        /// </summary>
        /// <param name="objects">A set of new scene objects that were selected.</param>
        /// <param name="paths">A set of absolute resource paths that were selected.</param>
        private void OnSelectionChanged(SceneObject[] objects, string[] paths)
        {
            Clear();
            modifyState = InspectableState.NotModified;

            if (objects.Length == 0 && paths.Length == 0)
            {
                currentType = InspectorType.None;
                inspectorScrollArea = new GUIScrollArea(ScrollBarType.ShowIfDoesntFit, ScrollBarType.NeverShow);
                GUI.AddElement(inspectorScrollArea);
                inspectorLayout = inspectorScrollArea.Layout;

                inspectorLayout.AddFlexibleSpace();
                GUILayoutX layoutMsg = inspectorLayout.AddLayoutX();
                layoutMsg.AddFlexibleSpace();
                layoutMsg.AddElement(new GUILabel(new LocEdString("No object selected")));
                layoutMsg.AddFlexibleSpace();
                inspectorLayout.AddFlexibleSpace();
            }
            else if ((objects.Length + paths.Length) > 1)
            {
                currentType = InspectorType.None;
                inspectorScrollArea = new GUIScrollArea(ScrollBarType.ShowIfDoesntFit, ScrollBarType.NeverShow);
                GUI.AddElement(inspectorScrollArea);
                inspectorLayout = inspectorScrollArea.Layout;

                inspectorLayout.AddFlexibleSpace();
                GUILayoutX layoutMsg = inspectorLayout.AddLayoutX();
                layoutMsg.AddFlexibleSpace();
                layoutMsg.AddElement(new GUILabel(new LocEdString("Multiple objects selected")));
                layoutMsg.AddFlexibleSpace();
                inspectorLayout.AddFlexibleSpace();
            }
            else if (objects.Length == 1)
            {
                if (objects[0] != null)
                    SetObjectToInspect(objects[0]);
            }
            else if (paths.Length == 1)
            {
                SetObjectToInspect(paths[0]);
            }
        }

        /// <summary>
        /// Updates the inspector if the inspected resource just got reimported.
        /// </summary>
        /// <param name="path">Path to the resource that got imported.</param>
        private void OnResourceImported(string path)
        {
            if (resourceInspectorInitialized || string.IsNullOrEmpty(activeResourcePath))
                return;

            if(path == activeResourcePath)
                SetObjectToInspect(path);
        }

        /// <summary>
        /// Triggered when the user closes or expands a component foldout, making the component fields visible or hidden.
        /// </summary>
        /// <param name="inspectorData">Contains GUI data for the component that was toggled.</param>
        /// <param name="expanded">Determines whether to display or hide component contents.</param>
        private void OnComponentFoldoutToggled(InspectorComponent inspectorData, bool expanded)
        {
            inspectorData.inspector.Persistent.SetBool(inspectorData.uuid + "_Expanded", expanded);
            inspectorData.inspector.SetVisible(expanded);
            inspectorData.folded = !expanded;

            UpdateDropAreas();
        }

        /// <summary>
        /// Triggered when the user clicks the component remove button. Removes that component from the active scene object.
        /// </summary>
        /// <param name="componentType">Type of the component to remove.</param>
        private void OnComponentRemoveClicked(Type componentType)
        {
            if (activeSO != null)
            {
                GameObjectUndo.RecordSceneObject(activeSO, false, $"Removed component \"{componentType.Name}\" from \"{activeSO.Name}\"");
                activeSO.RemoveComponent(componentType);

                modifyState = InspectableState.Modified;
                EditorApplication.SetSceneDirty();
                GameObjectUndo.ResolveDiffs();
            }
        }

        /// <summary>
        /// Destroys all inspector GUI elements.
        /// </summary>
        internal void Clear()
        {
            for (int i = 0; i < inspectorComponents.Count; i++)
            {
                inspectorComponents[i].foldout.Destroy();
                inspectorComponents[i].removeBtn.Destroy();
                inspectorComponents[i].inspector.Destroy();
            }

            inspectorComponents.Clear();

            if (inspectorResource != null)
            {
                inspectorResource.inspector.Destroy();
                inspectorResource = null;
            }

            if (inspectorScrollArea != null)
            {
                inspectorScrollArea.Destroy();
                inspectorScrollArea = null;
            }

            if (scrollAreaHighlight != null)
            {
                scrollAreaHighlight.Destroy();
                scrollAreaHighlight = null;
            }

            if (highlightPanel != null)
            {
                highlightPanel.Destroy();
                highlightPanel = null;
            }

            activeSO = null;
            soNameInput = null;
            soActiveToggle = null;
            soMobility = null;
            soPrefabLayout = null;
            soHasPrefab = false;
            soPos = null;
            soRot = null;
            soScale = null;
            dropAreas = new Rect2I[0];

            activeResourcePath = null;
            currentType = InspectorType.None;
        }

        /// <summary>
        /// Changes keyboard focus to a specific field on the component with the provided UUID.
        /// </summary>
        /// <param name="uuid">UUID of the component on which to select the field.</param>
        /// <param name="path">Path to the field on the object being inspected.</param>
        internal void FocusOnField(UUID uuid, string path)
        {
            if (activeSO == null)
                return;

            if (activeSO.UUID == uuid)
            {
                if(path == "position.X")
                    soPos.SetInputFocus(VectorComponent.X, true);
                else if(path == "position.Y")
                    soPos.SetInputFocus(VectorComponent.Y, true);
                else if(path == "position.Z")
                    soPos.SetInputFocus(VectorComponent.Z, true);
                else if(path == "rotation.X")
                    soRot.SetInputFocus(VectorComponent.X, true);
                else if(path == "rotation.Y")
                    soRot.SetInputFocus(VectorComponent.Y, true);
                else if(path == "rotation.Z")
                    soRot.SetInputFocus(VectorComponent.Z, true);
                else if(path == "scale.X")
                    soScale.SetInputFocus(VectorComponent.X, true);
                else if(path == "scale.Y")
                    soScale.SetInputFocus(VectorComponent.Y, true);
                else if(path == "scale.Z")
                    soScale.SetInputFocus(VectorComponent.Z, true);
                else if (path == "name")
                    soNameInput.Focus = true;
            }
            else
            {
                foreach (var entry in inspectorComponents)
                {
                    if (entry.uuid != uuid)
                        continue;

                    entry.inspector.FocusOnField(path);
                }
            }
        }

        /// <summary>
        /// Returns the size of the title bar area that is displayed for <see cref="SceneObject"/> specific fields.
        /// </summary>
        /// <returns>Area of the title bar, relative to the window.</returns>
        private Rect2I GetTitleBounds()
        {
            return new Rect2I(0, 0, Width, 135);
        }

        /// <summary>
        /// Triggered when the user changes the name of the currently active scene object.
        /// </summary>
        private void OnSceneObjectRename(string name)
        {
            if (activeSO != null)
            {
                activeSO.Name = name;

                modifyState |= InspectableState.ModifyInProgress;
                EditorApplication.SetSceneDirty();
            }
        }

        /// <summary>
        /// Triggered when the user changes the active state of the scene object.
        /// </summary>
        /// <param name="active">True if the object is active, false otherwise.</param>
        private void OnSceneObjectActiveStateToggled(bool active)
        {
            if (activeSO != null)
            {
                StartUndo("active");
                activeSO.Active = active;
                EndUndo();
            }
        }

        /// <summary>
        /// Triggered when the scene object modification is confirmed by the user.
        /// </summary>
        private void OnModifyConfirm()
        {
            if (modifyState.HasFlag(InspectableState.ModifyInProgress))
                modifyState = InspectableState.Modified;

            EndUndo();
        }

        /// <summary>
        /// Triggered when the position value in the currently active <see cref="SceneObject"/> changes. Updates the 
        /// necessary GUI elements.
        /// </summary>
        /// <param name="value">New value of the component that changed.</param>
        /// <param name="component">Identifier of the component that changed.</param>
        private void OnPositionChanged(float value, VectorComponent component)
        {
            if (activeSO == null)
                return;

            if (EditorApplication.ActiveCoordinateMode == HandleCoordinateMode.World)
                activeSO.Position = soPos.Value;
            else
                activeSO.LocalPosition = soPos.Value;

            modifyState = InspectableState.ModifyInProgress;
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Triggered when the rotation value in the currently active <see cref="SceneObject"/> changes. Updates the 
        /// necessary GUI elements.
        /// </summary>
        /// <param name="value">New value of the component that changed.</param>
        /// <param name="component">Identifier of the component that changed.</param>
        private void OnRotationChanged(float value, VectorComponent component)
        {
            if (activeSO == null)
                return;

            Quaternion rotation = Quaternion.FromEuler(soRot.Value);
            if (EditorApplication.ActiveCoordinateMode == HandleCoordinateMode.World)
                activeSO.Rotation = rotation;
            else
                activeSO.LocalRotation = rotation;

            lastRotation = rotation;
            modifyState = InspectableState.ModifyInProgress;
            EditorApplication.SetSceneDirty();
        }

        /// <summary>
        /// Triggered when the scale value in the currently active <see cref="SceneObject"/> changes. Updates the 
        /// necessary GUI elements.
        /// </summary>
        /// <param name="value">New value of the component that changed.</param>
        /// <param name="component">Identifier of the component that changed.</param>
        private void OnScaleChanged(float value, VectorComponent component)
        {
            if (activeSO == null)
                return;

            activeSO.LocalScale = soScale.Value;

            modifyState = InspectableState.ModifyInProgress;
            EditorApplication.SetSceneDirty();
        }

        /// <inheritdoc/>
        protected override void WindowResized(int width, int height)
        {
            base.WindowResized(width, height);

            UpdateDropAreas();
        }

        /// <summary>
        /// Updates drop areas used for dragging and dropping components on the inspector.
        /// </summary>
        private void UpdateDropAreas()
        {
            if (activeSO == null)
                return;

            Rect2I contentBounds = inspectorLayout.Bounds;
            dropAreas = new Rect2I[inspectorComponents.Count + 1];
            int yOffset = GetTitleBounds().height;
            for (int i = 0; i < inspectorComponents.Count; i++)
            {
                dropAreas[i] = new Rect2I(0, yOffset, contentBounds.width, COMPONENT_SPACING);
                yOffset += inspectorComponents[i].title.Bounds.height + COMPONENT_SPACING;

                if (!inspectorComponents[i].folded)
                    yOffset += inspectorComponents[i].panel.Bounds.height;
            }

            dropAreas[dropAreas.Length - 1] = new Rect2I(0, yOffset, contentBounds.width, contentBounds.height - yOffset);
        }

        /// <summary>
        /// Notifies the system to start recording a new undo command. Any changes to scene object fields after this is
        /// called will be recorded in the command. User must call <see cref="EndUndo"/> after the field is done being
        /// changed.
        /// </summary>
        /// <param name="name">Name of the field being changed.</param>
        private void StartUndo(string name)
        {
            if (activeSO != null)
                GameObjectUndo.RecordSceneObjectHeader(activeSO, name);
        }

        /// <summary>
        /// Finishes recording an undo command started via <see cref="StartUndo(string)"/>. If any changes are detected on
        /// the field an undo command is recorded onto the undo-redo stack, otherwise nothing is done.
        /// </summary>
        private void EndUndo()
        {
            GameObjectUndo.ResolveDiffs();
        }
    }

    /** @} */
}
