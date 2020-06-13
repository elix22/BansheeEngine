//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2017 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="LightProbeVolume"/> component.
    /// </summary>
    [CustomInspector(typeof(LightProbeVolume))]
    internal class LightProbeVolumeInspector : Inspector
    {
        private GUILabel manualLabel = new GUILabel(new LocEdString("Manual probe placement"));
        private GUIButton addProbeButton = new GUIButton(new LocEdString("Add probe"));
        private GUIButton removeProbeButton = new GUIButton(new LocEdString("Remove probe"));

        private GUILabel uniformLabel = new GUILabel(new LocEdString("Uniform grid probe placement"));
        private GUIVector3Field positionField = new GUIVector3Field(new LocEdString("Grid origin"));
        private GUIVector3Field sizeField = new GUIVector3Field(new LocEdString("Grid size"));
        private GUIVector3Field densityField = new GUIVector3Field(new LocEdString("Grid density"));

        private GUIButton resetToGridButton = new GUIButton(new LocEdString("Reset to grid"));
        private GUIButton clipOuterButton = new GUIButton(new LocEdString("Clip to volume"));

        private GUIButton renderButton = new GUIButton(new LocEdString("Render"));

        private InspectableState modifyState;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            Layout.Clear();

            LightProbeVolume lpv = InspectedObject as LightProbeVolume;
            if (lpv == null)
                return;

            // Set up callbacks
            addProbeButton.OnClick += () =>
            {
                StartUndo();
                lpv.AddProbe(Vector3.Zero);
                EndUndo();

                MarkAsModified();
                ConfirmModify();
            };

            removeProbeButton.OnClick += () =>
            {
                if (LightProbeVolumeNodeHandles.SelectedNode != uint.MaxValue)
                {
                    StartUndo();
                    lpv.RemoveProbe((int) LightProbeVolumeNodeHandles.SelectedNode);
                    EndUndo();

                    MarkAsModified();
                    ConfirmModify();
                }
            };

            positionField.OnConfirm += x =>
            {
                AABox gridVolume = lpv.GridVolume;
                Vector3 extents = (gridVolume.Maximum - gridVolume.Minimum) * 0.5f;

                Vector3 min = positionField.Value - extents;
                Vector3 max = positionField.Value + extents;

                Vector3I cellCount = lpv.CellCount;

                gridVolume = new AABox(min, max);

                StartUndo();
                lpv.Resize(gridVolume, cellCount);
                EndUndo();

                MarkAsModified();
                ConfirmModify();
            };

            sizeField.OnConfirm += x =>
            {
                AABox gridVolume = lpv.GridVolume;

                Vector3 min = gridVolume.Minimum;
                Vector3 max = min + sizeField.Value;

                Vector3I cellCount = lpv.CellCount;

                gridVolume = new AABox(min, max);

                StartUndo();
                lpv.Resize(gridVolume, cellCount);
                EndUndo();

                MarkAsModified();
                ConfirmModify();
            };

            densityField.OnConfirm += x =>
            {
                AABox gridVolume = lpv.GridVolume;

                Vector3 density = densityField.Value;
                Vector3I cellCount = new Vector3I((int)density.x, (int)density.y, (int)density.y);

                StartUndo();
                lpv.Resize(gridVolume, cellCount);
                EndUndo();

                MarkAsModified();
                ConfirmModify();
            };

            resetToGridButton.OnClick += () =>
            {
                StartUndo();
                lpv.Reset();
                EndUndo();

                MarkAsModified();
                ConfirmModify();
            };

            clipOuterButton.OnClick += () =>
            {
                StartUndo();
                lpv.Clip();
                EndUndo();

                MarkAsModified();
                ConfirmModify();
            };

            renderButton.OnClick += () =>
            {
                lpv.RenderProbes();

                MarkAsModified();
                ConfirmModify();
            };

            // Set up layout
            Layout.AddElement(manualLabel);
            GUILayout manualLayout = Layout.AddLayoutX();
            manualLayout.AddElement(addProbeButton);
            manualLayout.AddElement(removeProbeButton);

            Layout.AddSpace(10);

            Layout.AddElement(uniformLabel);
            Layout.AddElement(positionField);
            Layout.AddElement(sizeField);
            Layout.AddElement(densityField);

            Layout.AddSpace(5);

            GUILayout uniformLayout = Layout.AddLayoutX();
            uniformLayout.AddElement(resetToGridButton);
            uniformLayout.AddElement(clipOuterButton);

            Layout.AddSpace(10);

            Layout.AddElement(renderButton);
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            LightProbeVolume lpv = InspectedObject as LightProbeVolume;
            if (lpv == null)
                return InspectableState.NotModified;

            InspectableState oldState = modifyState;
            if (modifyState.HasFlag(InspectableState.Modified))
                modifyState = InspectableState.NotModified;

            AABox gridVolume = lpv.GridVolume;
            Vector3 size = gridVolume.Maximum - gridVolume.Minimum;
            Vector3 position = gridVolume.Minimum + size * 0.5f;

            if (!positionField.HasInputFocus || force)
                positionField.Value = position;

            if (!sizeField.HasInputFocus || force)
                sizeField.Value = size;

            if (!densityField.HasInputFocus || force)
            {
                Vector3I cellCount = lpv.CellCount;
                densityField.Value = new Vector3(cellCount.x, cellCount.y, cellCount.z);
            }

            return oldState;
        }

        /// <summary>
        /// Marks the contents of the inspector as modified.
        /// </summary>
        protected void MarkAsModified()
        {
            modifyState |= InspectableState.ModifyInProgress;
        }

        /// <summary>
        /// Confirms any queued modifications.
        /// </summary>
        protected void ConfirmModify()
        {
            if (modifyState.HasFlag(InspectableState.ModifyInProgress))
                modifyState |= InspectableState.Modified;
        }
    }

    /** @} */
}
