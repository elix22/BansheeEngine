//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /// <summary>
    /// Contains style information about inspectable items.
    /// </summary>
    public abstract class InspectableFieldStyle
    {
        /// <summary>
        /// Creates all the relevant style information for a SerializableField.
        /// </summary>
        /// <param name="field">Field to create the style info structure for.</param>
        /// <returns>Style information retrieved from the field.</returns>
        public static InspectableFieldStyleInfo Create(SerializableField field)
        {
            SerializableFieldStyle style = field.Style;
            SerializableFieldAttributes flags = field.Flags;

            var styleInfo = new InspectableFieldStyleInfo();
            styleInfo.RangeStyle = flags.HasFlag(SerializableFieldAttributes.Ranged) 
                ? new InspectableFieldRangeStyle(style.RangeMin, style.RangeMax, style.DisplayAsSlider) : null;
            styleInfo.StepStyle = flags.HasFlag(SerializableFieldAttributes.Stepped)
                ? new InspectableFieldStepStyle(style.StepIncrement) : null;
            styleInfo.CategoryStyle = flags.HasFlag(SerializableFieldAttributes.Category)
                ? new InspectableFieldCategoryStyle(style.CategoryName) : null;
            styleInfo.OrderStyle = flags.HasFlag(SerializableFieldAttributes.Order)
                ? new InspectableFieldOrderStyle(style.Order) : null;
            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.AsLayerMask)
                ? InspectableFieldStyleFlags.AsLayerMask : 0;
            styleInfo.StyleFlags |= style.DisplayAsSlider ? InspectableFieldStyleFlags.AsSlider : 0;
            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.PassByCopy)
                ? InspectableFieldStyleFlags.CopiedAsValue
                : 0;

            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.NotNull)
                ? InspectableFieldStyleFlags.NotNull
                : 0;

            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.NativeWrapper)
                ? InspectableFieldStyleFlags.NativeWrapper
                : 0;

            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.ApplyOnDirty)
                ? InspectableFieldStyleFlags.ApplyOnDirty
                : 0;

            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.AsQuaternion)
                ? InspectableFieldStyleFlags.AsQuaternion
                : 0;

            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.Inline)
                ? InspectableFieldStyleFlags.Inline
                : 0;

            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.LoadOnAssign)
                ? InspectableFieldStyleFlags.LoadOnAssign
                : 0;

            styleInfo.StyleFlags |= flags.HasFlag(SerializableFieldAttributes.HDR)
                ? InspectableFieldStyleFlags.HDR
                : 0;

            return styleInfo;
        }
    }
}
