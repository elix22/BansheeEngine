//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a floating point value.
    /// </summary>
    public class InspectableFloat : InspectableField
    {
        private GUIFloatField guiFloatField;
        private InspectableState state;
        private InspectableFieldStyleInfo style;

        /// <summary>
        /// Creates a new inspectable float GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        /// <param name="style">Information that can be used for customizing field rendering and behaviour.</param>
        public InspectableFloat(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout, 
            SerializableProperty property, InspectableFieldStyleInfo style)
            : base(context, title, path, SerializableProperty.FieldType.Float, depth, layout, property)
        {
            this.style = style;
        }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiFloatField = new GUIFloatField(new GUIContent(title));
                if (style != null)
                {
                    if (style.StepStyle != null && style.StepStyle.Step != 0)
                        guiFloatField.Step = style.StepStyle.Step;
                    if (style.RangeStyle != null)
                        guiFloatField.SetRange(style.RangeStyle.Min, style.RangeStyle.Max);
                }
                guiFloatField.OnChanged += OnFieldValueChanged;
                guiFloatField.OnConfirmed += () =>
                {
                    OnFieldValueConfirm();
                    StartUndo();
                };
                guiFloatField.OnFocusLost += OnFieldValueConfirm;
                guiFloatField.OnFocusGained += StartUndo;

                layout.AddElement(layoutIndex, guiFloatField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiFloatField != null && (!guiFloatField.HasInputFocus || force))
                guiFloatField.Value = property.GetValue<float>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            guiFloatField.Focus = true;
        }

        /// <summary>
        /// Triggered when the user inputs a new floating point value.
        /// </summary>
        /// <param name="newValue">New value of the float field.</param>
        private void OnFieldValueChanged(float newValue)
        {
            property.SetValue(newValue);
            state |= InspectableState.ModifyInProgress;
        }

        /// <summary>
        /// Triggered when the user confirms input in the float field.
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
