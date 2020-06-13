using System;
using bs;

namespace bs.Editor
{
    partial class GUIColorDistributionField
    {
        /// <summary>
        /// Triggered when the distribution in the field changes.
        /// </summary>
        public event Action OnChanged;

        partial void Callback_OnMinClicked()
        {
            ColorDistribution distribution = Value;

            if (DistributionType == PropertyDistributionType.Constant ||
                DistributionType == PropertyDistributionType.RandomRange)
            {
                ColorPicker.Show(distribution.GetMinConstant(), true, (success, value) =>
                {
                    if (!success)
                        return;

                    if (DistributionType == PropertyDistributionType.Constant)
                        Value = new ColorDistribution(value);
                    else
                        Value = new ColorDistribution(value, distribution.GetMaxConstant());

                    OnChanged?.Invoke();
                });
            }
            else if (DistributionType == PropertyDistributionType.Curve || 
                     DistributionType == PropertyDistributionType.RandomCurveRange)
            {
                GradientPicker.Show(distribution.GetMinGradient(), (success, colorGradient) =>
                {
                    if (!success)
                        return;

                    if(DistributionType == PropertyDistributionType.Curve)
                        Value = new ColorDistribution(colorGradient);
                    else
                        Value = new ColorDistribution(colorGradient, distribution.GetMaxGradient());

                    OnChanged?.Invoke();
                });
            }
        }

        partial void Callback_OnMaxClicked()
        {
            ColorDistribution distribution = Value;

            if (DistributionType == PropertyDistributionType.RandomRange)
            {
                ColorPicker.Show(distribution.GetMaxConstant(), true, (success, value) =>
                {
                    if (!success)
                        return;

                    Value = new ColorDistribution(distribution.GetMinConstant(), value);
                    OnChanged?.Invoke();
                });
            }
            else if (DistributionType == PropertyDistributionType.RandomCurveRange)
            {
                GradientPicker.Show(distribution.GetMaxGradient(), (success, colorGradient) =>
                {
                    if (!success)
                        return;

                    Value = new ColorDistribution(distribution.GetMinGradient(), colorGradient);
                    OnChanged?.Invoke();
                });
            }
        }
    }
}
