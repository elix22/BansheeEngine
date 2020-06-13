//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing an integer value displayed as a layer mask.
    /// </summary>
    public class InspectableLayerMask : InspectableField
    {
        private GUIListBoxField guiLayerMaskField;
        private InspectableState state;
        private ulong layersValue;

        /// <summary>
        /// Creates a new inspectable layer mask GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableLayerMask(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout,
            SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.Int, depth, layout, property)
        {
        }

        /// <inheritdoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiLayerMaskField = new GUIListBoxField(Layers.Names, true, new GUIContent(title));

                guiLayerMaskField.OnSelectionChanged += x =>
                {
                    ulong layers = 0;
                    bool[] states = guiLayerMaskField.States;
                    for (int i = 0; i < states.Length; i++)
                        layers |= states[i] ? Layers.Values[i] : 0;

                    layersValue = layers;

                    StartUndo();

                    property.SetValue(layers);
                    state |= InspectableState.ModifyInProgress | InspectableState.Modified;

                    EndUndo();
                };

                layout.AddElement(layoutIndex, guiLayerMaskField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiLayerMaskField != null)
            {
                ulong currentValue = property.GetValue<ulong>();
                if (layersValue != currentValue)
                {
                    bool[] states = new bool[64];
                    for (int i = 0; i < states.Length; i++)
                        states[i] = (currentValue & Layers.Values[i]) == Layers.Values[i];

                    guiLayerMaskField.States = states;
                    layersValue = currentValue;
                }
            }

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }
    }

    /** @} */
}
