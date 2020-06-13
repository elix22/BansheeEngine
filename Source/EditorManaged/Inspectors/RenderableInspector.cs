//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System.Collections;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="Renderable"/> component.
    /// </summary>
    [CustomInspector(typeof (Renderable))]
    internal class RenderableInspector : Inspector
    {
        private List<MaterialParamGUI[]> materialParams = new List<MaterialParamGUI[]>();
        private List<MaterialVariationGUI> materialVariations = new List<MaterialVariationGUI>();

        private RRef<Material>[] materials;
        private GUILayout materialsLayout;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            Renderable renderable = (Renderable)InspectedObject;

            drawer.AddDefault(renderable);

            materials = renderable.Materials;

            materialsLayout = Layout.AddLayoutY();
            BuildMaterialsGUI();
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            Renderable renderable = InspectedObject as Renderable;
            if (renderable == null)
                return InspectableState.NotModified;

            bool rebuildMaterialsGUI = false;

            RRef<Material>[] newMaterials = renderable.Materials;
            if (newMaterials == null)
                rebuildMaterialsGUI = materials != null;
            else
            {
                if (materials == null)
                    rebuildMaterialsGUI = true;
                else
                {
                    if (materials.Length != newMaterials.Length)
                        rebuildMaterialsGUI = true;
                    else
                    {
                        for (int i = 0; i < materials.Length; i++)
                        {
                            if (materials[i] != newMaterials[i])
                            {
                                rebuildMaterialsGUI = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (rebuildMaterialsGUI)
                BuildMaterialsGUI();

            if (materials != null)
            {
                for (int i = 0; i < materialParams.Count; i++)
                {
                    Material material = materials[i].Value;
                    if (material != null && materialParams[i] != null)
                    {
                        foreach (var param in materialParams[i])
                            param.Refresh(material);
                    }
                }
            }

            return base.Refresh(force);
        }

        /// <inheritdoc />
        internal override void FocusOnField(string path)
        {
            if (path.StartsWith("materialParams"))
            {
                string subPath = InspectableField.GetSubPath(path, 2);
                string[] subPathParts = subPath.Split('/');

                if (subPathParts.Length < 2)
                    return;

                int lastLeftIdx = subPathParts[0].LastIndexOf('[');
                int lastRightIdx = subPathParts[0].LastIndexOf(']', lastLeftIdx);

                if (lastLeftIdx == -1 || lastRightIdx == -1)
                    return;

                int count = lastRightIdx - 1 - lastLeftIdx;
                if (count <= 0)
                    return;

                string arrayIdxStr = subPath.Substring(lastLeftIdx, count);

                if (!int.TryParse(arrayIdxStr, out int idx))
                    return;

                if (idx >= materialParams.Count)
                    return;

                MaterialParamGUI[] entries = materialParams[idx];
                foreach (var entry in entries)
                {
                    string fieldSubPath = subPathParts.Length > 2 ? subPathParts[2] : null;

                    if (entry.Param.name == subPathParts[1])
                    {
                        entry.SetHasFocus(fieldSubPath);
                        break;
                    }
                }
            }

            base.FocusOnField(path);
        }

        /// <summary>
        /// Builds the portion of the GUI that displays details about individual materials.
        /// </summary>
        private void BuildMaterialsGUI()
        {
            materialsLayout.Clear();

            materialParams.Clear();
            materialVariations.Clear();
            if (materials != null && materials.Length > 0)
            {
                for (int i = 0; i < materials.Length; i++)
                {
                    string suffix = "";
                    if (materials.Length > 1)
                        suffix = " (" + i + ")";

                    materialsLayout.AddSpace(10);
                    GUIToggle foldout = new GUIToggle(new LocEdString("Material parameters" + suffix), EditorStyles.Foldout);

                    materialsLayout.AddElement(foldout);
                    GUILayoutY materialLayout = materialsLayout.AddLayoutY();

                    string tag = "Material" + i + "_Expanded";
                    foldout.OnToggled += x =>
                    {
                        materialLayout.Active = x;
                        Persistent.SetBool(tag, x);
                    };

                    materialLayout.Active = Persistent.GetBool(tag);

                    Material material = materials[i].Value;
                    if (material == null)
                    {
                        materialParams.Add(new MaterialParamGUI[0]);
                        continue;
                    }

                    MaterialVariationGUI variationGUI = new MaterialVariationGUI(material, materialLayout);
                    materialVariations.Add(variationGUI);

                    MaterialParamGUI[] matParams = MaterialInspector.CreateMaterialGUI(material,
                        "materialParams[" + i + "]", null, materialLayout);
                    materialParams.Add(matParams);
                }
            }
        }
    }

    /** @} */
}