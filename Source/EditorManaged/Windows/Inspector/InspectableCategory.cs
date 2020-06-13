//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System.Collections.Generic;
using System;
using System.Reflection;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Helper inspectable type that groups another inspectable fields under a common category.
    /// </summary>
    public class InspectableCategory : InspectableField
    {
        private const int IndentAmount = 5;

        public GUILayoutY ChildLayout { get; private set; }

        private List<InspectableField> children = new List<InspectableField>();

        private GUILayoutY guiLayout;
        private GUIPanel guiContentPanel;
        private bool isExpanded;

        /// <summary>
        /// Creates a new inspectable category. The category is initially empty and children must be added by calling
        /// <see cref="AddChild"/>.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the category.</param>
        /// <param name="path">Full path to the category (includes name of the category and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        public InspectableCategory(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout)
            : base(context, title, "", SerializableProperty.FieldType.Object, depth, layout, null)
        {
            isExpanded = context.Persistent.GetBool(path + "_Expanded");
        }

        /// <summary>
        /// Registers a new child field in the category.
        /// </summary>
        /// <param name="child">Child field to add to the category. The field must have the category
        /// <see cref="ChildLayout"/> as its parent.</param>
        public void AddChild(InspectableField child)
        {
            children.Add(child);
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            InspectableState state = InspectableState.NotModified;
            int currentIndex = 0;
            for (int i = 0; i < children.Count; i++)
            {
                state |= children[i].Refresh(currentIndex, force);
                currentIndex += children[i].GetNumLayoutElements();
            }

            return state;
        }

        /// <inheritdoc />
        public override InspectableField FindPath(string path)
        {
            return FindPath(path, depth, children);
        }

        /// <inheritdoc/>
        protected internal override void Initialize(int index)
        {
            guiLayout = layout.AddLayoutY(index);

            GUILayoutX guiTitleLayout = guiLayout.AddLayoutX();

            GUIToggle guiFoldout = new GUIToggle(title, EditorStyles.Foldout);
            guiFoldout.Value = isExpanded;
            guiFoldout.AcceptsKeyFocus = false;
            guiFoldout.OnToggled += OnFoldoutToggled;
            guiTitleLayout.AddElement(guiFoldout);

            GUILayoutX categoryContentLayout = guiLayout.AddLayoutX();
            categoryContentLayout.AddSpace(IndentAmount);

            guiContentPanel = categoryContentLayout.AddPanel();
            GUILayoutX guiIndentLayoutX = guiContentPanel.AddLayoutX();
            guiIndentLayoutX.AddSpace(IndentAmount);
            GUILayoutY guiIndentLayoutY = guiIndentLayoutX.AddLayoutY();
            guiIndentLayoutY.AddSpace(IndentAmount);
            ChildLayout = guiIndentLayoutY.AddLayoutY();
            guiIndentLayoutY.AddSpace(IndentAmount);
            guiIndentLayoutX.AddSpace(IndentAmount);
            categoryContentLayout.AddSpace(IndentAmount);

            short backgroundDepth = (short)(Inspector.START_BACKGROUND_DEPTH - depth - 1);
            string bgPanelStyle = depth % 2 == 0
                ? EditorStylesInternal.InspectorContentBgAlternate
                : EditorStylesInternal.InspectorContentBg;
            GUIPanel backgroundPanel = guiContentPanel.AddPanel(backgroundDepth);
            GUITexture inspectorContentBg = new GUITexture(null, bgPanelStyle);
            backgroundPanel.AddElement(inspectorContentBg);

            guiContentPanel.Active = isExpanded;
        }

        /// <summary>
        /// Triggered when the user clicks on the expand/collapse toggle in the title bar.
        /// </summary>
        /// <param name="expanded">Determines whether the contents were expanded or collapsed.</param>
        private void OnFoldoutToggled(bool expanded)
        {
            context.Persistent.SetBool(path + "_Expanded", expanded);
            isExpanded = expanded;
            guiContentPanel.Active = expanded;
        }
    }

    /** @} */
}
