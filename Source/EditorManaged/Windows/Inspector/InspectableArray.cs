//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Text;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing an array. Array contents are displayed as rows of entries 
    /// that can be shown, hidden or manipulated.
    /// </summary>
    public class InspectableArray : InspectableField
    {
        private InspectableArrayGUI arrayGUIField;
        private InspectableFieldStyleInfo style;

        /// <summary>
        /// Style applied to the elements of the array and the array itself.
        /// </summary>
        internal InspectableFieldStyleInfo Style
        {
            get { return style; }
        }

        /// <summary>
        /// Creates a new inspectable array GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the array whose contents to display.</param>
        /// <param name="style">Information that can be used for customizing field rendering and behaviour.</param>
        public InspectableArray(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout, 
            SerializableProperty property, InspectableFieldStyleInfo style)
            : base(context, title, path, SerializableProperty.FieldType.Array, depth, layout, property)
        {
            this.style = style;
        }

        /// <inheritdoc/>
        public override GUILayoutX GetTitleLayout()
        {
            return arrayGUIField.GetTitleLayout();
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            return arrayGUIField.Refresh(force);
        }

        /// <inheritdoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            GUILayout arrayLayout = layout.AddLayoutY(layoutIndex);

            arrayGUIField = InspectableArrayGUI.Create(context, title, path, property, arrayLayout, depth, style);
            arrayGUIField.IsExpanded = context.Persistent.GetBool(path + "_Expanded");
            arrayGUIField.OnExpand += x => context.Persistent.SetBool(path + "_Expanded", x);
        }

        /// <inheritdoc />
        public override InspectableField FindPath(string path)
        {
            string subPath = GetSubPath(path, depth + 1);

            if (string.IsNullOrEmpty(subPath))
                return null;

            int lastLeftIdx = subPath.LastIndexOf('[');
            int lastRightIdx = subPath.LastIndexOf(']', lastLeftIdx);

            if (lastLeftIdx == -1 || lastRightIdx == -1)
                return null;

            int count = lastRightIdx - 1 - lastLeftIdx;
            if (count <= 0)
                return null;

            string arrayIdxStr = subPath.Substring(lastLeftIdx, count);

            if (!int.TryParse(arrayIdxStr, out int idx))
                return null;

            if (idx >= arrayGUIField.NumRows)
                return null;

            InspectableArrayGUIRow row = arrayGUIField.GetRow(idx);
            InspectableField field = row?.Field;

            if (field != null)
            {
                if (field.Path == path)
                    return field;

                return field.FindPath(path);
            }

            return null;
        }

        /// <summary>
        /// Handles creation of GUI elements for a GUI list field that displays a <see cref="SerializableArray"/> object.
        /// </summary>
        private class InspectableArrayGUI : GUIListFieldBase
        {
            private Array array;
            private int numElements;
            private InspectableContext context;
            private SerializableProperty property;
            private string path;
            private InspectableFieldStyleInfo style;

            /// <summary>
            /// Context shared by all inspectable fields created by the same parent.
            /// </summary>
            public InspectableContext Context
            {
                get { return context; }
            }

            /// <summary>
            /// Returns a property path to the array field (name of the array field and all parent object fields).
            /// </summary>
            public string Path
            {
                get { return path; }
            }

            /// <summary>
            /// Style applied to the elements of the array and the array itself.
            /// </summary>
            public InspectableFieldStyleInfo Style
            {
                get { return style; }
            }

            /// <summary>
            /// Returns the number of rows in the array.
            /// </summary>
            public int NumRows
            {
                get { return GetNumRows();  }
            }

            /// <summary>
            /// Constructs a new inspectable array GUI.
            /// </summary>
            /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
            /// <param name="title">Label to display on the list GUI title.</param>
            /// <param name="path">Full path to this property (includes name of this property and all parent properties).
            /// </param>
            /// <param name="property">Serializable property referencing a single-dimensional array.</param>
            /// <param name="layout">Layout to which to append the list GUI elements to.</param>
            /// <param name="depth">Determines at which depth to render the background. Useful when you have multiple
            ///                     nested containers whose backgrounds are overlaping. Also determines background style,
            ///                     depths divisible by two will use an alternate style.</param>
            /// <param name="style">Information that can be used for customizing field rendering and behaviour.</param>
            public InspectableArrayGUI(InspectableContext context, LocString title, string path, SerializableProperty property, 
                GUILayout layout, int depth, InspectableFieldStyleInfo style)
                : base(title, layout, depth)
            {
                this.property = property;
                this.context = context;
                this.path = path;
                this.style = style;

                array = property.GetValue<Array>();
                if (array != null)
                    numElements = array.Length;
            }

            /// <summary>
            /// Creates a new inspectable array GUI object that displays the contents of the provided serializable property.
            /// </summary>
            /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
            /// <param name="title">Label to display on the list GUI title.</param>
            /// <param name="path">Full path to this property (includes name of this property and all parent properties).
            /// </param>
            /// <param name="property">Serializable property referencing a single-dimensional array.</param>
            /// <param name="layout">Layout to which to append the list GUI elements to.</param>
            /// <param name="depth">Determines at which depth to render the background. Useful when you have multiple
            ///                     nested containers whose backgrounds are overlaping. Also determines background style,
            ///                     depths divisible by two will use an alternate style.</param>
            /// <param name="style">Information that can be used for customizing field rendering and behaviour.</param>
            public static InspectableArrayGUI Create(InspectableContext context, LocString title, string path, 
                SerializableProperty property, GUILayout layout, int depth, InspectableFieldStyleInfo style)
            {
                InspectableArrayGUI guiArray = new InspectableArrayGUI(context, title, path, property, layout, depth, style);
                guiArray.BuildGUI();
                
                return guiArray;
            }

            /// <summary>
            /// Returns an array row at the specified index.
            /// </summary>
            /// <param name="idx">Index of the row.</param>
            /// <returns>Array row representation or null if index is out of range.</returns>
            public InspectableArrayGUIRow GetRow(int idx)
            {
                if (idx < GetNumRows())
                    return (InspectableArrayGUIRow)rows[idx];

                return null;
            }

            /// <inheritdoc/>
            public override InspectableState Refresh(bool force)
            {
                // Check if any modifications to the array were made outside the inspector
                Array newArray = property.GetValue<Array>();
                if (array == null && newArray != null)
                {
                    array = newArray;
                    numElements = array.Length;
                    BuildGUI(true);
                }
                else if (newArray == null && array != null)
                {
                    array = null;
                    numElements = 0;
                    BuildGUI(true);
                }
                else
                {
                    if (newArray != null)
                    {
                        if (numElements != newArray.Length || force)
                        {
                            array = newArray;
                            numElements = array.Length;
                            BuildGUI(true);
                        }
                    }
                }

                return base.Refresh(force);
            }

            /// <inheritdoc/>
            protected override GUIListFieldRow CreateRow()
            {
                return new InspectableArrayGUIRow();
            }

            /// <inheritdoc/>
            protected override bool IsNull()
            {
                return array == null;
            }

            /// <inheritdoc/>
            protected override int GetNumRows()
            {
                if (array != null)
                    return array.Length;

                return 0;
            }

            /// <inheritdoc/>
            protected internal override object GetValue(int seqIndex)
            {
                SerializableArray serzArray = property.GetArray();

                // Create a property wrapper for native arrays so we get notified when the array values change
                if (style.StyleFlags.HasFlag(InspectableFieldStyleFlags.NativeWrapper))
                {
                    SerializableProperty.Getter getter = () =>
                    {
                        Array array = property.GetValue<Array>();

                        if (array != null)
                            return array.GetValue(seqIndex);
                        else
                            return null;
                    };

                    SerializableProperty.Setter setter = (object value) =>
                    {
                        Array array = property.GetValue<Array>();

                        if (array != null)
                        {
                            array.SetValue(value, seqIndex);
                            property.SetValue(array);
                        }
                    };

                    return new SerializableProperty(serzArray.ElementPropertyType, serzArray.ElementType, getter, setter);
                }
                else
                    return serzArray.GetProperty(seqIndex);
            }

            /// <inheritdoc/>
            protected internal override void SetValue(int seqIndex, object value)
            {
                // Setting the value should be done through the property
                throw new InvalidOperationException();
            }

            /// <inheritdoc/>
            protected override void CreateList()
            {
                StartUndo();

                array = property.CreateArrayInstance(new int[1] { 0 });
                property.SetValue(array);
                numElements = 0;

                EndUndo();
            }

            /// <inheritdoc/>
            protected override void ResizeList()
            {
                StartUndo();

                int size = guiSizeField.Value; // TODO - Support multi-rank arrays

                Array newArray = property.CreateArrayInstance(new int[] { size });
                int maxSize = MathEx.Min(size, array.Length);

                for (int i = 0; i < maxSize; i++)
                    newArray.SetValue(array.GetValue(i), i);

                property.SetValue(newArray);
                array = newArray;
                numElements = size;

                EndUndo();
            }

            /// <inheritdoc/>
            protected override void ClearList()
            {
                // Native arrays cannot be set to null, so just clear their size to zero instead
                if (style.StyleFlags.HasFlag(InspectableFieldStyleFlags.NativeWrapper))
                    CreateList();
                else
                {
                    StartUndo();

                    property.SetValue<object>(null);
                    array = null;
                    numElements = 0;

                    EndUndo();
                }
            }

            /// <inheritdoc/>
            protected internal override void DeleteElement(int index)
            {
                StartUndo();

                int size = MathEx.Max(0, array.Length - 1);
                Array newArray = property.CreateArrayInstance(new int[] { size });

                int destIdx = 0;
                for (int i = 0; i < array.Length; i++)
                {
                    if (i == index)
                        continue;

                    newArray.SetValue(array.GetValue(i), destIdx);
                    destIdx++;
                }

                property.SetValue(newArray);
                array = newArray;
                numElements = array.Length;

                EndUndo();
            }

            /// <inheritdoc/>
            protected internal override void CloneElement(int index)
            {
                StartUndo();

                SerializableArray array = property.GetArray();

                int size = array.GetLength() + 1;
                Array newArray = property.CreateArrayInstance(new int[] { size });

                object clonedEntry = null;
                for (int i = 0; i < array.GetLength(); i++)
                {
                    object value = array.GetProperty(i).GetValue<object>();

                    newArray.SetValue(value, i);

                    if (i == index)
                    {
                        clonedEntry = SerializableUtility.Clone(array.GetProperty(i).GetValue<object>());
                    }
                }

                newArray.SetValue(clonedEntry, size - 1);

                property.SetValue(newArray);
                this.array = newArray;
                numElements = newArray.Length;

                EndUndo();
            }

            /// <inheritdoc/>
            protected internal override void MoveUpElement(int index)
            {
                if ((index - 1) >= 0)
                {
                    StartUndo();

                    object previousEntry = array.GetValue(index - 1);

                    array.SetValue(array.GetValue(index), index - 1);
                    array.SetValue(previousEntry, index);

                    // Natively wrapped arrays are passed by copy
                    if(style.StyleFlags.HasFlag(InspectableFieldStyleFlags.NativeWrapper))
                        property.SetValue(array);
                }
            }

            /// <inheritdoc/>
            protected internal override void MoveDownElement(int index)
            {
                if ((index + 1) < array.Length)
                {
                    StartUndo();

                    object nextEntry = array.GetValue(index + 1);

                    array.SetValue(array.GetValue(index), index + 1);
                    array.SetValue(nextEntry, index);

                    // Natively wrapped arrays are passed by copy
                    if(style.StyleFlags.HasFlag(InspectableFieldStyleFlags.NativeWrapper))
                        property.SetValue(array);

                    EndUndo();
                }
            }

            /// <summary>
            /// Notifies the system to start recording a new undo command. Any changes to the field after this is called
            /// will be recorded in the command. User must call <see cref="EndUndo"/> after field is done being changed.
            /// </summary>
            protected void StartUndo()
            {
                if (context.Component != null)
                    GameObjectUndo.RecordComponent(context.Component, path);
            }

            /// <summary>
            /// Finishes recording an undo command started via <see cref="StartUndo"/>. If any changes are detected on the
            /// field an undo command is recorded onto the undo-redo stack, otherwise nothing is done.
            /// </summary>
            protected void EndUndo()
            {
                GameObjectUndo.ResolveDiffs();
            }
        }

        /// <summary>
        /// Contains GUI elements for a single entry in the array.
        /// </summary>
        private class InspectableArrayGUIRow : GUIListFieldRow
        {
            /// <summary>
            /// Inspectable field displayed on the array row.
            /// </summary>
            public InspectableField Field { get; private set; }

            /// <inheritdoc/>
            protected override GUILayoutX CreateGUI(GUILayoutY layout)
            {
                InspectableArrayGUI arrayParent = (InspectableArrayGUI)parent;
                SerializableProperty property = GetValue<SerializableProperty>();

                InspectableFieldStyleInfo styleInfo = arrayParent.Style.Clone();
                styleInfo.StyleFlags &= ~InspectableFieldStyleFlags.NativeWrapper;

                string entryPath = arrayParent.Path + "[" + SeqIndex + "]";
                Field = CreateField(arrayParent.Context, SeqIndex + ".", entryPath, 0, Depth + 1,
                    new InspectableFieldLayout(layout), property, styleInfo);

                return Field.GetTitleLayout();
            }

            /// <inheritdoc/>
            protected internal override InspectableState Refresh(bool force)
            {
                Field.Property = GetValue<SerializableProperty>();
                return Field.Refresh(0, force);
            }
        }
    }

    /** @} */
}
