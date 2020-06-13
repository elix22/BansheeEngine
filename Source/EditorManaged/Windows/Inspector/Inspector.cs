//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using System;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI elements for all the inspectable fields in an object.
    /// </summary>
    public abstract class Inspector
    {
        public const short START_BACKGROUND_DEPTH = 50;

        /// <summary>
        /// Returns the main GUI layout for the inspector.
        /// </summary>
        protected GUILayoutY Layout
        {
            get { return layout; }
        }

        /// <summary>
        /// Returns the main GUI panel for the inspector. <see cref="Layout"/> is a child of this panel.
        /// </summary>
        protected GUIPanel GUI
        {
            get { return mainPanel; }
        }

        /// <summary>
        /// Returns the secondary GUI panel. Located at the bottom of the inspector window and unlike <see cref="GUI"/> has
        /// no padding or styling applied. Only available when inspecting resources.
        /// </summary>
        protected GUIPanel PreviewGUI
        {
            get { return previewPanel; }
        }

        /// <summary>
        /// Returns the object the inspector is currently displaying. If the current object is a resource use
        /// <see cref="InspectedResourcePath"/> instead;
        /// </summary>
        protected object InspectedObject
        {
            get { return inspectedObject; }
        }

        /// <summary>
        /// Returns the path to the resource the inspector is currently displaying.
        /// </summary>
        protected string InspectedResourcePath
        {
            get { return inspectedResourcePath; }
        }

        /// <summary>
        /// A set of properties that the inspector can read/write. They will be persisted even after the inspector is closed
        /// and restored when it is re-opened.
        /// </summary>
        protected internal SerializableProperties Persistent
        {
            get { return persistent; }
        }

        protected InspectorFieldDrawer drawer;

        private GUIPanel rootGUI;
        private GUIPanel mainPanel;
        private GUIPanel previewPanel;
        private GUILayoutY layout;
        private object inspectedObject;
        private string inspectedResourcePath;
        private SerializableProperties persistent;

        /// <summary>
        /// Common code called by both Initialize() overloads.
        /// </summary>
        /// <param name="mainGui">Primary GUI panel to add the GUI elements to.</param>
        /// <param name="previewGui">Secondary GUI panel located at the bottom of the inspector window, aimed primarily for
        /// resource previews, but can be used for any purpose.</param>
        /// <param name="persistent">A set of properties that the inspector can read/write. They will be persisted even 
        ///                          after the inspector is closed and restored when it is re-opened.</param>
        private void InitializeBase(GUIPanel mainGui, GUIPanel previewGui, SerializableProperties persistent)
        {
            rootGUI = mainGui;
            this.persistent = persistent;

            GUILayout contentLayoutX = mainGui.AddLayoutX();
            contentLayoutX.AddSpace(5);
            GUILayout contentLayoutY = contentLayoutX.AddLayoutY();
            contentLayoutY.AddSpace(5);
            GUIPanel contentPanel = contentLayoutY.AddPanel();
            contentLayoutY.AddSpace(5);
            contentLayoutX.AddSpace(5);

            GUIPanel backgroundPanel = mainGui.AddPanel(START_BACKGROUND_DEPTH);
            GUITexture inspectorContentBg = new GUITexture(null, EditorStylesInternal.InspectorContentBg);
            backgroundPanel.AddElement(inspectorContentBg);

            mainPanel = contentPanel;
            previewPanel = previewGui;
            layout = GUI.AddLayoutY();
        }

        /// <summary>
        /// Initializes the inspector using an object instance. Must be called after construction.
        /// </summary>
        /// <param name="gui">GUI panel to add the GUI elements to.</param>
        /// <param name="instance">Instance of the object whose fields to display GUI for.</param>
        /// <param name="persistent">A set of properties that the inspector can read/write. They will be persisted even 
        ///                          after the inspector is closed and restored when it is re-opened.</param>
        internal virtual void Initialize(GUIPanel gui, object instance, SerializableProperties persistent)
        {
            InitializeBase(gui, null, persistent);
            drawer = new InspectorFieldDrawer(new InspectableContext(Persistent, instance as Component), Layout);

            inspectedObject = instance;

            Initialize();
            Refresh();
        }

        /// <summary>
        /// Initializes the inspector using a resource path. Must be called after construction.
        /// </summary>
        /// <param name="mainGui">Primary GUI panel to add the GUI elements to.</param>
        /// <param name="previewGui">Secondary GUI panel located at the bottom of the inspector window, aimed primarily for
        /// resource previews, but can be used for any purpose.</param>
        /// <param name="path">Path to the resource for which to display GUI for.</param>
        /// <param name="persistent">A set of properties that the inspector can read/write. They will be persisted even 
        ///                          after the inspector is closed and restored when it is re-opened.</param>
        internal virtual void Initialize(GUIPanel mainGui, GUIPanel previewGui, string path, 
            SerializableProperties persistent)
        {
            InitializeBase(mainGui, previewGui, persistent);
            drawer = new InspectorFieldDrawer(new InspectableContext(Persistent), Layout);

            inspectedResourcePath = path;

            Initialize();
            Refresh();
        }

        /// <summary>
        /// Changes keyboard focus to the provided field.
        /// </summary>
        /// <param name="path">Path to the field on the object being inspected.</param>
        internal virtual void FocusOnField(string path)
        {
            drawer.FocusOnField(path);
        }

        /// <summary>
        /// Zero parameter wrapper for <see cref="StartUndo(string)"/>
        /// </summary>
        protected void StartUndo()
        {
            StartUndo(null);
        }

        /// <summary>
        /// Notifies the system to start recording a new undo command. Any changes to the field after this is called
        /// will be recorded in the command. User must call <see cref="EndUndo"/> after field is done being changed.
        /// </summary>
        /// <param name="field">Name of the field being modified.</param>
        protected void StartUndo(string field)
        {
            if (inspectedObject is Component component)
                GameObjectUndo.RecordComponent(component, field);
        }

        /// <summary>
        /// Finishes recording an undo command started via <see cref="StartUndo(string)"/>. If any changes are detected on
        /// the object an undo command is recorded onto the undo-redo stack, otherwise nothing is done.
        /// </summary>
        protected void EndUndo()
        {
            GameObjectUndo.ResolveDiffs();
        }

        /// <summary>
        /// Loads the currently inspected resource into the <see cref="InspectedObject"/> field. By default resources
        /// are not loaded and you can only retrieve their path through <see cref="InspectedResourcePath"/>.
        /// </summary>
        protected void LoadResource()
        {
            if(!string.IsNullOrEmpty(inspectedResourcePath))
                inspectedObject = ProjectLibrary.Load<Resource>(inspectedResourcePath);
        }

        /// <summary>
        /// Hides or shows the inspector GUI elements.
        /// </summary>
        /// <param name="visible">True to make the GUI elements visible.</param>
        internal virtual void SetVisible(bool visible)
        {
            rootGUI.Active = visible;
        }
        
        /// <summary>
        /// Destroys all inspector GUI elements.
        /// </summary>
        internal void Destroy()
        {
            Layout.Destroy();
            GUI.Destroy();
        }

        /// <summary>
        /// Called when the inspector is first created.
        /// </summary>
        protected internal abstract void Initialize();

        /// <summary>
        /// Checks if contents of the inspector have been modified, and updates them if needed.
        /// </summary>
        /// <param name="force">Forces the GUI fields to display the latest values assigned on the object.</param>
        /// <returns>State representing was anything modified between two last calls to <see cref="Refresh"/>.</returns>
        protected internal virtual InspectableState Refresh(bool force = false)
        {
            return drawer.Refresh(force);
        }
    }

    /// <summary>
    /// Helper class that draws the inspector field elements for an object, allowing you to draw default set of fields,
    /// override certain fields with your own, or add new fields manually.
    /// </summary>
    public sealed class InspectorFieldDrawer
    {
        /// <summary>
        /// Contains information about a conditional that determines whether a field will be active or not.
        /// </summary>
        private struct ConditionalInfo
        {
            public Func<bool> callback;
            public string field;

            public ConditionalInfo(string field, Func<bool> callback)
            {
                this.field = field;
                this.callback = callback;
            }
        }

        /// <summary>
        /// Allows the user to override the default inspector GUI for a specific field in an object. If this method
        /// returns null the default field will be used instead.
        /// </summary>
        /// <param name="field">Field to generate inspector GUI for.</param>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="path">Full path to the provided field (includes name of this field and all parent fields).</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="layoutIndex">Index into the parent layout at which to insert the GUI elements for the field .</param>
        /// <param name="depth">
        /// Determines how deep within the inspector nesting hierarchy is this field. Some fields may contain other fields,
        /// in which case you should increase this value by one.
        /// </param>
        /// <returns>
        /// Inspectable field implementation that can be used for displaying the GUI for the provided field. Or null if
        /// default field GUI should be used instead.
        /// </returns>
        public delegate InspectableField FieldOverrideCallback(SerializableField field, InspectableContext context, string path,
            InspectableFieldLayout layout, int layoutIndex, int depth);

        /// <summary>
        /// List of fields created and updated by the drawer.
        /// </summary>
        public List<InspectableField> Fields { get; } = new List<InspectableField>();

        private InspectableContext context;
        private GUILayoutY layout;
        private string path;
        private int depth;
        private int rootIndex;
        private List<ConditionalInfo> conditionals;

        // Category
        private int categoryIndex;
        private string categoryName;
        private InspectableCategory category;

        /// <summary>
        /// Creates new empty inspector field drawer.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="layout">Parent layout that all the field GUI elements will be added to.</param>
        /// <param name="path">Root path to be used for all created child fields.</param>
        /// <param name="depth">
        /// Determines how deep within the inspector nesting hierarchy will the generated fields be. Some fields may
        /// contain other fields, in which case you should increase this value by one.
        /// </param>
        public InspectorFieldDrawer(InspectableContext context, GUILayoutY layout, string path = "", int depth = 0)
        {
            this.context = context;
            this.layout = layout;
            this.path = path;
            this.depth = depth;
        }

        /// <summary>
        /// Creates the default inspector GUI for the provided object.
        /// </summary>
        /// <param name="obj">Object whose fields to create the GUI for.</param>
        /// <param name="subType">
        /// If not null, the added fields will be limited to this particular type (not including any base types the actual
        /// object type might be a part of). If null, then fields for the entire class hierarchy of the provided object's
        /// type will be created.
        /// </param>
        /// <param name="overrideCallback">
        /// Optional callback that allows you to override the look of individual fields in the object. If non-null the
        /// callback will be called with information about every field in the provided object. If the callback returns
        /// non-null that inspectable field will be used for drawing the GUI, otherwise the default inspector field type
        /// will be used.
        /// </param>
        public void AddDefault(object obj, Type subType = null, FieldOverrideCallback overrideCallback = null)
        {
            if (obj == null)
                return;

            SerializableObject serializableObject = new SerializableObject(obj.GetType(), obj);
            AddDefault(serializableObject, subType, overrideCallback);
        }

        /// <summary>
        /// Creates the default inspector GUI for the provided object.
        /// </summary>
        /// <param name="obj">Object whose fields to create the GUI for.</param>
        /// <param name="subType">
        /// If not null, the added fields will be limited to this particular type (not including any base types the actual
        /// object type might be a part of). If null, then fields for the entire class hierarchy of the provided object's
        /// type will be created.
        /// </param>
        /// <param name="overrideCallback">
        /// Optional callback that allows you to override the look of individual fields in the object. If non-null the
        /// callback will be called with information about every field in the provided object. If the callback returns
        /// non-null that inspectable field will be used for drawing the GUI, otherwise the default inspector field type
        /// will be used.
        /// </param>
        public void AddDefault(SerializableObject obj, Type subType = null, FieldOverrideCallback overrideCallback = null)
        {
            if (obj == null)
                return;

            // Retrieve fields and sort by order
            List<SerializableField> fields = new List<SerializableField>();
            while (obj != null)
            {
                if (subType == null || subType == obj.Type)
                {
                    SerializableField[] subTypeFields = obj.Fields;
                    Array.Sort(subTypeFields,
                        (x, y) =>
                        {
                            int orderX = x.Flags.HasFlag(SerializableFieldAttributes.Order) ? x.Style.Order : 0;
                            int orderY = y.Flags.HasFlag(SerializableFieldAttributes.Order) ? y.Style.Order : 0;

                            return orderX.CompareTo(orderY);
                        });

                    fields.AddRange(subTypeFields);
                }

                obj = obj.Base;
            }

            // Generate per-field GUI while grouping by category
            foreach (var field in fields)
            {
                if (!field.Flags.HasFlag(SerializableFieldAttributes.Inspectable))
                    continue;

                if (field.Flags.HasFlag(SerializableFieldAttributes.Category))
                {
                    string newCategory = field.Style.CategoryName;
                    if (!string.IsNullOrEmpty(newCategory) && categoryName != newCategory)
                        BeginCategory(newCategory);
                    else
                        EndCategory();
                }

                string fieldName = field.Name;
                string readableName = InspectableField.GetReadableIdentifierName(fieldName);

                Func<string, InspectableFieldLayout, int, int, InspectableField> callback = null;
                if (overrideCallback != null)
                {
                    callback = (path, fieldLayout, layoutIndex, depth) =>
                        overrideCallback(field, context, path, fieldLayout, layoutIndex, depth);
                }

                AddFieldInternal(readableName, fieldName, field.GetProperty(), InspectableFieldStyle.Create(field),
                    callback);
            }
        }

        /// <summary>
        /// Adds a custom inspectable field with a custom getter and setter.
        /// </summary>
        /// <typeparam name="T">Type the field is inspecting.</typeparam>
        /// <param name="name">Name of the field.</param>
        /// <param name="getter">Method that returns the current value of the field.</param>
        /// <param name="setter">Method that sets a new value of the field.</param>
        public void AddField<T>(string name, Func<T> getter, Action<T> setter)
        {
            SerializableProperty property = SerializableProperty.Create(getter, setter);
            AddFieldInternal(name, name, property, null, null);
        }

        /// <summary>
        /// Creates a new field accessing the provided property.
        /// </summary>
        /// <param name="title">Title to display on the field.</param>
        /// <param name="name">Name of the field.</param>
        /// <param name="property">Property used to access the field contents.</param>
        /// <param name="style">Optional style used to customize the look of the field.</param>
        /// <param name="fieldCreateCallback">
        /// Optional callback allowing the caller to override how is the field created.
        /// </param>
        private void AddFieldInternal(string title, string name, SerializableProperty property, 
            InspectableFieldStyleInfo style, 
            Func<string, InspectableFieldLayout, int, int, InspectableField> fieldCreateCallback)
        {
            int currentIndex;
            int childDepth;
            GUILayoutY parentLayout;
            if (category != null)
            {
                currentIndex = categoryIndex;
                parentLayout = category.ChildLayout;
                childDepth = depth + 1;
            }
            else
            {
                currentIndex = rootIndex;
                parentLayout = layout;
                childDepth = depth;
            }

            string childPath = string.IsNullOrEmpty(path) ? name : $"{path}/{name}";

            InspectableField inspectableField = null;

            if (fieldCreateCallback != null)
                inspectableField = fieldCreateCallback(path, new InspectableFieldLayout(parentLayout), currentIndex, depth);

            if (inspectableField == null)
            {
                inspectableField = InspectableField.CreateField(context, title, childPath,
                currentIndex, childDepth, new InspectableFieldLayout(parentLayout), property, style);
            }

            if (category != null)
                category.AddChild(inspectableField);
            else
                Fields.Add(inspectableField);

            currentIndex += inspectableField.GetNumLayoutElements();

            if (category != null)
                categoryIndex = currentIndex;
            else
                rootIndex = currentIndex;
        }

        /// <summary>
        /// Adds a condition that determines whether a field will be shown or hidde.
        /// </summary>
        /// <param name="field">Name of the field the condition applies to.</param>
        /// <param name="callback">The callback that returns true if the field should be shown, false otherwise.</param>
        public void AddConditional(string field, Func<bool> callback)
        {
            if(conditionals == null)
                conditionals = new List<ConditionalInfo>();

            conditionals.Add(new ConditionalInfo(field, callback));
        }

        /// <summary>
        /// Opens up a new category. Any new fields will be parented to this category. Category must be closed by calling
        /// <see cref="EndCategory"/> or by calling this method with a new category.
        /// </summary>
        /// <param name="name">Name of the category.</param>
        public void BeginCategory(string name)
        {
            if(category != null)
                EndCategory();

            string categoryPath = string.IsNullOrEmpty(path) ? $"[{name}]" : $"{path}/[{name}]";
            category = new InspectableCategory(context, name, categoryPath, depth,
                new InspectableFieldLayout(layout));

            category.Initialize(rootIndex);
            category.Refresh(rootIndex);
            rootIndex += category.GetNumLayoutElements();

            Fields.Add(category);

            categoryName = name;
            categoryIndex = 0;
        }

        /// <summary>
        /// Ends the category started with <see cref="BeginCategory"/>.
        /// </summary>
        public void EndCategory()
        {
            category = null;
            categoryName = null;
            categoryIndex = 0;
        }

        /// <summary>
        /// Checks if contents of the inspector fields have been modified, and updates them if needed.
        /// </summary>
        /// <param name="force">Forces the GUI fields to display the latest values assigned on the object.</param>
        /// <returns>State representing was anything modified between two last calls to <see cref="Refresh"/>.</returns>
        public InspectableState Refresh(bool force = false)
        {
            InspectableState state = InspectableState.NotModified;

            int currentIndex = 0;
            foreach (var field in Fields)
            {
                state |= field.Refresh(currentIndex, force);
                currentIndex += field.GetNumLayoutElements();

                if (conditionals != null)
                {
                    foreach (var conditional in conditionals)
                    {
                        if (conditional.field == field.Name && conditional.callback != null)
                        {
                            bool active = conditional.callback();

                            if (active != field.Active)
                                field.Active = active;
                        }
                    }
                }
            }

            return state;
        }

        /// <summary>
        /// Destroys and removes all the child inspector fields.
        /// </summary>
        public void Clear()
        {
            foreach(var field in Fields)
                field.Destroy();

            Fields.Clear();
            conditionals?.Clear();
            rootIndex = 0;

            category = null;
            categoryName = null;
            categoryIndex = 0;
        }

        /// <summary>
        /// Changes keyboard focus to the provided field.
        /// </summary>
        /// <param name="path">Path to the field on the object being inspected.</param>
        public void FocusOnField(string path)
        {
            InspectableField field = InspectableField.FindPath(path, 0, Fields);
            field?.SetHasFocus();
        }
    }

    /** @} */
}
