//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIFieldBase.h"
#include "Particles/BsParticleDistribution.h"
#include "Animation/BsAnimationUtility.h"

namespace bs
{
	class GUICurvesField;

	/** @addtogroup GUI-Editor-Internal
	 *  @{
	 */

	namespace impl
	{
		template<class T>
		struct TGUIDistributionMeta { };

		template<>
		struct TGUIDistributionMeta<float>
		{
			using ConstantField = GUIFloatField;
		};

		template<>
		struct TGUIDistributionMeta<Vector2>
		{
			using ConstantField = GUIVector2Field;
		};

		template<>
		struct TGUIDistributionMeta<Vector3>
		{
			using ConstantField = GUIVector3Field;
		};
	}

	/** Templated common class for all GUI distribution field types. */
	template<class T, class SELF>
	class BS_ED_EXPORT TGUIDistributionField : public TGUIField<SELF>
	{
		using PrivatelyConstruct = typename TGUIField<SELF>::PrivatelyConstruct;
		using GUIConstantType = typename impl::TGUIDistributionMeta<T>::ConstantField;
		enum { NumComponents = TCurveProperties<T>::NumComponents };

	public:
		TGUIDistributionField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
			const String& style, const GUIDimensions& dimensions, bool withLabel);

		/**	Returns the value of the field. */
		BS_SCRIPT_EXPORT(pr:getter,n:Value)
		TDistribution<T> getValue() const { return mValue; }

		/**	Changes the value of the field. */
		BS_SCRIPT_EXPORT(pr:setter,n:Value)
		void setValue(const TDistribution<T>& value);

		/** Returns the type of the currently selected distribution. */
		BS_SCRIPT_EXPORT(pr:getter,n:DistributionType)
		PropertyDistributionType getType() const { return mPropertyType; }

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		/**	Checks if any of the float input fields currently have input focus. Only relevant for non-curve distributions. */
		BS_SCRIPT_EXPORT(pr:getter,n:HasInputFocus)
		bool hasInputFocus() const;

		/** 
		 * Sets input focus to a specific component's input box. 
		 *
		 * @param[in]	rangeComponent		Whether to focus on the minimum or the maximum part of the range. Only relevant
		 *									if the distribution represents a constant range.
		 * @param[in]	vectorComponent		Vector component to focus on. Only relevant of the distribution constant is
		 *									a vector type, and if the current distribution type is a non-curve (constant)
		 *									type.
		 * @param[in]	focus				True to enable focus, false to disable.
		 */
		BS_SCRIPT_EXPORT()
		void setInputFocus(RangeComponent rangeComponent, VectorComponent vectorComponent, bool focus);

		/** 
		 * Triggered when the user clicks on the curve display. Only relevant if the distribution is a curve distribution. 
		 * Provides the index of the clicked curve.
		 */
		BS_SCRIPT_EXPORT(in:true)
		Event<void(VectorComponent)> onClicked;

		/** 
		 * Triggered when the user modifies the value of the non-curve (constant) values of the distribution. Only relevant 
		 * if the distribution is not a curve distribution.
		 */
		BS_SCRIPT_EXPORT()
		Event<void(RangeComponent, VectorComponent)> onConstantModified;

		/** 
		 * Triggered when the user confirms inputs in the non-curve (constant) values of the distribution. Only relevant 
		 * if the distribution is not a curve distribution.
		 */
		BS_SCRIPT_EXPORT()
		Event<void(RangeComponent, VectorComponent)> onConstantConfirmed;

		/**	
		 * Triggered when a GUI field representing an individual component loses or gains focus. This only applies to
		 * input fields representing the non-curve (constant) distribution types.
		 */
		BS_SCRIPT_EXPORT()
		Event<void(bool, RangeComponent, VectorComponent)> onConstantFocusChanged;

		/** @name Internal 
		 *  @{
		 */

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */

	protected:
		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		/** Rebuilds the internal GUI components for the current property type. */
		void rebuild();

		TDistribution<T> mValue = TCurveProperties<T>::getZero();
		GUIButton* mDropDownButton = nullptr;
		GUIConstantType* mMinInput = nullptr;
		GUIConstantType* mMaxInput = nullptr;
		std::array<GUILabel*, 2> mLabels = { nullptr, nullptr };
		GUICurvesField* mCurveDisplay[NumComponents] = { };
		PropertyDistributionType mPropertyType = PDT_Constant;

		T mMinConstant = TCurveProperties<T>::getZero();
		T mMaxConstant = TCurveProperties<T>::getZero();
		TAnimationCurve<float> mMinCurve[NumComponents];
		TAnimationCurve<float> mMaxCurve[NumComponents];
		SPtr<GUIContextMenu> mContextMenu;
	};

	/** @} */

	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays an input field for a floating point distribution.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) 
	GUIFloatDistributionField final : public TGUIDistributionField<float, GUIFloatDistributionField>
	{
	public:
		using TGUIDistributionField<float, GUIFloatDistributionField>::TGUIDistributionField;

		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();
	};
	
	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays an input field for a 2D vector distribution.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) 
	GUIVector2DistributionField final : public TGUIDistributionField<Vector2, GUIVector2DistributionField>
	{
	public:
		using TGUIDistributionField<Vector2, GUIVector2DistributionField>::TGUIDistributionField;

		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();
	};

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays an input field for a 3D vector distribution.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) 
	GUIVector3DistributionField final : public TGUIDistributionField<Vector3, GUIVector3DistributionField>
	{
	public:
		using TGUIDistributionField<Vector3, GUIVector3DistributionField>::TGUIDistributionField;

		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();
	};

	/** @} */
}
