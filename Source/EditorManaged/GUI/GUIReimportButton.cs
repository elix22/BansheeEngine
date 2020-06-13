using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bs.Editor
{
    /// <summary>
    /// Helper class for creating GUI elements used by various resource inspector windows.
    /// </summary>
    internal class GUIReimportButton
    {
        private string path;

        private GUIButton reimportButton = new GUIButton(new LocEdString("Reimport"));
        private GUITexture guiSpinner = new GUITexture(EditorBuiltin.GetSprite(EditorSprites.Spinner));

        /// <summary>
        /// Creates the reimport GUI elements in the provided layout.
        /// </summary>
        /// <param name="path">Path of the resource that can be reimported.</param>
        /// <param name="parent">Parent GUI layout to which to add the reimport GUI elements.</param>
        /// <param name="doReimport">User provided callback that triggers when the reimport button is clicked.</param>
        internal GUIReimportButton(string path, GUILayout parent, Action doReimport)
        {
            this.path = path;

            reimportButton.OnClick += () => doReimport();

            GUILayout reimportButtonLayout = parent.AddLayoutX();
            reimportButtonLayout.AddFlexibleSpace();
            reimportButtonLayout.AddElement(reimportButton);
            reimportButtonLayout.AddElement(guiSpinner);

            bool isImporting = ProjectLibrary.GetImportProgress(path) < 1.0f;
            guiSpinner.Active = isImporting;
            reimportButton.Active = !isImporting;
        }

        /// <summary>
        /// Checks the current state of import and updates the GUI elements accordingly.
        /// </summary>
        internal void Update()
        {
            bool isImporting = ProjectLibrary.GetImportProgress(path) < 1.0f;
            guiSpinner.Active = isImporting;
            reimportButton.Active = !isImporting;
        }
    }
}
