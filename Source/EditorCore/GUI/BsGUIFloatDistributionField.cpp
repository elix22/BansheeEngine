//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIFloatDistributionField.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIColor.h"
#include "GUI/BsGUIFloatField.h"
#include "GUI/BsGUIVector2Field.h"
#include "GUI/BsGUIVector3Field.h"
#include "GUI/BsGUICurvesField.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUIContextMenu.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

namespace bs
{
	/** Style type name for the internal float fields. */
	static constexpr const char* FLOAT_FIELD_STYLE_TYPE = "FloatField";

	/** Style type name for the internal curve display fields. */
	static constexpr const char* CURVES_FIELD_STYLE_TYPES[4] = 
		{ "CurvesFieldX", "CurvesFieldY", "CurvesFieldZ", "CurvesFieldW" };

	/** Style type name for the internal drop down button. */
	static constexpr const char* DROP_DOWN_FIELD_STYLE_TYPE = "DropDownButton";

	/** Spacing between two internal elements, in pixels. */
	static constexpr UINT32 ELEM_SPACING = 5;

	namespace impl
	{
		template<class T, class F>
		void addOnConfirmCallback(T* field, F func)
		{
			field->onConfirm.connect([func]() { func(VectorComponent::X); });
		}

		template<class F>
		void addOnConfirmCallback(GUIVector2Field* field, F func)
		{
			field->onConfirm.connect([func](VectorComponent x) { func(x); });
		}

		template<class F>
		void addOnConfirmCallback(GUIVector3Field* field, F func)
		{
			field->onConfirm.connect([func](VectorComponent x) { func(x); });
		}

		template<class T, class F>
		void addOnValueChangedCallback(T* field, F func)
		{
			field->onValueChanged.connect([func](float val) { func(val, VectorComponent::X); });
		}

		template<class F>
		void addOnValueChangedCallback(GUIVector2Field* field, F func)
		{
			field->onComponentChanged.connect([func](float val, VectorComponent x) { func(val, x); });
		}

		template<class F>
		void addOnValueChangedCallback(GUIVector3Field* field, F func)
		{
			field->onComponentChanged.connect([func](float val, VectorComponent x) { func(val, x); });
		}

		template<class T, class F>
		void addOnInputChangedCallback(T* field, F func)
		{
			field->onFocusChanged.connect([func](bool val) { func(val, VectorComponent::X); });
		}

		template<class F>
		void addOnInputChangedCallback(GUIVector2Field* field, F func)
		{
			field->onComponentFocusChanged.connect([func](bool val, VectorComponent x) { func(val, x); });
		}

		template<class F>
		void addOnInputChangedCallback(GUIVector3Field* field, F func)
		{
			field->onComponentFocusChanged.connect([func](bool val, VectorComponent x) { func(val, x); });
		}

		template<class T>
		void setFocus(T* field, VectorComponent component, bool focus)
		{
			field->setFocus(focus);
		}

		template<>
		void setFocus(GUIVector2Field* field, VectorComponent component, bool focus)
		{
			field->setInputFocus(component, focus);
		}

		template<>
		void setFocus(GUIVector3Field* field, VectorComponent component, bool focus)
		{
			field->setInputFocus(component, focus);
		}
	}

	template<class T, class SELF>
	TGUIDistributionField<T, SELF>::TGUIDistributionField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		: TGUIField<SELF>(dummy, labelContent, labelWidth, style, dimensions, withLabel)
	{
		mContextMenu = bs_shared_ptr_new<GUIContextMenu>();

		mContextMenu->addMenuItem("Constant", [this]()
		{
			mValue = TDistribution<T>(mMinConstant);
			mPropertyType = PDT_Constant;
			rebuild();
		}, 50);

		mContextMenu->addMenuItem("Range", [this]()
		{
			mValue = TDistribution<T>(mMinConstant, mMaxConstant);
			mPropertyType = PDT_RandomRange;
			rebuild();
		}, 40);

		mContextMenu->addMenuItem("Curve", [this]()
		{
			TAnimationCurve<T> combinedCurve;
			AnimationUtility::combineCurve<T>(mMinCurve, combinedCurve);

			mValue = TDistribution<T>(combinedCurve);
			mPropertyType = PDT_Curve;
			rebuild();
		}, 30);

		mContextMenu->addMenuItem("Curve range", [this]()
		{
			TAnimationCurve<T> combinedCurveMin;
			AnimationUtility::combineCurve<T>(mMinCurve, combinedCurveMin);

			TAnimationCurve<T> combinedCurveMax;
			AnimationUtility::combineCurve<T>(mMaxCurve, combinedCurveMax);

			mValue = TDistribution<T>(combinedCurveMin, combinedCurveMax);
			mPropertyType = PDT_RandomCurveRange;
			rebuild();
		}, 20);

		rebuild();
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::setValue(const TDistribution<T>& value)
	{
		if(mValue == value)
			return;

		mValue = value;

		switch (mPropertyType)
		{
		default:
		case PDT_Constant:
			mMinConstant = mValue.getMinConstant();
			mMaxConstant = mMinConstant;

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				mMinCurve[i] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 1.0f} });

				mMaxCurve[i] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 1.0f} });
			}

			mMinInput->setValue(mMinConstant);

			break;
		case PDT_RandomRange: 
			mMinConstant = mValue.getMinConstant();
			mMaxConstant = mValue.getMaxConstant();

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				mMinCurve[i] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 1.0f} });

				mMaxCurve[i] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMaxConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMaxConstant, i), 0.0f, 0.0f, 1.0f} });
			}

			mMinInput->setValue(mMinConstant);
			mMaxInput->setValue(mMaxConstant);

			break;
		case PDT_Curve:
			AnimationUtility::splitCurve(mValue.getMinCurve(), mMinCurve);
			AnimationUtility::splitCurve(mValue.getMinCurve(), mMaxCurve);

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				TCurveProperties<T>::setComponent(mMinConstant, i, mMinCurve[i].evaluate(0.0f));
				TCurveProperties<T>::setComponent(mMaxConstant, i, mMaxCurve[i].evaluate(0.0f));

				mCurveDisplay[i]->setCurve(mMinCurve[i]);
			}

			break;
		case PDT_RandomCurveRange: 
			AnimationUtility::splitCurve(mValue.getMinCurve(), mMinCurve);
			AnimationUtility::splitCurve(mValue.getMaxCurve(), mMaxCurve);

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				TCurveProperties<T>::setComponent(mMinConstant, i, mMinCurve[i].evaluate(0.0f));
				TCurveProperties<T>::setComponent(mMaxConstant, i, mMaxCurve[i].evaluate(0.0f));

				mCurveDisplay[i]->setCurveRange(mMinCurve[i], mMaxCurve[i]);
			}

			break;
		}
	}

	template<class T, class SELF>
	bool TGUIDistributionField<T, SELF>::hasInputFocus() const
	{
		if(mMinInput && mMinInput->hasInputFocus())
			return true;

		if(mMaxInput && mMaxInput->hasInputFocus())
			return true;

		return false;
	}

	template <class T, class SELF>
	void TGUIDistributionField<T, SELF>::setInputFocus(RangeComponent rangeComponent, VectorComponent vectorComponent, bool focus)
	{
		switch(mPropertyType)
		{
		case PDT_Constant:
		case PDT_RandomRange:
			if(rangeComponent == RangeComponent::Min)
				impl::setFocus(mMinInput, vectorComponent, focus);
			else
				impl::setFocus(mMaxInput, vectorComponent, focus);

			break;
		case PDT_Curve:
		case PDT_RandomCurveRange:
		{
			for (UINT32 i = 0; i < NumComponents; i++)
			{
				if ((VectorComponent)i == vectorComponent && mCurveDisplay[i])
					mCurveDisplay[i]->setFocus(focus);
			}
		}
			break;
		}
		
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::setTint(const Color& color)
	{
		mDropDownButton->setTint(color);

		if (this->mLabel)
			this->mLabel->setTint(color);

		if(mMinInput)
			mMinInput->setTint(color);

		if(mMaxInput)
			mMaxInput->setTint(color);

		for(auto& entry : mLabels)
		{
			if(entry)
				entry->setTint(color);
		}

		for(int i = 0; i < NumComponents; i++)
		{
			if(mCurveDisplay[i])
				mCurveDisplay[i]->setTint(color);
		}
	}

	template<class T, class SELF>
	Vector2I TGUIDistributionField<T, SELF>::_getOptimalSize() const
	{
		Vector2I optimalsize = Vector2I::ZERO;

		if (mMinInput)
		{
			Vector2I elemOptimal = mMinInput->_calculateLayoutSizeRange().optimal;

			optimalsize.x = std::max(optimalsize.x, elemOptimal.x);
			optimalsize.y += elemOptimal.y;
		}

		if (mMaxInput)
		{
			Vector2I elemOptimal = mMaxInput->_calculateLayoutSizeRange().optimal;

			optimalsize.x = std::max(optimalsize.x, elemOptimal.x);
			optimalsize.y += elemOptimal.y;

			optimalsize.y += ELEM_SPACING;
		}

		for (auto& entry : mLabels)
		{
			if (!entry)
				continue;

			Vector2I elemOptimal = entry->_calculateLayoutSizeRange().optimal;

			optimalsize.x = std::max(optimalsize.x, elemOptimal.x);
			optimalsize.y += elemOptimal.y;
		}

		for (UINT32 i = 0; i < NumComponents; i++)
		{
			if (mCurveDisplay[i])
			{
				Vector2I elemOptimal = mCurveDisplay[i]->_calculateLayoutSizeRange().optimal;

				optimalsize.x = std::max(optimalsize.x, elemOptimal.x);
				optimalsize.y += elemOptimal.y;

				if (i != 0)
					optimalsize.y += ELEM_SPACING;
			}
		}
		
		Vector2I dropDownSize = mDropDownButton->_calculateLayoutSizeRange().optimal;
		optimalsize.x += dropDownSize.x;
		optimalsize.y = std::max(optimalsize.y, dropDownSize.y);

		if (this->mLabel)
		{
			Vector2I elemOptimal = this->mLabel->_calculateLayoutSizeRange().optimal;

			optimalsize.x += elemOptimal.x;
			optimalsize.y = std::max(optimalsize.y, elemOptimal.y);
		}

		return optimalsize;
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::styleUpdated()
	{
		mDropDownButton->setStyle(this->getSubStyleName(DROP_DOWN_FIELD_STYLE_TYPE));

		if (this->mLabel)
			this->mLabel->setStyle(this->getSubStyleName(this->getLabelStyleType()));

		if (mMinInput)
			mMinInput->setStyle(this->getSubStyleName(FLOAT_FIELD_STYLE_TYPE));

		if (mMaxInput)
			mMaxInput->setStyle(this->getSubStyleName(FLOAT_FIELD_STYLE_TYPE));

		for (int i = 0; i < NumComponents; i++)
		{
			if (mCurveDisplay[i])
				mCurveDisplay[i]->setStyle(this->getSubStyleName(CURVES_FIELD_STYLE_TYPES[i]));
		}
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::rebuild()
	{
		constexpr const char* COMP_NAMES[] = { "X", "Y", "Z", "W" };
		constexpr UINT32 ELEMENT_LABEL_WIDTH = 15;

		if(this->mLabel)
			this->mLayout->removeElement(this->mLabel);

		this->mLayout->clear();
		this->mLayout->addElement(this->mLabel);

		GUILayout* valueLayout = this->mLayout->template addNewElement<GUILayoutY>();
		switch (mValue.getType())
		{
		default:
		case PDT_Constant:
			mMinInput = GUIConstantType::create(GUIOptions(), this->getSubStyleName(FLOAT_FIELD_STYLE_TYPE));
			mMaxInput = nullptr;
			mLabels = { nullptr, nullptr };

			for(int i = 0; i < NumComponents; i++)
				mCurveDisplay[i] = nullptr;

			mMinInput->setValue(mMinConstant);

			impl::addOnValueChangedCallback(mMinInput, [this](float value, VectorComponent component)
			{
				mMinConstant = mMinInput->getValue();
				mValue = TDistribution<T>(mMinConstant);

				onConstantModified(RangeComponent::Min, component);
			});
			impl::addOnConfirmCallback(mMinInput, [this](VectorComponent component)
			{
				onConstantConfirmed(RangeComponent::Min, component);
			});
			impl::addOnInputChangedCallback(mMinInput, [this](bool focus, VectorComponent component)
			{
				onConstantFocusChanged(focus, RangeComponent::Min, component);
			});

			valueLayout->addElement(mMinInput);
			break;
		case PDT_RandomRange: 
			mMinInput = GUIConstantType::create(GUIOptions(), this->getSubStyleName(FLOAT_FIELD_STYLE_TYPE));
			mMaxInput = GUIConstantType::create(GUIOptions(), this->getSubStyleName(FLOAT_FIELD_STYLE_TYPE));

			for(int i = 0; i < NumComponents; i++)
				mCurveDisplay[i] = nullptr;

			mMinInput->setValue(mMinConstant);

			impl::addOnValueChangedCallback(mMinInput, [this](float value, VectorComponent component)
			{
				mMinConstant = mMinInput->getValue();
				mValue = TDistribution<T>(mMinConstant, mMaxConstant);

				onConstantModified(RangeComponent::Min, component);
			});
			impl::addOnConfirmCallback(mMinInput, [this](VectorComponent component)
			{
				onConstantConfirmed(RangeComponent::Min, component);
			});
			impl::addOnInputChangedCallback(mMinInput, [this](bool focus, VectorComponent component)
			{
				onConstantFocusChanged(focus, RangeComponent::Min, component);
			});

			mMaxInput->setValue(mMaxConstant);

			impl::addOnValueChangedCallback(mMaxInput, [this](float value, VectorComponent component)
			{
				mMaxConstant = mMaxInput->getValue();
				mValue = TDistribution<T>(mMinConstant, mMaxConstant);

				onConstantModified(RangeComponent::Max, component);
			});
			impl::addOnConfirmCallback(mMaxInput, [this](VectorComponent component)
			{
				onConstantConfirmed(RangeComponent::Max, component);
			});
			impl::addOnInputChangedCallback(mMaxInput, [this](bool focus, VectorComponent component)
			{
				onConstantFocusChanged(focus, RangeComponent::Max, component);
			});

			mLabels[0] = valueLayout->addNewElement<GUILabel>(HString("Min."), "HeaderLight");
			valueLayout->addElement(mMinInput);
			valueLayout->addNewElement<GUIFixedSpace>(ELEM_SPACING);
			mLabels[1] = valueLayout->addNewElement<GUILabel>(HString("Max."), "HeaderLight");
			valueLayout->addElement(mMaxInput);
			break;
		case PDT_Curve: 
			mMinInput = nullptr;
			mMaxInput = nullptr;
			mLabels = { nullptr, nullptr };

			for(int i = 0; i < NumComponents; i++)
			{
				if(NumComponents > 1)
				{
					mCurveDisplay[i] = GUICurvesField::create(CurveDrawOption::DrawMarkers, HString(COMP_NAMES[i]),
						ELEMENT_LABEL_WIDTH, this->getSubStyleName(CURVES_FIELD_STYLE_TYPES[i]));
				}
				else
				{
					mCurveDisplay[i] = GUICurvesField::create(CurveDrawOption::DrawMarkers, 
						this->getSubStyleName(CURVES_FIELD_STYLE_TYPES[i]));
				}

				mCurveDisplay[i]->setCurve(mMinCurve[i]);

				mCurveDisplay[i]->setPadding(3);
				mCurveDisplay[i]->centerAndZoom();
				mCurveDisplay[i]->onClicked.connect([this,i]() { onClicked((VectorComponent)i); });

				if(i != 0)
					valueLayout->addNewElement<GUIFixedSpace>(ELEM_SPACING);

				valueLayout->addElement(mCurveDisplay[i]);
			}

			break;
		case PDT_RandomCurveRange: 
			mMinInput = nullptr;
			mMaxInput = nullptr;
			mLabels = { nullptr, nullptr };

			for(int i = 0; i < NumComponents; i++)
			{
				if(NumComponents > 1)
				{
					mCurveDisplay[i] = GUICurvesField::create(CurveDrawOption::DrawMarkers, HString(COMP_NAMES[i]),
						ELEMENT_LABEL_WIDTH, this->getSubStyleName(CURVES_FIELD_STYLE_TYPES[i]));
				}
				else
				{
					mCurveDisplay[i] = GUICurvesField::create(CurveDrawOption::DrawMarkers, 
						this->getSubStyleName(CURVES_FIELD_STYLE_TYPES[i]));
				}

				mCurveDisplay[i]->setCurveRange(mMinCurve[i], mMaxCurve[i]);
				mCurveDisplay[i]->setPadding(3);
				mCurveDisplay[i]->centerAndZoom();
				mCurveDisplay[i]->onClicked.connect([this,i]() { onClicked((VectorComponent)i); });

				if(i != 0)
					valueLayout->addNewElement<GUIFixedSpace>(ELEM_SPACING);

				valueLayout->addElement(mCurveDisplay[i]);
			}

			break;
		}

		mDropDownButton = GUIButton::create(HString::dummy(), this->getSubStyleName(DROP_DOWN_FIELD_STYLE_TYPE));
		mDropDownButton->onClick.connect([this]()
		{
			const Rect2I bounds = mDropDownButton->getBounds(this->mParentWidget->getPanel());
			const Vector2I center(bounds.x + bounds.width / 2, bounds.y + bounds.height / 2);

			mContextMenu->open(center, *(this->mParentWidget));
		});

		this->mLayout->template addNewElement<GUIFixedSpace>(10);
		this->mLayout->addElement(mDropDownButton);
	}

	template class BS_ED_EXPORT TGUIDistributionField<float, GUIFloatDistributionField>;
	template class BS_ED_EXPORT TGUIDistributionField<Vector2, GUIVector2DistributionField>;
	template class BS_ED_EXPORT TGUIDistributionField<Vector3, GUIVector3DistributionField>;

	const String& GUIFloatDistributionField::getGUITypeName()
	{
		static String typeName = "GUIFloatDistributionField";
		return typeName;
	}

	const String& GUIVector2DistributionField::getGUITypeName()
	{
		static String typeName = "GUIVector2DistributionField";
		return typeName;
	}

	const String& GUIVector3DistributionField::getGUITypeName()
	{
		static String typeName = "GUIVector3DistributionField";
		return typeName;
	}
}