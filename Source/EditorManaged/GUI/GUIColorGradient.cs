using System;
using bs;

namespace bs.Editor
{
    partial class GUIColorGradient
    {
        /// <summary>
        /// Callback triggered when the user clicks on the GUI element.
        /// </summary>
        partial void Callback_OnClicked()
        {
            GradientPicker.Show(Gradient, (success, colorGradient) =>
            {
                if (!success)
                    return;

                Gradient = colorGradient;
            });
        }
    }

    partial class GUIColorGradientHDR
    {
        /// <summary>
        /// Callback triggered when the user clicks on the GUI element.
        /// </summary>
        partial void Callback_OnClicked()
        {
            // Note: Should allow HDR color gradient
            ColorGradient gradient = new ColorGradient(Gradient.GetKeys());
            GradientPicker.Show(gradient, (success, colorGradient) =>
            {
                if (!success)
                    return;

                Gradient = new ColorGradientHDR(colorGradient.GetKeys());
            });
        }
    }
}
