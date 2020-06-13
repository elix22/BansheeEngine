//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a dictionary. Dictionary contents are displayed as rows of 
    /// entries that can be shown, hidden or manipulated.
    /// </summary>
    public class InspectableDictionary : InspectableField
    {
        private InspectableDictionaryGUI dictionaryGUIField;

        /// <summary>
        /// Creates a new inspectable dictionary GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the dictionary whose contents to display.</param>
        public InspectableDictionary(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout, 
            SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.Dictionary, depth, layout, property)
        {

        }

        /// <inheritdoc/>
        public override GUILayoutX GetTitleLayout()
        {
            return dictionaryGUIField.GetTitleLayout();
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            return dictionaryGUIField.Refresh(force);
        }

        /// <inheritdoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            GUILayout dictionaryLayout = layout.AddLayoutY(layoutIndex);

            dictionaryGUIField = InspectableDictionaryGUI.Create(context, title, path, property, dictionaryLayout, depth);
            dictionaryGUIField.IsExpanded = context.Persistent.GetBool(path + "_Expanded");
            dictionaryGUIField.OnExpand += x => context.Persistent.SetBool(path + "_Expanded", x);
        }

        /// <inheritdoc />
        public override InspectableField FindPath(string path)
        {
            string subPath = GetSubPath(path, depth + 1);

            if (string.IsNullOrEmpty(subPath))
                return null;

            int lastLeftIdx = subPath.LastIndexOf("Key[");
            int lastRightIdx = -1;
            bool isKey;
            if (lastLeftIdx != -1)
            {
                lastRightIdx = subPath.LastIndexOf(']', lastLeftIdx);
                isKey = true;
            }
            else
            {
                lastLeftIdx = subPath.LastIndexOf("Value[");
                lastRightIdx = subPath.LastIndexOf(']', lastLeftIdx);

                isKey = false;
            }

            if (lastLeftIdx == -1 || lastRightIdx == -1)
                return null;

            int count = lastRightIdx - 1 - lastLeftIdx;
            if (count <= 0)
                return null;

            string arrayIdxStr = subPath.Substring(lastLeftIdx, count);

            if (!int.TryParse(arrayIdxStr, out int idx))
                return null;;

            if (idx >= dictionaryGUIField.NumRows)
                return null;

            InspectableDictionaryGUIRow row = dictionaryGUIField.GetRow(idx);
            InspectableField field = null;
            if (isKey)
                field = row?.FieldKey;
            else
                field = row?.FieldValue;

            if (field != null)
            {
                if (field.Path == path)
                    return field;

                return field.FindPath(path);
            }

            return null;
        }

        /// <summary>
        /// Creates GUI elements that allow viewing and manipulation of a <see cref="SerializableDictionary"/> referenced
        /// by a serializable property.
        /// </summary>
        private class InspectableDictionaryGUI : GUIDictionaryFieldBase
        {
            private SerializableProperty property;
            private IDictionary dictionary;
            private int numElements;
            private InspectableContext context;
            private string path;

            private List<SerializableProperty> orderedKeys = new List<SerializableProperty>();

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
            /// Constructs a new dictionary GUI.
            /// </summary>
            /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
            /// <param name="title">Label to display on the list GUI title.</param>
            /// <param name="path">Full path to this property (includes name of this property and all parent properties).
            /// </param>
            /// <param name="property">Serializable property referencing a dictionary</param>
            /// <param name="layout">Layout to which to append the list GUI elements to.</param>
            /// <param name="depth">Determines at which depth to render the background. Useful when you have multiple
            ///                     nested containers whose backgrounds are overlaping. Also determines background style,
            ///                     depths divisible by two will use an alternate style.</param>
            protected InspectableDictionaryGUI(InspectableContext context, LocString title, string path, SerializableProperty property, 
                GUILayout layout, int depth = 0)
            : base(title, layout, depth)
            {
                this.property = property;
                this.context = context;
                this.path = path;

                dictionary = property.GetValue<IDictionary>();
                if (dictionary != null)
                    numElements = dictionary.Count;

                UpdateKeys();
            }

            /// <summary>
            /// Builds the inspectable dictionary GUI elements. Must be called at least once in order for the contents to 
            /// be populated.
            /// </summary>
            /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
            /// <param name="title">Label to display on the list GUI title.</param>
            /// <param name="path">Full path to this property (includes name of this property and all parent properties).
            /// </param>
            /// <param name="property">Serializable property referencing a dictionary</param>
            /// <param name="layout">Layout to which to append the list GUI elements to.</param>
            /// <param name="depth">Determines at which depth to render the background. Useful when you have multiple
            ///                     nested containers whose backgrounds are overlaping. Also determines background style,
            ///                     depths divisible by two will use an alternate style.</param>
            public static InspectableDictionaryGUI Create(InspectableContext context, LocString title, string path, 
                SerializableProperty property, GUILayout layout, int depth = 0)
            {
                InspectableDictionaryGUI guiDictionary = new InspectableDictionaryGUI(context, title, path, property, 
                    layout, depth);
                guiDictionary.BuildGUI();

                return guiDictionary;
            }

            /// <summary>
            /// Returns an array row at the specified index.
            /// </summary>
            /// <param name="idx">Index of the row.</param>
            /// <returns>Array row representation or null if index is out of range.</returns>
            public InspectableDictionaryGUIRow GetRow(int idx)
            {
                if (idx < GetNumRows())
                    return (InspectableDictionaryGUIRow)rows[idx];

                return null;
            }

            /// <inheritdoc/>
            public override InspectableState Refresh(bool force)
            {
                // Check if any modifications to the array were made outside the inspector
                IDictionary newDict = property.GetValue<IDictionary>();
                if (dictionary == null && newDict != null)
                {
                    dictionary = newDict;
                    numElements = dictionary.Count;
                    BuildGUI();
                }
                else if (newDict == null && dictionary != null)
                {
                    dictionary = null;
                    numElements = 0;
                    BuildGUI();
                }
                else
                {
                    if (newDict != null)
                    {
                        if (numElements != newDict.Count || force)
                        {
                            numElements = newDict.Count;
                            BuildGUI();
                        }
                    }
                }

                return base.Refresh(force);
            }

            /// <summary>
            /// Updates the ordered set of keys used for mapping sequential indexes to keys. Should be called whenever a 
            /// dictionary key changes.
            /// </summary>
            private void UpdateKeys()
            {
                orderedKeys.Clear();

                if (dictionary != null)
                {
                    SerializableDictionary dict = property.GetDictionary();
                    foreach (var key in dictionary.Keys)
                        orderedKeys.Add(dict.GetProperty(key).Key);
                }
            }

            /// <inheritdoc/>
            protected override GUIDictionaryFieldRow CreateRow()
            {
                return new InspectableDictionaryGUIRow();
            }

            /// <inheritdoc/>
            protected override int GetNumRows()
            {
                if (dictionary != null)
                    return dictionary.Count;

                return 0;
            }

            /// <inheritdoc/>
            protected override bool IsNull()
            {
                return dictionary == null;
            }

            /// <inheritdoc/>
            protected internal override object GetKey(int rowIdx)
            {
                return orderedKeys[rowIdx];
            }

            /// <inheritdoc/>
            protected internal override object GetValue(object key)
            {
                SerializableProperty keyProperty = (SerializableProperty)key;

                SerializableDictionary dictionary = property.GetDictionary();
                return dictionary.GetProperty(keyProperty.GetValue<object>()).Value;
            }

            /// <inheritdoc/>
            protected internal override void SetValue(object key, object value)
            {
                // Setting the value should be done through the property
                throw new InvalidOperationException();
            }

            /// <inheritdoc/>
            protected internal override bool Contains(object key)
            {
                SerializableProperty keyProperty = (SerializableProperty)key;
                return dictionary.Contains(keyProperty.GetValue<object>()); ;
            }

            /// <inheritdoc/>
            protected internal override void EditEntry(object oldKey, object newKey, object value)
            {
                SerializableProperty oldKeyProperty = (SerializableProperty)oldKey;
                SerializableProperty newKeyProperty = (SerializableProperty)newKey;
                SerializableProperty valueProperty = (SerializableProperty)value;

                dictionary.Remove(oldKeyProperty.GetValue<object>());
                dictionary.Add(newKeyProperty.GetValue<object>(), valueProperty.GetValue<object>());
                numElements = dictionary.Count;

                UpdateKeys();
            }

            /// <inheritdoc/>
            protected internal override void AddEntry(object key, object value)
            {
                StartUndo();

                SerializableProperty keyProperty = (SerializableProperty)key;
                SerializableProperty valueProperty = (SerializableProperty)value;

                dictionary.Add(keyProperty.GetValue<object>(), valueProperty.GetValue<object>());
                numElements = dictionary.Count;

                EndUndo();
                UpdateKeys();
            }

            /// <inheritdoc/>
            protected internal override void RemoveEntry(object key)
            {
                StartUndo();

                SerializableProperty keyProperty = (SerializableProperty)key;

                dictionary.Remove(keyProperty.GetValue<object>());
                numElements = dictionary.Count;

                EndUndo();
                UpdateKeys();
            }

            /// <inheritdoc/>
            protected internal override object CreateKey()
            {
                SerializableDictionary dictionary = property.GetDictionary();

                DictionaryDataWrapper data = new DictionaryDataWrapper();
                data.value = SerializableUtility.Create(dictionary.KeyType);

                SerializableProperty keyProperty = new SerializableProperty(dictionary.KeyPropertyType,
                    dictionary.KeyType,
                    () => data.value, (x) => data.value = x);

                return keyProperty;
            }

            /// <inheritdoc/>
            protected internal override object CreateValue()
            {
                SerializableDictionary dictionary = property.GetDictionary();

                DictionaryDataWrapper data = new DictionaryDataWrapper();
                data.value = SerializableUtility.Create(dictionary.ValueType);

                SerializableProperty valueProperty = new SerializableProperty(dictionary.ValuePropertyType,
                    dictionary.ValueType,
                    () => data.value, (x) => data.value = x);

                return valueProperty;
            }

            /// <inheritdoc/>
            protected internal override KeyValuePair<object, object> CloneElement(int index)
            {
                SerializableProperty keyProperty = (SerializableProperty)GetKey(index);
                SerializableProperty valueProperty = (SerializableProperty)GetValue(keyProperty);

                SerializableDictionary dictionary = property.GetDictionary();

                DictionaryDataWrapper keyData = new DictionaryDataWrapper();
                keyData.value = SerializableUtility.Clone(keyProperty.GetValue<object>());

                SerializableProperty clonedKeyProperty = new SerializableProperty(dictionary.KeyPropertyType,
                    dictionary.KeyType,
                    () => keyData.value, (x) => keyData.value = x);

                DictionaryDataWrapper valueData = new DictionaryDataWrapper();
                valueData.value = SerializableUtility.Clone(valueProperty.GetValue<object>());

                SerializableProperty clonedValueProperty = new SerializableProperty(dictionary.ValuePropertyType,
                    dictionary.ValueType,
                    () => valueData.value, (x) => valueData.value = x);

                return new KeyValuePair<object,object>(clonedKeyProperty, clonedValueProperty);
            }

            /// <inheritdoc/>
            protected override void CreateDictionary()
            {
                StartUndo();

                dictionary = property.CreateDictionaryInstance();
                numElements = dictionary.Count;
                property.SetValue(dictionary);

                EndUndo();
                UpdateKeys();
            }

            /// <inheritdoc/>
            protected override void DeleteDictionary()
            {
                StartUndo();

                dictionary = null;
                numElements = 0;
                property.SetValue<object>(null);

                EndUndo();
                UpdateKeys();
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

            /// <summary>
            /// Wraps a dictionary key or a value.
            /// </summary>
            class DictionaryDataWrapper
            {
                public object value;
            }
        }

        /// <summary>
        /// Contains GUI elements for a single key/value pair in the dictionary.
        /// </summary>
        private class InspectableDictionaryGUIRow : GUIDictionaryFieldRow
        {
            /// <summary>
            /// Inspectable field displaying the key on the dictionary row.
            /// </summary>
            public InspectableField FieldKey { get; private set; }

            /// <summary>
            /// Inspectable field displaying the value on the dictionary row.
            /// </summary>
            public InspectableField FieldValue { get; private set; }

            private GUILayoutY keyLayout;

            /// <inheritdoc/>
            protected override GUILayoutX CreateKeyGUI(GUILayoutY layout)
            {
                keyLayout = layout;
                InspectableDictionaryGUI dictParent = (InspectableDictionaryGUI)parent;
                SerializableProperty property = GetKey<SerializableProperty>();

                string entryPath = dictParent.Path + "Key[" + RowIdx + "]";
                FieldKey = CreateField(dictParent.Context, "Key", entryPath, 0, Depth + 1,
                    new InspectableFieldLayout(layout), property);

                return FieldKey.GetTitleLayout();
            }

            /// <inheritdoc/>
            protected override void CreateValueGUI(GUILayoutY layout)
            {
                InspectableDictionaryGUI dictParent = (InspectableDictionaryGUI)parent;
                SerializableProperty property = GetValue<SerializableProperty>();

                string entryPath = dictParent.Path + "Value[" + RowIdx + "]";
                FieldValue = CreateField(dictParent.Context, "Value", entryPath, 0, Depth + 1,
                    new InspectableFieldLayout(layout), property);
            }

            /// <inheritdoc/>
            protected override void OnEditModeChanged(bool editMode)
            {
                keyLayout.Disabled = !editMode;
            }

            /// <inheritdoc/>
            protected internal override InspectableState Refresh(bool force)
            {
                FieldKey.Property = GetKey<SerializableProperty>();
                FieldValue.Property = GetValue<SerializableProperty>();

                return FieldValue.Refresh(0, force);
            }
        }
    }

    /** @} */
}
