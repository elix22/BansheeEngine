//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a floating point distribution. GUI elements will switch between
    /// floating point and curve input depending on the distribution type. 
    /// </summary>
    public class InspectableFloatDistribution : InspectableField
    {
        private GUIFloatDistributionField guiDistributionField;
        private InspectableState state;

        /// <summary>
        /// Creates a new inspectable float distribution GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the field whose contents to display.</param>
        public InspectableFloatDistribution(InspectableContext context, string title, string path, int depth, 
            InspectableFieldLayout layout, SerializableProperty property)
            : base(context, title, path, SerializableProperty.FieldType.FloatDistribution, depth, layout, property)
        { }

        /// <inheritoc/>
        protected internal override void Initialize(int layoutIndex)
        {
            if (property != null)
            {
                guiDistributionField = new GUIFloatDistributionField(new GUIContent(title));
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

                    if(rangeComp == RangeComponent.Min)
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
                guiDistributionField.Value = property.GetValue<FloatDistribution>();

            InspectableState oldState = state;
            if (state.HasFlag(InspectableState.Modified))
                state = InspectableState.NotModified;

            return oldState;
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (subFieldName != null && subFieldName.StartsWith("min."))
                guiDistributionField.SetInputFocus(RangeComponent.Min, VectorComponent.X, true);

            if (subFieldName != null && subFieldName.StartsWith("max."))
                guiDistributionField.SetInputFocus(RangeComponent.Max, VectorComponent.X, true);
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