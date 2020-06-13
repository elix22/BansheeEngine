//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using System.Text;
using bs;
 
namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Inspectable field displays GUI elements for a single <see cref="SerializableProperty"/>. This is a base class that
    /// should be specialized for all supported types contained by <see cref="SerializableProperty"/>. Inspectable fields
    /// can and should be created recursively - normally complex types like objects and arrays will contain fields of their 
    /// own, while primitive types like integer or boolean will consist of only a GUI element.
    /// </summary>
    public abstract class InspectableField
    {
        protected InspectableContext context;
        protected InspectableFieldLayout layout;
        protected SerializableProperty property;
        protected string title;
        protected string path;
        protected string name;
        protected int depth;
        protected bool active = true;
        protected SerializableProperty.FieldType type;

        /// <summary>
        /// Property this field is displaying contents of.
        /// </summary>
        public SerializableProperty Property
        {
            get { return property; }
            set
            {
                if (value != null && value.Type != type)
                {
                    throw new ArgumentException(
                        "Attempting to initialize an inspectable field with a property of invalid type.");
                }

                property = value;
            }
        }

        /// <summary>
        /// Returns the path to the field.
        /// </summary>
        public string Path => path;

        /// <summary>
        /// Name portion of the field path.
        /// </summary>
        public string Name => name;

        /// <summary>
        /// Activates or deactivates the underlying GUI elements.
        /// </summary>
        public bool Active
        {
            get => active;
            set => SetActive(value);
        }

        /// <summary>
        /// Creates a new inspectable field GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="type">Type of property this field will be used for displaying.</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the array whose contents to display.</param>
        public InspectableField(InspectableContext context, string title, string path, SerializableProperty.FieldType type, 
            int depth, InspectableFieldLayout layout, SerializableProperty property)
        {
            this.context = context;
            this.layout = layout;
            this.title = title;
            this.path = path;
            this.type = type;
            this.depth = depth;

            if (path != null)
            {
                int lastSlash = path.LastIndexOf('/');
                if (lastSlash == -1)
                    name = path;
                else
                    name = path.Substring(lastSlash);
            }

            Property = property;
        }

        /// <summary>
        /// Checks if contents of the field have been modified, and updates them if needed.
        /// </summary>
        /// <param name="layoutIndex">Index in the parent's layout at which to insert the GUI elements for this field.
        ///                           </param>
        /// <param name="force">Forces the GUI fields to display the latest values assigned on the object.</param>
        /// <returns>State representing was anything modified between two last calls to <see cref="Refresh"/>.</returns>
        public virtual InspectableState Refresh(int layoutIndex, bool force = false)
        {
            return InspectableState.NotModified;
        }

        /// <summary>
        /// Returns the total number of GUI elements in the field's layout.
        /// </summary>
        /// <returns>Number of GUI elements in the field's layout.</returns>
        public int GetNumLayoutElements()
        {
            return layout.NumElements;
        }

        /// <summary>
        /// Returns an optional title layout. Certain fields may contain separate title and content layouts. Parent fields
        /// may use the separate title layout instead of the content layout to append elements. Having a separate title 
        /// layout is purely cosmetical.
        /// </summary>
        /// <returns>Title layout if the field has one, null otherwise.</returns>
        public virtual GUILayoutX GetTitleLayout()
        {
            return null;
        }

        /// <summary>
        /// Initializes the GUI elements for the field.
        /// </summary>
        /// <param name="layoutIndex">Index at which to insert the GUI elements.</param>
        protected internal abstract void Initialize(int layoutIndex);

        /// <summary>
        /// Destroys all GUI elements in the inspectable field.
        /// </summary>
        public virtual void Destroy()
        {
            layout.DestroyElements();
        }

        /// <summary>
        /// Moves keyboard focus to this field.
        /// </summary>
        /// <param name="subFieldName">
        /// Name of the sub-field to focus on. Only relevant if the inspectable field represents multiple GUI
        /// input elements.
        /// </param>
        public virtual void SetHasFocus(string subFieldName = null) { }

        /// <summary>
        /// Searches for a child field with the specified path.
        /// </summary>
        /// <param name="path">
        /// Path relative to the current field. Path entries are field names separated with "/". Fields within
        /// categories are placed within a special category group, surrounded by "[]". Some examples:
        /// - myField
        /// - myObject/myField
        /// - myObject/[myCategory]/myField
        /// </param>
        /// <returns>Matching field if one is found, null otherwise.</returns>
        public virtual InspectableField FindPath(string path)
        {
            return null;
        }

        /// <summary>
        /// Searches for a field with the specified path.
        /// </summary>
        /// <param name="path">
        /// Path to search for. Path entries are readable field names separated with "/". Fields within categories are
        /// placed within a special category group, surrounded by "[]". Some examples:
        /// - myField
        /// - myObject/myField
        /// - myObject/[myCategory]/myField
        /// </param>
        /// <param name="depth">Path depth at which the provided set of fields is at.</param>
        /// <param name="fields">List of fields to search. Children will be searched recursively.</param>
        /// <returns>Matching field if one is found, null otherwise.</returns>
        public static InspectableField FindPath(string path, int depth, IEnumerable<InspectableField> fields)
        {
            string subPath = GetSubPath(path, depth + 1);
            foreach (var field in fields)
            {
                InspectableField foundField = null;

                if (field.path == path)
                    foundField = field;
                else if (field.path == subPath)
                    foundField = field.FindPath(path);

                if (foundField != null)
                    return foundField;
            }

            return null;
        }

        /// <summary>
        /// Returns the top-most part of the provided field path.
        /// See <see cref="FindPath(string, int, IEnumerable{InspectableField})"/> for more information about paths.
        /// </summary>
        /// <param name="path">Path to return the sub-path of.</param>
        /// <param name="count">Number of path elements to retrieve.</param>
        /// <returns>First <paramref name="count"/> elements of the path.</returns>
        public static string GetSubPath(string path, int count)
        {
            if (count <= 0)
                return null;

            StringBuilder sb = new StringBuilder();
            int foundSections = 0;
            bool gotFirstChar = false;
            for (int i = 0; i < path.Length; i++)
            {
                if (path[i] == '/')
                {
                    if (!gotFirstChar)
                    {
                        gotFirstChar = true;
                        continue;
                    }

                    foundSections++;

                    if (foundSections == count)
                        break;
                }

                sb.Append(path[i]);
                gotFirstChar = true;
            }

            if (foundSections == 0)
                sb.Append(path);

            return sb.ToString();
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
        /// <param name="subPath">Additional path to append to the end of the current field path.</param>
        protected void StartUndo(string subPath)
        {
            if (context.Component != null)
            {
                string fullPath = path;
                if (!string.IsNullOrEmpty(subPath))
                    fullPath = path.TrimEnd('/') + '/' + subPath.TrimStart('/');

                GameObjectUndo.RecordComponent(context.Component, fullPath);
            }
        }

        /// <summary>
        /// Finishes recording an undo command started via <see cref="StartUndo(string)"/>. If any changes are detected on
        /// the field an undo command is recorded onto the undo-redo stack, otherwise nothing is done.
        /// </summary>
        protected void EndUndo()
        {
            GameObjectUndo.ResolveDiffs();
        }

        /// <summary>
        /// Activates or deactivates the underlying GUI elements.
        /// </summary>
        protected virtual void SetActive(bool active)
        {
            if (this.active != active)
            {
                layout.SetActive(active);
                this.active = active;
            }
        }

        /// <summary>
        /// Creates a new inspectable field, automatically detecting the most appropriate implementation for the type
        /// contained in the provided serializable property. This may be one of the built-in inspectable field implemetations
        /// (like ones for primitives like int or bool), or a user defined implementation defined with a 
        /// <see cref="CustomInspector"/> attribute.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="layoutIndex">Index into the parent layout at which to insert the GUI elements for the field .</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the array whose contents to display.</param>
        /// <param name="style">Information that can be used for customizing field rendering and behaviour.</param>
        /// <returns>Inspectable field implementation that can be used for displaying the GUI for a serializable property
        ///          of the provided type.</returns>
        public static InspectableField CreateField(InspectableContext context, string title, string path, int layoutIndex, 
            int depth, InspectableFieldLayout layout, SerializableProperty property, InspectableFieldStyleInfo style = null)
        {
            InspectableField field = null;

            Type type = property.InternalType;
            if (type.IsGenericType && type.GetGenericTypeDefinition() == typeof(RRef<>))
                type = type.GenericTypeArguments[0];

            Type customInspectable = InspectorUtility.GetCustomInspectable(type);
            if (customInspectable != null)
            {
                field = (InspectableField) Activator.CreateInstance(customInspectable, context, title, path, depth, layout, 
                    property, style);
            }
            else
            {
                switch (property.Type)
                {
                    case SerializableProperty.FieldType.Int:
                        if (style != null && style.StyleFlags.HasFlag(InspectableFieldStyleFlags.AsLayerMask))
                            field = new InspectableLayerMask(context, title, path, depth, layout, property);
                        else
                        {
                            if (style?.RangeStyle == null || !style.RangeStyle.Slider)
                                field = new InspectableInt(context, title, path, depth, layout, property, style);
                            else
                                field = new InspectableRangedInt(context, title, path, depth, layout, property, style);
                        }

                        break;
                    case SerializableProperty.FieldType.Float:
                        if (style?.RangeStyle == null || !style.RangeStyle.Slider)
                            field = new InspectableFloat(context, title, path, depth, layout, property, style);
                        else
                            field = new InspectableRangedFloat(context, title, path, depth, layout, property, style);
                        break;
                    case SerializableProperty.FieldType.Bool:
                        field = new InspectableBool(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Color:
                        field = new InspectableColor(context, title, path, depth, layout, property, style);
                        break;
                    case SerializableProperty.FieldType.ColorGradient:
                        field = new InspectableColorGradient(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.ColorGradientHDR:
                        field = new InspectableColorGradientHDR(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Curve:
                        field = new InspectableCurve(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.FloatDistribution:
                        field = new InspectableFloatDistribution(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Vector2Distribution:
                        field = new InspectableVector2Distribution(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Vector3Distribution:
                        field = new InspectableVector3Distribution(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.ColorDistribution:
                        field = new InspectableColorDistribution(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.String:
                        field = new InspectableString(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Vector2:
                        field = new InspectableVector2(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Vector3:
                        field = new InspectableVector3(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Vector4:
                        field = new InspectableVector4(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Quaternion:
                        if (style != null && style.StyleFlags.HasFlag(InspectableFieldStyleFlags.AsQuaternion))
                            field = new InspectableQuaternion(context, title, path, depth, layout, property);
                        else
                            field = new InspectableEuler(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Resource:
                        field = new InspectableResource(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.RRef:
                        field = new InspectableRRef(context, title, path, depth, layout, property, style);
                        break;
                    case SerializableProperty.FieldType.GameObjectRef:
                        field = new InspectableGameObjectRef(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Object:
                        field = new InspectableObject(context, title, path, depth, layout, property, style);
                        break;
                    case SerializableProperty.FieldType.Array:
                        field = new InspectableArray(context, title, path, depth, layout, property, style);
                        break;
                    case SerializableProperty.FieldType.List:
                        field = new InspectableList(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Dictionary:
                        field = new InspectableDictionary(context, title, path, depth, layout, property);
                        break;
                    case SerializableProperty.FieldType.Enum:
                        field = new InspectableEnum(context, title, path, depth, layout, property);
                        break;
                }
            }

            if (field == null)
                throw new Exception("No inspector exists for the provided field type.");

            field.Initialize(layoutIndex);
            field.Refresh(layoutIndex);

            return field;
        }

        /// <summary>
        /// Converts a name of an identifier (such as a field or a property) into a human readable name.
        /// </summary>
        /// <param name="input">Identifier to convert.</param>
        /// <returns>Human readable name with spaces.</returns>
        public static string GetReadableIdentifierName(string input)
        {
            if (string.IsNullOrEmpty(input))
                return "";

            StringBuilder sb = new StringBuilder();

            bool nextUpperIsSpace = true;

            if (input[0] == '_')
            {
                // Skip
                nextUpperIsSpace = false;
            }
            else if (input[0] == 'm' && input.Length > 1 && char.IsUpper(input[1]))
            {
                // Skip
                nextUpperIsSpace = false;
            }
            else if (char.IsLower(input[0]))
                sb.Append(char.ToUpper(input[0]));
            else
            {
                sb.Append(input[0]);
                nextUpperIsSpace = false;
            }

            for (int i = 1; i < input.Length; i++)
            {
                if (input[i] == '_')
                {
                    sb.Append(' ');
                    nextUpperIsSpace = false;
                    
                }
                else if (char.IsUpper(input[i]))
                {
                    if (nextUpperIsSpace)
                    {
                        sb.Append(' ');
                        sb.Append(input[i]);
                    }
                    else
                        sb.Append(char.ToLower(input[i]));


                    nextUpperIsSpace = false;
                }
                else
                {
                    sb.Append(input[i]);
                    nextUpperIsSpace = true;
                }
            }

            return sb.ToString();
        }
    }

    /// <summary>
    /// Contains information shared between multiple inspector fields.
    /// </summary>
    public class InspectableContext
    {
        /// <summary>
        /// Creates a new context.
        /// </summary>
        /// <param name="component">
        /// Component object that inspector fields are editing. Can be null if the object being edited is not a component.
        /// </param>
        public InspectableContext(Component component = null)
        {
            Persistent = new SerializableProperties();
            Component = component;
        }

        /// <summary>
        /// Creates a new context with user-provided persistent property storage.
        /// </summary>
        /// <param name="persistent">Existing object into which to inspectable fields can store persistent data.</param>
        /// <param name="component">
        /// Component object that inspector fields are editing. Can be null if the object being edited is not a component.
        /// </param>
        public InspectableContext(SerializableProperties persistent, Component component = null)
        {
            Persistent = persistent;
            Component = component;
        }

        /// <summary>
        /// A set of properties that the inspector can read/write. They will be persisted even after the inspector is closed
        /// and restored when it is re-opened.
        /// </summary>
        public SerializableProperties Persistent { get; }

        /// <summary>
        /// Component object that inspector fields are editing. Can be null if the object being edited is not a component.
        /// </summary>
        public Component Component { get; }
    }

    /** @} */
}
