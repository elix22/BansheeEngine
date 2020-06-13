//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a 3D vector.
    /// </summary>
    public class InspectableVector3 : InspectableField
    {
        private GUIVector3Field guiField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable 3D vector GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableVector3(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout, 
            SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.Vector3, depth, layout, property)
        {

        }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property.Type == SerializableProperty.FieldType.Vector3)
            {
                guiField = new GUIVector3Field(new GUIContent(title));
                guiField.OnComponentChanged += OnFieldValueChanged;
                guiField.OnConfirm += x =>
                {
                    OnFieldValueConfirm();
                    StartUndo(x.ToString());
                };
                guiField.OnComponentFocusChanged += (focus, comp) =>
                {
                    if(focus)
                        StartUndo(comp.ToString());
                    else
                        OnFieldValueConfirm();
                };

                layout.AddElement(layoutIndex, guiField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiField != null && (!guiField.HasInputFocus || force))
                guiField.Value = property.GetValue<Vector3>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <inheritdoc/>
        public override void SetHasFocus(string subFieldName = null)
        {
            if(subFieldName == "X")
                guiField.SetInputFocus(VectorComponent.X, true);
            else if(subFieldName == "Y")
                guiField.SetInputFocus(VectorComponent.Y, true);
            else if(subFieldName == "Z")
                guiField.SetInputFocus(VectorComponent.Z, true);
            else
                guiField.SetInputFocus(VectorComponent.X, true);
        }

        /// <summary>
        /// Triggered when the user changes the field value of a single component.
        /// </summary>
        /// <param name="newValue">New value of a single component in the 3D vector field.</param>
        /// <param name="component">Component that was changed.</param>
        private void OnFieldValueChanged(float newValue, VectorComponent component)
        {
            property.SetValue(guiField.Value);
            state |= InspectableState.ModifyInProgress;
        }

        /// <summary>
        /// Triggered when the user confirms input in the 3D vector field.
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
