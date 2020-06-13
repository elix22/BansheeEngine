//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//

namespace bs.Editor
{
    /// <summary>
    /// Contains information about a category in inspector to place the field in.
    /// </summary>
    public sealed class InspectableFieldCategoryStyle : InspectableFieldStyle
    {
        public InspectableFieldCategoryStyle(string category)
        {
            this.Category = category;
        }

        /// <summary>
        /// Name of the category to place the field in.
        /// </summary>
        public string Category { get; set; }
    }
}
