//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Windows
     *  @{
     */

    /// <summary>
    /// Editor window that displays the scene hierarchy tree view, displaying all scene objects in the current scene.
    /// </summary>
    public class HierarchyWindow : EditorWindow, IGlobalShortcuts
    {
        private GUIScrollArea treeScrollArea;
        private GUISceneTreeView treeView;

        private GUILayout progressLayout;
        private GUIProgressBar loadProgressBar;
        private GUILabel loadLabel;

        private bool loadingProgressShown = false;
        private UUID sceneUUID = UUID.Empty;

        /// <summary>
        /// Opens the hierarchy window.
        /// </summary>
        [MenuItem("Windows/Hierarchy", ButtonModifier.CtrlAlt, ButtonCode.H, 6000)]
        private static void OpenHierarchyWindow()
        {
            OpenWindow<HierarchyWindow>();
        }

        /// <inheritdoc/>
        protected override LocString GetDisplayName()
        {
            return new LocEdString("Hierarchy");
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnDeletePressed()
        {
            treeView.DeleteSelection();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnRenamePressed()
        {
            treeView.RenameSelection();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnDuplicatePressed()
        {
            treeView.DuplicateSelection();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnCopyPressed()
        {
            treeView.CopySelection();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnCutPressed()
        {
            treeView.CutSelection();
        }

        /// <inheritdoc/>
        void IGlobalShortcuts.OnPastePressed()
        {
            treeView.PasteToSelection();
        }

        private void OnInitialize()
        {
            treeScrollArea = new GUIScrollArea();
            GUI.AddElement(treeScrollArea);

            treeView = new GUISceneTreeView(GUIOption.FlexibleHeight(20), GUIOption.FlexibleWidth(20));
            treeScrollArea.Layout.AddElement(treeView);

            // Loading progress
            loadLabel = new GUILabel(new LocEdString("Loading scene..."));
            loadProgressBar = new GUIProgressBar();

            progressLayout = GUI.AddLayoutY();
            progressLayout.AddFlexibleSpace();
            GUILayout loadLabelLayout = progressLayout.AddLayoutX();
            loadLabelLayout.AddFlexibleSpace();
            loadLabelLayout.AddElement(loadLabel);
            loadLabelLayout.AddFlexibleSpace();

            GUILayout progressBarLayout = progressLayout.AddLayoutX();
            progressBarLayout.AddFlexibleSpace();
            progressBarLayout.AddElement(loadProgressBar);
            progressBarLayout.AddFlexibleSpace();
            progressLayout.AddFlexibleSpace();

            progressLayout.Active = false;

            EditorVirtualInput.OnButtonUp += OnButtonUp;
        }

        private void OnEditorUpdate()
        {
            UpdateLoadingProgress();
            treeView.Update();

            if (Scene.ActiveSceneUUID != sceneUUID)
            {
                if(EditorApplication.EditorSceneData != null)
                    LoadHierarchyState(EditorApplication.EditorSceneData);

                sceneUUID = Scene.ActiveSceneUUID;
            }
        }

        private void OnDestroy()
        {
            EditorVirtualInput.OnButtonUp -= OnButtonUp;
        }

        /// <summary>
        /// Updates the scene data with data from the hierarchy view.
        /// </summary>
        /// <param name="sceneData">Scene data to append hierarchy state to.</param>
        internal void SaveHierarchyState(EditorSceneData sceneData)
        {
            if (treeView == null)
                return;

            SceneTreeViewState state = treeView.State;
            Dictionary<UUID, EditorSceneObject> lookup = sceneData.GetLookup();

            foreach (var entry in state.Elements)
            {
                if (lookup.TryGetValue(entry.sceneObject.UUID, out EditorSceneObject editorSO))
                    editorSO.IsExpanded = entry.isExpanded;
            }
        }

        /// <summary>
        /// Loads hierarchy state from the stored scene data.
        /// </summary>
        /// <param name="sceneData">Scene data to restore the hierarcy state from.</param>
        internal void LoadHierarchyState(EditorSceneData sceneData)
        {
            SceneTreeViewState state = treeView.State;
            Dictionary<UUID, EditorSceneObject> lookup = sceneData.GetLookup();

            SceneTreeViewElement[] elements = state.Elements;
            for(int i = 0; i < elements.Length; i++)
            {
                if (lookup.TryGetValue(elements[i].sceneObject.UUID, out EditorSceneObject editorSO))
                    elements[i].isExpanded = editorSO.IsExpanded;
            }

            state.Elements = elements;
            treeView.State = state;
        }

        /// <summary>
        /// Checks if the load progress bar needs to be shown, shows/hides it and updates the progress accordingly.
        /// </summary>
        private void UpdateLoadingProgress()
        {
            bool needsProgress = EditorApplication.IsSceneLoading;

            if (needsProgress && !loadingProgressShown)
            {
                progressLayout.Active = true;
                treeScrollArea.Active = false;

                loadingProgressShown = true;
            }
            else if(!needsProgress && loadingProgressShown)
            {
                progressLayout.Active = false;
                treeScrollArea.Active = true;

                loadingProgressShown = false;
            }

            if (needsProgress)
                loadProgressBar.Percent = EditorApplication.SceneLoadProgress;
        }

        /// <summary>
        /// Triggered when the user presses a virtual button.
        /// </summary>
        /// <param name="btn">Button that was pressed.</param>
        /// <param name="deviceIdx">Index of the device it was pressed on. </param>
        private void OnButtonUp(VirtualButton btn, int deviceIdx)
        {
            if (!HasFocus)
                return;

            IGlobalShortcuts shortcuts = this;

            if (btn == EditorApplication.CopyKey)
                shortcuts.OnCopyPressed();
            else if (btn == EditorApplication.CutKey)
                shortcuts.OnCutPressed();
            else if (btn == EditorApplication.PasteKey)
                shortcuts.OnPastePressed();
            else if (btn == EditorApplication.DuplicateKey)
                shortcuts.OnDuplicatePressed();
            else if (btn == EditorApplication.RenameKey)
                shortcuts.OnRenamePressed();
            else if (btn == EditorApplication.DeleteKey)
                shortcuts.OnDeletePressed();
            else if(btn == EditorApplication.PasteKey)
                shortcuts.OnPastePressed();
        }
    }

    /** @} */
}
