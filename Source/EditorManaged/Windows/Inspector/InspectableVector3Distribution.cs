﻿//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a 3D vector distribution. GUI elements will switch between
    /// vector and curve input depending on the distribution type. 
    /// </summary>
    public class InspectableVector3Distribution : InspectableField
    {
        private GUIVector3DistributionField guiDistributionField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable 3D vector distribution GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableVector3Distribution(InspectableContext context, string title, string path, int depth, 
            InspectableFieldLayout layout, SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.Vector3Distribution, depth, layout, property)
        { }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiDistributionField = new GUIVector3DistributionField(new GUIContent(title));
                guiDistributionField.OnCurveChanged += () =>
                {
                    StartUndo();
                    property.SetValue(guiDistributionField.Value);
                    state |= InspectableState.ModifyInProgress;
                    EndUndo();
                };

                guiDistributionField.OnConstantModified += (x, y) => OnFieldValueChanged();
                guiDistributionField.OnConstantConfirmed += (rangeComp, vectorComp) =>
                {
                    OnFieldValueConfirm();

                    if (rangeComp == RangeComponent.Min)
                        StartUndo("min." + vectorComp.ToString());
                    else
                        StartUndo("max." + vectorComp.ToString());
                };
                guiDistributionField.OnConstantFocusChanged += (focus, rangeComp, vectorComp) =>
                {
                    if (focus)
                    {
                        if (rangeComp == RangeComponent.Min)
                            StartUndo("min." + vectorComp.ToString());
                        else
                            StartUndo("max." + vectorComp.ToString());
                    }
                    else
                        OnFieldValueConfirm();
                };

                layout.AddElement(layoutIndex, guiDistributionField);
            }
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            if (guiDistributionField != null && (!guiDistributionField.HasInputFocus || force))
                guiDistributionField.Value = property.GetValue<Vector3Distribution>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (subFieldName != null && subFieldName.StartsWith("min."))
            {
                string component = subFieldName.Remove(0, "min.".Length);
                if (component == "X")
                    guiDistributionField.SetInputFocus(RangeComponent.Min, VectorComponent.X, true);
                else if (component == "Y")
                    guiDistributionField.SetInputFocus(RangeComponent.Min, VectorComponent.Y, true);
                else if (component == "Z")
                    guiDistributionField.SetInputFocus(RangeComponent.Min, VectorComponent.Z, true);
                else
                    guiDistributionField.SetInputFocus(RangeComponent.Min, VectorComponent.X, true);
            }

            if (subFieldName != null && subFieldName.StartsWith("max."))
            {
                string component = subFieldName.Remove(0, "max.".Length);
                if (component == "X")
                    guiDistributionField.SetInputFocus(RangeComponent.Max, VectorComponent.X, true);
                else if (component == "Y")
                    guiDistributionField.SetInputFocus(RangeComponent.Max, VectorComponent.Y, true);
                else if (component == "Z")
                    guiDistributionField.SetInputFocus(RangeComponent.Max, VectorComponent.Z, true);
                else
                    guiDistributionField.SetInputFocus(RangeComponent.Max, VectorComponent.X, true);
            }
        }

        /// <summary>
        /// Triggered when the user edits the distribution.
        /// </summary>
        private void OnFieldValueChanged()
        {
            property.SetValue(guiDistributionField.Value);
            state |= InspectableState.ModifyInProgress;
        }

        /// <summary>
        /// Triggered when the user confirms input in the float fields used for displaying the non-curve distribution.
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
