namespace bs.Editor
{
    /** @addtogroup Scene-Editor
     *  @{
     */

    /// <summary>
    /// Shows a pinnable preview of a <see cref="Camera"/> on the scene editor window.
    /// </summary>
    internal class CameraPreview
    {
        public bool IsPinned { get; private set; } = false;
        public Camera Camera { get; }

        private GUIPanel previewPanel;

        // Render texture
        private GUIPanel renderTexturePanel;
        private GUIRenderTexture renderTextureGUI;

        // Controls
        private GUIPanel controlsPanel;

        private GUILabel cameraNameLabel;
        private GUIButton pinButton;

        public CameraPreview(Camera camera, GUIPanel previewsPanel)
        {
            Camera = camera;
            previewPanel = previewsPanel.AddPanel();

            // Render texture GUI
            renderTexturePanel = previewPanel.AddPanel();
            renderTextureGUI = new GUIRenderTexture(null);
            renderTexturePanel.AddElement(renderTextureGUI);

            // Control GUI
            controlsPanel = previewPanel.AddPanel(-1);
            GUILayoutX controlsLayout = controlsPanel.AddLayoutX();
            controlsLayout.SetHeight(16);

            cameraNameLabel = new GUILabel(string.Empty);
            pinButton = new GUIButton(string.Empty);
            pinButton.SetWidth(16);
            pinButton.SetHeight(16);
            pinButton.OnClick += () =>
            {
                IsPinned = !IsPinned;
                UpdatePinButton();
            };

            controlsLayout.AddElement(cameraNameLabel);
            controlsLayout.AddFlexibleSpace();
            controlsLayout.AddElement(pinButton);

            UpdatePinButton();
        }

        /// <summary>
        /// Sets the content of the pin button according to its current state.
        /// </summary>
        private void UpdatePinButton()
        {
            pinButton.SetContent(new GUIContent(IsPinned ? "-" : "+", IsPinned ? "Unpin" : "Pin"));
        }

        /// <summary>
        /// Shows a preview of the specified camera with the specified bounds.
        /// </summary>
        /// <param name="camera">The camera to preview</param>
        /// <param name="bounds">The bounds of the preview</param>
        public void ShowPreview(Camera camera, Rect2I bounds)
        {
            previewPanel.Bounds = bounds;
            cameraNameLabel.SetContent(camera.SceneObject?.Name);
            renderTextureGUI.SetWidth(bounds.width);
            renderTextureGUI.SetHeight(bounds.height);

            var cameraRenderTexture = (RenderTexture)camera.Viewport.Target;
            if (cameraRenderTexture != null)
            {
                var renderTexture = new RenderTexture(cameraRenderTexture.ColorSurface);
                renderTextureGUI.RenderTexture = renderTexture;
            }
            else
            {
                // TODO: We cannot preview cameras that don't have a render target because we need preview support for
                // setting a temporary render target for preview purposes
            }
        }

        /// <summary>
        /// Destroys the GUI elements representing the preview box.
        /// </summary>
        public void Destroy()
        {
            previewPanel?.Destroy();
        }
    }

    /** @} */
}
