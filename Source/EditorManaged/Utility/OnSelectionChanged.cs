//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;

namespace bs.Editor
{
    /** @addtogroup Utility-Editor
     *  @{
     */

    /// <summary>
    /// Notifies the runtime that the method this attribute is specified on serves as a callback for reporting when a new
    /// scene object containing a certain component type has been selected or deselected. The method must have two
    /// parameters. First parameter must derive from <see cref="bs.Component"/> type and determines for which
    /// components will be method be triggered. When callback is triggered this parameter will contain the instance of
    /// the component that was selected or deselected. The second parameter must be a boolean which determines whether the
    /// component as added (if true) or removed (if true) from the current selection.
    /// </summary>
    [AttributeUsage(AttributeTargets.Method)]
    public sealed class OnSelectionChanged : Attribute
    { }

    /** @} */
}
