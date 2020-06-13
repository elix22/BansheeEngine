//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a boolean. Boolean is displayed as a toggle button.
    /// </summary>
    public class InspectableBool : InspectableField
    {
        private GUIToggleField guiField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable boolean GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableBool(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout, 
            SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.Bool, depth, layout, property)
        {

        }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiField = new GUIToggleField(new GUIContent(title));
                guiField.OnChanged += OnFieldValueChanged;

                layout.AddElement(layoutIndex, guiField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiField != null)
                guiField.Value = property.GetValue<bool>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            guiField.Focus = true;
        }

        /// <summary>
        /// Triggered when the user toggles the toggle button.
        /// </summary>
        /// <param name="newValue">New value of the toggle button.</param>
        private void OnFieldValueChanged(bool newValue)
        {
            StartUndo();

            property.SetValue(newValue);
            state = InspectableState.Modified;

            EndUndo();
        }
    }

    /** @} */
}
