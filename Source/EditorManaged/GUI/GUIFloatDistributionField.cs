using System;
using bs;

namespace bs.Editor
{
    partial class GUIFloatDistributionField
    {
        /// <summary>
        /// Triggered when one of the curves in the distribution changes.
        /// </summary>
        public event Action OnCurveChanged;

        partial void Callback_OnClicked(VectorComponent component)
        {
            FloatDistribution distribution = Value;

            if (DistributionType == PropertyDistributionType.Curve)
            {
                CurveEditorWindow.Show(distribution.GetMinCurve(), (success, curve) =>
                {
                    if (!success)
                        return;

                    Value = new FloatDistribution(curve);
                    OnCurveChanged?.Invoke();
                });
            }
            else if (DistributionType == PropertyDistributionType.RandomCurveRange)
            {
                CurveEditorWindow.Show(distribution.GetMinCurve(), distribution.GetMaxCurve(), 
                    (success, minCurve, maxCurve) =>
                    {
                        if (!success)
                            return;

                        Value = new FloatDistribution(minCurve, maxCurve);
                        OnCurveChanged?.Invoke();
                    });
            }
        }
    }
}
