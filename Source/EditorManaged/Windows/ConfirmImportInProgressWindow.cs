//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using bs;

namespace bs.Editor
{
    /** @addtogroup Windows
     *  @{
     */

    /// <summary>
    /// A modal window that notifies the user that resource import is in the progress and allows him to wait until it
    /// finishes or to cancel import.
    /// </summary>
    public class ConfirmImportInProgressWindow : ModalWindow
    {
        private static ConfirmImportInProgressWindow instance;

        private GUIProgressBar progressBar;
        private GUILabel messageLabel;
        private GUIButton cancelImport;

        /// <summary>
        /// Shows the window.
        /// </summary>
        public static void Show()
        {
            if (instance == null)
            {
                instance = new ConfirmImportInProgressWindow();
                instance.BuildGUI();
            }
        }

        /// <summary>
        /// Hides the window.
        /// </summary>
        public static void Hide()
        {
            if (instance != null)
                instance.Close();

            instance = null;
        }

        /// <summary>
        /// Creates a window.
        /// </summary>
        protected ConfirmImportInProgressWindow()
            : base(false)
        {
            Width = 350;
            Height = 180;
            Title = "Import still in progress";
        }

        private void BuildGUI()
        {
            progressBar = new GUIProgressBar();
            messageLabel = new GUILabel("", EditorStyles.MultiLineLabelCentered, GUIOption.FixedHeight(60));
            cancelImport = new GUIButton(new LocEdString("Cancel import"));
            cancelImport.OnClick += () =>
            {
                ProjectLibrary.CancelImport();
                cancelImport.Disabled = true;
            };

            GUILayoutY layoutY = GUI.AddLayoutY();

            layoutY.AddFlexibleSpace();
            GUILayoutX messageLayout = layoutY.AddLayoutX();
            messageLayout.AddSpace(15);
            messageLayout.AddElement(messageLabel);
            messageLayout.AddSpace(15);

            layoutY.AddSpace(10);

            GUILayoutX barLayout = layoutY.AddLayoutX();
            barLayout.AddSpace(30);
            barLayout.AddElement(progressBar);
            barLayout.AddSpace(30);

            layoutY.AddSpace(20);

            GUILayoutX buttonLayout = layoutY.AddLayoutX();
            buttonLayout.AddFlexibleSpace();
            buttonLayout.AddElement(cancelImport);
            buttonLayout.AddFlexibleSpace();

            layoutY.AddFlexibleSpace();

            messageLabel.SetContent(new LocEdString("Resource import is still in progress. You can wait until it " +
                "finishes or cancel import. \n\nNote that even when cancelling you will need to wait for active import threads to finish."));
        }

        private void OnEditorUpdate()
        {
            progressBar.Percent = ProjectLibrary.ImportProgressPercent;
        }
    }

    /** @} */
}
