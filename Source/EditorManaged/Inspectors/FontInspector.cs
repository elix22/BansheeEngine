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
    /// Renders an inspector for the <see cref="Font"/> resource.
    /// </summary>
    [CustomInspector(typeof(Font))]
    internal class FontInspector : Inspector
    {
        private GUIArrayField<int, FontSizeArrayRow> fontSizes;
        private GUIArrayField<CharRange, CharRangeArrayRow> charRanges;
        private GUIEnumField renderModeField;
        private GUIToggleField boldField;
        private GUIToggleField italicField;
        private GUIIntField dpiField;
        private GUIReimportButton reimportButton;

        private FontImportOptions importOptions;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            importOptions = GetImportOptions();
            BuildGUI();
        }

        /// <summary>
        /// Recreates all the GUI elements used by this inspector.
        /// </summary>
        private void BuildGUI()
        {
            Layout.Clear();

            fontSizes = GUIArrayField<int, FontSizeArrayRow>.Create(
                new LocEdString("Font sizes"), importOptions.FontSizes, Layout);
            fontSizes.OnChanged += x => importOptions.FontSizes = x;
            fontSizes.IsExpanded = Persistent.GetBool("fontSizes_Expanded");
            fontSizes.OnExpand += x => Persistent.SetBool("fontSizes_Expanded", x);

            charRanges = GUIArrayField<CharRange, CharRangeArrayRow>.Create(
                new LocEdString("Character ranges"), importOptions.CharIndexRanges, Layout);
            charRanges.OnChanged += x => importOptions.CharIndexRanges = x;
            charRanges.IsExpanded = Persistent.GetBool("charRanges_Expanded");
            charRanges.OnExpand += x => Persistent.SetBool("charRanges_Expanded", x);

            renderModeField = new GUIEnumField(typeof(FontRenderMode), new LocEdString("Render mode"));
            renderModeField.OnSelectionChanged += x => importOptions.RenderMode = (FontRenderMode)x;

            boldField = new GUIToggleField(new LocEdString("Bold"));
            boldField.OnChanged += x => importOptions.Bold = x;

            italicField = new GUIToggleField(new LocEdString("Italic"));
            italicField.OnChanged += x => importOptions.Italic = x;

            dpiField = new GUIIntField(new LocEdString("DPI"));
            dpiField.OnChanged += x => importOptions.Dpi = x;

            Layout.AddElement(renderModeField);
            Layout.AddElement(boldField);
            Layout.AddElement(italicField);
            Layout.AddElement(dpiField);
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
            bool rebuildGUI = false;

            int[] newFontSizes = importOptions.FontSizes;
            if (newFontSizes == null)
                rebuildGUI |= fontSizes.Array != null;
            else
            {
                if (fontSizes.Array == null)
                    rebuildGUI = true;
                else
                    rebuildGUI |= newFontSizes.Length != fontSizes.Array.GetLength(0);
            }

            CharRange[] newCharRanges = importOptions.CharIndexRanges;
            if (newCharRanges == null)
                rebuildGUI |= charRanges.Array != null;
            else
            {
                if (charRanges.Array == null)
                    rebuildGUI = true;
                else
                    rebuildGUI |= newCharRanges.Length != charRanges.Array.GetLength(0);
            }

            if (rebuildGUI)
                BuildGUI();

            fontSizes.Refresh(false);
            charRanges.Refresh(false);

            renderModeField.Value = (ulong)importOptions.RenderMode;
            boldField.Value = importOptions.Bold;
            italicField.Value = importOptions.Italic;
            dpiField.Value = importOptions.Dpi;
        }

        /// <summary>
        /// Retrieves import options for the texture we're currently inspecting.
        /// </summary>
        /// <returns>Font import options object.</returns>
        private FontImportOptions GetImportOptions()
        {
            FontImportOptions output = null;

            LibraryEntry texEntry = ProjectLibrary.GetEntry(InspectedResourcePath);
            if (texEntry != null && texEntry.Type == LibraryEntryType.File)
            {
                FileEntry texFileEntry = (FileEntry)texEntry;
                output = texFileEntry.Options as FontImportOptions;
            }

            if (output == null)
            {
                if (importOptions == null)
                    output = new FontImportOptions();
                else
                    output = importOptions;
            }

            return output;
        }

        /// <summary>
        /// Row element used for displaying GUI for font size array elements.
        /// </summary>
        internal class FontSizeArrayRow : GUIListFieldRow
        {
            private GUIIntField sizeField;

            /// <inheritdoc/>
            protected override GUILayoutX CreateGUI(GUILayoutY layout)
            {
                GUILayoutX titleLayout = layout.AddLayoutX();
                sizeField = new GUIIntField(new LocEdString(SeqIndex + ". "));
                titleLayout.AddElement(sizeField);

                sizeField.OnChanged += x => { SetValue(x); MarkAsModified(); };
                sizeField.OnFocusLost += ConfirmModify;
                sizeField.OnConfirmed += ConfirmModify;

                return titleLayout;
            }

            /// <inheritdoc/>
            protected internal override InspectableState Refresh(bool force)
            {
                sizeField.Value = GetValue<int>();

                return base.Refresh(force);
            }
        }

        /// <summary>
        /// Row element used for displaying GUI for character range array elements.
        /// </summary>
        internal class CharRangeArrayRow : GUIListFieldRow
        {
            private GUIIntField rangeStartField;
            private GUIIntField rangeEndField;

            /// <inheritdoc/>
            protected override GUILayoutX CreateGUI(GUILayoutY layout)
            {
                GUILayoutX titleLayout = layout.AddLayoutX();

                rangeStartField = new GUIIntField(new LocEdString(SeqIndex + ". Start"));
                rangeEndField = new GUIIntField(new LocEdString("End"));

                titleLayout.AddElement(rangeStartField);
                titleLayout.AddElement(rangeEndField);

                rangeStartField.OnChanged += x =>
                {
                    CharRange range = GetValue<CharRange>();
                    range.start = x;
                    SetValue(range);

                    MarkAsModified();
                };

                rangeEndField.OnChanged += x =>
                {
                    CharRange range = GetValue<CharRange>();
                    range.end = x;
                    SetValue(range);

                    MarkAsModified();
                };

                rangeStartField.OnFocusLost += ConfirmModify;
                rangeStartField.OnConfirmed += ConfirmModify;

                rangeEndField.OnFocusLost += ConfirmModify;
                rangeEndField.OnConfirmed += ConfirmModify;

                return titleLayout;
            }

            /// <inheritdoc/>
            protected internal override InspectableState Refresh(bool force)
            {
                CharRange newValue = GetValue<CharRange>();
                rangeStartField.Value = newValue.start;
                rangeEndField.Value = newValue.end;

                return base.Refresh(force);
            }
        }
    }

    /** @} */
}
