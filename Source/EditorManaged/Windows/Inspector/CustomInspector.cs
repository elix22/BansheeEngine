﻿//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Specifies that a class or a struct uses a custom inspector for GUI display. This attribute can be placed on an 
    /// implementation of <see cref="Inspector"/> in which case the type must reference a <see cref="Component"/> or a
    /// <see cref="Resource"/>. Or it can be placed on an implementation of <see cref="InspectableField"/> in which
    /// case their type must reference any serializable class/struct, but may also reference an attribute in which case
    /// any serializable type with that attribute applied with have a custom inspector.
    /// </summary>
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
    public sealed class CustomInspector : Attribute
    {
        #pragma warning disable 0414
        private Type type;
        #pragma warning restore 0414

        /// <summary>
        /// Creates a new custom inspector attribute.
        /// </summary>
        /// <param name="type">Type for which the custom inspector should be displayed.</param>
        public CustomInspector(Type type)
        {
            this.type = type;
        }
    }

    /** @} */
}
