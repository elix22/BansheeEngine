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
    /// Renders an inspector for the <see cref="Texture"/> resource.
    /// </summary>
    [CustomInspector(typeof(Texture))]
    internal class TextureInspector : Inspector
    {
        private GUIEnumField formatField = new GUIEnumField(typeof(PixelFormat), new LocEdString("Format"));
        private GUIToggleField generateMipsField = new GUIToggleField(new LocEdString("Generate mipmaps"));
        private GUIIntField maximumMipsField = new GUIIntField(new LocEdString("Maximum mipmap level"));
        private GUIToggleField srgbField = new GUIToggleField(new LocEdString("Gamma space"));
        private GUIToggleField cpuCachedField = new GUIToggleField(new LocEdString("CPU cached"));
        private GUIToggleField isCubemapField = new GUIToggleField(new LocEdString("Cubemap"));
        private GUIEnumField cubemapSourceTypeField = 
            new GUIEnumField(typeof(CubemapSourceType), new LocEdString("Cubemap source"));

        private GUIReimportButton reimportButton;
        private TextureImportOptions importOptions;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            importOptions = GetImportOptions();

            formatField.OnSelectionChanged += x => importOptions.Format = (PixelFormat)x;
            generateMipsField.OnChanged += x => importOptions.GenerateMips = x;
            maximumMipsField.OnChanged += x => importOptions.MaxMip = x;
            srgbField.OnChanged += x => importOptions.SRGB = x;
            cpuCachedField.OnChanged += x => importOptions.CpuCached = x;
            isCubemapField.OnChanged += x =>
            {
                importOptions.Cubemap = x;

                cubemapSourceTypeField.Active = x;
            };
            cubemapSourceTypeField.OnSelectionChanged += x => importOptions.CubemapSourceType = (CubemapSourceType)x;

            Layout.AddElement(formatField);
            Layout.AddElement(generateMipsField);
            Layout.AddElement(maximumMipsField);
            Layout.AddElement(srgbField);
            Layout.AddElement(cpuCachedField);
            Layout.AddElement(isCubemapField);
            Layout.AddElement(cubemapSourceTypeField);
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
            generateMipsField.Value = importOptions.GenerateMips;
            maximumMipsField.Value = importOptions.MaxMip;
            srgbField.Value = importOptions.SRGB;
            cpuCachedField.Value = importOptions.CpuCached;
            isCubemapField.Value = importOptions.Cubemap;
            cubemapSourceTypeField.Value = (ulong) importOptions.CubemapSourceType;

            cubemapSourceTypeField.Active = importOptions.Cubemap;
        }

        /// <summary>
        /// Retrieves import options for the texture we're currently inspecting.
        /// </summary>
        /// <returns>Texture import options object.</returns>
        private TextureImportOptions GetImportOptions()
        {
            TextureImportOptions output = null;

            LibraryEntry texEntry = ProjectLibrary.GetEntry(InspectedResourcePath);
            if (texEntry != null && texEntry.Type == LibraryEntryType.File)
            {
                FileEntry texFileEntry = (FileEntry)texEntry;
                output = texFileEntry.Options as TextureImportOptions;
            }

            if (output == null)
            {
                if (importOptions == null)
                    output = new TextureImportOptions();
                else
                    output = importOptions;
            }

            return output;
        }
    }

    /** @} */
}
