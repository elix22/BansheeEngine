//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a list. List contents are displayed as rows of entries 
    /// that can be shown, hidden or manipulated.
    /// </summary>
    public class InspectableList : InspectableField
    {
        private InspectableListGUI listGUIField;

        /// <summary>
        /// Creates a new inspectable list GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the list whose contents to display.</param>
        public InspectableList(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout,
            SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.List, depth, layout, property)
        {

        }

        /// <inheritdoc/>
        public override GUILayoutX GetTitleLayout()
        {
            return listGUIField.GetTitleLayout();
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            return listGUIField.Refresh(force);
        }

        /// <inheritdoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            GUILayout arrayLayout = layout.AddLayoutY(layoutIndex);

            listGUIField = InspectableListGUI.Create(context, title, path, property, arrayLayout, depth);
            listGUIField.IsExpanded = context.Persistent.GetBool(path + "_Expanded");
            listGUIField.OnExpand += x => context.Persistent.SetBool(path + "_Expanded", x);
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

            if (idx >= listGUIField.NumRows)
                return null;

            InspectableListGUIRow row = listGUIField.GetRow(idx);
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
        /// Handles creation of GUI elements for a GUI list field that displays a <see cref="SerializableList"/> object.
        /// </summary>
        private class InspectableListGUI : GUIListFieldBase
        {
            private IList list;
            private int numElements;
            private InspectableContext context;
            private SerializableProperty property;
            private string path;

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
            /// Returns the number of rows in the array.
            /// </summary>
            public int NumRows
            {
                get { return GetNumRows(); }
            }

            /// <summary>
            /// Constructs a new empty inspectable list GUI.
            /// </summary>
            /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
            /// <param name="title">Label to display on the list GUI title.</param>
            /// <param name="path">Full path to this property (includes name of this property and all parent properties).
            /// </param>
            /// <param name="property">Serializable property referencing a list.</param>
            /// <param name="layout">Layout to which to append the list GUI elements to.</param>
            /// <param name="depth">Determines at which depth to render the background. Useful when you have multiple
            ///                     nested containers whose backgrounds are overlaping. Also determines background style,
            ///                     depths divisible by two will use an alternate style.</param>
            public InspectableListGUI(InspectableContext context, LocString title, string path, 
                SerializableProperty property, GUILayout layout, int depth)
                : base(title, layout, depth)
            {
                this.property = property;
                this.context = context;
                this.path = path;

                list = property.GetValue<IList>();
                if (list != null)
                    numElements = list.Count;
            }

            /// <summary>
            /// Creates a new inspectable list GUI object that displays the contents of the provided serializable property.
            /// </summary>
            /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
            /// <param name="title">Label to display on the list GUI title.</param>
            /// <param name="path">Full path to this property (includes name of this property and all parent properties).
            /// </param>
            /// <param name="property">Serializable property referencing a list.</param>
            /// <param name="layout">Layout to which to append the list GUI elements to.</param>
            /// <param name="depth">Determines at which depth to render the background. Useful when you have multiple
            ///                     nested containers whose backgrounds are overlaping. Also determines background style,
            ///                     depths divisible by two will use an alternate style.</param>
            public static InspectableListGUI Create(InspectableContext context, LocString title, string path, 
                SerializableProperty property, GUILayout layout, int depth)
            {
                InspectableListGUI listGUI = new InspectableListGUI(context, title, path, property, layout, depth);
                listGUI.BuildGUI();

                return listGUI;
            }

            /// <summary>
            /// Returns a list row at the specified index.
            /// </summary>
            /// <param name="idx">Index of the row.</param>
            /// <returns>List row representation or null if index is out of range.</returns>
            public InspectableListGUIRow GetRow(int idx)
            {
                if (idx < GetNumRows())
                    return (InspectableListGUIRow)rows[idx];

                return null;
            }

            /// <inheritdoc/>
            public override InspectableState Refresh(bool force)
            {
                // Check if any modifications to the array were made outside the inspector
                IList newList = property.GetValue<IList>();
                if (list == null && newList != null)
                {
                    list = newList;
                    numElements = list.Count;
                    BuildGUI(true);
                }
                else if (newList == null && list != null)
                {
                    list = null;
                    numElements = 0;
                    BuildGUI(true);
                }
                else
                {
                    if (newList != null)
                    {
                        if (numElements != newList.Count || force)
                        {
                            list = newList;
                            numElements = list.Count;
                            BuildGUI(true);
                        }
                    }
                }

                return base.Refresh(force);
            }

            /// <inheritdoc/>
            protected override GUIListFieldRow CreateRow()
            {
                return new InspectableListGUIRow();
            }

            /// <inheritdoc/>
            protected override bool IsNull()
            {
                return list == null;
            }

            /// <inheritdoc/>
            protected override int GetNumRows()
            {
                if (list != null)
                    return list.Count;

                return 0;
            }

            /// <inheritdoc/>
            protected internal override object GetValue(int seqIndex)
            {
                SerializableList list = property.GetList();

                return list.GetProperty(seqIndex);
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

                list = property.CreateListInstance(0);
                property.SetValue(list);
                numElements = 0;

                EndUndo();
            }

            /// <inheritdoc/>
            protected override void ResizeList()
            {
                StartUndo();

                int size = guiSizeField.Value;

                IList newList = property.CreateListInstance(size);

                int maxSize = MathEx.Min(size, list.Count);
                for (int i = 0; i < maxSize; i++)
                    newList[i] = list[i];

                property.SetValue(newList);
                list = newList;
                numElements = list.Count;

                EndUndo();
            }

            /// <inheritdoc/>
            protected override void ClearList()
            {
                StartUndo();

                property.SetValue<object>(null);
                list = null;
                numElements = 0;

                EndUndo();
            }

            /// <inheritdoc/>
            protected internal override void DeleteElement(int index)
            {
                StartUndo();

                if (index >= 0 && index < list.Count)
                    list.RemoveAt(index);

                numElements = list.Count;

                EndUndo();
            }

            /// <inheritdoc/>
            protected internal override void CloneElement(int index)
            {
                StartUndo();

                SerializableList serializableList = property.GetList();

                if (index >= 0 && index < list.Count)
                    list.Add(SerializableUtility.Clone(serializableList.GetProperty(index).GetValue<object>()));

                numElements = list.Count;

                EndUndo();
            }

            /// <inheritdoc/>
            protected internal override void MoveUpElement(int index)
            {
                StartUndo();

                if ((index - 1) >= 0)
                {
                    object previousEntry = list[index - 1];

                    list[index - 1] = list[index];
                    list[index] = previousEntry;
                }

                EndUndo();
            }

            /// <inheritdoc/>
            protected internal override void MoveDownElement(int index)
            {
                StartUndo();

                if ((index + 1) < list.Count)
                {
                    object nextEntry = list[index + 1];

                    list[index + 1] = list[index];
                    list[index] = nextEntry;
                }

                EndUndo();
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
        /// Contains GUI elements for a single entry in the list.
        /// </summary>
        private class InspectableListGUIRow : GUIListFieldRow
        {
            /// <summary>
            /// Inspectable field displayed on the list row.
            /// </summary>
            public InspectableField Field { get; private set; }

            /// <inheritdoc/>
            protected override GUILayoutX CreateGUI(GUILayoutY layout)
            {
                InspectableListGUI listParent = (InspectableListGUI)parent;
                SerializableProperty property = GetValue<SerializableProperty>();

                string entryPath = listParent.Path + "[" + SeqIndex + "]";
                Field = CreateField(listParent.Context, SeqIndex + ".", entryPath, 0, Depth + 1,
                    new InspectableFieldLayout(layout), property, new InspectableFieldStyleInfo());

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
