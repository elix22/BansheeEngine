//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using System;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Default implementation of the inspector used when no specified inspector is provided for the type. Inspector 
    /// displays GUI for all the inspectable fields in the object.
    /// </summary>
    internal sealed class GenericInspector : Inspector
    {
        private bool isEmpty = true;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            if (InspectedObject == null)
                LoadResource();

            drawer.AddDefault(InspectedObject);

            isEmpty = drawer.Fields.Count == 0;
            base.SetVisible(!isEmpty);
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            return drawer.Refresh(force);
        }

        /// <inheritdoc/>
        internal override void SetVisible(bool visible)
        {
            base.SetVisible(!isEmpty && visible);
        }
    }

    /** @} */
}
