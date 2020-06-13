using System;
using bs;

namespace bs.Editor
{
    partial class GUIVector2DistributionField
    {
        /// <summary>
        /// Triggered when one of the curves in the distribution changes.
        /// </summary>
        public event Action OnCurveChanged;

        partial void Callback_OnClicked(VectorComponent component)
        {
            int componentIdx = (int) component;
            Vector2Distribution distribution = Value;

            if (DistributionType == PropertyDistributionType.Curve)
            {
                AnimationCurve[] curves = AnimationUtility.SplitCurve2D(distribution.GetMinCurve());
                if (componentIdx < curves.Length)
                {
                    CurveEditorWindow.Show(curves[componentIdx], (success, curve) =>
                    {
                        if (!success)
                            return;

                        curves[componentIdx] = curve;

                        Vector2Curve compoundCurve = AnimationUtility.CombineCurve2D(curves);
                        Value = new Vector2Distribution(compoundCurve);
                        OnCurveChanged?.Invoke();
                    });
                }
            }
            else if (DistributionType == PropertyDistributionType.RandomCurveRange)
            {
                AnimationCurve[] minCurves = AnimationUtility.SplitCurve2D(distribution.GetMinCurve());
                AnimationCurve[] maxCurves = AnimationUtility.SplitCurve2D(distribution.GetMaxCurve());

                if (componentIdx < minCurves.Length && componentIdx < maxCurves.Length)
                {
                    CurveEditorWindow.Show(minCurves[componentIdx], maxCurves[componentIdx],
                        (success, minCurve, maxCurve) =>
                        {
                            if (!success)
                                return;

                            minCurves[componentIdx] = minCurve;
                            maxCurves[componentIdx] = maxCurve;

                            Vector2Curve minCompoundCurves = AnimationUtility.CombineCurve2D(minCurves);
                            Vector2Curve maxCompoundCurves = AnimationUtility.CombineCurve2D(maxCurves);

                            Value = new Vector2Distribution(minCompoundCurves, maxCompoundCurves);
                            OnCurveChanged?.Invoke();
                        });
                }
            }
        }
    }
}
