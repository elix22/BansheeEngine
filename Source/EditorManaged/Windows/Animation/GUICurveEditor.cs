//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup AnimationEditor
     *  @{
     */

    /// <summary>
    /// Displays a set of animation curves and events. Allows manipulation of both by adding, removing and modifying
    /// curve keyframes, and animation events.
    /// </summary>
    internal class GUICurveEditor
    {
        /// <summary>
        /// Information about currently selected set of keyframes for a specific curve.
        /// </summary>
        class SelectedKeyframes
        {
            public int curveIdx;
            public List<int> keyIndices = new List<int>();
        }

        /// <summary>
        /// Information about a keyframe that is currently being dragged.
        /// </summary>
        struct DraggedKeyframe
        {
            public DraggedKeyframe(int index, KeyFrame original)
            {
                this.index = index;
                this.original = original;
            }

            public int index;
            public KeyFrame original;
        }

        /// <summary>
        /// Information about all keyframes of a specific curve that are currently being dragged.
        /// </summary>
        class DraggedKeyframes
        {
            public int curveIdx;
            public List<DraggedKeyframe> keys = new List<DraggedKeyframe>();
        }

        /// <summary>
        /// Data about an animation event.
        /// </summary>
        class EventInfo
        {
            public AnimationEvent animEvent;
            public bool selected;
        }

        private const int TIMELINE_HEIGHT = 20;
        private const int VERT_PADDING = 2;
        private const int EVENTS_HEIGHT = 15;
        private const int SIDEBAR_WIDTH = 30;
        private const int DRAG_START_DISTANCE = 3;
        private const float DRAG_SCALE = 3.0f;
        private const float ZOOM_SCALE = 0.1f/120.0f; // One scroll step is usually 120 units, we want 1/10 of that

        private GUILayout gui;
        private GUILayout mainPanel;
        private GUIPanel drawingPanel;
        private GUIPanel eventsPanel;

        private GUITexture timelineBackground;
        private GUITexture eventsBackground;

        private GUIGraphTime guiTimeline;
        private GUIAnimEvents guiEvents;
        private GUICurves guiCurveDrawing;
        private GUIGraphValues guiSidebar;

        private int scrollBarWidth;
        private int scrollBarHeight;

        private GUIResizeableScrollBarH horzScrollBar;
        private GUIResizeableScrollBarV vertScrollBar;

        private ContextMenu blankContextMenu;
        private ContextMenu keyframeContextMenu;
        private ContextMenu blankEventContextMenu;
        private ContextMenu eventContextMenu;
        private Vector2I contextClickPosition;

        private EdCurveDrawInfo[] curveInfos = new EdCurveDrawInfo[0];
        private bool disableCurveEdit = false;

        private float xRange = 60.0f;
        private float yRange = 10.0f;
        private Vector2 offset;

        private int width;
        private int height;

        private int markedFrameIdx;
        private List<SelectedKeyframes> selectedKeyframes = new List<SelectedKeyframes>();

        private bool showEvents = true;
        private List<EventInfo> events = new List<EventInfo>();
        private SceneObject eventsSO;

        private bool isPointerHeld;
        private bool isMousePressedOverKey;
        private bool isMousePressedOverTangent;
        private bool isDragInProgress;
        private bool isModifiedDuringDrag;
        private List<DraggedKeyframes> draggedKeyframes = new List<DraggedKeyframes>();
        private TangentRef draggedTangent;
        private Vector2I dragStart;

        /// <summary>
        /// Triggers whenever user selects a new frame. Reports the index of the selected frame.
        /// </summary>
        public Action<int> OnFrameSelected;

        /// <summary>
        /// Triggered whenever a new animation event is added.
        /// </summary>
        public Action OnEventAdded;

        /// <summary>
        /// Triggered whenever values in an animation event change.
        /// </summary>
        public Action OnEventModified;

        /// <summary>
        /// Triggered whenever an animation event is deleted.
        /// </summary>
        public Action OnEventDeleted;

        /// <summary>
        /// Triggered whenever keyframe in a curve is modified (added, removed or edited).
        /// </summary>
        public Action OnCurveModified;

        /// <summary>
        /// Triggered when the user clicks anywhere on the curve editor area.
        /// </summary>
        public Action OnClicked;

        /// <summary>
        /// The displayed range of the curve, where:
        ///   .x - Range of the horizontal area. Displayed area ranges from [0, x].
        ///   .y - Range of the vertical area. Displayed area ranges from [-y, y].                 
        /// </summary>
        public Vector2 Range
        {
            get { return new Vector2(xRange, yRange); }
            set
            {
                xRange = value.x;
                yRange = value.y;

                guiTimeline.SetRange(xRange);
                guiCurveDrawing.SetRange(xRange, yRange * 2.0f);
                guiSidebar.SetRange(offset.y - yRange, offset.y + yRange);

                if(showEvents)
                    guiEvents.SetRange(xRange);

                Redraw();
            }
        }

        /// <summary>
        /// Determines how much to offset the displayed curve values.
        /// </summary>
        public Vector2 Offset
        {
            get { return offset; }
            set
            {
                offset = value;

                guiTimeline.SetOffset(offset.x);
                guiCurveDrawing.SetOffset(offset);
                guiSidebar.SetRange(offset.y - yRange, offset.y + yRange);

                if(showEvents)
                    guiEvents.SetOffset(offset.x);

                Redraw();
            }
        }

        /// <summary>
        /// Returns the width of the curve editor, in pixels.
        /// </summary>
        public int Width
        {
            get { return width; }
        }

        /// <summary>
        /// Returns the height of the curve editor, in pixels.
        /// </summary>
        public int Height
        {
            get { return height; }
        }

        /// <summary>
        /// Set to true if curves are not allowed to be edited.
        /// </summary>
        public bool DisableCurveEdit
        {
            set { disableCurveEdit = value; }
        }

        /// <summary>
        /// Animation events displayed on the curve editor.
        /// </summary>
        public AnimationEvent[] Events
        {
            get
            {
                AnimationEvent[] animEvents = new AnimationEvent[events.Count];

                // Note: Hidden dependency. Returned events must point to the same event class this object is using, so
                // that any modifications made in this class will be visible in the returned values.
                for (int i = 0; i < events.Count; i++)
                    animEvents[i] = events[i].animEvent;

                return animEvents;
            }

            set
            {
                events.Clear();

                for (int i = 0; i < value.Length; i++)
                {
                    EventInfo eventInfo = new EventInfo();
                    eventInfo.animEvent = value[i];

                    events.Add(eventInfo);
                }

                UpdateEventsGUI();
            }
        }

        /// <summary>
        /// Creates a new curve editor GUI elements.
        /// </summary>
        /// <param name="gui">GUI layout into which to place the GUI element.</param>
        /// <param name="width">Width in pixels.</param>
        /// <param name="height">Height in pixels.</param>
        /// <param name="showEvents">If true show events on the graph and allow their editing.</param>
        /// <param name="drawOptions">Options that control which elements to display when drawing curves.</param>
        public GUICurveEditor(GUILayout gui, int width, int height, bool showEvents, 
            CurveDrawOptions drawOptions = CurveDrawOptions.DrawMarkers | CurveDrawOptions.DrawKeyframes)
        {
            this.gui = gui;
            this.showEvents = showEvents;

            blankContextMenu = new ContextMenu();
            blankContextMenu.AddItem("Add keyframe", AddKeyframeAtPosition);

            blankEventContextMenu = new ContextMenu();
            blankEventContextMenu.AddItem("Add event", AddEventAtPosition);

            keyframeContextMenu = new ContextMenu();
            keyframeContextMenu.AddItem("Delete", DeleteSelectedKeyframes);
            keyframeContextMenu.AddItem("Edit", EditSelectedKeyframe);
            keyframeContextMenu.AddItem("Tangents/Auto", () => { ChangeSelectionTangentMode(TangentMode.Auto); });
            keyframeContextMenu.AddItem("Tangents/Free", () => { ChangeSelectionTangentMode(TangentMode.Free); });
            keyframeContextMenu.AddItem("Tangents/In/Auto", () => { ChangeSelectionTangentMode(TangentMode.InAuto); });
            keyframeContextMenu.AddItem("Tangents/In/Free", () => { ChangeSelectionTangentMode(TangentMode.InFree); });
            keyframeContextMenu.AddItem("Tangents/In/Linear", () => { ChangeSelectionTangentMode(TangentMode.InLinear); });
            keyframeContextMenu.AddItem("Tangents/In/Step", () => { ChangeSelectionTangentMode(TangentMode.InStep); });
            keyframeContextMenu.AddItem("Tangents/Out/Auto", () => { ChangeSelectionTangentMode(TangentMode.OutAuto); });
            keyframeContextMenu.AddItem("Tangents/Out/Free", () => { ChangeSelectionTangentMode(TangentMode.OutFree); });
            keyframeContextMenu.AddItem("Tangents/Out/Linear", () => { ChangeSelectionTangentMode(TangentMode.OutLinear); });
            keyframeContextMenu.AddItem("Tangents/Out/Step", () => { ChangeSelectionTangentMode(TangentMode.OutStep); });

            eventContextMenu = new ContextMenu();
            eventContextMenu.AddItem("Delete", DeleteSelectedEvents);
            eventContextMenu.AddItem("Edit", EditSelectedEvent);

            horzScrollBar = new GUIResizeableScrollBarH();
            horzScrollBar.OnScrollOrResize += OnHorzScrollOrResize;

            vertScrollBar = new GUIResizeableScrollBarV();
            vertScrollBar.OnScrollOrResize += OnVertScrollOrResize;

            GUILayout mainLayout = gui.AddLayoutY();

            GUILayout curveLayoutHorz = mainLayout.AddLayoutX();
            GUILayout horzScrollBarLayout = mainLayout.AddLayoutX();
            horzScrollBarLayout.AddElement(horzScrollBar);
            horzScrollBarLayout.AddFlexibleSpace();

            mainPanel = curveLayoutHorz.AddPanel();
            curveLayoutHorz.AddElement(vertScrollBar);
            curveLayoutHorz.AddFlexibleSpace();

            scrollBarHeight = horzScrollBar.Bounds.height;
            scrollBarWidth = vertScrollBar.Bounds.width;

            this.width = Math.Max(0, width - scrollBarWidth);
            this.height = Math.Max(0, height - scrollBarHeight);

            GUIPanel timelinePanel = mainPanel.AddPanel();
            guiTimeline = new GUIGraphTime(timelinePanel, this.width, TIMELINE_HEIGHT);

            GUIPanel timelineBgPanel = mainPanel.AddPanel(1);

            timelineBackground = new GUITexture(null, EditorStyles.Header);
            timelineBackground.Bounds = new Rect2I(0, 0, this.width, TIMELINE_HEIGHT + VERT_PADDING);
            timelineBgPanel.AddElement(timelineBackground);

            int eventsHeaderHeight = 0;
            if (showEvents)
            {
                eventsPanel = mainPanel.AddPanel();
                eventsPanel.SetPosition(0, TIMELINE_HEIGHT + VERT_PADDING);
                guiEvents = new GUIAnimEvents(eventsPanel, this.width, EVENTS_HEIGHT);

                GUIPanel eventsBgPanel = eventsPanel.AddPanel(1);

                eventsBackground = new GUITexture(null, EditorStyles.Header);
                eventsBackground.Bounds = new Rect2I(0, 0, this.width, EVENTS_HEIGHT + VERT_PADDING);
                eventsBgPanel.AddElement(eventsBackground);

                eventsHeaderHeight = EVENTS_HEIGHT;
            }

            drawingPanel = mainPanel.AddPanel();
            drawingPanel.SetPosition(0, TIMELINE_HEIGHT + eventsHeaderHeight + VERT_PADDING);

            guiCurveDrawing = new GUICurves(drawOptions);
            guiCurveDrawing.SetWidth(this.width);
            guiCurveDrawing.SetHeight(Math.Max(0, this.height - TIMELINE_HEIGHT - eventsHeaderHeight - VERT_PADDING * 2));
            guiCurveDrawing.SetRange(60.0f, 20.0f);
            guiCurveDrawing.Curves = GetPlainCurveDrawInfos();
            drawingPanel.AddElement(guiCurveDrawing);

            GUIPanel sidebarPanel = mainPanel.AddPanel(-10);
            sidebarPanel.SetPosition(0, TIMELINE_HEIGHT + eventsHeaderHeight + VERT_PADDING);

            guiSidebar = new GUIGraphValues(sidebarPanel, SIDEBAR_WIDTH, 
                Math.Max(0, this.height - TIMELINE_HEIGHT - eventsHeaderHeight - VERT_PADDING * 2));
            guiSidebar.SetRange(-10.0f, 10.0f);

            horzScrollBar.SetWidth(this.width);
            vertScrollBar.SetHeight(this.height);

            UpdateScrollBarSize();
        }

        /// <summary>
        /// Change the set of curves to display.
        /// </summary>
        /// <param name="curveInfos">New set of curves to draw on the GUI element.</param>
        public void SetCurves(EdCurveDrawInfo[] curveInfos)
        {
            this.curveInfos = curveInfos;
            guiCurveDrawing.Curves = GetPlainCurveDrawInfos();

            Redraw();
        }

        /// <summary>
        /// Change the physical size of the GUI element.
        /// </summary>
        /// <param name="width">Width of the element in pixels.</param>
        /// <param name="height">Height of the element in pixels.</param>
        public void SetSize(int width, int height)
        {
            this.width = Math.Max(0, width - scrollBarWidth);
            this.height = Math.Max(0, height - scrollBarHeight);

            int eventsHeaderHeight = 0;
            if (showEvents)
            {
                eventsHeaderHeight = EVENTS_HEIGHT;
                guiEvents.SetSize(this.width, EVENTS_HEIGHT);
                eventsBackground.Bounds = new Rect2I(0, 0, this.width, EVENTS_HEIGHT + VERT_PADDING);
            }

            guiTimeline.SetSize(this.width, TIMELINE_HEIGHT);
            guiCurveDrawing.SetWidth(this.width);
            guiCurveDrawing.SetHeight(Math.Max(0, this.height - TIMELINE_HEIGHT - eventsHeaderHeight));
            guiSidebar.SetSize(SIDEBAR_WIDTH, Math.Max(0, this.height - TIMELINE_HEIGHT - eventsHeaderHeight));

            timelineBackground.Bounds = new Rect2I(0, 0, this.width, TIMELINE_HEIGHT + VERT_PADDING);

            horzScrollBar.SetWidth(this.width);
            vertScrollBar.SetHeight(this.height);

            UpdateScrollBarSize();
            UpdateScrollBarPosition();
        }

        /// <summary>
        /// Number of frames per second, used for frame selection and marking.
        /// </summary>
        /// <param name="fps">Number of prames per second.</param>
        public void SetFPS(int fps)
        {
            guiTimeline.SetFPS(fps);
            guiCurveDrawing.FPS = fps;

            if(showEvents)
                guiEvents.SetFPS(fps);

            Redraw();
        }

        /// <summary>
        /// Sets a scene object that will be used for enumerating components/methods used for adding events.
        /// </summary>
        /// <param name="so">Scene object containing the animation component.</param>
        public void SetEventSceneObject(SceneObject so)
        {
            eventsSO = so;
        }

        /// <summary>
        /// Returns time for a frame with the specified index. Depends on set range and FPS.
        /// </summary>
        /// <param name="frameIdx">Index of the frame (not a key-frame) to get the time for.</param>
        /// <returns>Time of the frame with the provided index. </returns>
        public float GetTimeForFrame(int frameIdx)
        {
            return guiCurveDrawing.GetTimeForFrame(frameIdx);
        }

        /// <summary>
        /// Sets the frame at which to display the frame marker.
        /// </summary>
        /// <param name="frameIdx">Index of the frame to display the marker on, or -1 to clear the marker.</param>
        public void SetMarkedFrame(int frameIdx)
        {
            markedFrameIdx = frameIdx;

            guiTimeline.SetMarkedFrame(frameIdx);
            guiCurveDrawing.MarkedFrame = frameIdx;

            if(showEvents)
                guiEvents.SetMarkedFrame(frameIdx);

            Redraw();
        }

        /// <summary>
        /// Adds a new keyframe at the currently selected frame.
        /// </summary>
        public void AddKeyFrameAtMarker()
        {
            ClearSelection();

            if (!disableCurveEdit)
            {
                foreach (var curveInfo in curveInfos)
                {
                    float t = guiCurveDrawing.GetTimeForFrame(markedFrameIdx);
                    float value = curveInfo.curve.Evaluate(t);

                    curveInfo.curve.AddOrUpdateKeyframe(t, value);
                    curveInfo.curve.Apply();
                }
            }
            else
                ShowReadOnlyMessage();

            OnCurveModified?.Invoke();
            RefreshCurveDrawing();
            UpdateEventsGUI();
        }

        /// <summary>
        /// Adds a new event at the currently selected event.
        /// </summary>
        public void AddEventAtMarker()
        {
            ClearSelection();

            if (!showEvents)
                return;

            float eventTime = guiEvents.GetTimeForFrame(markedFrameIdx);
            EventInfo eventInfo = new EventInfo();
            eventInfo.animEvent = new AnimationEvent("", eventTime);

            events.Add(eventInfo);
            OnEventAdded?.Invoke();

            UpdateEventsGUI();
            StartEventEdit(events.Count - 1);
        }

        /// <summary>
        /// Rebuilds GUI displaying the animation events list.
        /// </summary>
        private void UpdateEventsGUI()
        {
            if (!showEvents)
                return;

            AnimationEvent[] animEvents = new AnimationEvent[events.Count];
            bool[] selected = new bool[events.Count];

            for (int i = 0; i < events.Count; i++)
            {
                animEvents[i] = events[i].animEvent;
                selected[i] = events[i].selected;
            }

            guiEvents.SetEvents(animEvents, selected);
            guiEvents.Rebuild();
        }

        /// <summary>
        /// Rebuilds the entire curve editor GUI.
        /// </summary>
        public void Redraw()
        {
            guiTimeline.Rebuild();
            guiSidebar.Rebuild();

            if(showEvents)
                guiEvents.Rebuild();
        }

        /// <summary>
        /// Updates the curve drawing. Should be called after animation curves change.
        /// </summary>
        private void RefreshCurveDrawing()
        {
            guiCurveDrawing.Curves = GetPlainCurveDrawInfos();
        }

        /// <summary>
        /// Changes the tangent mode for all currently selected keyframes.
        /// </summary>
        /// <param name="mode">Tangent mode to set. If only in or out tangent mode is provided, the mode for the opposite 
        ///                    tangent will be kept as is.</param>
        private void ChangeSelectionTangentMode(TangentMode mode)
        {
            if (disableCurveEdit)
            {
                ShowReadOnlyMessage();
                return;
            }

            foreach (var selectedEntry in selectedKeyframes)
            {
                EdAnimationCurve curve = curveInfos[selectedEntry.curveIdx].curve;

                foreach (var keyframeIdx in selectedEntry.keyIndices)
                {
                    if (mode == TangentMode.Auto || mode == TangentMode.Free)
                    {
                        curve.SetTangentMode(keyframeIdx, mode);

                        // Refresh tangent display
                        guiCurveDrawing.SelectKeyframe(new KeyframeRef(selectedEntry.curveIdx, keyframeIdx), mode, true);
                    }
                    else
                    {
                        TangentMode newMode = curve.TangentModes[keyframeIdx];

                        if (mode.HasFlag((TangentMode) TangentType.In))
                        {
                            // Replace only the in tangent mode, keeping the out tangent as is
                            TangentMode inFlags = (TangentMode.InAuto | TangentMode.InFree | TangentMode.InLinear |
                                                   TangentMode.InStep);

                            newMode &= ~inFlags;
                            newMode |= (mode & inFlags);
                        }
                        else
                        {
                            // Replace only the out tangent mode, keeping the in tangent as is
                            TangentMode outFlags = (TangentMode.OutAuto | TangentMode.OutFree | TangentMode.OutLinear |
                                                    TangentMode.OutStep);

                            newMode &= ~outFlags;
                            newMode |= (mode & outFlags);
                        }

                        curve.SetTangentMode(keyframeIdx, newMode);

                        // Refresh tangent display
                        guiCurveDrawing.SelectKeyframe(new KeyframeRef(selectedEntry.curveIdx, keyframeIdx), newMode, true);
                    }
                }

                curve.Apply();
            }

            OnCurveModified?.Invoke();
            RefreshCurveDrawing();
        }

        /// <summary>
        /// Adds a new keyframe at the specified time on the provided curve.
        /// </summary>
        /// <param name="curveIdx">Index of the curve to add the keyframe to.</param>
        /// <param name="time">Time at which to add the keyframe.</param>
        private void AddKeyframe(int curveIdx, float time)
        {
            ClearSelection();

            if (!disableCurveEdit)
            {
                if (curveIdx < curveInfos.Length)
                {
                    EdAnimationCurve curve = curveInfos[curveIdx].curve;

                    float value = curve.Evaluate(time, false);

                    curve.AddOrUpdateKeyframe(time, value);
                    curve.Apply();
                }
            }
            else
                ShowReadOnlyMessage();

            OnCurveModified?.Invoke();
            RefreshCurveDrawing();
            UpdateEventsGUI();
        }

        /// <summary>
        /// Adds a new keyframe at the position the context menu was opened at.
        /// </summary>
        private void AddKeyframeAtPosition()
        {
            Vector2 curveCoord;
            if (guiCurveDrawing.PixelToCurveSpace(contextClickPosition, out curveCoord))
            {
                ClearSelection();

                if (!disableCurveEdit)
                {
                    foreach (var curveInfo in curveInfos)
                    {
                        float t = curveCoord.x;
                        float value = curveCoord.y;

                        curveInfo.curve.AddOrUpdateKeyframe(t, value);
                        curveInfo.curve.Apply();
                    }
                }
                else
                    ShowReadOnlyMessage();

                OnCurveModified?.Invoke();
                RefreshCurveDrawing();
                UpdateEventsGUI();
            }
        }

        /// <summary>
        /// Adds a new event at the position the context menu was opened at.
        /// </summary>
        private void AddEventAtPosition()
        {
            if (!showEvents)
                return;

            int frame = guiEvents.GetFrame(contextClickPosition);
            if (frame != -1)
            {
                ClearSelection();
                float time = guiEvents.GetTime(contextClickPosition.x);

                EventInfo eventInfo = new EventInfo();
                eventInfo.animEvent = new AnimationEvent("", time);

                events.Add(eventInfo);

                OnEventAdded?.Invoke();
                UpdateEventsGUI();
                StartEventEdit(events.Count - 1);
            }
        }

        /// <summary>
        /// Removes all currently selected keyframes from the curves.
        /// </summary>
        private void DeleteSelectedKeyframes()
        {
            if (!disableCurveEdit)
            {
                foreach (var selectedEntry in selectedKeyframes)
                {
                    EdAnimationCurve curve = curveInfos[selectedEntry.curveIdx].curve;

                    // Sort keys from highest to lowest so the indices don't change
                    selectedEntry.keyIndices.Sort((x, y) =>
                    {
                        return y.CompareTo(x);
                    });

                    foreach (var keyframeIdx in selectedEntry.keyIndices)
                        curve.RemoveKeyframe(keyframeIdx);

                    curve.Apply();
                }
            }
            else
                ShowReadOnlyMessage();

            ClearSelection();

            OnCurveModified?.Invoke();
            RefreshCurveDrawing();
            UpdateEventsGUI();
        }

        /// <summary>
        /// Deletes all currently selected events.
        /// </summary>
        private void DeleteSelectedEvents()
        {
            List<EventInfo> newEvents = new List<EventInfo>();
            foreach (var entry in events)
            {
                if(!entry.selected)
                    newEvents.Add(entry);
            }

            events = newEvents;

            OnEventDeleted?.Invoke();
            ClearSelection();
            UpdateEventsGUI();
        }

        /// <summary>
        /// Unselects any selected keyframes and events.
        /// </summary>
        private void ClearSelection()
        {
            guiCurveDrawing.ClearSelectedKeyframes();
            selectedKeyframes.Clear();

            foreach (var entry in events)
                entry.selected = false;
        }

        /// <summary>
        /// Adds the provided keyframe to the selection list (doesn't clear existing ones).
        /// </summary>
        /// <param name="keyframeRef">Keyframe to select.</param>
        private void SelectKeyframe(KeyframeRef keyframeRef)
        {
            TangentMode tangentMode = TangentMode.Auto;
            if (keyframeRef.curveIdx >= 0 && keyframeRef.curveIdx < curveInfos.Length)
            {
                EdAnimationCurve curve = curveInfos[keyframeRef.curveIdx].curve;
                if (keyframeRef.keyIdx >= 0 && keyframeRef.keyIdx < curve.TangentModes.Length)
                    tangentMode = curve.TangentModes[keyframeRef.keyIdx];
            }

            guiCurveDrawing.SelectKeyframe(keyframeRef, tangentMode, true);

            if (!IsSelected(keyframeRef))
            {
                int curveIdx = selectedKeyframes.FindIndex(x =>
                {
                    return x.curveIdx == keyframeRef.curveIdx;
                });

                if (curveIdx == -1)
                {
                    curveIdx = selectedKeyframes.Count;

                    SelectedKeyframes newKeyframes = new SelectedKeyframes();
                    newKeyframes.curveIdx = keyframeRef.curveIdx;

                    selectedKeyframes.Add(newKeyframes);
                }

                selectedKeyframes[curveIdx].keyIndices.Add(keyframeRef.keyIdx);
            }
        }

        /// <summary>
        /// Checks is the provided keyframe currently selected.
        /// </summary>
        /// <param name="keyframeRef">Keyframe to check.</param>
        /// <returns>True if selected, false otherwise.</returns>
        private bool IsSelected(KeyframeRef keyframeRef)
        {
            int curveIdx = selectedKeyframes.FindIndex(x =>
            {
                return x.curveIdx == keyframeRef.curveIdx;
            });

            if (curveIdx == -1)
                return false;

            int keyIdx = selectedKeyframes[curveIdx].keyIndices.FindIndex(x =>
            {
                return x == keyframeRef.keyIdx;
            });

            return keyIdx != -1;
        }

        /// <summary>
        /// Opens the edit window for the currently selected keyframe.
        /// </summary>
        private void EditSelectedKeyframe()
        {
            if (disableCurveEdit)
            {
                ShowReadOnlyMessage();
                return;
            }

            if (selectedKeyframes.Count == 0)
                return;

            EdAnimationCurve curve = curveInfos[selectedKeyframes[0].curveIdx].curve;
            KeyFrame[] keyFrames = curve.KeyFrames;

            int keyIndex = selectedKeyframes[0].keyIndices[0];
            KeyFrame keyFrame = keyFrames[keyIndex];
            Vector2I position = guiCurveDrawing.CurveToPixelSpace(new Vector2(keyFrame.time, keyFrame.value));

            Rect2I drawingBounds = drawingPanel.Bounds;
            position.x = MathEx.Clamp(position.x, 0, drawingBounds.width);
            position.y = MathEx.Clamp(position.y, 0, drawingBounds.height);

            KeyframeEditWindow editWindow = DropDownWindow.Open<KeyframeEditWindow>(drawingPanel, position);
            editWindow.Initialize(keyFrame, x =>
            {
                curve.UpdateKeyframe(keyIndex, x.time, x.value);
                curve.Apply();

                RefreshCurveDrawing();
            },
            x =>
            {
                if (x)
                    OnCurveModified?.Invoke();
            });
        }

        /// <summary>
        /// Opens the edit window for the currently selected event.
        /// </summary>
        private void EditSelectedEvent()
        {
            if (!showEvents)
                return;

            for (int i = 0; i < events.Count; i++)
            {
                if (events[i].selected)
                {
                    StartEventEdit(i);
                    break;
                }
            }
        }

        /// <summary>
        /// Opens the event edit window for the specified event.
        /// </summary>
        /// <param name="eventIdx">Event index to open the edit window for.</param>
        private void StartEventEdit(int eventIdx)
        {
            if (!showEvents || eventsSO == null)
                return;

            AnimationEvent animEvent = events[eventIdx].animEvent;

            Vector2I position = new Vector2I();
            position.x = guiEvents.GetOffset(animEvent.time);
            position.y = EVENTS_HEIGHT/2;

            Component[] components = eventsSO.GetComponents();
            string[] componentNames = new string[components.Length];
            for (int i = 0; i < components.Length; i++)
                componentNames[i] = components[i].GetType().Name;

            EventEditWindow editWindow = DropDownWindow.Open<EventEditWindow>(eventsPanel, position);
            editWindow.Initialize(animEvent, componentNames, () =>
            {
                UpdateEventsGUI();
            },
            x =>
            {
                if(x)
                    OnEventModified?.Invoke();
            });
        }

        /// <summary>
        /// Shows a dialog box that notifies the user that the animation clip is read only.
        /// </summary>
        private void ShowReadOnlyMessage()
        {
            LocEdString title = new LocEdString("Warning");
            LocEdString message =
                new LocEdString("You cannot edit keyframes on animation clips that" +
                                " are imported from an external file.");

            DialogBox.Open(title, message, DialogBox.Type.OK);
        }

        #region Input

        /// <summary>
        /// Handles input. Should be called by the owning window whenever a pointer is pressed.
        /// </summary>
        /// <param name="ev">Object containing pointer press event information.</param>
        internal void OnPointerPressed(PointerEvent ev)
        {
            if (ev.IsUsed)
                return;

            Rect2I elementBounds = mainPanel.ScreenBounds;
            Vector2I pointerPos = ev.ScreenPos - new Vector2I(elementBounds.x, elementBounds.y);

            bool isOverEditor = pointerPos.x >= 0 && pointerPos.x < width && pointerPos.y >= 0 && pointerPos.y < height;
            if (!isOverEditor)
                return;
            else
                OnClicked?.Invoke();

            Rect2I drawingBounds = drawingPanel.Bounds;
            Vector2I drawingPos = pointerPos - new Vector2I(drawingBounds.x, drawingBounds.y);

            Rect2I eventBounds = new Rect2I();
            if(showEvents)
                eventBounds = eventsPanel.Bounds;

            Vector2I eventPos = pointerPos - new Vector2I(eventBounds.x, eventBounds.y);

            if (ev.Button == PointerButton.Left)
            {
                Vector2 curveCoord;
                if (guiCurveDrawing.PixelToCurveSpace(drawingPos, out curveCoord, true))
                {
                    KeyframeRef keyframeRef;
                    if (!guiCurveDrawing.FindKeyFrame(drawingPos, out keyframeRef))
                    {
                        TangentRef tangentRef;
                        if (guiCurveDrawing.FindTangent(drawingPos, out tangentRef))
                        {
                            isMousePressedOverTangent = true;
                            dragStart = drawingPos;
                            draggedTangent = tangentRef;
                        }
                        else
                            ClearSelection();
                    }
                    else
                    {
                        if (!IsSelected(keyframeRef))
                        {
                            if (!Input.IsButtonHeld(ButtonCode.LeftShift) && !Input.IsButtonHeld(ButtonCode.RightShift))
                                ClearSelection();

                            SelectKeyframe(keyframeRef);
                        }

                        isMousePressedOverKey = true;
                        dragStart = drawingPos;
                    }

                    UpdateEventsGUI();
                }
                else
                {
                    int frameIdx = guiTimeline.GetFrame(pointerPos);

                    if (frameIdx != -1)
                        SetMarkedFrame(frameIdx);
                    else
                    {
                        int eventIdx;
                        if (showEvents && guiEvents.FindEvent(eventPos, out eventIdx))
                        {
                            if (!Input.IsButtonHeld(ButtonCode.LeftShift) && !Input.IsButtonHeld(ButtonCode.RightShift))
                                ClearSelection();

                            events[eventIdx].selected = true;
                            UpdateEventsGUI();
                        }
                        else
                        {
                            ClearSelection();
                            UpdateEventsGUI();
                        }
                    }

                    OnFrameSelected?.Invoke(frameIdx);
                }

                isPointerHeld = true;
            }
            else if (ev.Button == PointerButton.Right)
            {
                Vector2 curveCoord;
                if (guiCurveDrawing.PixelToCurveSpace(drawingPos, out curveCoord, true))
                {
                    contextClickPosition = drawingPos;

                    KeyframeRef keyframeRef;
                    if (!guiCurveDrawing.FindKeyFrame(drawingPos, out keyframeRef))
                    {
                        ClearSelection();

                        blankContextMenu.Open(pointerPos, mainPanel);
                        UpdateEventsGUI();
                    }
                    else
                    {
                        // If clicked outside of current selection, just select the one keyframe
                        if (!IsSelected(keyframeRef))
                        {
                            ClearSelection();
                            SelectKeyframe(keyframeRef);
                            UpdateEventsGUI();
                        }

                        keyframeContextMenu.Open(pointerPos, mainPanel);
                    }
                }
                else if (showEvents && guiEvents.GetFrame(eventPos) != -1) // Clicked over events bar
                {
                    contextClickPosition = eventPos;

                    int eventIdx;
                    if (!guiEvents.FindEvent(eventPos, out eventIdx))
                    {
                        ClearSelection();

                        blankEventContextMenu.Open(pointerPos, mainPanel);
                        UpdateEventsGUI();
                    }
                    else
                    {
                        // If clicked outside of current selection, just select the one event
                        if (!events[eventIdx].selected)
                        {
                            ClearSelection();
                            events[eventIdx].selected = true;
                            UpdateEventsGUI();
                        }

                        eventContextMenu.Open(pointerPos, mainPanel);
                    }
                }
            }
            else if (ev.button == PointerButton.Middle)
            {
                Vector2 curvePos;
                if (WindowToCurveSpace(pointerPos, out curvePos))
                {
                    dragStartPos = pointerPos;
                    isMiddlePointerHeld = true;
                }
            }
        }

        /// <summary>
        /// Handles input. Should be called by the owning window whenever a pointer is double-clicked.
        /// </summary>
        /// <param name="ev">Object containing pointer press event information.</param>
        internal void OnPointerDoubleClicked(PointerEvent ev)
        {
            if (ev.IsUsed)
                return;

            Rect2I elementBounds = mainPanel.ScreenBounds;
            Vector2I pointerPos = ev.ScreenPos - new Vector2I(elementBounds.x, elementBounds.y);

            bool isOverEditor = pointerPos.x >= 0 && pointerPos.x < width && pointerPos.y >= 0 && pointerPos.y < height;
            if (!isOverEditor)
                return;

            Rect2I drawingBounds = drawingPanel.Bounds;
            Vector2I drawingPos = pointerPos - new Vector2I(drawingBounds.x, drawingBounds.y);

            if (guiCurveDrawing.PixelToCurveSpace(drawingPos, out var curveCoord, true))
            {
                int curveIdx = (int)guiCurveDrawing.FindCurve(drawingPos);
                if (curveIdx == -1)
                    return;

                AddKeyframe(curveIdx, curveCoord.x);
            }
        }

        /// <summary>
        /// Handles input. Should be called by the owning window whenever a pointer is moved.
        /// </summary>
        /// <param name="ev">Object containing pointer move event information.</param>
        internal void OnPointerMoved(PointerEvent ev)
        {
            if (ev.Button != PointerButton.Left)
                return;

            if (isPointerHeld)
            {
                Rect2I elementBounds = mainPanel.ScreenBounds;
                Vector2I pointerPos = ev.ScreenPos - new Vector2I(elementBounds.x, elementBounds.y);

                if (isMousePressedOverKey || isMousePressedOverTangent)
                {
                    Rect2I drawingBounds = drawingPanel.Bounds;
                    Vector2I drawingPos = pointerPos - new Vector2I(drawingBounds.x, drawingBounds.y);

                    if (!isDragInProgress)
                    {
                        int distance = Vector2I.Distance(drawingPos, dragStart);
                        if (distance >= DRAG_START_DISTANCE)
                        {
                            if (isMousePressedOverKey && !disableCurveEdit)
                            {
                                draggedKeyframes.Clear();
                                foreach (var selectedEntry in selectedKeyframes)
                                {
                                    EdAnimationCurve curve = curveInfos[selectedEntry.curveIdx].curve;
                                    KeyFrame[] keyFrames = curve.KeyFrames;

                                    DraggedKeyframes newEntry = new DraggedKeyframes();
                                    newEntry.curveIdx = selectedEntry.curveIdx;
                                    draggedKeyframes.Add(newEntry);

                                    foreach (var keyframeIdx in selectedEntry.keyIndices)
                                        newEntry.keys.Add(new DraggedKeyframe(keyframeIdx, keyFrames[keyframeIdx]));
                                }
                            }

                            isDragInProgress = true;
                        }
                    }

                    if (isDragInProgress)
                    {
                        if (isMousePressedOverKey && !disableCurveEdit)
                        {
                            Vector2 diff = Vector2.Zero;

                            Vector2 dragStartCurve;
                            if (guiCurveDrawing.PixelToCurveSpace(dragStart, out dragStartCurve, true))
                            {
                                Vector2 currentPosCurve;
                                if (guiCurveDrawing.PixelToCurveSpace(drawingPos, out currentPosCurve, true))
                                    diff = currentPosCurve - dragStartCurve;
                            }

                            foreach (var draggedEntry in draggedKeyframes)
                            {
                                EdAnimationCurve curve = curveInfos[draggedEntry.curveIdx].curve;

                                for (int i = 0; i < draggedEntry.keys.Count; i++)
                                {
                                    DraggedKeyframe draggedKey = draggedEntry.keys[i];

                                    float newTime = Math.Max(0.0f, draggedKey.original.time + diff.x);
                                    float newValue = draggedKey.original.value + diff.y;

                                    int newIndex = curve.UpdateKeyframe(draggedKey.index, newTime, newValue);

                                    // It's possible key changed position due to time change, but since we're moving all
                                    // keys at once they cannot change position relative to one another, otherwise we would
                                    // need to update indices for other keys as well.
                                    draggedKey.index = newIndex;
                                    draggedEntry.keys[i] = draggedKey;
                                }

                                curve.Apply();
                            }

                            // Rebuild selected keys from dragged keys (after potential sorting)
                            ClearSelection();
                            foreach (var draggedEntry in draggedKeyframes)
                            {
                                foreach (var keyframe in draggedEntry.keys)
                                    SelectKeyframe(new KeyframeRef(draggedEntry.curveIdx, keyframe.index));
                            }

                            isModifiedDuringDrag = true;

                            RefreshCurveDrawing();
                            UpdateEventsGUI();
                        }
                        else if (isMousePressedOverTangent && !disableCurveEdit)
                        {
                            EdAnimationCurve curve = curveInfos[draggedTangent.keyframeRef.curveIdx].curve;
                            KeyFrame keyframe = curve.KeyFrames[draggedTangent.keyframeRef.keyIdx];

                            Vector2 keyframeCurveCoords = new Vector2(keyframe.time, keyframe.value);

                            Vector2 currentPosCurve;
                            if (guiCurveDrawing.PixelToCurveSpace(drawingPos, out currentPosCurve, true))
                            {
                                Vector2 normal = currentPosCurve - keyframeCurveCoords;
                                normal = normal.Normalized;

                                float tangent = EdAnimationCurve.NormalToTangent(normal);

                                if (draggedTangent.type == TangentType.In)
                                {
                                    if (normal.x > 0.0f)
                                        tangent = float.PositiveInfinity;
                                    else
                                        tangent = -tangent;

                                    keyframe.inTangent = tangent;
                                    if(curve.TangentModes[draggedTangent.keyframeRef.keyIdx] == TangentMode.Free)
                                        keyframe.outTangent = tangent;
                                }
                                else
                                {
                                    if (normal.x < 0.0f)
                                        tangent = float.PositiveInfinity;

                                    keyframe.outTangent = tangent;
                                    if (curve.TangentModes[draggedTangent.keyframeRef.keyIdx] == TangentMode.Free)
                                        keyframe.inTangent = tangent;
                                }

                                curve.KeyFrames[draggedTangent.keyframeRef.keyIdx] = keyframe;
                                curve.Apply();

                                isModifiedDuringDrag = true;
                                RefreshCurveDrawing();
                            }
                        }
                    }
                }
                else // Move frame marker
                {
                    int frameIdx = guiTimeline.GetFrame(pointerPos);

                    if (frameIdx != -1)
                        SetMarkedFrame(frameIdx);

                    OnFrameSelected?.Invoke(frameIdx);
                }
            }

            if (isMiddlePointerHeld)
            {
                Rect2I elementBounds = mainPanel.ScreenBounds;
                Vector2I pointerPos = ev.ScreenPos - new Vector2I(elementBounds.x, elementBounds.y);

                int distance = Vector2I.Distance(dragStartPos, pointerPos);
                if (distance >= DRAG_START_DISTANCE)
                {
                    isZoomDragInProgress = true;

                    Cursor.Hide();

                    Rect2I clipRect;
                    clipRect.x = ev.ScreenPos.x - 2;
                    clipRect.y = ev.ScreenPos.y - 2;
                    clipRect.width = 4;
                    clipRect.height = 4;

                    Cursor.ClipToRect(clipRect);
                }
            }
        }

        /// <summary>
        /// Handles input. Should be called by the owning window whenever a pointer is released.
        /// </summary>
        /// <param name="ev">Object containing pointer release event information.</param>
        internal void OnPointerReleased(PointerEvent ev)
        {
            if (isZoomDragInProgress)
            {
                Cursor.Show();
                Cursor.ClipDisable();
            }

            if (isModifiedDuringDrag)
                OnCurveModified?.Invoke();

            isMiddlePointerHeld = false;
            isZoomDragInProgress = false;
            isPointerHeld = false;
            isDragInProgress = false;
            isMousePressedOverKey = false;
            isMousePressedOverTangent = false;
            isModifiedDuringDrag = false;
        }

        /// <summary>
        /// Handles input. Should be called by the owning window whenever a button is released.
        /// </summary>
        /// <param name="ev">Object containing button release event information.</param>
        internal void OnButtonUp(ButtonEvent ev)
        {
            if(ev.Button == ButtonCode.Delete)
                DeleteSelectedKeyframes();
        }

        #endregion

        #region Scroll, drag, zoom
        private Vector2I dragStartPos;
        private bool isMiddlePointerHeld;
        private bool isZoomDragInProgress;

        private float zoomAmount;

        /// <summary>
        /// Handles mouse scroll wheel and dragging events in order to zoom or drag the displayed curve editor contents.
        /// Should be called every frame.
        /// </summary>
        internal void HandleDragAndZoomInput()
        {
            // Handle middle mouse dragging
            if (isZoomDragInProgress)
            {
                float lengthPerPixel = Range.x / Width;
                float heightPerPixel = Range.y / Height;

                float dragX = Input.GetAxisValue(InputAxis.MouseX) * DRAG_SCALE * lengthPerPixel;
                float dragY = Input.GetAxisValue(InputAxis.MouseY) * DRAG_SCALE * heightPerPixel;

                Vector2 offset = Offset;
                offset.x = Math.Max(0.0f, offset.x + dragX);
                offset.y -= dragY;

                Offset = offset;
                UpdateScrollBarSize();
                UpdateScrollBarPosition();
            }

            // Handle zoom in/out
            float scroll = Input.GetAxisValue(InputAxis.MouseZ);
            if (scroll != 0.0f)
            {
                Rect2I elementBounds = mainPanel.ScreenBounds;
                Vector2I pointerPos = Input.PointerPosition - new Vector2I(elementBounds.x, elementBounds.y);

                Vector2 curvePos;
                if (WindowToCurveSpace(pointerPos, out curvePos))
                {
                    float zoom = scroll * ZOOM_SCALE;
                    Zoom(curvePos, zoom);
                }
            }
        }

        /// <summary>
        /// Moves or resizes the vertical scroll bar under the curve editor.
        /// </summary>
        /// <param name="position">New position of the scrollbar, in range [0, 1].</param>
        /// <param name="size">New size of the scrollbar handle, in range [0, 1].</param>
        private void SetVertScrollbarProperties(float position, float size)
        {
            Vector2 visibleRange = Range;
            Vector2 totalRange = GetTotalRange();

            visibleRange.y = totalRange.y*size;
            Range = visibleRange;

            float scrollableRange = totalRange.y - visibleRange.y;

            Vector2 offset = Offset;
            offset.y = -scrollableRange * (position * 2.0f - 1.0f);

            Offset = offset;
        }

        /// <summary>
        /// Moves or resizes the horizontal scroll bar under the curve editor.
        /// </summary>
        /// <param name="position">New position of the scrollbar, in range [0, 1].</param>
        /// <param name="size">New size of the scrollbar handle, in range [0, 1].</param>
        private void SetHorzScrollbarProperties(float position, float size)
        {
            Vector2 visibleRange = Range;
            Vector2 totalRange = GetTotalRange();

            visibleRange.x = totalRange.x * size;
            Range = visibleRange;

            float scrollableRange = totalRange.x - visibleRange.x;

            Vector2 offset = Offset;
            offset.x = scrollableRange * position;

            Offset = offset;
        }

        /// <summary>
        /// Updates the size of both scrollbars depending on the currently visible curve area vs. the total curve area.
        /// </summary>
        private void UpdateScrollBarSize()
        {
            Vector2 visibleRange = Range;
            Vector2 totalRange = GetTotalRange();

            horzScrollBar.HandleSize = visibleRange.x / totalRange.x;
            vertScrollBar.HandleSize = visibleRange.y / totalRange.y;
        }

        /// <summary>
        /// Updates the position of both scrollbars depending on the offset currently applied to the visible curve area.
        /// </summary>
        private void UpdateScrollBarPosition()
        {
            Vector2 visibleRange = Range;
            Vector2 totalRange = GetTotalRange();
            Vector2 scrollableRange = totalRange - visibleRange;

            Vector2 offset = Offset;
            if (scrollableRange.x > 0.0f)
                horzScrollBar.Position = offset.x / scrollableRange.x;
            else
                horzScrollBar.Position = 0.0f;

            if (scrollableRange.y > 0.0f)
            {
                float pos = offset.y/scrollableRange.y;
                float sign = MathEx.Sign(pos);
                pos = sign*MathEx.Clamp01(MathEx.Abs(pos));
                pos = (1.0f - pos) /2.0f;

                vertScrollBar.Position = pos;
            }
            else
                vertScrollBar.Position = 0.0f;
        }

        /// <summary>
        /// Calculates the width/height of the curve area depending on the current zoom level.
        /// </summary>
        /// <returns>Width/height of the curve area, in curve space (value, time).</returns>
        private Vector2 GetZoomedRange()
        {
            float zoomLevel = MathEx.Pow(2, zoomAmount);

            GetOptimalRangeAndOffset(out _, out var optimalRange);
            return optimalRange / zoomLevel;
        }

        /// <summary>
        /// Returns the total width/height of the contents of the curve area.
        /// </summary>
        /// <returns>Width/height of the curve area, in curve space (value, time).</returns>
        private Vector2 GetTotalRange()
        {
            // Return optimal range (that covers the visible curve)
            GetOptimalRangeAndOffset(out _, out var range);

            // Increase range in case user zoomed out
            Vector2 zoomedRange = GetZoomedRange();
            return Vector2.Max(range, zoomedRange);
        }

        /// <summary>
        /// Zooms in or out at the provided position in the curve display.
        /// </summary>
        /// <param name="curvePos">Position to zoom towards, relative to the curve display area, in curve space 
        ///                        (value, time)</param>
        /// <param name="amount">Amount to zoom in (positive), or out (negative).</param>
        private void Zoom(Vector2 curvePos, float amount)
        {
            // Increase or decrease the visible range depending on zoom level
            Vector2 oldZoomedRange = GetZoomedRange();
            zoomAmount = MathEx.Clamp(zoomAmount + amount, -10.0f, 10.0f);
            Vector2 zoomedRange = GetZoomedRange();

            Vector2 zoomedDiff = zoomedRange - oldZoomedRange;

            Vector2 currentRange = Range;
            Vector2 newRange = currentRange + zoomedDiff;
            newRange.x = Math.Max(0, newRange.x);
            Range = newRange;

            // When zooming, make sure to focus on the point provided, so adjust the offset
            Vector2 rangeScale = newRange;
            rangeScale.x /= currentRange.x;
            rangeScale.y /= currentRange.y;

            UpdateScrollBarSize();
            UpdateScrollBarPosition();
        }

        /// <summary>
        /// Updates the offset and range of the curve display to fully fit the currently selected set of curves.
        /// </summary>
        /// <param name="resetTime">If true the time offset/range will be recalculated, otherwise current time offset will
        ///                         be kept as is.</param>
        internal void CenterAndResize(bool resetTime)
        {
            GetOptimalRangeAndOffset(out var offset, out var range);

            if (!resetTime)
            {
                offset.x = Offset.x;
                range.x = Range.x;
            }

            Range = range;
            Offset = offset;

            UpdateScrollBarPosition();
            UpdateScrollBarSize();
        }

        /// <summary>
        /// Triggered when the user moves or resizes the horizontal scrollbar.
        /// </summary>
        /// <param name="position">New position of the scrollbar, in range [0, 1].</param>
        /// <param name="size">New size of the scrollbar, in range [0, 1].</param>
        private void OnHorzScrollOrResize(float position, float size)
        {
            SetHorzScrollbarProperties(position, size);
        }

        /// <summary>
        /// Triggered when the user moves or resizes the vertical scrollbar.
        /// </summary>
        /// <param name="position">New position of the scrollbar, in range [0, 1].</param>
        /// <param name="size">New size of the scrollbar, in range [0, 1].</param>
        private void OnVertScrollOrResize(float position, float size)
        {
            SetVertScrollbarProperties(position, size);
        }

        #endregion

        #region Helpers

        /// <summary>
        /// Returns width/height required to show the entire contents of the currently displayed curves.
        /// </summary>
        /// <param name="offset">Offset used for centering the curves.</param>
        /// <param name="range">Range representing the width/height in curve space (time, value). </param>
        private void GetOptimalRangeAndOffset(out Vector2 offset, out Vector2 range)
        {
            AnimationCurve[] curves = new AnimationCurve[curveInfos.Length];
            for(int i = 0; i < curveInfos.Length; i++)
                curves[i] = curveInfos[i].curve.Normal;

            float xMin, xMax;
            float yMin, yMax;
            AnimationUtility.CalculateRange(curves, out xMin, out xMax, out yMin, out yMax);

            float xRange = xMax - xMin;

            float yRange = (yMax - yMin) * 0.5f;
            float yOffset = yMin + yRange;

            // Add padding to y range
            yRange *= 1.05f;

            // Don't allow zero range
            if (xRange == 0.0f)
                xRange = 60.0f;

            if (yRange == 0.0f)
                yRange = 10.0f;

            offset = new Vector2(xMin, yOffset);
            range = new Vector2(xRange, yRange);
        }

        /// <summary>
        /// Returns a set of current curve draw infos with non-editor curves.
        /// </summary>
        /// <returns>Curve draw infos.</returns>
        private CurveDrawInfo[] GetPlainCurveDrawInfos()
        {
            CurveDrawInfo[] output = new CurveDrawInfo[curveInfos.Length];
            for(int i = 0; i < curveInfos.Length; i++)
                output[i] = new CurveDrawInfo(curveInfos[i].curve.Normal, curveInfos[i].color);

            return output;
        }


        /// <summary>
        /// Converts coordinate in curve space (time, value) into pixel coordinates relative to the curve drawing area
        /// origin.
        /// </summary>
        /// <param name="curveCoords">Time and value of the location to convert.</param>
        /// <returns>Coordinates relative to curve drawing area's origin, in pixels.</returns>
        public Vector2I CurveToPixelSpace(Vector2 curveCoords)
        {
            return guiCurveDrawing.CurveToPixelSpace(curveCoords);
        }

        /// <summary>
        /// Converts coordinates in window space (relative to the parent window origin) into coordinates in curve space.
        /// </summary>
        /// <param name="pointerPos">Coordinates relative to this GUI element, in pixels.</param>
        /// <param name="curveCoord">Curve coordinates within the range as specified by <see cref="Range"/>. Only
        ///                          valid when function returns true.</param>
        /// <returns>True if the coordinates are within the curve area, false otherwise.</returns>
        public bool WindowToCurveSpace(Vector2I pointerPos, out Vector2 curveCoord)
        {
            Rect2I drawingBounds = drawingPanel.Bounds;
            Vector2I drawingPos = pointerPos - new Vector2I(drawingBounds.x, drawingBounds.y);

            return guiCurveDrawing.PixelToCurveSpace(drawingPos, out curveCoord);
        }

        #endregion
    }    
    
    /// <summary>
    /// Information necessary to drawing an editor curve.
    /// </summary>
    public struct EdCurveDrawInfo
    {
        public EdCurveDrawInfo(EdAnimationCurve curve, Color color)
        {
            this.curve = curve;
            this.color = color;
        }

        public EdAnimationCurve curve;
        public Color color;
    }

    /// <summary>
    /// Drop down window that displays input boxes used for editing a keyframe.
    /// </summary>
    [DefaultSize(120, 80)]
    internal class KeyframeEditWindow : DropDownWindow
    {
        private Action<bool> closeCallback;
        private bool changesMade;

        /// <summary>
        /// Initializes the drop down window by creating the necessary GUI. Must be called after construction and before
        /// use.
        /// </summary>
        /// <param name="keyFrame">Keyframe whose properties to edit.</param>
        /// <param name="updateCallback">Callback triggered when event values change.</param>
        /// <param name="closeCallback">Callback triggered just before the window closes.</param>
        internal void Initialize(KeyFrame keyFrame, Action<KeyFrame> updateCallback, Action<bool> closeCallback)
        {
            GUIFloatField timeField = new GUIFloatField(new LocEdString("Time"), 40, "");
            timeField.Value = keyFrame.time;
            timeField.OnChanged += x => { keyFrame.time = x; changesMade = true; updateCallback(keyFrame); };

            GUIFloatField valueField = new GUIFloatField(new LocEdString("Value"), 40, "");
            valueField.Value = keyFrame.value;
            valueField.OnChanged += x => { keyFrame.value = x; changesMade = true; updateCallback(keyFrame); };

            GUILayoutY vertLayout = GUI.AddLayoutY();

            vertLayout.AddFlexibleSpace();
            GUILayoutX horzLayout = vertLayout.AddLayoutX();
            horzLayout.AddFlexibleSpace();
            GUILayout contentLayout = horzLayout.AddLayoutY();
            GUILayout timeLayout = contentLayout.AddLayoutX();
            timeLayout.AddSpace(5);
            timeLayout.AddElement(timeField);
            timeLayout.AddFlexibleSpace();
            GUILayout componentLayout = contentLayout.AddLayoutX();
            componentLayout.AddSpace(5);
            componentLayout.AddElement(valueField);
            componentLayout.AddFlexibleSpace();
            horzLayout.AddFlexibleSpace();
            vertLayout.AddFlexibleSpace();

            this.closeCallback = closeCallback;
        }

        private void OnDestroy()
        {
            closeCallback?.Invoke(changesMade);
        }
    }

    /// <summary>
    /// Drop down window that displays input boxes used for editing an event.
    /// </summary>
    [DefaultSize(200, 80)]
    internal class EventEditWindow : DropDownWindow
    {
        private Action<bool> closeCallback;
        private bool changesMade;

        /// <summary>
        /// Initializes the drop down window by creating the necessary GUI. Must be called after construction and before
        /// use.
        /// </summary>
        /// <param name="animEvent">Event whose properties to edit.</param>
        /// <param name="componentNames">List of component names that the user can select from.</param>
        /// <param name="updateCallback">Callback triggered when event values change.</param>
        /// <param name="closeCallback">Callback triggered just before the window closes.</param>
        internal void Initialize(AnimationEvent animEvent, string[] componentNames, Action updateCallback, 
            Action<bool> closeCallback)
        {
            int selectedIndex = -1;
            string methodName = "";
            if (!string.IsNullOrEmpty(animEvent.name))
            {
                string[] nameEntries = animEvent.name.Split('/');
                if (nameEntries.Length > 1)
                {
                    string typeName = nameEntries[0];
                    for (int i = 0; i < componentNames.Length; i++)
                    {
                        if (componentNames[i] == typeName)
                        {
                            selectedIndex = i;
                            break;
                        }
                    }

                    methodName = nameEntries[nameEntries.Length - 1];
                }
            }

            GUIFloatField timeField = new GUIFloatField(new LocEdString("Time"), 40, "");
            timeField.Value = animEvent.time;
            timeField.OnChanged += x => { animEvent.time = x; changesMade = true; updateCallback(); };

            GUIListBoxField componentField = new GUIListBoxField(componentNames, new LocEdString("Component"), 40);
            if (selectedIndex != -1)
                componentField.Index = selectedIndex;

            componentField.OnSelectionChanged += x =>
            {
                string compName = "";
                if (x != -1)
                    compName = componentNames[x] + "/";

                animEvent.name = compName + x;

                changesMade = true;
                updateCallback();
            };

            GUITextField methodField = new GUITextField(new LocEdString("Method"), 40, false, "", GUIOption.FixedWidth(190));
            methodField.Value = methodName;
            methodField.OnChanged += x =>
            {
                string compName = "";
                if(componentField.Index != -1)
                    compName = componentNames[componentField.Index] + "/";

                animEvent.name = compName + x;

                changesMade = true;
                updateCallback();
            };

            GUILayoutY vertLayout = GUI.AddLayoutY();

            vertLayout.AddFlexibleSpace();
            GUILayoutX horzLayout = vertLayout.AddLayoutX();
            horzLayout.AddFlexibleSpace();
            GUILayout contentLayout = horzLayout.AddLayoutY();
            GUILayout timeLayout = contentLayout.AddLayoutX();
            timeLayout.AddSpace(5);
            timeLayout.AddElement(timeField);
            timeLayout.AddFlexibleSpace();
            GUILayout componentLayout = contentLayout.AddLayoutX();
            componentLayout.AddSpace(5);
            componentLayout.AddElement(componentField);
            componentLayout.AddFlexibleSpace();
            GUILayout methodLayout = contentLayout.AddLayoutX();
            methodLayout.AddSpace(5);
            methodLayout.AddElement(methodField);
            methodLayout.AddFlexibleSpace();
            horzLayout.AddFlexibleSpace();
            vertLayout.AddFlexibleSpace();

            this.closeCallback = closeCallback;
        }

        private void OnDestroy()
        {
            closeCallback?.Invoke(changesMade);
        }
    }

    /** @} */
}
