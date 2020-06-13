//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using bs;

namespace bs.Editor
{
    /** @addtogroup Utility-Editor
     *  @{
     */

    /// <summary>
    /// Handles undo & redo operations for changes made on game objects. Game objects can be recorded just before a change
    /// is made and the system will calculate the difference between that state and the state at the end of current frame.
    /// This difference will then be recorded as a undo/redo operation. The undo/redo operation will also take care of
    /// selecting the object & field it is acting upon.
    /// </summary>
    internal class GameObjectUndo
    {
        /// <summary>
        /// Contains information about a component that needs its diff recorded.
        /// </summary>
        private struct ComponentToRecord
        {
            private Component obj;
            private string path;
            private SerializedObject orgState;

            /// <summary>
            /// Creates a new object instance, recording the current state of the component.
            /// </summary>
            /// <param name="obj">Component to record the state of.</param>
            /// <param name="path">
            /// Path to the field which should be focused when performing the undo/redo operation. This should be the path
            /// as provided by <see cref="InspectableField"/>.
            /// </param>
            internal ComponentToRecord(Component obj, string path)
            {
                this.obj = obj;
                this.path = path;

                orgState = SerializedObject.Create(obj);
            }

            /// <summary>
            /// Generates the diff from the previously recorded state and the current state. If there is a difference
            /// an undo command is recorded.
            /// </summary>
            internal void RecordCommand()
            {
                if (obj.IsDestroyed)
                    return;

                SerializedObject newState = SerializedObject.Create(obj);

                SerializedDiff oldToNew = SerializedDiff.Create(orgState, newState);
                if (oldToNew == null || oldToNew.IsEmpty)
                    return;

                SerializedDiff newToOld = SerializedDiff.Create(newState, orgState);
                UndoRedo.Global.RegisterCommand(new RecordComponentUndo(obj, path, oldToNew, newToOld));
            }
        }

        /// <summary>
        /// Contains information about scene objects that needs their diff recorded. Note this will not record the entire
        /// scene object, but rather just its name, transform, active state and potentially other similar properties.
        /// It's components as well as hierarchy state are ignored.
        /// </summary>
        private struct SceneObjectHeaderToRecord
        {
            private SceneObject[] objs;
            private string path;
            private SceneObjectState[] orgStates;

            /// <summary>
            /// Creates a new object instance, recording the current state of the scene object header.
            /// </summary>
            /// <param name="obj">Scene object to record the state of.</param>
            /// <param name="path">
            /// Path to the field which should be focused when performing the undo/redo operation.
            /// </param>
            internal SceneObjectHeaderToRecord(SceneObject obj, string path)
            {
                this.objs = new [] { obj };
                this.path = path;

                orgStates = new [] { SceneObjectState.Create(obj) };
            }

            /// <summary>
            /// Creates a new object instance, recording the current state of the scene object header for multiple scene
            /// objects.
            /// </summary>
            /// <param name="objs">Scene objects to record the state of.</param>
            /// <param name="path">
            /// Path to the field which should be focused when performing the undo/redo operation.
            /// </param>
            internal SceneObjectHeaderToRecord(SceneObject[] objs, string path)
            {
                this.objs = objs;
                this.path = path;

                orgStates = new SceneObjectState[objs.Length];
                for(int i = 0; i < orgStates.Length; i++)
                    orgStates[i] = SceneObjectState.Create(objs[i]);
            }

            /// <summary>
            /// Generates the diff from the previously recorded state and the current state. If there is a difference
            /// an undo command is recorded.
            /// </summary>
            internal void RecordCommand()
            {
                if (objs == null)
                    return;

                List<SceneObjectHeaderUndo> headers = new List<SceneObjectHeaderUndo>();
                for (int i = 0; i < objs.Length; i++)
                {
                    SceneObject obj = objs[i];
                    SceneObjectState orgState = orgStates[i];

                    if (obj.IsDestroyed)
                        continue;

                    SceneObjectDiff oldToNew = SceneObjectDiff.Create(orgState, SceneObjectState.Create(obj));
                    if (oldToNew.flags == 0)
                        continue;

                    SceneObjectDiff newToOld = SceneObjectDiff.Create(SceneObjectState.Create(obj), orgState);
                    headers.Add(new SceneObjectHeaderUndo(obj, newToOld, oldToNew));

                }

                if (headers.Count > 0)
                    UndoRedo.Global.RegisterCommand(new RecordSceneObjectHeaderUndo(headers, path));
            }
        }

        /// <summary>
        /// Contains information about a scene object that needs its diff recorded. Unlike
        /// <see cref="SceneObjectHeaderToRecord"/> this will record the entire scene object,
        /// including its components and optionally the child hierarchy.
        /// </summary>
        private struct SceneObjectToRecord
        {
            private SceneObject obj;
            private string description;
            private SerializedSceneObject orgState;

            /// <summary>
            /// Creates a new object instance, recording the current state of the scene object.
            /// </summary>
            /// <param name="obj">Scene object to record the state of.</param>
            /// <param name="hierarchy">If true, the child scene objects will be recorded as well.</param>
            /// <param name="description">
            /// Optional description that describes the change that is happening.
            /// </param>
            internal SceneObjectToRecord(SceneObject obj, bool hierarchy, string description)
            {
                this.obj = obj;
                this.description = description;

                orgState = new SerializedSceneObject(obj, hierarchy);
            }

            /// <summary>
            /// Generates the diff from the previously recorded state and the current state. If there is a difference
            /// an undo command is recorded.
            /// </summary>
            internal void RecordCommand()
            {
                if (obj.IsDestroyed)
                    return;

                var newState = new SerializedSceneObject(obj);
                UndoRedo.Global.RegisterCommand(new RecordSceneObjectUndo(obj, orgState, newState, description));
            }
        }


        /// <summary>
        /// Contains information about a newly created scene object and its state immediately following the creation.
        /// </summary>
        private struct NewSceneObjectToRecord
        {
            private SceneObject obj;

            /// <summary>
            /// Creates a new object instance, recording that a new scene object has been created.
            /// </summary>
            /// <param name="obj">Newly created scene object.</param>
            internal NewSceneObjectToRecord(SceneObject obj)
            {
                this.obj = obj;
            }

            /// <summary>
            /// Records the current scene object state.
            /// </summary>
            internal void RecordCommand()
            {
                if (obj.IsDestroyed)
                    return;

                var state = new SerializedSceneObject(obj);
                UndoRedo.Global.RegisterCommand(new NewSceneObjectUndo(obj, state));
            }
        }

        private static List<ComponentToRecord> components = new List<ComponentToRecord>();
        private static List<SceneObjectHeaderToRecord> sceneObjectHeaders = new List<SceneObjectHeaderToRecord>();
        private static List<SceneObjectToRecord> sceneObjects = new List<SceneObjectToRecord>();
        private static List<NewSceneObjectToRecord> newSceneObjects = new List<NewSceneObjectToRecord>();

        /// <summary>
        /// Records the current state of the provided component, and generates a diff with the next state at the end of the
        /// frame. If change is detected an undo operation will be recorded. Generally you want to call this just before
        /// you are about to make a change to the component.
        /// </summary>
        /// <param name="obj">Component to record the state of.</param>
        /// <param name="fieldPath">
        /// Path to the field which should be focused when performing the undo/redo operation. This should be the path
        /// as provided by <see cref="InspectableField"/>.
        /// </param>
        public static void RecordComponent(Component obj, string fieldPath)
        {
            ComponentToRecord cmp = new ComponentToRecord(obj, fieldPath);
            components.Add(cmp);
        }

        /// <summary>
        /// Records the current state of the provided scene object header, and generates a diff with the next state at the
        /// end of the frame. If change is detected an undo operation will be recorded. Generally you want to call this
        /// just before you are about to make a change to the scene object header.
        ///
        /// Note this will not record the entire scene object, but rather just its name, transform, active state and
        /// potentially other similar properties. It's components as well as hierarchy state are ignored.
        /// </summary>
        /// <param name="obj">Scene object to record the state of.</param>
        /// <param name="fieldName">
        /// Name to the field which should be focused when performing the undo/redo operation.
        /// </param>
        public static void RecordSceneObjectHeader(SceneObject obj, string fieldName)
        {
            SceneObjectHeaderToRecord so = new SceneObjectHeaderToRecord(obj, fieldName);
            sceneObjectHeaders.Add(so);
        }

        /// <summary>
        /// Records the current state of the provided scene object header, and generates a diff with the next state at the
        /// end of the frame. If change is detected an undo operation will be recorded. Generally you want to call this
        /// just before you are about to make a change to the scene object header.
        ///
        /// Note this will not record the entire scene object, but rather just its name, transform, active state and
        /// potentially other similar properties. It's components as well as hierarchy state are ignored.
        /// </summary>
        /// <param name="objs">Scene objects to record the state of.</param>
        public static void RecordSceneObjectHeader(SceneObject[] objs)
        {
            SceneObjectHeaderToRecord so = new SceneObjectHeaderToRecord(objs, null);
            sceneObjectHeaders.Add(so);
        }

        /// <summary>
        /// Records the current state of the provided scene object header, and generates a diff with the next state at the
        /// end of the frame. If change is detected an undo operation will be recorded. Generally you want to call this
        /// just before you are about to make a change to the scene object.
        /// 
        /// Note this records the complete state of a scene object, including its header, its components and optionally its
        /// child hierarchy.
        /// </summary>
        /// <param name="obj">Scene object to record.</param>
        /// <param name="hierarchy">
        /// If true the child objects will be recorded as well, otherwise just the provided object.
        /// </param>
        /// <param name="description">Optional description specifying the type of changes about to be made.</param>
        public static void RecordSceneObject(SceneObject obj, bool hierarchy, string description)
        {
            SceneObjectToRecord so = new SceneObjectToRecord(obj, hierarchy, description);
            sceneObjects.Add(so);
        }

        /// <summary>
        /// Creates a brand new scene object and begins <see cref="RecordSceneObject(SceneObject,bool,string)"/> operation
        /// on the newly created scene object. Both the creation and the initially recorded set of data will be recorded
        /// as a single undo/redo command. Generally you want to call this when creating a new scene object and immediately
        /// make some initial changes to it (such as adding component or modifying their properties).
        /// </summary>
        /// <param name="obj">Newly created scene object on which to initiate the record operation on.</param>
        public static void RecordNewSceneObject(SceneObject obj)
        {
            if (obj == null || obj.IsDestroyed)
                return;

            NewSceneObjectToRecord record = new NewSceneObjectToRecord(obj);
            newSceneObjects.Add(record);

            Selection.SceneObject = obj;
        }

        /// <summary>
        /// Generates diffs for any objects that were previously recorded using any of the Record* methods. The diff is
        /// generated by comparing the state at the time Record* was called, compared to the current object state.
        /// </summary>
        public static void ResolveDiffs()
        {
            foreach (var entry in components)
                entry.RecordCommand();

            foreach (var entry in sceneObjectHeaders)
                entry.RecordCommand();

            foreach (var entry in sceneObjects)
                entry.RecordCommand();

            foreach (var entry in newSceneObjects)
                entry.RecordCommand();

            components.Clear();
            sceneObjectHeaders.Clear();
            sceneObjects.Clear();
            newSceneObjects.Clear();
        }
    }

    /// <summary>
    /// Contains information about scene object state, excluding information about its components and hierarchy.
    /// </summary>
    internal struct SceneObjectState
    {
        internal string name;
        internal Vector3 position;
        internal Quaternion rotation;
        internal Vector3 scale;
        internal bool active;

        /// <summary>
        /// Initializes the state from a scene object.
        /// </summary>
        /// <param name="so">Scene object to initialize the state from.</param>
        /// <returns>New state object.</returns>
        internal static SceneObjectState Create(SceneObject so)
        {
            SceneObjectState state = new SceneObjectState();
            state.name = so.Name;
            state.position = so.LocalPosition;
            state.rotation = so.LocalRotation;
            state.scale = so.LocalScale;
            state.active = so.Active;

            return state;
        }
    }

    /// <summary>
    /// Contains the difference between two <see cref="SceneObjectState"/> objects and allows the changes to be applied to
    /// a <see cref="SceneObject"/>. The value of the different fields is stored as its own state, while the flags field
    /// specified which of the properties is actually different.
    /// </summary>
    internal struct SceneObjectDiff
    {
        internal SceneObjectState state;
        internal SceneObjectDiffFlags flags;

        /// <summary>
        /// Creates a diff object storing the difference between two <see cref="SceneObjectState"/> objects.
        /// </summary>
        /// <param name="oldState">State of the scene object to compare from.</param>
        /// <param name="newState">State of the scene object to compare to.</param>
        /// <returns>Difference between the two scene object states.</returns>
        internal static SceneObjectDiff Create(SceneObjectState oldState, SceneObjectState newState)
        {
            SceneObjectDiff diff = new SceneObjectDiff();
            diff.state = new SceneObjectState();

            if (oldState.name != newState.name)
            {
                diff.state.name = newState.name;
                diff.flags |= SceneObjectDiffFlags.Name;
            }

            if (oldState.position != newState.position)
            {
                diff.state.position = newState.position;
                diff.flags |= SceneObjectDiffFlags.Position;
            }

            if (oldState.rotation != newState.rotation)
            {
                diff.state.rotation = newState.rotation;
                diff.flags |= SceneObjectDiffFlags.Rotation;
            }

            if (oldState.scale != newState.scale)
            {
                diff.state.scale = newState.scale;
                diff.flags |= SceneObjectDiffFlags.Scale;
            }

            if (oldState.active != newState.active)
            {
                diff.state.active = newState.active;
                diff.flags |= SceneObjectDiffFlags.Active;
            }

            return diff;
        }

        /// <summary>
        /// Applies the diff to an actual scene object.
        /// </summary>
        /// <param name="sceneObject">Scene object to apply the diff to.</param>
        internal void Apply(SceneObject sceneObject)
        {
            if (flags.HasFlag(SceneObjectDiffFlags.Name))
                sceneObject.Name = state.name;

            if (flags.HasFlag(SceneObjectDiffFlags.Position))
                sceneObject.LocalPosition = state.position;

            if (flags.HasFlag(SceneObjectDiffFlags.Rotation))
                sceneObject.LocalRotation = state.rotation;

            if (flags.HasFlag(SceneObjectDiffFlags.Scale))
                sceneObject.LocalScale = state.scale;

            if (flags.HasFlag(SceneObjectDiffFlags.Active))
                sceneObject.Active = state.active;
        }
    }

    /// <summary>
    /// Contains information about two separate states of a scene object header.
    /// </summary>
    internal struct SceneObjectHeaderUndo
    {
        internal SceneObject obj;
        internal SceneObjectDiff newToOld;
        internal SceneObjectDiff oldToNew;

        internal SceneObjectHeaderUndo(SceneObject obj, SceneObjectDiff newToOld, SceneObjectDiff oldToNew)
        {
            this.obj = obj;
            this.newToOld = newToOld;
            this.oldToNew = oldToNew;
        }
    }

    /// <summary>
    /// Stores the field changes in a <see cref="SceneObject"/> as a difference between two states. Allows those changes to
    /// be reverted and re-applied. Does not record changes to scene object components or hierarchy, but just the fields
    /// considered its header (such as name, local transform and active state).
    /// </summary>
    [SerializeObject]
    internal class RecordSceneObjectHeaderUndo : UndoableCommand
    {
        private string fieldPath;
        private List<SceneObjectHeaderUndo> headers;

        private RecordSceneObjectHeaderUndo() { }

        /// <summary>
        /// Creates the new scene object header undo command.
        /// </summary>
        /// <param name="headers">Information about recorded states of scene object headers.</param>
        /// <param name="fieldPath">
        /// Optional path that controls which is the field being modified and should receive input focus when the command
        /// is executed. Note that the diffs applied have no restriction on how many fields they can modify at once, but
        /// only one field will receive focus.</param>
        public RecordSceneObjectHeaderUndo(List<SceneObjectHeaderUndo> headers, string fieldPath)
        {
            this.headers = headers;
            this.fieldPath = fieldPath;
        }

        /// <inheritdoc/>
        protected override void Commit()
        {
            if (headers == null)
                return;

            foreach (var header in headers)
            {
                if (header.obj == null)
                    continue;

                if (header.obj.IsDestroyed)
                {
                    Debug.LogWarning("Attempting to commit state on a destroyed game-object.");
                    continue;
                }

                header.oldToNew.Apply(header.obj);
            }

            FocusOnField();
            RefreshInspector();
        }

        /// <inheritdoc/>
        protected override void Revert()
        {
            if (headers == null)
                return;

            foreach (var header in headers)
            {
                if (header.obj == null)
                    continue;

                if (header.obj.IsDestroyed)
                {
                    Debug.LogWarning("Attempting to revert state on a destroyed game-object.");
                    continue;
                }

                header.newToOld.Apply(header.obj);
            }

            FocusOnField();
            RefreshInspector();
        }

        /// <summary>
        /// Selects the component's scene object and focuses on the specific field in the inspector, if the inspector
        /// window is open.
        /// </summary>
        private void FocusOnField()
        {
            if (headers != null && headers.Count > 0)
            {
                if (headers.Count == 1)
                {
                    if (Selection.SceneObject != headers[0].obj)
                        Selection.SceneObject = headers[0].obj;
                }
                else
                {
                    List<SceneObject> objs = new List<SceneObject>();
                    foreach (var header in headers)
                    {
                        if(header.obj != null)
                            objs.Add(header.obj);
                    }

                    Selection.SceneObjects = objs.ToArray();
                }

                if (!string.IsNullOrEmpty(fieldPath))
                {
                    InspectorWindow inspectorWindow = EditorWindow.GetWindow<InspectorWindow>();
                    inspectorWindow?.FocusOnField(headers[0].obj.UUID, fieldPath);
                }
            }
        }

        /// <summary>
        /// Updates the values of the fields displayed in the inspector window.
        /// </summary>
        private void RefreshInspector()
        {
            InspectorWindow inspectorWindow = EditorWindow.GetWindow<InspectorWindow>();
            inspectorWindow?.RefreshSceneObjectFields(true);
        }
    }

    /// <summary>
    /// Records the creation of a new <see cref="SceneObject"/> and its recorded state immediately following its creation.
    /// Undo will destroy the created object, while redo will restore the object with the recorded state.
    /// </summary>
    [SerializeObject]
    internal class NewSceneObjectUndo : UndoableCommand
    {
        private SceneObject obj;
        private SerializedSceneObject state;

        private NewSceneObjectUndo() { }

        /// <summary>
        /// Creates the new scene object undo command.
        /// </summary>
        /// <param name="obj">Newly created scene object on which to apply the command.</param>
        /// <param name="state">Recorded state of the scene object.</param>
        public NewSceneObjectUndo(SceneObject obj, SerializedSceneObject state)
        {
            this.obj = obj;
            this.state = state;
        }

        /// <inheritdoc/>
        protected override void Commit()
        {
            state?.Restore();

            if(obj != null && !obj.IsDestroyed)
                Selection.SceneObject = obj;

            FocusOnObject();
            RefreshInspector();
        }

        /// <inheritdoc/>
        protected override void Revert()
        {
            obj.Destroy(true);
        }

        /// <summary>
        /// Selects the scene object if not already selected.
        /// </summary>
        private void FocusOnObject()
        {
            if (obj != null && !obj.IsDestroyed)
            {
                if (Selection.SceneObject != obj)
                    Selection.SceneObject = obj;
            }
        }

        /// <summary>
        /// Updates the values of the fields displayed in the inspector window.
        /// </summary>
        private void RefreshInspector()
        {
            InspectorWindow inspectorWindow = EditorWindow.GetWindow<InspectorWindow>();
            inspectorWindow?.RefreshSceneObjectFields(true);
        }
    }

    /// <summary>
    /// Stores the field changes in a <see cref="SceneObject"/> as a difference between two states. Allows those changes to
    /// be reverted and re-applied. Unlike <see cref="bs.Editor.RecordSceneObjectHeaderUndo"/> this command records the
    /// full scene object state, including changes to its components and optionally any child objects.
    /// </summary>
    [SerializeObject]
    internal class RecordSceneObjectUndo : UndoableCommand
    {
        private SceneObject obj;
        private SerializedSceneObject oldState;
        private SerializedSceneObject newState;

        private RecordSceneObjectUndo() { }

        /// <summary>
        /// Creates the new scene object undo command.
        /// </summary>
        /// <param name="obj">Scene object on which to apply the performed changes.</param>
        /// <param name="oldState">Recorded original state of the scene object(s).</param>
        /// <param name="newState">Recorded new state of the scene object(s).</param>
        /// <param name="description">
        /// Optional description of the changes made to the scene object between the two states.
        /// </param>
        public RecordSceneObjectUndo(SceneObject obj, SerializedSceneObject oldState, SerializedSceneObject newState,
            string description)
        {
            this.obj = obj;
            this.oldState = oldState;
            this.newState = newState;
        }

        /// <inheritdoc/>
        protected override void Commit()
        {
            newState?.Restore();

            FocusOnObject();
            RefreshInspector();
        }

        /// <inheritdoc/>
        protected override void Revert()
        {
            oldState?.Restore();

            FocusOnObject();
            RefreshInspector();
        }

        /// <summary>
        /// Selects the scene object if not already selected.
        /// </summary>
        private void FocusOnObject()
        {
            if (obj != null)
            {
                if (Selection.SceneObject != obj)
                    Selection.SceneObject = obj;
            }
        }

        /// <summary>
        /// Updates the values of the fields displayed in the inspector window.
        /// </summary>
        private void RefreshInspector()
        {
            InspectorWindow inspectorWindow = EditorWindow.GetWindow<InspectorWindow>();
            inspectorWindow?.RefreshSceneObjectFields(true);
        }
    }

    /// <summary>
    /// Stores the field changes in a <see cref="Component"/> as a difference between two states. Allows those changes to
    /// be reverted and re-applied. 
    /// </summary>
    [SerializeObject]
    internal class RecordComponentUndo : UndoableCommand
    {
        private Component obj;
        private string fieldPath;
        private SerializedDiff newToOld;
        private SerializedDiff oldToNew;

        private RecordComponentUndo() { }

        /// <summary>
        /// Creates the new component undo command.
        /// </summary>
        /// <param name="obj">Component on which to apply the performed changes.</param>
        /// <param name="fieldPath">
        /// Optional path that controls which is the field being modified and should receive input focus when the command
        /// is executed. Note that the diffs applied have no restriction on how many fields they can modify at once, but
        /// only one field will receive focus.</param>
        /// <param name="oldToNew">
        /// Difference that can be applied to the old object in order to get the new object state.
        /// </param>
        /// <param name="newToOld">
        /// Difference that can be applied to the new object in order to get the old object state.
        /// </param>
        public RecordComponentUndo(Component obj, string fieldPath, SerializedDiff oldToNew, SerializedDiff newToOld)
        {
            this.obj = obj;
            this.fieldPath = fieldPath;
            this.oldToNew = oldToNew;
            this.newToOld = newToOld;
        }

        /// <inheritdoc/>
        protected override void Commit()
        {
            if (oldToNew == null || obj == null)
                return;

            if (obj.IsDestroyed)
            {
                Debug.LogWarning("Attempting to commit state on a destroyed game-object.");
                return;
            }

            oldToNew.Apply(obj);
            FocusOnField();
            RefreshInspector();
        }

        /// <inheritdoc/>
        protected override void Revert()
        {
            if (newToOld == null || obj == null)
                return;

            if (obj.IsDestroyed)
            {
                Debug.LogWarning("Attempting to revert state on a destroyed game-object.");
                return;
            }

            newToOld.Apply(obj);
            FocusOnField();
            RefreshInspector();
        }

        /// <summary>
        /// Selects the component's scene object and focuses on the specific field in the inspector, if the inspector
        /// window is open.
        /// </summary>
        private void FocusOnField()
        {
            SceneObject so = obj.SceneObject;
            if (so != null)
            {
                if (Selection.SceneObject != so)
                    Selection.SceneObject = so;

                if (!string.IsNullOrEmpty(fieldPath))
                {
                    InspectorWindow inspectorWindow = EditorWindow.GetWindow<InspectorWindow>();
                    inspectorWindow?.FocusOnField(obj.UUID, fieldPath);
                }
            }
        }

        /// <summary>
        /// Updates the values of the fields displayed in the inspector window.
        /// </summary>
        private void RefreshInspector()
        {
            InspectorWindow inspectorWindow = EditorWindow.GetWindow<InspectorWindow>();
            inspectorWindow?.RefreshComponentFields(obj);
        }
    }

    /** @} */
}
