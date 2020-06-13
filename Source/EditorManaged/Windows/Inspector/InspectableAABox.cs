//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing an AABox.
    /// </summary>
    [CustomInspector(typeof(AABox))]
    public class InspectableAABox : InspectableField
    {
        private GUIVector3Field centerField;
        private GUIVector3Field sizeField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable AABox GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        /// <param name="style">Information that can be used for customizing field rendering and behaviour.</param>
        public InspectableAABox(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout,
            SerializableProperty property, InspectableFieldStyleInfo style)
            : base(context, title, path, SerializableProperty.FieldType.Object, depth, layout, property)
        { }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            GUILayoutX boundsLayout = new GUILayoutX();
            centerField = new GUIVector3Field(new LocEdString("Center"), 50);
            sizeField = new GUIVector3Field(new LocEdString("Size"), 50);

            layout.AddElement(layoutIndex, boundsLayout);

            boundsLayout.AddElement(new GUILabel(new LocEdString(title), GUIOption.FixedWidth(100)));

            GUILayoutY boundsContent = boundsLayout.AddLayoutY();
            boundsContent.AddElement(centerField);
            boundsContent.AddElement(sizeField);

            centerField.OnValueChanged += x =>
            {
                AABox bounds = property.GetValue<AABox>();
                Vector3 min = x - bounds.Size * 0.5f;
                Vector3 max = x + bounds.Size * 0.5f;

                property.SetValue(new AABox(min, max));
                state |= InspectableState.ModifyInProgress;
            };
            centerField.OnConfirm += x =>
            {
                OnFieldValueConfirm();
                StartUndo("center." + x.ToString());
            };
            centerField.OnComponentFocusChanged += (focus, comp) =>
            {
                if (focus)
                    StartUndo("center." + comp.ToString());
                else
                    OnFieldValueConfirm();
            };

            sizeField.OnValueChanged += x =>
            {
                AABox bounds = property.GetValue<AABox>();
                Vector3 min = bounds.Center - x * 0.5f;
                Vector3 max = bounds.Center + x * 0.5f;

                property.SetValue(new AABox(min, max));
                state |= InspectableState.ModifyInProgress;
            };
            sizeField.OnConfirm += x =>
            {
                OnFieldValueConfirm();
                StartUndo("size." + x.ToString());
            };
            sizeField.OnComponentFocusChanged += (focus, comp) =>
            {
                if (focus)
                    StartUndo("size." + comp.ToString());
                else
                    OnFieldValueConfirm();
            };
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if ((centerField != null && (!centerField.HasInputFocus || force)) &&
                (sizeField != null && (!sizeField.HasInputFocus || force)))
            {
                AABox box = property.GetValue<AABox>();
                centerField.Value = box.Center;
                sizeField.Value = box.Size;
            }

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (subFieldName != null && subFieldName.StartsWith("center."))
            {
                string component = subFieldName.Remove(0, "center.".Length);
                if (component == "X")
                    centerField.SetInputFocus(VectorComponent.X, true);
                else if (component == "Y")
                    centerField.SetInputFocus(VectorComponent.Y, true);
                else if (component == "Z")
                    centerField.SetInputFocus(VectorComponent.Z, true);
                else
                    centerField.SetInputFocus(VectorComponent.X, true);
            }

            if (subFieldName != null && subFieldName.StartsWith("size."))
            {
                string component = subFieldName.Remove(0, "size.".Length);
                if (component == "X")
                    sizeField.SetInputFocus(VectorComponent.X, true);
                else if (component == "Y")
                    sizeField.SetInputFocus(VectorComponent.Y, true);
                else if (component == "Z")
                    sizeField.SetInputFocus(VectorComponent.Z, true);
                else
                    sizeField.SetInputFocus(VectorComponent.X, true);
            }
        }

        /// <summary>
        /// Triggered when the user confirms input in either of the 3D vector fields.
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