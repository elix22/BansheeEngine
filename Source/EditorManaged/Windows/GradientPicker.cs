//********************************** Banshee Engine (www.banshee4d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Windows
     *  @{
     */

    /// <summary>
    /// A color gradient editor window that allows the user to add or modify colors from a color gradient.
    /// </summary>
    public class GradientPicker : ModalWindow
    {
        private const int EDITOR_HORZ_PADDING = 10;
        private const int TEX_WIDTH = 256;
        private const int TEX_HEIGHT = 4;

        private ColorGradient gradient;
        private GradientKeyEditor editor;
        private GUITexture guiGradientTexture;
        private GUICanvas overlayCanvas;

        private GUIPanel editorPanel;
        private GUIButton guiOK;
        private GUIButton guiCancel;

        private Texture texture;
        private SpriteTexture spriteTexture;


        private Action<bool, ColorGradient> closedCallback;

        /// <summary>
        /// Shows the gradient picker window.
        /// </summary>
        /// <param name="closedCallback">Optional callback to trigger when the user finishes editing the gradient or
        ///                              cancels out of the dialog.</param>
        /// <returns>An instance of the gradient picker window.</returns>
        public static GradientPicker Show(Action<bool, ColorGradient> closedCallback = null)
        {
            GradientPicker picker = new GradientPicker(new ColorGradient(), closedCallback);
            return picker;
        }

        /// <summary>
        /// Shows the gradient picker window and sets the initial gradient to show.
        /// </summary>
        /// <param name="gradient">Gradient to initially display in the window.</param>
        /// <param name="closedCallback">Optional callback to trigger when the user finishes editing the gradient or
        ///                              cancels out of the dialog.</param>
        /// <returns>A. instance of the gradient picker window.</returns>
        public static GradientPicker Show(ColorGradient gradient, Action<bool, ColorGradient> closedCallback = null)
        {
            GradientPicker picker = new GradientPicker(gradient, closedCallback);
            return picker;
        }

        /// <summary>
        /// Constructs a new gradient picker window.
        /// </summary>
        /// <param name="gradient">Gradient to initially display in the window.</param>
        /// <param name="closedCallback">Optional callback to trigger when the user finishes editing the gradient or
        ///                              cancels out of the dialog.</param>
        protected GradientPicker(ColorGradient gradient, Action<bool, ColorGradient> closedCallback = null)
            : base(false)
        {
            Title = new LocEdString("Gradient Picker");
            Width = 270;
            Height = 135;

            this.gradient = gradient;
            this.closedCallback = closedCallback;
        }

        private void OnInitialize()
        {
            guiOK = new GUIButton(new LocEdString("OK"));
            guiCancel = new GUIButton(new LocEdString("Cancel"));

            guiOK.OnClick += OnOK;
            guiCancel.OnClick += OnCancel;

            GUILayout mainVertLayout = GUI.AddLayoutY();
            mainVertLayout.AddSpace(10);

            GUILayout editorHorzLayout = mainVertLayout.AddLayoutX();
            editorHorzLayout.AddSpace(EDITOR_HORZ_PADDING);
            GUIPanel gradientEditorPanel = editorHorzLayout.AddPanel();
            editorHorzLayout.AddSpace(EDITOR_HORZ_PADDING);

            mainVertLayout.AddSpace(15);

            GUILayout buttonHorzLayout = mainVertLayout.AddLayoutX();
            buttonHorzLayout.AddFlexibleSpace();
            buttonHorzLayout.AddElement(guiOK);
            buttonHorzLayout.AddSpace(10);
            buttonHorzLayout.AddElement(guiCancel);
            buttonHorzLayout.AddFlexibleSpace();

            mainVertLayout.AddFlexibleSpace();

            editorPanel = gradientEditorPanel.AddPanel(0);
            GUIPanel editorOverlay = gradientEditorPanel.AddPanel(-1);

            overlayCanvas = new GUICanvas();
            editorOverlay.AddElement(overlayCanvas);

            GUILayout editorVertLayout = editorPanel.AddLayoutY();

            GUILayout guiGradientLayout = editorVertLayout.AddLayoutX();
            guiGradientLayout.AddSpace(GradientKeyEditor.RECT_WIDTH / 2);

            texture = Texture.Create2D(TEX_WIDTH, TEX_HEIGHT);
            spriteTexture = new SpriteTexture(texture);

            guiGradientTexture = new GUITexture(spriteTexture, GUITextureScaleMode.StretchToFit);
            guiGradientTexture.SetHeight(30);

            UpdateTexture();

            guiGradientLayout.AddElement(guiGradientTexture);
            guiGradientLayout.AddSpace(GradientKeyEditor.RECT_WIDTH / 2);

            editorVertLayout.AddSpace(10);

            editor = new GradientKeyEditor(editorVertLayout, gradient.GetKeys(), Width - EDITOR_HORZ_PADDING * 2, 20);
            editor.OnGradientModified += colorGradient =>
            {
                gradient = colorGradient;

                UpdateTexture();
                UpdateKeyLines();
            };

            editorVertLayout.AddFlexibleSpace();

            GUITexture containerBg = new GUITexture(null, EditorStylesInternal.ContainerBg);
            Rect2I containerBounds = editor.GetBounds(GUI);
            containerBounds.x -= 2;
            containerBounds.y -= 2;
            containerBounds.width += 4;
            containerBounds.height += 6;
            containerBg.Bounds = containerBounds;

            GUIPanel editorUnderlay = GUI.AddPanel(1);
            editorUnderlay.AddElement(containerBg);

            UpdateKeyLines();

            EditorInput.OnPointerPressed += OnPointerPressed;
            EditorInput.OnPointerDoubleClick += OnPointerDoubleClicked;
            EditorInput.OnPointerMoved += OnPointerMoved;
            EditorInput.OnPointerReleased += OnPointerReleased;
            EditorInput.OnButtonUp += OnButtonUp;
        }

        private void OnDestroy()
        {
            EditorInput.OnPointerPressed -= OnPointerPressed;
            EditorInput.OnPointerDoubleClick -= OnPointerDoubleClicked;
            EditorInput.OnPointerMoved -= OnPointerMoved;
            EditorInput.OnPointerReleased -= OnPointerReleased;
            EditorInput.OnButtonUp -= OnButtonUp;
        }

        /// <summary>
        /// Draws lines connecting the individual key color over the gradient itself.
        /// </summary>
        private void UpdateKeyLines()
        {
            overlayCanvas.Clear();

            ColorGradientKey[] keys = gradient.GetKeys();
            for (int i = 0; i < keys.Length; i++)
            {
                int pixel = editor.TimeToPixel(keys[i].time);

                overlayCanvas.DrawLine(new Vector2I(pixel, 0), new Vector2I(pixel, 40), Color.DarkGray);
            }
        }

        /// <summary>
        /// Updates the gradient texture.
        /// </summary>
        public void UpdateTexture()
        {
            const int width = 256;
            const int height = 4;
            Color[] colors = new Color[width * height];

            float halfPixel = 0.5f / width;
            if (gradient.NumKeys > 0)
            {
                for (int x = 0; x < width; x++)
                {
                    Color color = gradient.Evaluate(x / (float) width + halfPixel);

                    for (int y = 0; y < height; y++)
                        colors[y * width + x] = color;
                }
            }
            else
            {
                for (int x = 0; x < width; x++)
                {
                    for (int y = 0; y < height; y++)
                        colors[y * width + x] = Color.Black;
                }
            }

            texture.SetPixels(colors);
            guiGradientTexture.SetTexture(spriteTexture);
        }

        /// <summary>
        /// Triggered when the user selects a gradient and closes the dialog.
        /// </summary>
        void OnOK()
        {
            closedCallback?.Invoke(true, gradient);

            Close();
        }

        /// <summary>
        /// Triggered when the user cancels gradient editing and closes the dialog.
        /// </summary>
        void OnCancel()
        {
            closedCallback?.Invoke(false, gradient);

            Close();
        }

        #region Input callbacks
        /// <summary>
        /// Triggered when the user presses a mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse press event.</param>
        private void OnPointerPressed(PointerEvent ev)
        {
            if (ev.IsUsed)
                return;

            editor.OnPointerPressed(ScreenToKeyEditorPos(ev.ScreenPos), ev.Button);
        }

        /// <summary>
        /// Triggered when the user double clicks the left mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse event.</param>
        private void OnPointerDoubleClicked(PointerEvent ev)
        {
            if (ev.IsUsed)
                return;

            Vector2I panelPos = ScreenToKeyEditorPos(ev.ScreenPos);
            Rect2I guiGradientBounds = guiGradientTexture.Bounds;

            if (guiGradientBounds.Contains(panelPos))
            {
                Vector2I canvasPos = panelPos - new Vector2I(guiGradientBounds.x, guiGradientBounds.y);

                float time = canvasPos.x / (float) Math.Max(guiGradientBounds.width - 1, 1);
                if (time >= 0.0f && time <= 1.0f)
                {
                    List<ColorGradientKey> keys = new List<ColorGradientKey>(gradient.GetKeys());

                    keys.Add(new ColorGradientKey(Color.Black, time));
                    keys.Sort((lhs, rhs) => lhs.time.CompareTo(rhs.time));

                    gradient = new ColorGradient(keys.ToArray());

                    UpdateTexture();
                    editor.Rebuild(keys);
                    UpdateKeyLines();
                }
            }
            else
                editor.OnPointerDoubleClicked(panelPos, ev.Button);
        }

        /// <summary>
        /// Triggered when the user moves the mouse.
        /// </summary>
        /// <param name="ev">Information about the mouse move event.</param>
        private void OnPointerMoved(PointerEvent ev)
        {
            if (ev.IsUsed)
                return;

            editor.OnPointerMoved(ScreenToKeyEditorPos(ev.ScreenPos), ev.Button);
        }

        /// <summary>
        /// Triggered when the user releases a mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse release event.</param>
        private void OnPointerReleased(PointerEvent ev)
        {
            if (ev.IsUsed)
                return;

            editor.OnPointerReleased(ScreenToKeyEditorPos(ev.ScreenPos));
        }

        /// <summary>
        /// Triggered when the user releases a keyboard button.
        /// </summary>
        /// <param name="ev">Information about the keyboard release event.</param>
        private void OnButtonUp(ButtonEvent ev)
        {
            editor.OnButtonUp(ev);
        }

        /// <summary>
        /// Converts screen coordinates in coordinates relative to the panel containing the key editor control.
        /// </summary>
        /// <param name="screenPos">Coordinates in screen space.</param>
        /// <returns>Coordinates relative to the key editor control.</returns>
        private Vector2I ScreenToKeyEditorPos(Vector2I screenPos)
        {
            Vector2I windowPos = ScreenToWindowPos(screenPos);

            Rect2I elementBounds = GUIUtility.CalculateBounds(editorPanel, GUI);
            return windowPos - new Vector2I(elementBounds.x, elementBounds.y);
        }
        #endregion

        /// <summary>
        /// Handles drawing and editing of individual keys present on the color gradient.
        /// </summary>
        public class GradientKeyEditor
        {
            public const int RECT_WIDTH = 14;
            private static readonly Color SELECTED_COLOR = Color.BansheeOrange;
            private static readonly Color PLAIN_COLOR = Color.DarkGray;
            private const int DRAG_START_DISTANCE = 3;

            private GUICanvas canvas;
            private List<ColorGradientKey> keys = new List<ColorGradientKey>();
            private int width;
            private int height;
            private int selectedIdx = -1;

            private bool isMousePressedOverKey;
            private bool isDragInProgress;
            private Vector2I dragStart;

            /// <summary>
            /// Triggered whenever keyframe in a gradient is modified (added, removed or edited).
            /// </summary>
            public Action<ColorGradient> OnGradientModified;

            /// <summary>
            /// Constructs a new gradient key editor control.
            /// </summary>
            /// <param name="parent">GUI layout to attach the child GUI controls to.</param>
            /// <param name="keys">Set of keys to initially display on the editor.</param>
            /// <param name="width">Width of the editor in pixels.</param>
            /// <param name="height">Height of the editor in pixels.</param>
            public GradientKeyEditor(GUILayout parent, ColorGradientKey[] keys, int width, int height)
            {
                canvas = new GUICanvas();
                parent.AddElement(canvas);

                Rebuild(new List<ColorGradientKey>(keys), width, height);
            }

            /// <summary>
            /// Rebuilds the editor display using the provided color keys.
            /// </summary>
            /// <param name="keys">Set of keys to initially display on the editor.</param>
            public void Rebuild(List<ColorGradientKey> keys)
            {
                this.keys = keys;

                canvas.Clear();
                for (int i = 0; i < keys.Count; i++)
                    DrawColor(keys[i].color, MathEx.Clamp01(keys[i].time), i == selectedIdx);
            }

            /// <summary>
            /// Returns the bounds of the GUI element relative to the provided panel.
            /// </summary>
            public Rect2I GetBounds(GUIPanel relativeTo)
            {
                return GUIUtility.CalculateBounds(canvas, relativeTo);
            }

            /// <summary>
            /// Rebuilds the editor display using the provided size and color keys.
            /// </summary>
            /// <param name="keys">Set of keys to initially display on the editor.</param>
            /// <param name="width">Width of the editor in pixels.</param>
            /// <param name="height">Height of the editor in pixels.</param>
            private void Rebuild(List<ColorGradientKey> keys, int width, int height)
            {
                this.width = width;
                this.height = height;

                canvas.SetWidth(width);
                canvas.SetHeight(height);

                Rebuild(keys);
            }

            /// <summary>
            /// Rebuilds the editor display using the currently set properties.
            /// </summary>
            private void Rebuild()
            {
                Rebuild(keys, width, height);
            }

            /// <summary>
            /// Attempts to find a color key rectangle at the specified location.
            /// </summary>
            /// <param name="pixelCoords">Pixel position to look at, relative to the canvas element.</param>
            /// <param name="keyIdx">Index of the color key, if any was found.</param>
            /// <returns>True if the key was found under the provided position, false otherwise.</returns>
            public bool FindKey(Vector2I pixelCoords, out int keyIdx)
            {
                keyIdx = -1;

                if (pixelCoords.y < 0 || pixelCoords.y >= height)
                    return false;

                for (int i = 0; i < keys.Count; i++)
                {
                    float t = MathEx.Clamp01(keys[i].time);
                    int x = (int)(t * Math.Max(width - RECT_WIDTH, 0));

                    if (pixelCoords.x >= x & pixelCoords.x < (x + RECT_WIDTH))
                    {
                        keyIdx = i;
                        return true;
                    }
                }

                return false;
            }

            /// <summary>
            /// Handles input. Should be called by the owning window whenever a pointer is pressed.
            /// </summary>
            /// <param name="panelPos">Position of the pointer relative to the panel parent to this element.</param>
            /// <param name="button">Pointer button involved in the event.</param>
            internal void OnPointerPressed(Vector2I panelPos, PointerButton button)
            {
                Rect2I canvasBounds = canvas.Bounds;
                Vector2I canvasPos = panelPos - new Vector2I(canvasBounds.x, canvasBounds.y);

                if (button == PointerButton.Left)
                {
                    int keyIdx;
                    if (FindKey(canvasPos, out keyIdx))
                    {
                        selectedIdx = keyIdx;
                        isMousePressedOverKey = true;
                        dragStart = canvasPos;
                    }
                    else
                        selectedIdx = -1;
                    
                    Rebuild();
                }
            }

            /// <summary>
            /// Handles input. Should be called by the owning window whenever a pointer is double-clicked.
            /// </summary>
            /// <param name="panelPos">Position of the pointer relative to the panel parent to this element.</param>
            /// <param name="button">Pointer button involved in the event.</param>
            internal void OnPointerDoubleClicked(Vector2I panelPos, PointerButton button)
            {
                Rect2I canvasBounds = canvas.Bounds;
                if (!canvasBounds.Contains(panelPos))
                    return;

                Vector2I canvasPos = panelPos - new Vector2I(canvasBounds.x, canvasBounds.y);

                int keyIdx;
                if (FindKey(canvasPos, out keyIdx))
                {
                    ColorPicker.Show(keys[keyIdx].color, false,
                        (b, color) =>
                        {
                            if (b)
                            {
                                ColorGradientKey key = keys[keyIdx];
                                key.color = color;
                                keys[keyIdx] = key;

                                OnGradientModified?.Invoke(new ColorGradient(keys.ToArray()));
                                Rebuild();
                            }
                        });
                }
                else
                {
                    float time = PixelToTime(canvasPos.x);
                    if (time >= 0.0f && time <= 1.0f)
                    {
                        keys.Add(new ColorGradientKey(Color.Black, time));
                        keys.Sort((lhs, rhs) => lhs.time.CompareTo(rhs.time));

                        OnGradientModified?.Invoke(new ColorGradient(keys.ToArray()));
                    }

                    Rebuild();
                }
            }

            /// <summary>
            /// Handles input. Should be called by the owning window whenever a pointer is moved.
            /// </summary>
            /// <param name="panelPos">Position of the pointer relative to the panel parent to this element.</param>
            /// <param name="button">Pointer button involved in the event.</param>
            internal void OnPointerMoved(Vector2I panelPos, PointerButton button)
            {
                if (button != PointerButton.Left)
                    return;

                if (isMousePressedOverKey)
                {
                    Rect2I canvasBounds = canvas.Bounds;
                    Vector2I canvasPos = panelPos - new Vector2I(canvasBounds.x, canvasBounds.y);

                    if (!isDragInProgress)
                    {
                        int distance = Vector2I.Distance(canvasPos, dragStart);
                        if (distance >= DRAG_START_DISTANCE)
                            isDragInProgress = true;
                    }

                    if (isDragInProgress)
                    {
                        float time = PixelToTime(canvasPos.x);
                        if (time >= 0.0f && time <= 1.0f)
                        {
                            ColorGradientKey key = keys[selectedIdx];
                            key.time = time;
                            keys[selectedIdx] = key;

                            OnGradientModified?.Invoke(new ColorGradient(keys.ToArray()));
                        }

                        Rebuild();
                    }
                }
            }

            /// <summary>
            /// Handles input. Should be called by the owning window whenever a pointer is released.
            /// </summary>
            /// <param name="panelPos">Position of the pointer relative to the panel parent to this element.</param>
            internal void OnPointerReleased(Vector2I panelPos)
            {
                isDragInProgress = false;
                isMousePressedOverKey = false;
            }

            /// <summary>
            /// Handles input. Should be called by the owning window whenever a button is released.
            /// </summary>
            /// <param name="ev">Object containing button release event information.</param>
            internal void OnButtonUp(ButtonEvent ev)
            {
                if (ev.Button == ButtonCode.Delete && selectedIdx != -1 && !isMousePressedOverKey)
                {
                    if (selectedIdx < keys.Count)
                    {
                        keys.RemoveAt(selectedIdx);
                        OnGradientModified?.Invoke(new ColorGradient(keys.ToArray()));
                    }

                    selectedIdx = -1;
                    Rebuild();
                }
            }

            /** Converts a pixel position over the editor (on x axis) to a time in range [0, 1]. */
            internal float PixelToTime(int x)
            {
                x -= RECT_WIDTH / 2;

                return x / (float) Math.Max(this.width - RECT_WIDTH, 0);
            }

            /** Converts a time in range [0, 1] to a pixel position over the editor (on x axis). */
            internal int TimeToPixel(float t)
            {
                return (int) Math.Min(t * Math.Max(width - RECT_WIDTH, 0), width - RECT_WIDTH - 1) + RECT_WIDTH / 2;
            }

            /// <summary>
            /// Draws a rectangle representing a single color key in the gradient.
            /// </summary>
            /// <param name="color">Color to display.</param>
            /// <param name="t">Time at which to draw the rectangle, in range [0, 1].</param>
            /// <param name="selected">True to draw the rectangle as selected, plain otherwise.</param>
            private void DrawColor(Color color, float t, bool selected)
            {
                int x = TimeToPixel(t);

                Vector2I a = new Vector2I(x + RECT_WIDTH / 2, 0);
                Vector2I b = new Vector2I(x + RECT_WIDTH / 2, height - 1);
                Vector2I c = new Vector2I(x - RECT_WIDTH / 2, 0);
                Vector2I d = new Vector2I(x - RECT_WIDTH / 2, height - 1);

                Vector2I[] linePoints = new Vector2I[] { a, b, d, c, a };
                Vector2I[] trianglePoints = new Vector2I[] { a, b, c, d };

                Color colorNoAlpha = color;
                colorNoAlpha.a = 1.0f;

                canvas.DrawTriangleStrip(trianglePoints, colorNoAlpha, 102);
                canvas.DrawPolyLine(linePoints, selected ? SELECTED_COLOR : PLAIN_COLOR, 100);

                Vector2I alphaA = new Vector2I(x - 1, height - 1);
                Vector2I alphaB = new Vector2I(x + RECT_WIDTH / 2, height / 2);
                Vector2I alphaC = new Vector2I(x + RECT_WIDTH / 2, height - 1);

                canvas.DrawTriangleList(new [] { alphaA, alphaB, alphaC }, Color.White * color.a, 101);
            }
        };
    }

    /** @} */
}
