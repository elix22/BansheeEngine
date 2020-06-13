//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="AudioClip"/> resource.
    /// </summary>
    [CustomInspector(typeof(AudioClip))]
    internal class AudioClipInspector : Inspector
    {
        private GUIEnumField formatField = new GUIEnumField(typeof(AudioFormat), new LocEdString("Format"));
        private GUIEnumField readModeField = new GUIEnumField(typeof(AudioReadMode), new LocEdString("Read mode"));
        private GUIEnumField bitDepthField = new GUIEnumField(typeof(AudioBitDepth), new LocEdString("Bit depth"));
        private GUIToggleField is3DField = new GUIToggleField(new LocEdString("3D"));

        private GUIReimportButton reimportButton;

        private AudioClipImportOptions importOptions;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            importOptions = GetImportOptions();

            formatField.OnSelectionChanged += x => importOptions.Format = (AudioFormat)x;
            readModeField.OnSelectionChanged += x => importOptions.ReadMode = (AudioReadMode)x;
            bitDepthField.OnSelectionChanged += x => importOptions.BitDepth = (int)x;
            is3DField.OnChanged += x => importOptions.Is3D = x;

            Layout.AddElement(formatField);
            Layout.AddElement(readModeField);
            Layout.AddElement(bitDepthField);
            Layout.AddElement(is3DField);
            Layout.AddSpace(10);

            reimportButton = new GUIReimportButton(InspectedResourcePath, Layout, () =>
            {
                ProjectLibrary.Reimport(InspectedResourcePath, importOptions, true);
            });

            UpdateGUIValues();
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            reimportButton.Update();

            return InspectableState.NotModified;
        }

        /// <summary>
        /// Updates the GUI element values from the current import options object.
        /// </summary>
        private void UpdateGUIValues()
        {
            formatField.Value = (ulong)importOptions.Format;
            readModeField.Value = (ulong)importOptions.ReadMode;
            bitDepthField.Value = (ulong)importOptions.BitDepth;
            is3DField.Value = importOptions.Is3D; ;
        }

        /// <summary>
        /// Retrieves import options for the audio clip we're currently inspecting.
        /// </summary>
        /// <returns>Audio clip import options object.</returns>
        private AudioClipImportOptions GetImportOptions()
        {
            AudioClipImportOptions output = null;

            LibraryEntry meshEntry = ProjectLibrary.GetEntry(InspectedResourcePath);
            if (meshEntry != null && meshEntry.Type == LibraryEntryType.File)
            {
                FileEntry meshFileEntry = (FileEntry)meshEntry;
                output = meshFileEntry.Options as AudioClipImportOptions;
            }

            if (output == null)
            {
                if (importOptions == null)
                    output = new AudioClipImportOptions();
                else
                    output = importOptions;
            }

            return output;
        }
    }

    /** @} */
}
