//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing an enum. Enum is displayed as a list box.
    /// </summary>
    public class InspectableEnum : InspectableField
    {
        private GUIEnumField guiField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable enum GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableEnum(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout,
            SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.Enum, depth, layout, property)
        { }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiField = new GUIEnumField(property.InternalType, new GUIContent(title));
                guiField.OnSelectionChanged += OnFieldValueChanged;

                layout.AddElement(layoutIndex, guiField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiField != null)
                guiField.Value = property.GetValue<ulong>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <summary>
        /// Triggered when the user changes the selection in the list box.
        /// </summary>
        /// <param name="newValue">Newly selected list box value.</param>
        private void OnFieldValueChanged(ulong newValue)
        {
            StartUndo();

            property.SetValue(newValue);
            state = InspectableState.Modified;

            EndUndo();
        }
    }

    /** @} */
}
