//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIToggleField.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUIToggle.h"
#include "GUI/BsGUILabel.h"

using namespace std::placeholders;

namespace bs
{
	GUIToggleField::GUIToggleField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		:TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel), mToggle(nullptr), mValue(false)
	{
		mToggle = GUIToggle::create(HString(""), getSubStyleName(getToggleStyleType()));
		mToggle->onToggled.connect(std::bind(&GUIToggleField::valueChanged, this, _1));

		mLayout->addElement(mToggle);
	}

	void GUIToggleField::setValue(bool value)
	{
		if (mValue == value)
			return;

		mValue = value;

		if(value) 
			mToggle->_toggleOn(false);
		else
			mToggle->_toggleOff(false);
	}

	void GUIToggleField::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mToggle->setTint(color);
	}

	void GUIToggleField::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		mToggle->setStyle(getSubStyleName(getToggleStyleType()));
	}

	void GUIToggleField::valueChanged(bool newValue)
	{
		setValue(newValue);
		onValueChanged(newValue);
	}

	const String& GUIToggleField::getGUITypeName()
	{
		static String typeName = "GUIToggleField";
		return typeName;
	}

	const String& GUIToggleField::getToggleStyleType()
	{
		static String STYLE_TYPE = "EditorFieldToggleInput";
		return STYLE_TYPE;
	}
}