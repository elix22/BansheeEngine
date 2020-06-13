//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIColorGradientField.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIColorGradient.h"

using namespace std::placeholders;

namespace bs
{
	template<class T, class TGUI, class TSELF>
	TGUIColorGradientField<T, TGUI, TSELF>::TGUIColorGradientField(const PrivatelyConstruct& dummy, 
		const GUIContent& labelContent, UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		: TGUIField<TSELF>(dummy, labelContent, labelWidth, style, dimensions, withLabel)
	{
		mGradient = TGUI::create(this->getSubStyleName(GRADIENT_FIELD_STYLE_TYPE));
		mGradient->onClicked.connect(std::bind(&TGUIColorGradientField::clicked, this));

		this->mLayout->addElement(mGradient);
	}

	template<class T, class TGUI, class TSELF>
	void TGUIColorGradientField<T, TGUI, TSELF>::setValue(const T& gradient)
	{
		if(mValue == gradient)
			return;

		mValue = gradient;
		mGradient->setGradient(gradient);
	}

	template<class T, class TGUI, class TSELF>
	void TGUIColorGradientField<T, TGUI, TSELF>::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mGradient->setTint(color);
	}

	template<class T, class TGUI, class TSELF>
	Vector2I TGUIColorGradientField<T, TGUI, TSELF>::_getOptimalSize() const
	{
		Vector2I optimalsize = mGradient->_getOptimalSize();

		if(mLabel != nullptr)
		{
			optimalsize.x += mLabel->_getOptimalSize().x;
			optimalsize.y = std::max(optimalsize.y, mLabel->_getOptimalSize().y);
		}

		return optimalsize;
	}

	template<class T, class TGUI, class TSELF>
	void TGUIColorGradientField<T, TGUI, TSELF>::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(this->getSubStyleName(this->getLabelStyleType()));

		mGradient->setStyle(this->getSubStyleName(GRADIENT_FIELD_STYLE_TYPE));
	}

	template<class T, class TGUI, class TSELF>
	void TGUIColorGradientField<T, TGUI, TSELF>::clicked()
	{
		onClicked();
	}

	template<class T, class TGUI, class TSELF>
	const String& TGUIColorGradientField<T, TGUI, TSELF>::getGUITypeName()
	{
		static String typeName = "GUIColorGradientField";
		return typeName;
	}

	template class BS_ED_EXPORT TGUIColorGradientField<ColorGradient, GUIColorGradient, GUIColorGradientField>;
	template class BS_ED_EXPORT TGUIColorGradientField<ColorGradientHDR, GUIColorGradientHDR, GUIColorGradientHDRField>;
}
