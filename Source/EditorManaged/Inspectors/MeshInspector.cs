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
    /// Renders an inspector for the <see cref="Mesh"/> resource.
    /// </summary>
    [CustomInspector(typeof(Mesh))]
    internal class MeshInspector : Inspector
    {
        private GUIToggleField normalsField;
        private GUIToggleField tangentsField;
        private GUIToggleField skinField;
        private GUIToggleField blendShapesField;
        private GUIToggleField animationField;
        private GUIFloatField scaleField;
        private GUIToggleField cpuCachedField;
        private GUIEnumField collisionMeshTypeField;
        private GUIToggleField keyFrameReductionField;
        private GUIToggleField rootMotionField;
        private GUIArrayField<AnimationSplitInfo, AnimSplitArrayRow> animSplitInfoField;
        private GUIReimportButton reimportButton;

        private MeshImportOptions importOptions;
        private AnimationSplitInfo[] splitInfos;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            importOptions = GetImportOptions();
            BuildGUI();
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            importOptions = GetImportOptions();
            reimportButton.Update();

            return InspectableState.NotModified;
        }

        /// <summary>
        /// Recreates all the GUI elements used by this inspector.
        /// </summary>
        private void BuildGUI()
        {
            Layout.Clear();

            normalsField = new GUIToggleField(new LocEdString("Import Normals"));
            tangentsField = new GUIToggleField(new LocEdString("Import Tangents"));
            skinField = new GUIToggleField(new LocEdString("Import Skin"));
            blendShapesField = new GUIToggleField(new LocEdString("Import Blend Shapes"));
            animationField = new GUIToggleField(new LocEdString("Import Animation"));
            scaleField = new GUIFloatField(new LocEdString("Scale"));
            cpuCachedField = new GUIToggleField(new LocEdString("CPU cached"));
            collisionMeshTypeField = new GUIEnumField(typeof(CollisionMeshType), new LocEdString("Collision mesh"));
            keyFrameReductionField = new GUIToggleField(new LocEdString("Keyframe Reduction"));
            rootMotionField = new GUIToggleField(new LocEdString("Import root motion"));

            normalsField.OnChanged += x => importOptions.ImportNormals = x;
            tangentsField.OnChanged += x => importOptions.ImportTangents = x;
            skinField.OnChanged += x => importOptions.ImportSkin = x;
            blendShapesField.OnChanged += x => importOptions.ImportBlendShapes = x;
            animationField.OnChanged += x => importOptions.ImportAnimation = x;
            scaleField.OnChanged += x => importOptions.ImportScale = x;
            cpuCachedField.OnChanged += x => importOptions.CpuCached = x;
            collisionMeshTypeField.OnSelectionChanged += x => importOptions.CollisionMeshType = (CollisionMeshType)x;
            keyFrameReductionField.OnChanged += x => importOptions.ReduceKeyFrames = x;
            rootMotionField.OnChanged += x => importOptions.ImportRootMotion = x;

            Layout.AddElement(normalsField);
            Layout.AddElement(tangentsField);
            Layout.AddElement(skinField);
            Layout.AddElement(blendShapesField);
            Layout.AddElement(animationField);
            Layout.AddElement(scaleField);
            Layout.AddElement(cpuCachedField);
            Layout.AddElement(collisionMeshTypeField);
            Layout.AddElement(keyFrameReductionField);
            Layout.AddElement(rootMotionField);

            splitInfos = importOptions.AnimationSplits;

            animSplitInfoField = GUIArrayField<AnimationSplitInfo, AnimSplitArrayRow>.Create(
                new LocEdString("Animation splits"), splitInfos, Layout);
            animSplitInfoField.OnChanged += x => { splitInfos = x; };
            animSplitInfoField.IsExpanded = Persistent.GetBool("animSplitInfos_Expanded");
            animSplitInfoField.OnExpand += x => Persistent.SetBool("animSplitInfos_Expanded", x);

            Layout.AddSpace(10);

            reimportButton = new GUIReimportButton(InspectedResourcePath, Layout, () =>
            {
                importOptions.AnimationSplits = splitInfos;
                ProjectLibrary.Reimport(InspectedResourcePath, importOptions, true);
            });

            UpdateGUIValues();
        }

        /// <summary>
        /// Updates the GUI element values from the current import options object.
        /// </summary>
        private void UpdateGUIValues()
        {
            animSplitInfoField.Refresh(false);

            normalsField.Value = importOptions.ImportNormals;
            tangentsField.Value = importOptions.ImportTangents;
            skinField.Value = importOptions.ImportSkin;
            blendShapesField.Value = importOptions.ImportBlendShapes;
            animationField.Value = importOptions.ImportAnimation;
            scaleField.Value = importOptions.ImportScale;
            cpuCachedField.Value = importOptions.CpuCached;
            collisionMeshTypeField.Value = (ulong)importOptions.CollisionMeshType;
            keyFrameReductionField.Value = importOptions.ReduceKeyFrames;
            rootMotionField.Value = importOptions.ImportRootMotion;
        }

        /// <summary>
        /// Retrieves import options for the mesh we're currently inspecting.
        /// </summary>
        /// <returns>Mesh import options object.</returns>
        private MeshImportOptions GetImportOptions()
        {
            MeshImportOptions output = null;

            LibraryEntry meshEntry = ProjectLibrary.GetEntry(InspectedResourcePath);
            if (meshEntry != null && meshEntry.Type == LibraryEntryType.File)
            {
                FileEntry meshFileEntry = (FileEntry)meshEntry;
                output = meshFileEntry.Options as MeshImportOptions;
            }

            if (output == null)
            {
                if (importOptions == null)
                    output = new MeshImportOptions();
                else
                    output = importOptions;
            }

            return output;
        }

        /// <summary>
        /// Row element used for displaying GUI for animation clip split information.
        /// </summary>
        private class AnimSplitArrayRow : GUIListFieldRow
        {
            private GUITextField nameField;
            private GUIIntField startFrameField;
            private GUIIntField endFrameField;
            private GUIToggleField isAdditiveField;

            /// <inheritdoc/>
            protected override GUILayoutX CreateGUI(GUILayoutY layout)
            {
                AnimationSplitInfo rowSplitInfo = GetValue<AnimationSplitInfo>();
                if (rowSplitInfo == null)
                {
                    rowSplitInfo = new AnimationSplitInfo();
                    SetValue(rowSplitInfo);
                }

                GUILayoutX titleLayout = layout.AddLayoutX();
                GUILayoutX contentLayout = layout.AddLayoutX();

                GUILabel title = new GUILabel(new LocEdString(SeqIndex + ". "));
                nameField = new GUITextField(new LocEdString("Name"), 40, false, "", GUIOption.FixedWidth(160));
                startFrameField = new GUIIntField(new LocEdString("Start"), 40, "", GUIOption.FixedWidth(80));
                endFrameField = new GUIIntField(new LocEdString("End"), 40, "", GUIOption.FixedWidth(80));
                isAdditiveField = new GUIToggleField(new LocEdString("Is additive"), 50, "", GUIOption.FixedWidth(80));

                startFrameField.SetRange(0, int.MaxValue);
                endFrameField.SetRange(0, int.MaxValue);

                titleLayout.AddElement(title);
                titleLayout.AddElement(nameField);
                titleLayout.AddFlexibleSpace();
                contentLayout.AddSpace(10);
                contentLayout.AddElement(startFrameField);
                contentLayout.AddSpace(5);
                contentLayout.AddElement(endFrameField);
                contentLayout.AddSpace(5);
                contentLayout.AddElement(isAdditiveField);

                nameField.OnChanged += x =>
                {
                    AnimationSplitInfo splitInfo = GetValue<AnimationSplitInfo>();
                    splitInfo.Name = x;
 
                    MarkAsModified();
                };

                nameField.OnFocusLost += ConfirmModify;
                nameField.OnConfirmed += ConfirmModify;

                startFrameField.OnChanged += x =>
                {
                    AnimationSplitInfo splitInfo = GetValue<AnimationSplitInfo>();
                    splitInfo.StartFrame = x;

                    MarkAsModified();
                };

                startFrameField.OnFocusLost += ConfirmModify;
                startFrameField.OnConfirmed += ConfirmModify;

                endFrameField.OnChanged += x =>
                {
                    AnimationSplitInfo splitInfo = GetValue<AnimationSplitInfo>();
                    splitInfo.EndFrame = x;

                    MarkAsModified();
                };

                endFrameField.OnFocusLost += ConfirmModify;
                endFrameField.OnConfirmed += ConfirmModify;

                isAdditiveField.OnChanged += x =>
                {
                    AnimationSplitInfo splitInfo = GetValue<AnimationSplitInfo>();
                    splitInfo.IsAdditive = x;

                    MarkAsModified();
                    ConfirmModify();
                };

                return titleLayout;
            }

            /// <inheritdoc/>
            protected internal override InspectableState Refresh(bool force)
            {
                AnimationSplitInfo splitInfo = GetValue<AnimationSplitInfo>();

                if (splitInfo != null)
                {
                    nameField.Value = splitInfo.Name;
                    startFrameField.Value = splitInfo.StartFrame;
                    endFrameField.Value = splitInfo.EndFrame;
                    isAdditiveField.Value = splitInfo.IsAdditive;
                }

                return base.Refresh(force);
            }
        }
    }

    /** @} */
}
