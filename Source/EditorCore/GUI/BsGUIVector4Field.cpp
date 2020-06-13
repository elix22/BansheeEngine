//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIVector4Field.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIFloatField.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUIVector4Field::ELEMENT_LABEL_WIDTH = 15;

	GUIVector4Field::GUIVector4Field(const PrivatelyConstruct& dummy, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		:TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel)
	{
		mFieldX = GUIFloatField::create(HString("X"), ELEMENT_LABEL_WIDTH, getSubStyleName(X_FIELD_STYLE_TYPE));
		mFieldY = GUIFloatField::create(HString("Y"), ELEMENT_LABEL_WIDTH, getSubStyleName(Y_FIELD_STYLE_TYPE));
		mFieldZ = GUIFloatField::create(HString("Z"), ELEMENT_LABEL_WIDTH, getSubStyleName(Z_FIELD_STYLE_TYPE));
		mFieldW = GUIFloatField::create(HString("W"), ELEMENT_LABEL_WIDTH, getSubStyleName(W_FIELD_STYLE_TYPE));

		mFieldX->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::X); });
		mFieldY->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::Y); });
		mFieldZ->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::Z); });
		mFieldW->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::W); });

		mFieldX->onConfirm.connect([this]() { inputConfirmed(VectorComponent::X); });
		mFieldY->onConfirm.connect([this]() { inputConfirmed(VectorComponent::Y); });
		mFieldZ->onConfirm.connect([this]() { inputConfirmed(VectorComponent::Z); });
		mFieldW->onConfirm.connect([this]() { inputConfirmed(VectorComponent::W); });

		mFieldX->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::X); });
		mFieldY->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::Y); });
		mFieldZ->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::Z); });
		mFieldW->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::W); });

		mLayout->removeElement(mLabel);

		GUILayout* layout = mLayout->addNewElement<GUILayoutY>();
		layout->addElement(mLabel);
		mLabel->resetDimensions();

		GUILayout* elementLayout = layout->addNewElement<GUILayoutX>();

		elementLayout->addElement(mFieldX);
		elementLayout->addNewElement<GUIFixedSpace>(5);
		elementLayout->addElement(mFieldY);
		elementLayout->addNewElement<GUIFixedSpace>(5);
		elementLayout->addElement(mFieldZ);
		elementLayout->addNewElement<GUIFixedSpace>(5);
		elementLayout->addElement(mFieldW);
	}

	Vector4 GUIVector4Field::getValue() const
	{
		Vector4 value;
		value.x = mFieldX->getValue();
		value.y = mFieldY->getValue();
		value.z = mFieldZ->getValue();
		value.w = mFieldW->getValue();

		return value;
	}

	void GUIVector4Field::setValue(const Vector4& value)
	{
		mFieldX->setValue(value.x);
		mFieldY->setValue(value.y);
		mFieldZ->setValue(value.z);
		mFieldW->setValue(value.w);
	}

	bool GUIVector4Field::hasInputFocus() const
	{
		return mFieldX->hasInputFocus() || mFieldY->hasInputFocus() || mFieldZ->hasInputFocus() || mFieldW->hasInputFocus();
	}

	void GUIVector4Field::setInputFocus(VectorComponent component, bool focus)
	{
		switch(component)
		{
		case VectorComponent::X: mFieldX->setFocus(focus, true); break;
		case VectorComponent::Y: mFieldY->setFocus(focus, true); break;
		case VectorComponent::Z: mFieldZ->setFocus(focus, true); break;
		case VectorComponent::W: mFieldW->setFocus(focus, true); break;
		default: break;
		}
	}

	void GUIVector4Field::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mFieldX->setTint(color);
		mFieldY->setTint(color);
		mFieldZ->setTint(color);
		mFieldW->setTint(color);
	}

	void GUIVector4Field::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		mFieldX->setStyle(getSubStyleName(X_FIELD_STYLE_TYPE));
		mFieldY->setStyle(getSubStyleName(Y_FIELD_STYLE_TYPE));
		mFieldZ->setStyle(getSubStyleName(Z_FIELD_STYLE_TYPE));
		mFieldW->setStyle(getSubStyleName(W_FIELD_STYLE_TYPE));
	}

	void GUIVector4Field::valueChanged(float newValue, VectorComponent component)
	{
		onComponentChanged(newValue, component);

		Vector4 value = getValue();
		onValueChanged(value);
	}

	void GUIVector4Field::inputConfirmed(VectorComponent component)
	{
		onConfirm(component);
	}

	const String& GUIVector4Field::getGUITypeName()
	{
		static String typeName = "GUIVector4Field";
		return typeName;
	}
}