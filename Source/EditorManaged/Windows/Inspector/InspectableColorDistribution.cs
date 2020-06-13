//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a color distribution. GUI elements will switch between
    /// color and color gradient input depending on the distribution type. 
    /// </summary>
    public class InspectableColorDistribution : InspectableField
    {
        private GUIColorDistributionField guiDistributionField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable color distribution GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableColorDistribution(InspectableContext context, string title, string path, int depth,
            InspectableFieldLayout layout, SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.ColorDistribution, depth, layout, property)
        { }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiDistributionField = new GUIColorDistributionField(new GUIContent(title));
                guiDistributionField.OnChanged += OnFieldValueChanged;

                layout.AddElement(layoutIndex, guiDistributionField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiDistributionField != null)
                guiDistributionField.Value = property.GetValue<ColorDistribution>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <summary>
        /// Triggered when the user edits the distribution.
        /// </summary>
        private void OnFieldValueChanged()
        {
            StartUndo();

            property.SetValue(guiDistributionField.Value);
            state |= InspectableState.ModifyInProgress | InspectableState.Modified;

            EndUndo();
        }
    }

    /** @} */
}