//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIVector2Field.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIFloatField.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUIVector2Field::ELEMENT_LABEL_WIDTH = 15;

	GUIVector2Field::GUIVector2Field(const PrivatelyConstruct& dummy, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		:TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel)
	{
		mFieldX = GUIFloatField::create(HString("X"), ELEMENT_LABEL_WIDTH, getSubStyleName(X_FIELD_STYLE_TYPE));
		mFieldY = GUIFloatField::create(HString("Y"), ELEMENT_LABEL_WIDTH, getSubStyleName(Y_FIELD_STYLE_TYPE));

		mFieldX->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::X); });
		mFieldY->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::Y); });

		mFieldX->onConfirm.connect([this]() { inputConfirmed(VectorComponent::X); });
		mFieldY->onConfirm.connect([this]() { inputConfirmed(VectorComponent::Y); });

		mFieldX->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::X); });
		mFieldY->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::Y); });

		mLayout->addElement(mFieldX);
		mLayout->addNewElement<GUIFixedSpace>(5);
		mLayout->addElement(mFieldY);
	}

	Vector2 GUIVector2Field::getValue() const
	{
		Vector2 value;
		value.x = mFieldX->getValue();
		value.y = mFieldY->getValue();

		return value;
	}

	void GUIVector2Field::setValue(const Vector2& value)
	{
		mFieldX->setValue(value.x);
		mFieldY->setValue(value.y);
	}

	bool GUIVector2Field::hasInputFocus() const
	{
		return mFieldX->hasInputFocus() || mFieldY->hasInputFocus();
	}

	void GUIVector2Field::setInputFocus(VectorComponent component, bool focus)
	{
		switch(component)
		{
		case VectorComponent::X: mFieldX->setFocus(focus, true); break;
		case VectorComponent::Y: mFieldY->setFocus(focus, true); break;
		default: break;
		}
	}

	void GUIVector2Field::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mFieldX->setTint(color);
		mFieldY->setTint(color);
	}

	void GUIVector2Field::valueChanged(float newValue, VectorComponent component)
	{
		onComponentChanged(newValue, component);

		Vector2 value = getValue();
		onValueChanged(value);
	}

	void GUIVector2Field::inputConfirmed(VectorComponent component)
	{
		onConfirm(component);
	}

	void GUIVector2Field::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		mFieldX->setStyle(getSubStyleName(X_FIELD_STYLE_TYPE));
		mFieldY->setStyle(getSubStyleName(Y_FIELD_STYLE_TYPE));
	}

	const String& GUIVector2Field::getGUITypeName()
	{
		static String typeName = "GUIVector2Field";
		return typeName;
	}
}