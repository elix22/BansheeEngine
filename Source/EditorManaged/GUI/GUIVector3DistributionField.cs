using System;
using bs;

namespace bs.Editor
{
    partial class GUIVector3DistributionField
    {
        /// <summary>
        /// Triggered when one of the curves in the distribution changes.
        /// </summary>
        public event Action OnCurveChanged;

        partial void Callback_OnClicked(VectorComponent component)
        {
            int componentIdx = (int) component;
            Vector3Distribution distribution = Value;

            if (DistributionType == PropertyDistributionType.Curve)
            {
                AnimationCurve[] curves = AnimationUtility.SplitCurve3D(distribution.GetMinCurve());
                if (componentIdx < curves.Length)
                {
                    CurveEditorWindow.Show(curves[componentIdx], (success, curve) =>
                    {
                        if (!success)
                            return;

                        curves[componentIdx] = curve;

                        Vector3Curve compoundCurve = AnimationUtility.CombineCurve3D(curves);
                        Value = new Vector3Distribution(compoundCurve);
                        OnCurveChanged?.Invoke();
                    });
                }
            }
            else if (DistributionType == PropertyDistributionType.RandomCurveRange)
            {
                AnimationCurve[] minCurves = AnimationUtility.SplitCurve3D(distribution.GetMinCurve());
                AnimationCurve[] maxCurves = AnimationUtility.SplitCurve3D(distribution.GetMaxCurve());

                if (componentIdx < minCurves.Length && componentIdx < maxCurves.Length)
                {
                    CurveEditorWindow.Show(minCurves[componentIdx], maxCurves[componentIdx],
                        (success, minCurve, maxCurve) =>
                        {
                            if (!success)
                                return;

                            minCurves[componentIdx] = minCurve;
                            maxCurves[componentIdx] = maxCurve;

                            Vector3Curve minCompoundCurves = AnimationUtility.CombineCurve3D(minCurves);
                            Vector3Curve maxCompoundCurves = AnimationUtility.CombineCurve3D(maxCurves);

                            Value = new Vector3Distribution(minCompoundCurves, maxCompoundCurves);
                            OnCurveChanged?.Invoke();
                        });
                }
            }
        }
    }
}
