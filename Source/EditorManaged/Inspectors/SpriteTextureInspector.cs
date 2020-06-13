//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using System;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="SpriteTexture"/> resource.
    /// </summary>
    [CustomInspector(typeof(SpriteTexture))]
    internal class SpriteTextureInspector : Inspector
    {
        private GUILayoutWithBackground previewTitleLayout;
        private GUILayoutWithBackground previewContentLayout;

        private GUITexture previewTexture;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            LoadResource();

            SpriteTexture spriteTexture = InspectedObject as SpriteTexture;
            if (spriteTexture == null)
                return;

            drawer.AddDefault(spriteTexture);

            GUILayout previewLayout = PreviewGUI.AddLayoutY();
            previewTitleLayout = GUILayoutWithBackground.Create<GUILayoutX>(previewLayout, Builtin.WhiteTexture,
                new Color(0.129f, 0.129f, 0.129f), new RectOffset(11, 0, 2, 0));

            GUILabel title = new GUILabel(new LocEdString("Preview"));
            previewTitleLayout.Layout.AddElement(title);
            previewTitleLayout.Layout.AddFlexibleSpace();

            previewContentLayout = GUILayoutWithBackground.Create<GUILayoutX>(previewLayout, Builtin.WhiteTexture,
                new Color(0.09f, 0.09f, 0.09f), new RectOffset(5, 5, 5, 5));

            previewContentLayout.MainPanel.SetHeight(250);

            previewTexture = new GUITexture(spriteTexture, GUITextureScaleMode.ScaleToFit,
                GUIOption.FlexibleWidth(), GUIOption.FlexibleHeight());
            previewContentLayout.Layout.AddElement(previewTexture);
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            SpriteTexture spriteTexture = InspectedObject as SpriteTexture;
            if (spriteTexture == null)
                return InspectableState.NotModified;

            InspectableState state = drawer.Refresh();
            if (state != InspectableState.NotModified)
            {
                EditorApplication.SetDirty(spriteTexture);

                // The inspector will by default just assign a resource reference without loading it, make sure we load it
                // so it can be previewed
                if (spriteTexture.Texture != null && !spriteTexture.Texture.IsLoaded)
                    Resources.Load<Texture>(spriteTexture.Texture.UUID);

                // Make sure GUI redraws as the sprite texture properties were updated
                previewTexture.SetTexture(spriteTexture);
            }

            return state;
        }
    }

    /** @} */
}