//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a string.
    /// </summary>
    public class InspectableString : InspectableField
    {
        private GUITextField guiField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable string GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableString(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout,
            SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.String, depth, layout, property)
        {

        }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property.Type == SerializableProperty.FieldType.String)
            {
                guiField = new GUITextField(new GUIContent(title));
                guiField.OnChanged += OnFieldValueChanged;
                guiField.OnConfirmed += () =>
                {
                    OnFieldValueConfirm();
                    StartUndo();
                };
                guiField.OnFocusLost += OnFieldValueConfirm;
                guiField.OnFocusGained += StartUndo;

                layout.AddElement(layoutIndex, guiField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiField != null && (!guiField.HasInputFocus || force))
                guiField.Value = property.GetValue<string>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <summary>
        /// Triggered when the user inputs a new string.
        /// </summary>
        /// <param name="newValue">New value of the text field.</param>
        private void OnFieldValueChanged(string newValue)
        {
            property.SetValue(newValue);
            state |= InspectableState.ModifyInProgress;
        }

        /// <summary>
        /// Triggered when the user confirms input in the text field.
        /// </summary>
        private void OnFieldValueConfirm()
        {
            if (state.HasFlag(InspectableState.ModifyInProgress))
                state |= InspectableState.Modified;

            EndUndo();
        }
    }

    /** @} */
}
