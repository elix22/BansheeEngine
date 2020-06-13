//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a color. Color is displayed as a GUI color field that allows
    /// the user to manipulate the color using a color picker.
    /// </summary>
    public class InspectableColor : InspectableField
    {
        private GUIColorField guiField;
        private InspectableState state;
        private bool hdr;

        /// <summary>
        /// Creates a new inspectable color GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        /// <param name="style">Contains information about the field style.</param>
        public InspectableColor(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout, 
            SerializableProperty property, InspectableFieldStyleInfo style)
            : base(context, title, path, SerializableProperty.FieldType.Color, depth, layout, property)
        {
            hdr = style?.StyleFlags.HasFlag(InspectableFieldStyleFlags.HDR) ?? false;
        }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiField = new GUIColorField(new GUIContent(title));
                guiField.OnChanged += OnFieldValueChanged;

                if (hdr)
                    guiField.AllowHDR = true;

                layout.AddElement(layoutIndex, guiField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiField != null)
                guiField.Value = property.GetValue<Color>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <summary>
        /// Triggered when the user selects a new color.
        /// </summary>
        /// <param name="newValue">New value of the color field.</param>
        private void OnFieldValueChanged(Color newValue)
        {
            StartUndo();

            property.SetValue(newValue);
            state = InspectableState.Modified;

            EndUndo();
        }
    }

    /** @} */
}
