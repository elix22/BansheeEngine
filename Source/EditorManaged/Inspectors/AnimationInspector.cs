//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="Animation"/> component.
    /// </summary>
    [CustomInspector(typeof(Animation))]
    internal class AnimationInspector : Inspector
    {
        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            Animation animation = (Animation)InspectedObject;

            drawer.AddDefault(animation);

            // Morph shapes
            Renderable renderable = animation.SceneObject.GetComponent<Renderable>();
            MorphShapes morphShapes = renderable?.Mesh.Value?.MorphShapes;
            if (morphShapes != null)
            {
                GUIToggle morphShapesToggle = new GUIToggle(new LocEdString("Morph shapes"), EditorStyles.Foldout);

                Layout.AddElement(morphShapesToggle);
                GUILayoutY channelsLayout = Layout.AddLayoutY();

                morphShapesToggle.OnToggled += x =>
                {
                    channelsLayout.Active = x;
                    Persistent.SetBool("Channels_Expanded", x);
                };

                channelsLayout.Active = Persistent.GetBool("Channels_Expanded");

                MorphChannel[] channels = morphShapes.Channels;
                for (int i = 0; i < channels.Length; i++)
                {
                    GUILayoutY channelLayout = channelsLayout.AddLayoutY();

                    GUILayoutX channelTitleLayout = channelLayout.AddLayoutX();
                    channelLayout.AddSpace(5);
                    GUILayoutY channelContentLayout = channelLayout.AddLayoutY();

                    string channelName = channels[i].Name;
                    GUIToggle channelNameField = new GUIToggle(channelName, EditorStyles.Expand, GUIOption.FlexibleWidth());

                    channelTitleLayout.AddSpace(15); // Indent
                    channelTitleLayout.AddElement(channelNameField);
                    channelTitleLayout.AddFlexibleSpace();

                    channelNameField.OnToggled += x =>
                    {
                        channelContentLayout.Active = x;

                        Persistent.SetBool(channelName + "_Expanded", x);
                    };

                    channelContentLayout.Active = Persistent.GetBool(channelName + "_Expanded");

                    MorphShape[] shapes = channels[i].Shapes;
                    for (int j = 0; j < shapes.Length; j++)
                    {
                        GUILayoutX shapeLayout = channelContentLayout.AddLayoutX();
                        channelContentLayout.AddSpace(5);

                        LocString nameString = new LocString("[{0}]. {1}");
                        nameString.SetParameter(0, j.ToString());
                        nameString.SetParameter(1, shapes[j].Name);
                        GUILabel shapeNameField = new GUILabel(shapes[j].Name);

                        LocString weightString = new LocEdString("Weight: {0}");
                        weightString.SetParameter(0, shapes[j].Weight.ToString());
                        GUILabel weightField = new GUILabel(weightString);

                        shapeLayout.AddSpace(30); // Indent
                        shapeLayout.AddElement(shapeNameField);
                        shapeLayout.AddFlexibleSpace();
                        shapeLayout.AddElement(weightField);
                    }
                }
            }
        }
    }

    /** @} */
}