//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a <see cref="Texture"/> reference.
    /// </summary>
    [CustomInspector(typeof(Texture))]
    public class InspectableTexture : InspectableField
    {
        private GUITextureField guiField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable texture reference GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableTexture(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout,
            SerializableProperty property, InspectableFieldStyleInfo style)
            : base(context, title, path, property.Type, depth, layout, property)
        { }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            guiField = new GUITextureField(GUITextureFieldType.Texture, new GUIContent(title));
            guiField.OnChanged += OnFieldValueChanged;

            layout.AddElement(layoutIndex, guiField);
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiField != null)
            {
                if (property.Type == SerializableProperty.FieldType.Resource)
                    guiField.Texture = property.GetValue<Texture>();
                else
                    guiField.TextureRef = property.GetValue<RRef<Texture>>();
            }

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <summary>
        /// Triggered when the user drops a new resource onto the field, or clears the current value.
        /// </summary>
        /// <param name="newValue">New resource to reference.</param>
        private void OnFieldValueChanged(RRefBase newValue)
        {
            StartUndo();

            if (property.Type == SerializableProperty.FieldType.Resource)
                property.SetValue(newValue.GenericValue);
            else
                property.SetValue(newValue);

            state = InspectableState.Modified;

            EndUndo();
        }
    }

    /** @} */
}
