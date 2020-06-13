//********************************** Banshee Engine (www.banshee5d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using bs;

namespace bs.Editor
{
    /// <summary>
    /// Opens a window that allows the user to edit a single animation curve or two curves representing a range.
    /// </summary>
    public class CurveEditorWindow : ModalWindow
    {
        private EdAnimationCurve curveA; 
        private EdAnimationCurve curveB; 

        private GUICurveEditor curveEditor;
        private GUIButton guiOK;
        private GUIButton guiCancel;

        private Action<bool, AnimationCurve, AnimationCurve> closedCallbackRange;
        private Action<bool, AnimationCurve> closedCallback;

        /// <summary>
        /// Shows the curve editor window that allows the user to edit a single curve.
        /// </summary>
        /// <param name="curve">Curve to initialize the window with.</param>
        /// <param name="closedCallback">Optional callback to trigger when the user finishes editing the curve or
        ///                              cancels out of the dialog.</param>
        /// <returns>An instance of the curve editor window.</returns>
        public static CurveEditorWindow Show(AnimationCurve curve, Action<bool, AnimationCurve> closedCallback = null)
        {
            CurveEditorWindow picker = new CurveEditorWindow(curve, closedCallback);
            return picker;
        }

        /// <summary>
        /// Shows the curve editor window that allows the user to edit a curve range (two curves).
        /// </summary>
        /// <param name="curveA">First curve of the range to display/edit.</param>
        /// <param name="curveB">Second curve of the range to display/edit.</param>
        /// <param name="closedCallback">Optional callback to trigger when the user finishes editing the curve or
        ///                              cancels out of the dialog.</param>
        /// <returns>An instance of the curve editor window.</returns>
        public static CurveEditorWindow Show(AnimationCurve curveA, AnimationCurve curveB, 
            Action<bool, AnimationCurve, AnimationCurve> closedCallback = null)
        {
            CurveEditorWindow picker = new CurveEditorWindow(curveA, curveB, closedCallback);
            return picker;
        }

        #region Overrides

        private CurveEditorWindow(AnimationCurve curve, Action<bool, AnimationCurve> closedCallback = null)
            : base(false)
        {
            Title = new LocString("Curve editor");
            Width = 600;
            Height = 460;

            curveA = new EdAnimationCurve(curve ?? new AnimationCurve(new KeyFrame[] {}), null);
            this.closedCallback = closedCallback;
        }

        private CurveEditorWindow(AnimationCurve curveA, AnimationCurve curveB, 
            Action<bool, AnimationCurve, AnimationCurve> closedCallback = null)
            : base(false)
        {
            Title = new LocString("Curve editor");
            Width = 600;
            Height = 460;

            this.curveA = new EdAnimationCurve(curveA ?? new AnimationCurve(new KeyFrame[] {}), null);
            this.curveB = new EdAnimationCurve(curveB ?? new AnimationCurve(new KeyFrame[] {}), null);

            this.closedCallbackRange = closedCallback;
        }

        private void OnInitialize()
        {
            GUILayout vertLayout = GUI.AddLayoutY();
            GUILayout editorPanel = vertLayout.AddPanel(GUIOption.FixedHeight(400));
            GUILayout buttonLayout = vertLayout.AddLayoutX(GUIOption.FixedHeight(40));

            guiOK = new GUIButton(new LocEdString("OK"));
            guiCancel = new GUIButton(new LocEdString("Cancel"));

            guiOK.OnClick += OnOK;
            guiCancel.OnClick += OnCancel;

            CurveDrawOptions drawOptions = CurveDrawOptions.DrawKeyframes | CurveDrawOptions.DrawMarkers;
            if (curveB != null)
                drawOptions |= CurveDrawOptions.DrawRange;

            curveEditor = new GUICurveEditor(editorPanel, 600, 400, false, drawOptions); 
            curveEditor.Redraw();

            EdCurveDrawInfo[] drawinfo;

            if (curveB != null)
            {
                drawinfo = new []
                {
                    new EdCurveDrawInfo(curveA, Color.BansheeOrange),
                    new EdCurveDrawInfo(curveB, Color.Green),
                };
            }
            else
            {
                drawinfo = new [] { new EdCurveDrawInfo(curveA, Color.BansheeOrange), };
            }

            curveEditor.SetCurves(drawinfo);
            curveEditor.CenterAndResize(true);

            buttonLayout.AddFlexibleSpace();
            buttonLayout.AddElement(guiOK);
            buttonLayout.AddSpace(10);
            buttonLayout.AddElement(guiCancel);
            buttonLayout.AddFlexibleSpace();

            EditorInput.OnPointerPressed += OnPointerPressed;
            EditorInput.OnPointerDoubleClick += OnPointerDoubleClicked;
            EditorInput.OnPointerMoved += OnPointerMoved;
            EditorInput.OnPointerReleased += OnPointerReleased;
            EditorInput.OnButtonUp += OnButtonUp;
        }

        private void OnEditorUpdate()
        {
            curveEditor.HandleDragAndZoomInput();
        }

        private void OnDestroy()
        {
            EditorInput.OnPointerPressed -= OnPointerPressed;
            EditorInput.OnPointerDoubleClick -= OnPointerDoubleClicked;
            EditorInput.OnPointerMoved -= OnPointerMoved;
            EditorInput.OnPointerReleased -= OnPointerReleased;
            EditorInput.OnButtonUp -= OnButtonUp;
        }

        #endregion

        #region GUI

        /// <summary>
        /// Triggered when the user finishes editing the curve(s) and closes the dialog.
        /// </summary>
        void OnOK()
        {
            if (curveB != null)
                closedCallbackRange?.Invoke(true, curveA.Normal, curveB.Normal);
            else
                closedCallback?.Invoke(true, curveA.Normal);

            Close();
        }

        /// <summary>
        /// Triggered when the user cancels editing the curve(s) closes the dialog.
        /// </summary>
        void OnCancel()
        {
            if (curveB != null)
                closedCallbackRange?.Invoke(false, curveA.Normal, curveB.Normal);
            else
                closedCallback?.Invoke(false, curveA.Normal);

            Close();
        }

        #endregion

        #region Input callbacks
        /// <summary>
        /// Triggered when the user presses a mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse press event.</param>
        private void OnPointerPressed(PointerEvent ev)
        {
            curveEditor.OnPointerPressed(ev);
        }

        /// <summary>
        /// Triggered when the user double clicks the left mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse event.</param>
        private void OnPointerDoubleClicked(PointerEvent ev)
        {
            curveEditor.OnPointerDoubleClicked(ev);
        }

        /// <summary>
        /// Triggered when the user moves the mouse.
        /// </summary>
        /// <param name="ev">Information about the mouse move event.</param>
        private void OnPointerMoved(PointerEvent ev)
        {
            curveEditor.OnPointerMoved(ev);
        }

        /// <summary>
        /// Triggered when the user releases a mouse button.
        /// </summary>
        /// <param name="ev">Information about the mouse release event.</param>
        private void OnPointerReleased(PointerEvent ev)
        {
            curveEditor.OnPointerReleased(ev);
        }

        /// <summary>
        /// Triggered when the user releases a keyboard button.
        /// </summary>
        /// <param name="ev">Information about the keyboard release event.</param>
        private void OnButtonUp(ButtonEvent ev)
        {
            curveEditor.OnButtonUp(ev);
        }
        #endregion
    }

    /** @} */
}
