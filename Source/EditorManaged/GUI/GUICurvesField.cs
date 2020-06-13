using System;
using bs;

namespace bs.Editor
{
    partial class GUICurvesField
    {
        /// <summary>
        /// Triggered when the curve in the field changes.
        /// </summary>
        public event Action<AnimationCurve> OnChanged;

        /// <summary>
        /// Callback triggered when the user clicks on the GUI element.
        /// </summary>
        partial void Callback_OnClicked()
        {
            CurveEditorWindow.Show(Curve, (success, curve) =>
            {
                if (!success)
                    return;

                SetCurve(curve);
                OnChanged?.Invoke(curve);
            });
        }
    }
}
