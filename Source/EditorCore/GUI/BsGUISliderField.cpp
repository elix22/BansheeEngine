//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUISliderField.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUIInputBox.h"
#include "GUI/BsGUISpace.h"
#include "GUI/BsGUISlider.h"
#include "GUI/BsGUILabel.h"
#include <regex>

using namespace std::placeholders;

namespace bs
{
	GUISliderField::GUISliderField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
		const String& style, const GUIDimensions& dimensions, bool withLabel)
		:TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel), mInputBox(nullptr), mSlider(nullptr), mHasInputFocus(false)
	{
		mSlider = GUISliderHorz::create(GUIOptions(GUIOption::flexibleWidth()), getSubStyleName(getSliderStyleType()));
		mSlider->onChanged.connect(std::bind(&GUISliderField::sliderChanged, this, _1));

		mInputBox = GUIInputBox::create(false, GUIOptions(GUIOption::fixedWidth(75)), getSubStyleName(getInputStyleType()));
		mInputBox->setFilter(&GUISliderField::floatFilter);

		mInputBox->onValueChanged.connect(std::bind((void(GUISliderField::*)(const String&))&GUISliderField::inputBoxValueChanging, this, _1));
		mInputBox->onConfirm.connect(std::bind(&GUISliderField::inputBoxValueChanged, this, true));
		mInputBox->onFocusChanged.connect(std::bind(&GUISliderField::inputBoxFocusChanged, this, _1));

		mLayout->addElement(mSlider);
		mLayout->addNewElement<GUIFixedSpace>(5);
		mLayout->addElement(mInputBox);

		setValue(0);
		mInputBox->setText("0");
		mFocusElement = mInputBox;
	}

	float GUISliderField::getValue() const
	{
		return mSlider->getValue();
	}

	float GUISliderField::getStep() const
	{
		return mSlider->getStep();
	}

	float GUISliderField::setValue(float value)
	{
		float origValue = mSlider->getValue();
		if (origValue != value)
			mSlider->setValue(value);

		float clampedValue = mSlider->getValue();

		// Only update with new value if it actually changed, otherwise problems can occur when user types in "0." and the
		// field updates back to "0" effectively making "." unusable
		float curValue = parseFloat(mInputBox->getText());
		if (clampedValue != curValue)
			mInputBox->setText(toString(clampedValue));
		return clampedValue;
	}

	void GUISliderField::setRange(float min, float max)
	{
		mSlider->setRange(min, max);
	}

	void GUISliderField::setStep(float step)
	{
		mSlider->setStep(step);
	}

	void GUISliderField::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mInputBox->setTint(color);
	}

	void GUISliderField::_setValue(float value, bool triggerEvent)
	{
		float clamped = setValue(value);

		if (triggerEvent)
			onValueChanged(clamped);
	}

	const String& GUISliderField::getGUITypeName()
	{
		static String typeName = "GUISliderField";
		return typeName;
	}

	const String& GUISliderField::getInputStyleType()
	{
		static String LABEL_STYLE_TYPE = "EditorFieldInput";
		return LABEL_STYLE_TYPE;
	}

	const String& GUISliderField::getSliderStyleType()
	{
		static String SLIDER_STYLE_TYPE = "EditorSliderInput";
		return SLIDER_STYLE_TYPE;
	}

	void GUISliderField::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		mSlider->setStyle(getSubStyleName(getSliderStyleType()));
		mInputBox->setStyle(getSubStyleName(getInputStyleType()));
	}

	void GUISliderField::inputBoxValueChanging(const String& newValue) 
	{
		inputBoxValueChanged(false);
	}

	void GUISliderField::inputBoxValueChanged(bool confirmed)
	{
		float newFloatValue = parseFloat(mInputBox->getText());
		if (mSlider->getValue() != newFloatValue) {
			if (confirmed)
				_setValue(newFloatValue, true);
			else
			{
				mSlider->setValue(newFloatValue);
				onValueChanged(mSlider->getValue());
			}
		}
		else if (mInputBox->getText().empty() && confirmed) //Avoid leaving label blank
		{
			mInputBox->setText("0");
		}
	}

	void GUISliderField::inputBoxFocusChanged(bool focus)
	{
		if (focus)
		{
			mHasInputFocus = true;
			onFocusChanged(true);
		}
		else
		{
			inputBoxValueChanged();

			mHasInputFocus = false;
			onFocusChanged(false);
		}
	}

	void GUISliderField::sliderChanged(float newValue)
	{
		_setValue(newValue, true);
	}

	bool GUISliderField::floatFilter(const String& str)
	{
		bool result = std::regex_match(str, std::regex("-?(\\d*(\\.\\d*)?)?"));
		return result;
	}
}