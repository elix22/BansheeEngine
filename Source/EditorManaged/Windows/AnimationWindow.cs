//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using bs;

namespace bs.Editor
{
    /** @addtogroup Windows
     *  @{
     */

    /// <summary>
    /// Displays animation curve editor window. Allows the user to manipulate keyframes of animation curves, add/remove
    /// curves from an animation clip, and manipulate animation events.
    /// </summary>
    [DefaultSize(900, 500), UndoRedoLocal]
    internal class AnimationWindow : EditorWindow
    {
        private const int FIELD_DISPLAY_WIDTH = 300;

        private SceneObject selectedSO;

        #region Overrides

        /// <summary>
        /// Opens the animation window.
        /// </summary>
        [MenuItem("Windows/Animation", ButtonModifier.CtrlAlt, ButtonCode.A, 6000)]
        private static void OpenGameWindow()
        {
            OpenWindow<AnimationWindow>();
        }

        /// <inheritdoc/>
        protected override LocString GetDisplayName()
        {
            return new LocEdString("Animation");
        }

        private void OnInitialize()
        {
            Selection.OnSelectionChanged += OnSelectionChanged;

            UpdateSelectedSO(true);
        }

        private void OnEditorUpdate()
        {
            if (selectedSO == null)
                return;

            guiCurveEditor.HandleDragAndZoomInput();

            if (state == State.Playback)
            {
                Animation animation = selectedSO.GetComponent<Animation>();
                if (animation != null)
                {
                    float time = animation.EditorGetTime();
                    int frameIdx = (int)(time * fps);

                    SetCurrentFrame(frameIdx);

                    animation.UpdateFloatProperties();
                }
            }
            else if (state == State.Recording)
            {
                if (!delayRecord)
                {
                    float time = guiCurveEditor.GetTimeForFrame(currentFrameIdx);
                    if (RecordState(time))
                    {
                        ApplyClipChanges();
                        guiCurveEditor.Redraw();
                    }
                }
            }

            delayRecord = false;
        }

        private void OnDestroy()
        {
            SwitchState(State.Empty);

            Selection.OnSelectionChanged -= OnSelectionChanged;

            if (selectedSO != null)
            {
                EditorInput.OnPointerPressed -= OnPointerPressed;
                EditorInput.OnPointerDoubleClick -= OnPointerDoubleClicked;
                EditorInput.OnPointerMoved -= OnPointerMoved;
                EditorInput.OnPointerReleased -= OnPointerReleased;
                EditorInput.OnButtonUp -= OnButtonUp;
            }
        }

        /// <inheritdoc/>
        protected override void WindowResized(int width, int height)
        {
            if (selectedSO == null)
                return;

            ResizeGUI(width, height);
        }
        #endregion

        #region GUI
        private GUIToggle playButton;
        private GUIToggle recordButton;

        private GUIButton prevFrameButton;
        private GUIIntField frameInputField;
        private GUIButton nextFrameButton;

        private GUIButton addKeyframeButton;
        private GUIButton addEventButton;

        private GUIButton optionsButton;

        private GUIButton addPropertyBtn;
        private GUIButton delPropertyBtn;

        private GUILayout buttonLayout;

        private int buttonLayoutHeight;

        private GUIAnimFieldDisplay guiFieldDisplay;
        private GUICurveEditor guiCurveEditor;

        /// <summary>
        /// Recreates the entire curve editor GUI depending on the currently selected scene object.
        /// </summary>
        private void RebuildGUI()
        {
            GUI.Clear();
            guiCurveEditor = null;
            guiFieldDisplay = null;

            if (selectedSO == null)
            {
                GUILabel warningLbl = new GUILabel(new LocEdString("Select an object to animate in the Hierarchy or Scene windows."));

                GUILayoutY vertLayout = GUI.AddLayoutY();
                vertLayout.AddFlexibleSpace();
                GUILayoutX horzLayout = vertLayout.AddLayoutX();
                vertLayout.AddFlexibleSpace();

                horzLayout.AddFlexibleSpace();
                horzLayout.AddElement(warningLbl);
                horzLayout.AddFlexibleSpace();

                return;
            }

            // Top button row
            GUIContent playIcon = new GUIContent(EditorBuiltin.GetAnimationWindowIcon(AnimationWindowIcon.Play),
                new LocEdString("Play"));
            GUIContent recordIcon = new GUIContent(EditorBuiltin.GetAnimationWindowIcon(AnimationWindowIcon.Record),
                new LocEdString("Record"));

            GUIContent prevFrameIcon = new GUIContent(EditorBuiltin.GetAnimationWindowIcon(AnimationWindowIcon.FrameBack), 
                new LocEdString("Previous frame"));
            GUIContent nextFrameIcon = new GUIContent(EditorBuiltin.GetAnimationWindowIcon(AnimationWindowIcon.FrameForward), 
                new LocEdString("Next frame"));

            GUIContent addKeyframeIcon = new GUIContent(EditorBuiltin.GetAnimationWindowIcon(AnimationWindowIcon.AddKeyframe), 
                new LocEdString("Add keyframe"));
            GUIContent addEventIcon = new GUIContent(EditorBuiltin.GetAnimationWindowIcon(AnimationWindowIcon.AddEvent), 
                new LocEdString("Add event"));

            GUIContent optionsIcon = new GUIContent(EditorBuiltin.GetLibraryWindowIcon(LibraryWindowIcon.Options), 
                new LocEdString("Options"));

            playButton = new GUIToggle(playIcon, EditorStyles.Button);
            recordButton = new GUIToggle(recordIcon, EditorStyles.Button);

            prevFrameButton = new GUIButton(prevFrameIcon);
            frameInputField = new GUIIntField();
            nextFrameButton = new GUIButton(nextFrameIcon);

            addKeyframeButton = new GUIButton(addKeyframeIcon);
            addEventButton = new GUIButton(addEventIcon);

            optionsButton = new GUIButton(optionsIcon);

            playButton.OnToggled += x =>
            {
                if(x)
                    SwitchState(State.Playback);
                else
                    SwitchState(State.Normal);
            };

            recordButton.OnToggled += x =>
            {
                if (x)
                    SwitchState(State.Recording);
                else
                    SwitchState(State.Normal);
            };

            prevFrameButton.OnClick += () =>
            {
                SetCurrentFrame(currentFrameIdx - 1);

                switch (state)
                {
                    case State.Recording:
                    case State.Normal:
                        PreviewFrame(currentFrameIdx);
                        break;
                    default:
                        SwitchState(State.Normal);
                        break;
                }
            };

            frameInputField.OnChanged += x =>
            {
                SetCurrentFrame(x);

                switch (state)
                {
                    case State.Recording:
                    case State.Normal:
                        PreviewFrame(currentFrameIdx);
                        break;
                    default:
                        SwitchState(State.Normal);
                        break;
                }
            };

            nextFrameButton.OnClick += () =>
            {
                SetCurrentFrame(currentFrameIdx + 1);

                switch (state)
                {
                    case State.Recording:
                    case State.Normal:
                        PreviewFrame(currentFrameIdx);
                        break;
                    default:
                        SwitchState(State.Normal);
                        break;
                }
            };

            addKeyframeButton.OnClick += () =>
            {
                SwitchState(State.Normal);
                guiCurveEditor.AddKeyFrameAtMarker();
            };

            addEventButton.OnClick += () =>
            {
                SwitchState(State.Normal);
                guiCurveEditor.AddEventAtMarker();
            };

            optionsButton.OnClick += () =>
            {
                Vector2I openPosition = ScreenToWindowPos(Input.PointerPosition);
                AnimationOptions dropDown = DropDownWindow.Open<AnimationOptions>(GUI, openPosition);
                dropDown.Initialize(this);
            };

            // Property buttons
            addPropertyBtn = new GUIButton(new LocEdString("Add property"));
            delPropertyBtn = new GUIButton(new LocEdString("Delete selected"));

            addPropertyBtn.OnClick += () =>
            {
                Action openPropertyWindow = () =>
                {
                    Vector2I windowPos = ScreenToWindowPos(Input.PointerPosition);
                    FieldSelectionWindow fieldSelection = DropDownWindow.Open<FieldSelectionWindow>(GUI, windowPos);
                    fieldSelection.OnFieldSelected += OnFieldAdded;
                };

                if (clipInfo.clip == null)
                {
                    LocEdString title = new LocEdString("Warning");
                    LocEdString message =
                        new LocEdString("Selected object doesn't have an animation clip assigned. Would you like to create" +
                                        " a new animation clip?");

                    DialogBox.Open(title, message, DialogBox.Type.YesNoCancel, type =>
                    {
                        if (type == DialogBox.ResultType.Yes)
                        {
                            string clipSavePath;
                            if (BrowseDialog.SaveFile(ProjectLibrary.ResourceFolder, "*.asset", out clipSavePath))
                            {
                                SwitchState(State.Empty);
                                clipSavePath = Path.ChangeExtension(clipSavePath, ".asset");

                                AnimationClip newClip = new AnimationClip();

                                ProjectLibrary.Create(newClip, clipSavePath);
                                LoadAnimClip(newClip);

                                Animation animation = selectedSO.GetComponent<Animation>();
                                if (animation == null)
                                    animation = selectedSO.AddComponent<Animation>();

                                animation.DefaultClip = newClip;
                                EditorApplication.SetSceneDirty();

                                SwitchState(State.Normal);
                                openPropertyWindow();
                            }
                        }
                    });
                }
                else
                {
                    if (clipInfo.isImported)
                    {
                        LocEdString title = new LocEdString("Warning");
                        LocEdString message =
                            new LocEdString("You cannot add/edit/remove curves from animation clips that" +
                                            " are imported from an external file.");

                        DialogBox.Open(title, message, DialogBox.Type.OK);
                    }
                    else
                    {
                        SwitchState(State.Normal);
                        openPropertyWindow();
                    }
                }
            };

            delPropertyBtn.OnClick += () =>
            {
                if (clipInfo.clip == null)
                    return;

                SwitchState(State.Normal);

                if (clipInfo.isImported)
                {
                    LocEdString title = new LocEdString("Warning");
                    LocEdString message =
                        new LocEdString("You cannot add/edit/remove curves from animation clips that" +
                                        " are imported from an external file.");

                    DialogBox.Open(title, message, DialogBox.Type.OK);
                }
                else
                {
                    LocEdString title = new LocEdString("Warning");
                    LocEdString message = new LocEdString("Are you sure you want to remove all selected fields?");

                    DialogBox.Open(title, message, DialogBox.Type.YesNo, x =>
                    {
                        if (x == DialogBox.ResultType.Yes)
                        {
                            RemoveSelectedFields();
                            ApplyClipChanges();
                        }
                    });
                }
            };

            GUIPanel mainPanel = GUI.AddPanel();
            GUIPanel backgroundPanel = GUI.AddPanel(1);

            GUILayout mainLayout = mainPanel.AddLayoutY();

            buttonLayout = mainLayout.AddLayoutX();
            buttonLayout.AddSpace(5);
            buttonLayout.AddElement(playButton);
            buttonLayout.AddElement(recordButton);
            buttonLayout.AddSpace(5);
            buttonLayout.AddElement(prevFrameButton);
            buttonLayout.AddElement(frameInputField);
            buttonLayout.AddElement(nextFrameButton);
            buttonLayout.AddSpace(5);
            buttonLayout.AddElement(addKeyframeButton);
            buttonLayout.AddElement(addEventButton);
            buttonLayout.AddSpace(5);
            buttonLayout.AddElement(optionsButton);
            buttonLayout.AddFlexibleSpace();

            buttonLayoutHeight = playButton.Bounds.height;

            GUITexture buttonBackground = new GUITexture(null, EditorStyles.HeaderBackground);
            buttonBackground.Bounds = new Rect2I(0, 0, Width, buttonLayoutHeight);
            backgroundPanel.AddElement(buttonBackground);

            GUILayout contentLayout = mainLayout.AddLayoutX();
            GUILayout fieldDisplayLayout = contentLayout.AddLayoutY(GUIOption.FixedWidth(FIELD_DISPLAY_WIDTH));

            guiFieldDisplay = new GUIAnimFieldDisplay(fieldDisplayLayout, FIELD_DISPLAY_WIDTH,
                Height - buttonLayoutHeight * 2, selectedSO);
            guiFieldDisplay.OnEntrySelected += OnFieldSelected;

            GUILayout bottomButtonLayout = fieldDisplayLayout.AddLayoutX();
            bottomButtonLayout.AddElement(addPropertyBtn);
            bottomButtonLayout.AddElement(delPropertyBtn);

            GUITexture separator = new GUITexture(null, EditorStyles.Separator, GUIOption.FixedWidth(3));
            contentLayout.AddElement(separator);

            GUILayout curveLayout = contentLayout.AddLayoutY();

            Vector2I curveEditorSize = GetCurveEditorSize();
            guiCurveEditor = new GUICurveEditor(curveLayout, curveEditorSize.x, curveEditorSize.y, true);
            guiCurveEditor.SetEventSceneObject(selectedSO);

            guiCurveEditor.OnFrameSelected += OnFrameSelected;
            guiCurveEditor.OnEventAdded += () =>
            {
                OnEventsChanged();
                RecordClipState();
            };
            guiCurveEditor.OnEventModified += () =>
            {
                EditorApplication.SetProjectDirty();
                RecordClipState();
            };
            guiCurveEditor.OnEventDeleted += () =>
            {
                OnEventsChanged();
                RecordClipState();
            };
            guiCurveEditor.OnCurveModified += () =>
            {
                RecordClipState();
                SwitchState(State.Normal);

                ApplyClipChanges();
                PreviewFrame(currentFrameIdx);

                EditorApplication.SetProjectDirty();
            };
            guiCurveEditor.OnClicked += () =>
            {
                if(state != State.Recording)
                    SwitchState(State.Normal);
            };

            guiCurveEditor.Redraw();
        }

        /// <summary>
        /// Resizes GUI elements so they fit within the provided boundaries.
        /// </summary>
        /// <param name="width">Width of the GUI bounds, in pixels.</param>
        /// <param name="height">Height of the GUI bounds, in pixels.</param>
        private void ResizeGUI(int width, int height)
        {
            guiFieldDisplay.SetSize(FIELD_DISPLAY_WIDTH, height - buttonLayoutHeight * 2);

            Vector2I curveEditorSize = GetCurveEditorSize();
            guiCurveEditor.SetSize(curveEditorSize.x, curveEditorSize.y);
            guiCurveEditor.Redraw();
        }
        #endregion

        #region Curve save/load
        private EditorAnimClipInfo clipInfo;

        /// <summary>
        /// Refreshes the contents of the curve and property display by loading animation curves from the provided
        /// animation clip.
        /// </summary>
        /// <param name="clip">Clip containing the animation to load.</param>
        private void LoadAnimClip(AnimationClip clip)
        {
            EditorPersistentData persistentData = EditorApplication.PersistentData;

            if (persistentData.dirtyAnimClips.TryGetValue(clip.UUID, out clipInfo))
            {
                // If an animation clip is imported, we don't care about it's cached curve values as they could have changed
                // since last modification, so we re-load the clip. But we persist the events as those can only be set
                // within the editor.
                if (clipInfo.isImported)
                {
                    EditorAnimClipInfo newClipInfo = EditorAnimClipInfo.Create(clip);
                    newClipInfo.events = clipInfo.events;
                }
            }
            else
                clipInfo = EditorAnimClipInfo.Create(clip);

            persistentData.dirtyAnimClips[clip.UUID] = clipInfo;

            AnimFieldInfo[] fieldInfos = new AnimFieldInfo[clipInfo.curves.Count];

            int idx = 0;
            foreach (var curve in clipInfo.curves)
                fieldInfos[idx++] = new AnimFieldInfo(curve.Key, curve.Value);

            guiFieldDisplay.SetFields(fieldInfos);

            guiCurveEditor.Events = clipInfo.events;
            guiCurveEditor.DisableCurveEdit = clipInfo.isImported;
            guiCurveEditor.SetFPS(clipInfo.sampleRate);

            SetCurrentFrame(0);
            fps = clipInfo.sampleRate;
        }

        /// <summary>
        /// Applies any changes made to the animation curves and events to the actual animation clip resource.
        /// </summary>
        private void ApplyClipChanges()
        {
            if (clipInfo == null)
                return;

            clipInfo.Apply(out _);
        }

        /// <summary>
        /// Checks if the currently selected object has changed, and rebuilds the GUI and loads the animation clip if needed.
        /// </summary>
        /// <param name="force">If true the GUI rebuild and animation clip load will be forced regardless if the active
        ///                     scene object changed.</param>
        private void UpdateSelectedSO(bool force)
        {
            SceneObject so = Selection.SceneObject;
            if (selectedSO != so || force)
            {
                if (selectedSO != null && so == null)
                {
                    EditorInput.OnPointerPressed -= OnPointerPressed;
                    EditorInput.OnPointerDoubleClick -= OnPointerDoubleClicked;
                    EditorInput.OnPointerMoved -= OnPointerMoved;
                    EditorInput.OnPointerReleased -= OnPointerReleased;
                    EditorInput.OnButtonUp -= OnButtonUp;
                }
                else if (selectedSO == null && so != null)
                {
                    EditorInput.OnPointerPressed += OnPointerPressed;
                    EditorInput.OnPointerDoubleClick += OnPointerDoubleClicked;
                    EditorInput.OnPointerMoved += OnPointerMoved;
                    EditorInput.OnPointerReleased += OnPointerReleased;
                    EditorInput.OnButtonUp += OnButtonUp;
                }

                SwitchState(State.Empty);

                selectedSO = so;
                selectedFields.Clear();
                clipInfo = null;
                UndoRedo.Clear();

                RebuildGUI();

                // Load existing clip if one exists
                if (selectedSO != null)
                {
                    Animation animation = selectedSO.GetComponent<Animation>();
                    if (animation != null)
                    {
                        AnimationClip clip = animation.DefaultClip.Value;
                        if (clip != null)
                            LoadAnimClip(clip);
                    }
                }

                if(clipInfo == null)
                    clipInfo = new EditorAnimClipInfo();

                SwitchState(State.Normal);

                currentClipState = CreateClipState();
                if (selectedSO != null)
                {
                    // Select first curve by default
                    foreach (var KVP in clipInfo.curves)
                    {
                        SelectField(KVP.Key, false);
                        break;
                    }

                    UpdateDisplayedCurves(true);
                }
            }
        }

        /// <summary>
        /// Stops animation preview on the selected object and resets it back to its non-animated state.
        /// </summary>
        private void ClearSO()
        {
            if (selectedSO != null)
            {
                Animation animation = selectedSO.GetComponent<Animation>();
                if (animation != null)
                    animation.EditorStop();

                // Reset generic curves to their initial values
                UpdateGenericCurves(0.0f);
            }
        }

        #endregion

        #region Undo/Redo

        private AnimationClipState currentClipState;

        /// <summary>
        /// Records current clip state for undo/redo purposes.
        /// </summary>
        private void RecordClipState()
        {
            AnimationClipState clipState = CreateClipState();

            AnimationUndo undoCommand = new AnimationUndo(currentClipState, clipState);
            UndoRedo.RegisterCommand(undoCommand);

            currentClipState = clipState;
        }

        /// <summary>
        /// Records current clip state for undo/redo purposes.
        /// </summary>
        private AnimationClipState CreateClipState()
        {
            AnimationClipState clipState = new AnimationClipState();

            clipState.events = new AnimationEvent[clipInfo.events.Length];
            for (int i = 0; i < clipState.events.Length; i++)
                clipState.events[i] = new AnimationEvent(clipInfo.events[i].name, clipInfo.events[i].time);

            foreach (var curveField in clipInfo.curves)
            {
                AnimationCurveState[] curveData = new AnimationCurveState[curveField.Value.curveInfos.Length];
                for (int i = 0; i < curveData.Length; i++)
                {
                    curveData[i] = new AnimationCurveState();

                    TangentMode[] tangentModes = curveField.Value.curveInfos[i].curve.TangentModes;
                    int numTangentModes = tangentModes.Length;
                    curveData[i].tangentModes = new TangentMode[numTangentModes];
                    Array.Copy(tangentModes, curveData[i].tangentModes, numTangentModes);

                    KeyFrame[] keyFrames = curveField.Value.curveInfos[i].curve.KeyFrames;
                    int numKeyframes = keyFrames.Length;
                    curveData[i].keyFrames = new KeyFrame[numKeyframes];
                    Array.Copy(keyFrames, curveData[i].keyFrames, numKeyframes);
                }

                clipState.curves[curveField.Key] = curveData;
            }

            return clipState;
        }

        /// <summary>
        /// Updates the current animation fields from the keyframes and events in the provided state.
        /// </summary>
        /// <param name="animationClipState">Saved state of an animation clip.</param>
        internal void ApplyClipState(AnimationClipState animationClipState)
        {
            if (state == State.Empty)
                return;

            SwitchState(State.Normal);

            AnimationEvent[] events = animationClipState.events;
            clipInfo.events = new AnimationEvent[events.Length];
            for(int i = 0; i < events.Length; i++)
                clipInfo.events[i] = new AnimationEvent(events[i].name, events[i].time);

            foreach (var KVP in animationClipState.curves)
            {
                FieldAnimCurves fieldCurves;
                if (!clipInfo.curves.TryGetValue(KVP.Key, out fieldCurves))
                    continue;

                for (int i = 0; i < fieldCurves.curveInfos.Length; i++)
                {
                    AnimationCurve curve = new AnimationCurve(KVP.Value[i].keyFrames);
                    fieldCurves.curveInfos[i].curve = new EdAnimationCurve(curve, KVP.Value[i].tangentModes);
                }

                clipInfo.curves[KVP.Key] = fieldCurves;
            }

            // Clear all keyframes from curves not in the stored state
            foreach (var currentKVP in clipInfo.curves)
            {
                bool found = false;
                foreach (var stateKVP in animationClipState.curves)
                {
                    if (currentKVP.Key == stateKVP.Key)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                    continue;

                FieldAnimCurves fieldCurves = currentKVP.Value;
                for (int i = 0; i < fieldCurves.curveInfos.Length; i++)
                {
                    AnimationCurve curve = new AnimationCurve(new KeyFrame[0]);
                    fieldCurves.curveInfos[i].curve = new EdAnimationCurve(curve, new TangentMode[0]);
                }
            }

            currentClipState = animationClipState;

            UpdateDisplayedCurves();

            ApplyClipChanges();
            PreviewFrame(currentFrameIdx);

            EditorApplication.SetProjectDirty();
        }

        #endregion

        #region Record/Playback

        /// <summary>
        /// Possible states the animation window can be in.
        /// </summary>
        private enum State
        {
            Empty,
            Normal,
            Recording,
            Playback
        }

        private State state = State.Empty;
        private bool delayRecord = false;

        /// <summary>
        /// Transitions the window into a different state. Caller must validate state transitions.
        /// </summary>
        /// <param name="state">New state to transition to.</param>
        private void SwitchState(State state)
        {
            switch (this.state)
            {
                case State.Normal:
                {
                    switch (state)
                    {
                        case State.Playback:
                            StartPlayback();
                        break;
                        case State.Recording:
                            StartRecord();
                        break;
                        case State.Empty:
                            ClearSO();
                        break;
                    }
                }
                    break;
                case State.Playback:
                {
                    switch (state)
                    {
                        case State.Normal:
                            EndPlayback();
                            PreviewFrame(currentFrameIdx);
                        break;
                        case State.Recording:
                            EndPlayback();
                            StartRecord();
                        break;
                        case State.Empty:
                            EndPlayback();
                            ClearSO();
                        break;
                        }
                }
                    break;
                case State.Recording:
                {
                    switch (state)
                    {
                        case State.Normal:
                            EndRecord();
                            PreviewFrame(currentFrameIdx);
                        break;
                        case State.Playback:
                            EndRecord();
                            StartPlayback();
                        break;
                        case State.Empty:
                            EndRecord();
                            ClearSO();
                        break;
                    }
                }
                    break;
                case State.Empty:
                {
                    switch (state)
                    {
                        case State.Normal:
                            PreviewFrame(currentFrameIdx);
                            break;
                        case State.Playback:
                            StartPlayback();
                            break;
                        case State.Recording:
                            StartRecord();
                            break;
                    }
                }
                break;
            }

            this.state = state;
        }

        /// <summary>
        /// Plays back the animation on the currently selected object.
        /// </summary>
        private void StartPlayback()
        {
            clipInfo.Apply(out _);

            Animation animation = selectedSO.GetComponent<Animation>();
            if (animation != null && clipInfo.clip != null)
            {
                float time = guiCurveEditor.GetTimeForFrame(currentFrameIdx);

                animation.EditorPlay(clipInfo.clip, time);
                EditorApplication.ToggleOnDemandDrawing("__AnimationPreview", false);
            }

            playButton.Value = true;
        }

        /// <summary>
        /// Ends playback started with <see cref="StartPlayback"/>
        /// </summary>
        private void EndPlayback()
        {
            PreviewFrame(currentFrameIdx);
            EditorApplication.ToggleOnDemandDrawing("__AnimationPreview", true);
            playButton.Value = false;
        }

        /// <summary>
        /// Updates the visible animation to display the provided frame.
        /// </summary>
        /// <param name="frameIdx">Index of the animation frame to display.</param>
        private void PreviewFrame(int frameIdx)
        {
            if (selectedSO == null)
                return;

            float time = guiCurveEditor.GetTimeForFrame(frameIdx);

            Animation animation = selectedSO.GetComponent<Animation>();
            if (animation != null && clipInfo.clip != null)
                animation.EditorPlay(clipInfo.clip, time, true);

            UpdateGenericCurves(time);
            EditorApplication.NotifyNeedsRedraw();
        }

        /// <summary>
        /// Start recording modifications made to the selected scene object.
        /// </summary>
        private void StartRecord()
        {
            float time = guiCurveEditor.GetTimeForFrame(currentFrameIdx);
            if (RecordState(time))
            {
                ApplyClipChanges();
                guiCurveEditor.Redraw();
            }

            recordButton.Value = true;
        }

        /// <summary>
        /// Stops recording modifications made to the selected scene object.
        /// </summary>
        private void EndRecord()
        {
            recordButton.Value = false;
        }


        /// <summary>
        /// Updates the states of all properties controlled by curves to the value of the curves at the provided time.
        /// </summary>
        /// <param name="time">Time at which to evaluate the curves controlling the properties.</param>
        private void UpdateGenericCurves(float time)
        {
            foreach (var KVP in clipInfo.curves)
            {
                FieldAnimCurves curves;
                if (!clipInfo.curves.TryGetValue(KVP.Key, out curves))
                    continue;

                string suffix;
                SerializableProperty property = Animation.FindProperty(selectedSO, KVP.Key, out suffix);

                if (property == null)
                    continue;

                float DoOnComponent(int index)
                {
                    EdAnimationCurve curve = curves.curveInfos[index].curve;
                    return curve.Evaluate(time);
                }

                ForEachPropertyComponentSet(property, DoOnComponent);
            }
        }

        /// <summary>
        /// Iterates over all curve path fields and records their current state. If the state differs from the current
        /// curve values, new keyframes are added.
        /// </summary>
        /// <param name="time">Time for which to record the state, in seconds.</param>
        /// <returns>True if any changes were recorded, false otherwise.</returns>
        private bool RecordState(float time)
        {
            bool changesMade = false;
            foreach (var KVP in clipInfo.curves)
            {
                if (RecordState(KVP.Key, time))
                    changesMade = true;
            }

            return changesMade;
        }

        /// <summary>
        /// Records the state of the provided property and adds it as a keyframe to the related animation curve (or updates
        /// an existing keyframe if the value is different).
        /// </summary>
        /// <param name="path">Path to the property whose state to record.</param>
        /// <param name="time">Time for which to record the state, in seconds.</param>
        /// <returns>True if any changes were recorded, false otherwise.</returns>
        private bool RecordState(string path, float time)
        {
            FieldAnimCurves curves;
            if (!clipInfo.curves.TryGetValue(path, out curves))
                return false;

            string suffix;
            SerializableProperty property = Animation.FindProperty(selectedSO, path, out suffix);

            if (property == null)
                return false;

            bool changesMade = false;
            void DoOnComponent(float value, int index)
            {
                EdAnimationCurve curve = curves.curveInfos[index].curve;

                float curveVal = curve.Evaluate(time);
                if (!MathEx.ApproxEquals(value, curveVal, 0.001f))
                {
                    curve.AddOrUpdateKeyframe(time, value);
                    curve.Apply();

                    changesMade = true;
                }
            }

            ForEachPropertyComponentGet(property, DoOnComponent);

            return changesMade;
        }

        #endregion

        #region Curve display

        private int currentFrameIdx;
        private int fps = 1;

        /// <summary>
        /// Sampling rate of the animation in frames per second. Determines granularity at which positions keyframes can be
        /// placed.
        /// </summary>
        internal int FPS
        {
            get { return fps; }
            set
            {
                fps = MathEx.Max(value, 1);
                guiCurveEditor.SetFPS(fps);

                if (clipInfo != null)
                    clipInfo.sampleRate = fps;
            }
        }

        /// <summary>
        /// Changes the currently selected frame in the curve display.
        /// </summary>
        /// <param name="frameIdx">Index of the frame to select.</param>
        private void SetCurrentFrame(int frameIdx)
        {
            currentFrameIdx = Math.Max(0, frameIdx);

            frameInputField.Value = currentFrameIdx;
            guiCurveEditor.SetMarkedFrame(currentFrameIdx);

            float time = guiCurveEditor.GetTimeForFrame(currentFrameIdx);

            List<GUIAnimFieldPathValue> values = new List<GUIAnimFieldPathValue>();
            foreach (var kvp in clipInfo.curves)
            {
                GUIAnimFieldPathValue fieldValue = new GUIAnimFieldPathValue();
                fieldValue.path = kvp.Key;

                switch (kvp.Value.type)
                {
                    case SerializableProperty.FieldType.Vector2:
                        {
                            Vector2 value = new Vector2();

                            for (int i = 0; i < 2; i++)
                                value[i] = kvp.Value.curveInfos[i].curve.Evaluate(time, false);

                            fieldValue.value = value;
                        }
                        break;
                    case SerializableProperty.FieldType.Vector3:
                        {
                            Vector3 value = new Vector3();

                            for (int i = 0; i < 3; i++)
                                value[i] = kvp.Value.curveInfos[i].curve.Evaluate(time, false);

                            fieldValue.value = value;
                        }
                        break;
                    case SerializableProperty.FieldType.Vector4:
                        {
                            Vector4 value = new Vector4();

                            for (int i = 0; i < 4; i++)
                                value[i] = kvp.Value.curveInfos[i].curve.Evaluate(time, false);

                            fieldValue.value = value;
                        }
                        break;
                    case SerializableProperty.FieldType.Color:
                        {
                            Color value = new Color();

                            for (int i = 0; i < 4; i++)
                                value[i] = kvp.Value.curveInfos[i].curve.Evaluate(time, false);

                            fieldValue.value = value;
                        }
                        break;
                    case SerializableProperty.FieldType.Bool:
                    case SerializableProperty.FieldType.Int:
                    case SerializableProperty.FieldType.Float:
                        fieldValue.value = kvp.Value.curveInfos[0].curve.Evaluate(time, false); ;
                        break;
                }

                values.Add(fieldValue);
            }

            guiFieldDisplay.SetDisplayValues(values.ToArray());
        }

        /// <summary>
        /// Returns a list of all animation curves that should be displayed in the curve display.
        /// </summary>
        /// <returns>Array of curves to display.</returns>
        private EdCurveDrawInfo[] GetDisplayedCurves()
        {
            List<EdCurveDrawInfo> curvesToDisplay = new List<EdCurveDrawInfo>();

            if (clipInfo == null)
                return curvesToDisplay.ToArray();

            for (int i = 0; i < selectedFields.Count; i++)
            {
                EdCurveDrawInfo[] curveInfos;
                if (TryGetCurve(selectedFields[i], out curveInfos))
                    curvesToDisplay.AddRange(curveInfos);
            }

            return curvesToDisplay.ToArray();
        }

        /// <summary>
        /// Calculates an unique color for each animation curve.
        /// </summary>
        private void UpdateCurveColors()
        {
            int globalCurveIdx = 0;
            foreach (var curveGroup in clipInfo.curves)
            {
                for (int i = 0; i < curveGroup.Value.curveInfos.Length; i++)
                    curveGroup.Value.curveInfos[i].color = EditorAnimClipInfo.GetUniqueColor(globalCurveIdx++);
            }
        }

        /// <summary>
        /// Updates the curve display with currently selected curves.
        /// </summary>
        /// <param name="resetTime">If true the time offset/range will be recalculated, otherwise current time offset will
        ///                         be kept as is.</param>
        private void UpdateDisplayedCurves(bool resetTime = false)
        {
            EdCurveDrawInfo[] curvesToDisplay = GetDisplayedCurves();
            guiCurveEditor.SetCurves(curvesToDisplay);
            guiCurveEditor.CenterAndResize(resetTime);
        }

        #endregion 

        #region Field display
        private List<string> selectedFields = new List<string>();

        /// <summary>
        /// Registers a new animation curve field.
        /// </summary>
        /// <param name="path">Path of the field, see <see cref="GUIFieldSelector.OnElementSelected"/></param>
        /// <param name="type">Type of the field (float, vector, etc.)</param>
        private void AddNewField(string path, SerializableProperty.FieldType type)
        {
            bool isPropertyCurve = !clipInfo.isImported && !EditorAnimClipInfo.IsMorphShapeCurve(path);

            switch (type)
            {
                case SerializableProperty.FieldType.Vector4:
                    {
                        FieldAnimCurves fieldCurves = new FieldAnimCurves();
                        fieldCurves.type = type;
                        fieldCurves.isPropertyCurve = isPropertyCurve;
                        fieldCurves.curveInfos = new EdCurveDrawInfo[4];

                        string[] subPaths = { ".x", ".y", ".z", ".w" };
                        for (int i = 0; i < subPaths.Length; i++)
                        {
                            string subFieldPath = path + subPaths[i];
                            fieldCurves.curveInfos[i].curve = new EdAnimationCurve();
                            selectedFields.Add(subFieldPath);
                        }

                        clipInfo.curves[path] = fieldCurves;
                    }
                    break;
                case SerializableProperty.FieldType.Vector3:
                    {
                        FieldAnimCurves fieldCurves = new FieldAnimCurves();
                        fieldCurves.type = type;
                        fieldCurves.isPropertyCurve = isPropertyCurve;
                        fieldCurves.curveInfos = new EdCurveDrawInfo[3];

                        string[] subPaths = { ".x", ".y", ".z" };
                        for (int i = 0; i < subPaths.Length; i++)
                        {
                            string subFieldPath = path + subPaths[i];
                            fieldCurves.curveInfos[i].curve = new EdAnimationCurve();
                            selectedFields.Add(subFieldPath);
                        }

                        clipInfo.curves[path] = fieldCurves;
                    }
                    break;
                case SerializableProperty.FieldType.Vector2:
                    {
                        FieldAnimCurves fieldCurves = new FieldAnimCurves();
                        fieldCurves.type = type;
                        fieldCurves.isPropertyCurve = isPropertyCurve;
                        fieldCurves.curveInfos = new EdCurveDrawInfo[2];

                        string[] subPaths = { ".x", ".y" };
                        for (int i = 0; i < subPaths.Length; i++)
                        {
                            string subFieldPath = path + subPaths[i];
                            fieldCurves.curveInfos[i].curve = new EdAnimationCurve();
                            selectedFields.Add(subFieldPath);
                        }

                        clipInfo.curves[path] = fieldCurves;
                    }
                    break;
                case SerializableProperty.FieldType.Color:
                    {
                        FieldAnimCurves fieldCurves = new FieldAnimCurves();
                        fieldCurves.type = type;
                        fieldCurves.isPropertyCurve = isPropertyCurve;
                        fieldCurves.curveInfos = new EdCurveDrawInfo[4];

                        string[] subPaths = { ".r", ".g", ".b", ".a" };
                        for (int i = 0; i < subPaths.Length; i++)
                        {
                            string subFieldPath = path + subPaths[i];
                            fieldCurves.curveInfos[i].curve = new EdAnimationCurve();
                            selectedFields.Add(subFieldPath);
                        }

                        clipInfo.curves[path] = fieldCurves;
                    }
                    break;
                default: // Primitive type
                    {
                        FieldAnimCurves fieldCurves = new FieldAnimCurves();
                        fieldCurves.type = type;
                        fieldCurves.isPropertyCurve = isPropertyCurve;
                        fieldCurves.curveInfos = new EdCurveDrawInfo[1];

                        fieldCurves.curveInfos[0].curve = new EdAnimationCurve();
                        selectedFields.Add(path);

                        clipInfo.curves[path] = fieldCurves;
                    }
                    break;
            }

            UpdateCurveColors();
            UpdateDisplayedFields();

            EditorApplication.SetProjectDirty();
            UpdateDisplayedCurves();
        }

        /// <summary>
        /// Selects a new animation curve field, making the curve display in the curve display GUI element.
        /// </summary>
        /// <param name="path">Path of the field to display.</param>
        /// <param name="additive">If true the field will be shown along with any already selected fields, or if false
        ///                        only the provided field will be shown.</param>
        private void SelectField(string path, bool additive)
        {
            if (!additive)
                selectedFields.Clear();

            if (!string.IsNullOrEmpty(path))
            {
                selectedFields.RemoveAll(x => { return x == path || IsPathParent(x, path); });
                selectedFields.Add(path);
            }

            guiFieldDisplay.SetSelection(selectedFields.ToArray());
            UpdateDisplayedCurves();
        }

        /// <summary>
        /// Deletes all currently selecting fields, removing them their curves permanently.
        /// </summary>
        private void RemoveSelectedFields()
        {
            for (int i = 0; i < selectedFields.Count; i++)
                clipInfo.curves.Remove(GetSubPathParent(selectedFields[i]));

            UpdateCurveColors();
            UpdateDisplayedFields();

            selectedFields.Clear();
            EditorApplication.SetProjectDirty();
            UpdateDisplayedCurves();
        }

        /// <summary>
        /// Updates the GUI element displaying the current animation curve fields.
        /// </summary>
        private void UpdateDisplayedFields()
        {
            List<AnimFieldInfo> existingFields = new List<AnimFieldInfo>();
            foreach (var KVP in clipInfo.curves)
                existingFields.Add(new AnimFieldInfo(KVP.Key, KVP.Value));

            guiFieldDisplay.SetFields(existingFields.ToArray());
        }
        #endregion

        #region Helpers
        /// <summary>
        /// Returns the size of the curve editor GUI element.
        /// </summary>
        /// <returns>Width/height of the curve editor, in pixels.</returns>
        private Vector2I GetCurveEditorSize()
        {
            Vector2I output = new Vector2I();
            output.x = Math.Max(0, Width - FIELD_DISPLAY_WIDTH);
            output.y = Math.Max(0, Height - buttonLayoutHeight);

            return output;
        }

        /// <summary>
        /// Attempts to find a curve field at the specified path.
        /// </summary>
        /// <param name="path">Path of the curve field to look for.</param>
        /// <param name="curveInfos">One or multiple curves found for the specific path (one field can have multiple curves
        ///                          if it is a complex type, like a vector).</param>
        /// <returns>True if the curve field was found, false otherwise.</returns>
        private bool TryGetCurve(string path, out EdCurveDrawInfo[] curveInfos)
        {
            int index = path.LastIndexOf(".");
            string parentPath;
            string subPathSuffix = null;
            if (index == -1)
            {
                parentPath = path;
            }
            else
            {
                parentPath = path.Substring(0, index);
                subPathSuffix = path.Substring(index, path.Length - index);
            }

            FieldAnimCurves fieldCurves;
            if (clipInfo.curves.TryGetValue(parentPath, out fieldCurves))
            {
                if (!string.IsNullOrEmpty(subPathSuffix))
                {
                    if (subPathSuffix == ".x" || subPathSuffix == ".r")
                    {
                        curveInfos = new [] { fieldCurves.curveInfos[0] };
                        return true;
                    }
                    else if (subPathSuffix == ".y" || subPathSuffix == ".g")
                    {
                        curveInfos = new[] { fieldCurves.curveInfos[1] };
                        return true;
                    }
                    else if (subPathSuffix == ".z" || subPathSuffix == ".b")
                    {
                        curveInfos = new[] { fieldCurves.curveInfos[2] };
                        return true;
                    }
                    else if (subPathSuffix == ".w" || subPathSuffix == ".a")
                    {
                        curveInfos = new[] { fieldCurves.curveInfos[3] };
                        return true;
                    }
                }
                else
                {
                    curveInfos = fieldCurves.curveInfos;
                    return true;
                }
            }

            curveInfos = new EdCurveDrawInfo[0];
            return false;
        }

        /// <summary>
        /// Checks if one curve field path a parent of the other.
        /// </summary>
        /// <param name="child">Path to check if it is a child of <paramref name="parent"/>.</param>
        /// <param name="parent">Path to check if it is a parent of <paramref name="child"/>.</param>
        /// <returns>True if <paramref name="child"/> is a child of <paramref name="parent"/>.</returns>
        private bool IsPathParent(string child, string parent)
        {
            string[] childEntries = child.Split('/', '.');
            string[] parentEntries = parent.Split('/', '.');

            if (parentEntries.Length >= child.Length)
                return false;

            int compareLength = Math.Min(childEntries.Length, parentEntries.Length);
            for (int i = 0; i < compareLength; i++)
            {
                if (childEntries[i] != parentEntries[i])
                    return false;
            }

            return true;
        }

        /// <summary>
        /// If a path has sub-elements (e.g. .x, .r), returns a path without those elements. Otherwise returns the original
        /// path.
        /// </summary>
        /// <param name="path">Path to check.</param>
        /// <returns>Path without sub-elements.</returns>
        private string GetSubPathParent(string path)
        {
            int index = path.LastIndexOf(".");
            if (index == -1)
                return path;

            return path.Substring(0, index);
        }

        /// <summary>
        /// Iterates over all components of a property and calls the provided action for every component with the current
        /// value of the property. Only works with floating point (any dimension), integer, color and boolean property
        /// types. Since reported values are always floating point booleans are encoded as -1.0f for false and 1.0f for
        /// true, and integers are converted to floating point.
        /// </summary>
        /// <param name="property">Property whose components to iterate over.</param>
        /// <param name="action">
        /// Callback to trigger for each component. The callback receives the current value of the property's component
        /// and the sequential index of the component.
        /// </param>
        private void ForEachPropertyComponentGet(SerializableProperty property, Action<float, int> action)
        {
            switch (property.Type)
            {
                case SerializableProperty.FieldType.Vector2:
                    {
                        Vector2 value = property.GetValue<Vector2>();

                        for (int i = 0; i < 2; i++)
                            action(value[i], i);
                    }
                    break;
                case SerializableProperty.FieldType.Vector3:
                    {
                        Vector3 value = property.GetValue<Vector3>();

                        for (int i = 0; i < 3; i++)
                            action(value[i], i);
                    }
                    break;
                case SerializableProperty.FieldType.Vector4:
                    {
                        Vector4 value = property.GetValue<Vector4>();

                        for (int i = 0; i < 4; i++)
                            action(value[i], i);
                    }
                    break;
                case SerializableProperty.FieldType.Color:
                    {
                        Color value = property.GetValue<Color>();

                        for (int i = 0; i < 4; i++)
                            action(value[i], i);
                    }
                    break;
                case SerializableProperty.FieldType.Bool:
                    {
                        bool value = property.GetValue<bool>();
                        action(value ? 1.0f : -1.0f, 0);
                    }
                    break;
                case SerializableProperty.FieldType.Int:
                    {
                        int value = property.GetValue<int>();
                        action(value, 0);
                    }
                    break;
                case SerializableProperty.FieldType.Float:
                    {
                        float value = property.GetValue<float>();
                        action(value, 0);
                    }
                    break;
            }
        }

        /// <summary>
        /// Iterates over all components of a property, calls the provided action which returns a new value to be
        /// assigned to the property component. Only works with floating point (any dimension), integer, color and boolean
        /// property types. Since reported values are always floating point booleans are encoded as -1.0f for false and
        /// 1.0f for true, and integers are converted to floating point.
        /// </summary>
        /// <param name="property">Property whose components to iterate over.</param>
        /// <param name="action">
        /// Callback to trigger for each component. The callback receives the current value of the property's component
        /// and the sequential index of the component.
        /// </param>
        private void ForEachPropertyComponentSet(SerializableProperty property, Func<int, float> action)
        {
            switch (property.Type)
            {
                case SerializableProperty.FieldType.Vector2:
                    {
                        Vector2 value = new Vector2();

                        for (int i = 0; i < 2; i++)
                            value[i] = action(i);

                        property.SetValue(value);
                    }
                    break;
                case SerializableProperty.FieldType.Vector3:
                    {
                        Vector3 value = new Vector3();

                        for (int i = 0; i < 3; i++)
                            value[i] = action(i);

                        property.SetValue(value);
                    }
                    break;
                case SerializableProperty.FieldType.Vector4:
                    {
                        Vector4 value = new Vector4();

                        for (int i = 0; i < 4; i++)
                            value[i] = action(i);

                        property.SetValue(value);
                    }
                    break;
                case SerializableProperty.FieldType.Color:
                    {
                        Color value = new Color();

                        for (int i = 0; i < 4; i++)
                            value[i] = action(i);

                        property.SetValue(value);
                    }
                    break;
                case SerializableProperty.FieldType.Bool:
                    {
                        bool value = action(0) > 0.0f ? true : false;
                        property.SetValue(value);
                    }
                    break;
                case SerializableProperty.FieldType.Int:
                    {
                        int value = (int)action(0);
                        property.SetValue(value);
                    }
                    break;
                case SerializableProperty.FieldType.Float:
                    {
                        float value = action(0);
                        property.SetValue(value);
                    }
                    break;
            }
        }

        #endregion

        #region Input callbacks
        /// <summary>
        /// Triggered when the user presses a mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse press event.</param>
        private void OnPointerPressed(PointerEvent ev)
        {
            guiCurveEditor.OnPointerPressed(ev);

        }

        /// <summary>
        /// Triggered when the user double clicks the left mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse event.</param>
        private void OnPointerDoubleClicked(PointerEvent ev)
        {
            guiCurveEditor.OnPointerDoubleClicked(ev);
        }

        /// <summary>
        /// Triggered when the user moves the mouse.
        /// </summary>
        /// <param name="ev">Information about the mouse move event.</param>
        private void OnPointerMoved(PointerEvent ev)
        {
            guiCurveEditor.OnPointerMoved(ev);

        }

        /// <summary>
        /// Triggered when the user releases a mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse release event.</param>
        private void OnPointerReleased(PointerEvent ev)
        {
            guiCurveEditor.OnPointerReleased(ev);
        }

        /// <summary>
        /// Triggered when the user releases a keyboard button.
        /// </summary>
        /// <param name="ev">Information about the keyboard release event.</param>
        private void OnButtonUp(ButtonEvent ev)
        {
            guiCurveEditor.OnButtonUp(ev);
        }
        #endregion

        #region General callbacks
        /// <summary>
        /// Triggered by the field selector, when user selects a new curve field.
        /// </summary>
        /// <param name="path">Path of the selected curve field.</param>
        /// <param name="type">Type of the selected curve field (float, vector, etc.).</param>
        private void OnFieldAdded(string path, SerializableProperty.FieldType type)
        {
            // Remove the root scene object from the path (we know which SO it is, no need to hardcode its name in the path)
            string pathNoRoot = path.TrimStart('/');
            int separatorIdx = pathNoRoot.IndexOf("/");
            if (separatorIdx == -1 || (separatorIdx + 1) >= pathNoRoot.Length)
                return;

            pathNoRoot = pathNoRoot.Substring(separatorIdx + 1, pathNoRoot.Length - separatorIdx - 1);

            AddNewField(pathNoRoot, type);
            RecordState(pathNoRoot, 0.0f);
            ApplyClipChanges();
        }

        /// <summary>
        /// Triggered when the user selects a new curve field.
        /// </summary>
        /// <param name="path">Path of the selected curve field.</param>
        private void OnFieldSelected(string path)
        {
            bool additive = Input.IsButtonHeld(ButtonCode.LeftShift) || Input.IsButtonHeld(ButtonCode.RightShift);
            SelectField(path, additive);
        }

        /// <summary>
        /// Triggered when the user selects a new scene object or a resource.
        /// </summary>
        /// <param name="sceneObjects">Newly selected scene objects.</param>
        /// <param name="resourcePaths">Newly selected resources.</param>
        private void OnSelectionChanged(SceneObject[] sceneObjects, string[] resourcePaths)
        {
            // While recording allow other objects to be selected so the user can modify them
            if (state == State.Recording)
                return;

            UpdateSelectedSO(false);
        }

        /// <summary>
        /// Triggered when the user selects a new frame in the curve display.
        /// </summary>
        /// <param name="frameIdx">Index of the selected frame.</param>
        private void OnFrameSelected(int frameIdx)
        {
            SetCurrentFrame(frameIdx);
            PreviewFrame(currentFrameIdx);

            // HACK: Skip checking for record changes this frame, to give the preview a chance to update, otherwise
            // the changes would be detected any time a frame is delayed. A proper fix for this would be to force the
            // animation to be evaluated synchronously when PreviewFrame is called.
            delayRecord = true;
        }

        /// <summary>
        /// Triggered when the user changed (add, removed or modified) animation events in the curve display.
        /// </summary>
        private void OnEventsChanged()
        {
            clipInfo.events = guiCurveEditor.Events;
            EditorApplication.SetProjectDirty();
        }
        #endregion
    }

    /// <summary>
    /// Drop down window that displays options used by the animation window.
    /// </summary>
    [DefaultSize(100, 50)]
    internal class AnimationOptions : DropDownWindow
    {
        /// <summary>
        /// Initializes the drop down window by creating the necessary GUI. Must be called after construction and before
        /// use.
        /// </summary>
        /// <param name="parent">Animation window that this drop down window is a part of.</param>
        internal void Initialize(AnimationWindow parent)
        {
            GUIIntField fpsField = new GUIIntField(new LocEdString("FPS"), 40);
            fpsField.Value = parent.FPS;
            fpsField.OnChanged += x => { parent.FPS = x; };
            
            GUILayoutY vertLayout = GUI.AddLayoutY();

            vertLayout.AddFlexibleSpace();
            GUILayoutX contentLayout = vertLayout.AddLayoutX();
            contentLayout.AddFlexibleSpace();
            contentLayout.AddElement(fpsField);
            contentLayout.AddFlexibleSpace();
            vertLayout.AddFlexibleSpace();
        }
    }

    /// <summary>
    /// Raw data representing a single animation curve.
    /// </summary>
    [SerializeObject]
    class AnimationCurveState
    {
        public KeyFrame[] keyFrames;
        public TangentMode[] tangentModes;
    }

    /// <summary>
    /// Raw data representing a single animation clip state.
    /// </summary>
    [SerializeObject]
    class AnimationClipState
    {
        public Dictionary<string, AnimationCurveState[]> curves = new Dictionary<string, AnimationCurveState[]>();
        public AnimationEvent[] events;
    }

    /// <summary>
    /// Undo command used in the AnimationWindow.
    /// </summary>
    [SerializeObject]
    internal class AnimationUndo : UndoableCommand
    {
        private AnimationClipState prevClipState;
        private AnimationClipState clipState;

        public AnimationUndo(AnimationClipState prevClipState, AnimationClipState clipState)
        {
            this.prevClipState = prevClipState;
            this.clipState = clipState;
        }

        /// <inheritdoc/>
        protected override void Commit()
        {
            AnimationWindow window = EditorWindow.GetWindow<AnimationWindow>();
            window?.ApplyClipState(clipState);
        }

        /// <inheritdoc/>
        protected override void Revert()
        {
            AnimationWindow window = EditorWindow.GetWindow<AnimationWindow>();
            window?.ApplyClipState(prevClipState);
        }
    }

    /** @} */
}
