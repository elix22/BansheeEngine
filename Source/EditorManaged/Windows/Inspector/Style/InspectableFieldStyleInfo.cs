//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;

namespace bs.Editor
{
    /// <summary>
    /// Contains boolean information about a field style.
    /// </summary>
    [Flags]
    public enum InspectableFieldStyleFlags
    {
        None          = 0,

        /// <summary>
        /// Floating point field should be represented as a slider rather than a plain input field.
        /// </summary>
        AsSlider     = 1 << 0,

        /// <summary>
        /// Integer field should be represented as a layer mask drop down instead of a plain integer input field.
        /// </summary>
        AsLayerMask  = 1 << 1,

        /// <summary>
        /// Object accessed by the inspectable field is passed as a copy. This means modifications to the object returned by
        /// the property getter will not be registered until the object is re-assigned to the property. Similarly,
        /// modifications to the object passed to the setter, after the setter has been called, will not be registered by
        /// the field. Only relevant if a field type is a reference type.
        /// </summary>
        CopiedAsValue = 1 << 2,

        /// <summary>
        /// Object returned by the inspectable field will never be null, and null should never be passed to the field. Only
        /// relevant if a field type is a reference type.
        /// </summary>
        NotNull       = 1 << 3,

        /// <summary>
        /// Field represents a property that wraps a native object. Getters and setters of such a property issue calls into
        /// native code to update the native object.
        /// </summary>
        NativeWrapper = 1 << 4,

        /// <summary>
        /// When a field changes those changes need to be applied to the parent object by calling the field setter. Only
        /// applicable to properties containing reference types.
        /// </summary>
        ApplyOnDirty = 1 << 5,

        /// <summary>
        /// When a quaternion is displayed in the inspector, by default it will be displayed as converted into euler angles.
        /// Use this flag to force it to be displayed as a quaternion (4D value) with no conversion instead.
        /// </summary>
        AsQuaternion = 1 << 6,

        /// <summary>
        /// Singifies that the field containing a class/struct should display the child fields of that objects as if they
        /// were part of the parent class in the inspector.
        /// </summary>
        Inline = 1 << 7,

        /// <summary>
        /// Signifies that a <see cref="RRef{T}"/> should be loaded when assigned to field through the inspector.
        /// </summary>
        LoadOnAssign = 1 << 8,

        /// <summary>
        /// Specifies that a <see cref="Color"/> is allowed to have a value outside of the [0, 1] range.
        /// </summary>
        HDR = 1 << 9
    }

    /// <summary>
    /// Contains all the information about a field style.
    /// </summary>
    public class InspectableFieldStyleInfo
    {
        /// <summary>
        /// Information about the field range.
        /// </summary>
        public InspectableFieldRangeStyle RangeStyle;

        /// <summary>
        /// Information about the field stepping.
        /// </summary>
        public InspectableFieldStepStyle StepStyle;

        /// <summary>
        /// Determines the category to place the field in. If null field will be placed in the default category.
        /// </summary>
        public InspectableFieldCategoryStyle CategoryStyle;

        /// <summary>
        /// Determines the order of the field displayed in the inspector, relative to other fields.
        /// </summary>
        public InspectableFieldOrderStyle OrderStyle;

        /// <summary>
        /// Boolean information about the field.
        /// </summary>
        public InspectableFieldStyleFlags StyleFlags;

        /// <summary>
        /// Creates an empty set of information about a field style.
        /// </summary>
        public InspectableFieldStyleInfo()
        {
            
        }

        /// <summary>
        /// Makes a deep copy of this object.
        /// </summary>
        public InspectableFieldStyleInfo Clone()
        {
            InspectableFieldStyleInfo style = new InspectableFieldStyleInfo();
            style.StyleFlags = StyleFlags;

            if(RangeStyle != null)
                style.RangeStyle = new InspectableFieldRangeStyle(RangeStyle.Min, RangeStyle.Max, RangeStyle.Slider);

            if(StepStyle != null)
                style.StepStyle = new InspectableFieldStepStyle(StepStyle.Step);

            if(CategoryStyle != null)
                style.CategoryStyle = new InspectableFieldCategoryStyle(CategoryStyle.Category);

            if(OrderStyle != null)
                style.OrderStyle = new InspectableFieldOrderStyle(OrderStyle.Index);

            return style;
        }
    }
}
