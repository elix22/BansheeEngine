//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//

namespace bs.Editor
{
    /// <summary>
    /// Contains information about the order of a field when displayed in the inspector
    /// </summary>
    public sealed class InspectableFieldOrderStyle : InspectableFieldStyle
    {
        public InspectableFieldOrderStyle(int index)
        {
            this.Index = index;
        }

        /// <summary>
        /// Index that determines the order relative to other fields. Lower index means the field will be displayed before
        /// the fields with higher index.
        /// </summary>
        public int Index { get; set; }
    }
}
