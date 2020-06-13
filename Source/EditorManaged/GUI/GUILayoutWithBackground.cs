//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup GUI-Editor
     *  @{
     */

    /// <summary>
    /// Helper class that creates a GUI layout with a texture background.
    /// </summary>
    public class GUILayoutWithBackground
    {
        /// <summary>
        /// Content layout into which you should add your own GUI elements.
        /// </summary>
        public GUILayout Layout { get; }

        /// <summary>
        /// Root panel of the content layout, background panel and any helper backgrounds. Can be used for changing size,
        /// destroying or hiding the GUI elements. You should not add or remove GUI elements from the panel.
        /// </summary>
        public GUIPanel MainPanel { get; }

        /// <summary>
        /// Constructs a new object.
        /// </summary>
        /// <param name="mainPanel">Root panel in which all the child GUI elements of this object belong to.</param>
        /// <param name="layout">Content layout that's to be provided to the user.</param>
        private GUILayoutWithBackground(GUIPanel mainPanel, GUILayout layout)
        {
            MainPanel = mainPanel;
            Layout = layout;
        }

        /// <summary>
        /// Creates a new GUI layout of the specified type, with a texture background.
        /// </summary>
        /// <typeparam name="T">Type of layout to create.</typeparam>
        /// <param name="layout">Parent layout to add the layout to.</param>
        /// <param name="background">Texture to display on the background.</param>
        /// <param name="backgroundColor">Color to apply to the background texture.</param>
        /// <param name="padding">Optional padding to apply between element borders and content.</param>
        /// <returns>New GUI layout with background object.</returns>
        public static GUILayoutWithBackground Create<T>(GUILayout layout, SpriteTexture background, Color backgroundColor,
            RectOffset padding = new RectOffset()) where T : GUILayout, new()
        {
            GUIPanel mainPanel = layout.AddPanel();
            GUILayoutX mainLayout = mainPanel.AddLayoutX();

            GUILayout contentLayout;
            if (padding.top > 0 || padding.bottom > 0)
            {
                GUILayoutY paddingVertLayout = mainLayout.AddLayoutY();

                if (padding.top > 0)
                    paddingVertLayout.AddSpace(padding.top);

                if (padding.left > 0 || padding.right > 0)
                {
                    GUILayoutX paddingHorzLayout = paddingVertLayout.AddLayoutX();

                    if (padding.left > 0)
                        paddingHorzLayout.AddSpace(padding.left);

                    contentLayout = new T();
                    paddingHorzLayout.AddElement(contentLayout);

                    if (padding.right > 0)
                        paddingHorzLayout.AddSpace(padding.right);
                }
                else
                {
                    contentLayout = new T();
                    paddingVertLayout.AddElement(contentLayout);
                }

                if (padding.bottom > 0)
                    paddingVertLayout.AddSpace(padding.bottom);
            }
            else
            {
                if (padding.left > 0 || padding.right > 0)
                {
                    GUILayoutX paddingHorzLayout = mainLayout.AddLayoutX();

                    if (padding.left > 0)
                        paddingHorzLayout.AddSpace(padding.left);

                    contentLayout = new T();
                    paddingHorzLayout.AddElement(contentLayout);

                    if (padding.right > 0)
                        paddingHorzLayout.AddSpace(padding.right);
                }
                else
                {
                    contentLayout = new T();
                    mainLayout.AddElement(contentLayout);
                }
            }

            GUIPanel bgPanel = mainPanel.AddPanel(1);
            GUITexture bgTexture = new GUITexture(Builtin.WhiteTexture);
            bgTexture.SetTint(backgroundColor);
            bgPanel.AddElement(bgTexture);

            return new GUILayoutWithBackground(mainPanel, contentLayout);
        }
    }

    /** @} */
}