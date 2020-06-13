using System;
using bs;

namespace bs.Editor
{
    partial class GUIColorGradientField
    {
        /// <summary>
        /// Triggered when the gradient in the field changes.
        /// </summary>
        public event Action<ColorGradient> OnChanged;

        /// <summary>
        /// Callback triggered when the user clicks on the GUI element.
        /// </summary>
        partial void Callback_OnClicked()
        {
            GradientPicker.Show(Value, (success, colorGradient) =>
            {
                if (!success)
                    return;

                Value = colorGradient;
                OnChanged?.Invoke(colorGradient);
            });

        }
    }

    partial class GUIColorGradientHDRField
    {
        /// <summary>
        /// Triggered when the gradient in the field changes.
        /// </summary>
        public event Action<ColorGradientHDR> OnChanged;

        /// <summary>
        /// Callback triggered when the user clicks on the GUI element.
        /// </summary>
        partial void Callback_OnClicked()
        {
            // Note: Should allow HDR color gradient
            ColorGradient gradient = new ColorGradient(Value.GetKeys());
            GradientPicker.Show(gradient, (success, colorGradient) =>
            {
                if (!success)
                    return;

                Value = new ColorGradientHDR(colorGradient.GetKeys());
                OnChanged?.Invoke(Value);
            });
        }
    }
}
