//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIVector3Field.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIFloatField.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUIVector3Field::ELEMENT_LABEL_WIDTH = 15;

	GUIVector3Field::GUIVector3Field(const PrivatelyConstruct& dummy, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		:TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel)
	{
		mFieldX = GUIFloatField::create(HString("X"), ELEMENT_LABEL_WIDTH, getSubStyleName(X_FIELD_STYLE_TYPE));
		mFieldY = GUIFloatField::create(HString("Y"), ELEMENT_LABEL_WIDTH, getSubStyleName(Y_FIELD_STYLE_TYPE));
		mFieldZ = GUIFloatField::create(HString("Z"), ELEMENT_LABEL_WIDTH, getSubStyleName(Z_FIELD_STYLE_TYPE));

		mFieldX->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::X); });
		mFieldY->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::Y); });
		mFieldZ->onValueChanged.connect([this](float val) { valueChanged(val, VectorComponent::Z); });

		mFieldX->onConfirm.connect([this]() { inputConfirmed(VectorComponent::X); });
		mFieldY->onConfirm.connect([this]() { inputConfirmed(VectorComponent::Y); });
		mFieldZ->onConfirm.connect([this]() { inputConfirmed(VectorComponent::Z); });

		mFieldX->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::X); });
		mFieldY->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::Y); });
		mFieldZ->onFocusChanged.connect([this](bool val) { onComponentFocusChanged(val, VectorComponent::Z); });

		mLayout->addElement(mFieldX);
		mLayout->addNewElement<GUIFixedSpace>(5);
		mLayout->addElement(mFieldY);
		mLayout->addNewElement<GUIFixedSpace>(5);
		mLayout->addElement(mFieldZ);
	}

	Vector3 GUIVector3Field::getValue() const
	{
		Vector3 value;
		value.x = mFieldX->getValue();
		value.y = mFieldY->getValue();
		value.z = mFieldZ->getValue();

		return value;
	}

	void GUIVector3Field::setValue(const Vector3& value)
	{
		mFieldX->setValue(value.x);
		mFieldY->setValue(value.y);
		mFieldZ->setValue(value.z);
	}

	bool GUIVector3Field::hasInputFocus() const
	{
		return mFieldX->hasInputFocus() || mFieldY->hasInputFocus() || mFieldZ->hasInputFocus();
	}

	void GUIVector3Field::setInputFocus(VectorComponent component, bool focus)
	{
		switch(component)
		{
		case VectorComponent::X: mFieldX->setFocus(focus, true); break;
		case VectorComponent::Y: mFieldY->setFocus(focus, true); break;
		case VectorComponent::Z: mFieldZ->setFocus(focus, true); break;
		default: break;
		}
	}

	void GUIVector3Field::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mFieldX->setTint(color);
		mFieldY->setTint(color);
		mFieldZ->setTint(color);
	}

	void GUIVector3Field::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		mFieldX->setStyle(getSubStyleName(X_FIELD_STYLE_TYPE));
		mFieldY->setStyle(getSubStyleName(Y_FIELD_STYLE_TYPE));
		mFieldZ->setStyle(getSubStyleName(Z_FIELD_STYLE_TYPE));
	}

	void GUIVector3Field::valueChanged(float newValue, VectorComponent component)
	{
		onComponentChanged(newValue, component);

		Vector3 value = getValue();
		onValueChanged(value);
	}

	void GUIVector3Field::inputConfirmed(VectorComponent component)
	{
		onConfirm(component);
	}

	const String& GUIVector3Field::getGUITypeName()
	{
		static String typeName = "GUIVector3Field";
		return typeName;
	}
}