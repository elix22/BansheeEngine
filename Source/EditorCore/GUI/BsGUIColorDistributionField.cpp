//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIColorDistributionField.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIColorField.h"
#include "GUI/BsGUIColorGradientField.h"
#include "GUI/BsGUICurves.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUIContextMenu.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

namespace bs
{
	/** Style type name for the internal color fields. */
	static constexpr const char* COLOR_FIELD_STYLE_TYPE = "ColorField";

	/** Style type name for the internal color gradient field. */
	static constexpr const char* COLOR_GRADIENT_FIELD_STYLE_TYPE = "GradientField";

	/** Style type name for the internal drop down button. */
	static constexpr const char* DROP_DOWN_FIELD_STYLE_TYPE = "DropDownButton";

	/** Spacing between two internal elements, in pixels. */
	static constexpr UINT32 ELEM_SPACING = 5;

	GUIColorDistributionField::GUIColorDistributionField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		: TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel)
	{
		mContextMenu = bs_shared_ptr_new<GUIContextMenu>();

		mContextMenu->addMenuItem("Constant", [this]()
		{
			mValue = ColorDistribution(mMinConstant);
			mPropertyType = PDT_Constant;
			rebuild();
		}, 50);

		mContextMenu->addMenuItem("Range", [this]()
		{
			mValue = ColorDistribution(mMinConstant, mMaxConstant);
			mPropertyType = PDT_RandomRange;
			rebuild();
		}, 40);

		mContextMenu->addMenuItem("Gradient", [this]()
		{
			mValue = ColorDistribution(mMinGradient);
			mPropertyType = PDT_Curve;
			rebuild();
		}, 30);

		mContextMenu->addMenuItem("Gradient range", [this]()
		{
			mValue = ColorDistribution(mMinGradient, mMaxGradient);
			mPropertyType = PDT_RandomCurveRange;
			rebuild();
		}, 20);

		rebuild();
	}

	void GUIColorDistributionField::setValue(const ColorDistribution& value)
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
			mMinGradient = mMinConstant;
			mMaxGradient = mMinConstant;

			mMinColorField->setValue(mMinConstant);
			break;
		case PDT_RandomRange: 
			mMinConstant = mValue.getMinConstant();
			mMaxConstant = mValue.getMaxConstant();
			mMinGradient = mMinConstant;
			mMaxGradient = mMaxConstant;

			mMinColorField->setValue(mMinConstant);
			mMaxColorField->setValue(mMaxConstant);
			break;
		case PDT_Curve: 
			mMinGradient = mValue.getMinGradient();
			mMaxGradient = mMinGradient;
			mMinConstant = Color::fromRGBA(mMinGradient.evaluate(0.0f));
			mMaxConstant = Color::fromRGBA(mMaxGradient.evaluate(0.0f));

			mMinGradientField->setValue(mMinGradient);
			break;
		case PDT_RandomCurveRange: 
			mMinGradient = mValue.getMinGradient();
			mMaxGradient = mValue.getMaxGradient();
			mMinConstant = Color::fromRGBA(mMinGradient.evaluate(0.0f));
			mMaxConstant = Color::fromRGBA(mMaxGradient.evaluate(0.0f));

			mMinGradientField->setValue(mMinGradient);
			mMaxGradientField->setValue(mMaxGradient);
			break;
		}
	}

	void GUIColorDistributionField::setTint(const Color& color)
	{
		mDropDownButton->setTint(color);

		if (mLabel)
			mLabel->setTint(color);

		if(mMinColorField)
			mMinColorField->setTint(color);

		if(mMaxColorField)
			mMaxColorField->setTint(color);

		if(mMinGradientField)
			mMinGradientField->setTint(color);

		if(mMaxGradientField)
			mMaxGradientField->setTint(color);

		for(auto& entry : mLabels)
		{
			if(entry)
				entry->setTint(color);
		}
	}

	Vector2I GUIColorDistributionField::_getOptimalSize() const
	{
		Vector2I optimalsize = Vector2I::ZERO;

		if(mMinColorField)
		{
			Vector2I elemOptimal = mMinColorField->_calculateLayoutSizeRange().optimal;

			optimalsize.x = std::max(optimalsize.x, elemOptimal.x);
			optimalsize.y += elemOptimal.y;
		}

		if(mMaxColorField)
		{
			Vector2I elemOptimal = mMaxColorField->_calculateLayoutSizeRange().optimal;

			optimalsize.x = std::max(optimalsize.x, elemOptimal.x);
			optimalsize.y += elemOptimal.y;

			optimalsize.y += ELEM_SPACING;
		}

		if(mMinGradientField)
		{
			Vector2I elemOptimal = mMinGradientField->_calculateLayoutSizeRange().optimal;

			optimalsize.x = std::max(optimalsize.x, elemOptimal.x);
			optimalsize.y += elemOptimal.y;
		}

		if(mMaxGradientField)
		{
			Vector2I elemOptimal = mMaxGradientField->_calculateLayoutSizeRange().optimal;

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

		Vector2I dropDownSize = mDropDownButton->_calculateLayoutSizeRange().optimal;
		optimalsize.x += dropDownSize.x;
		optimalsize.y = std::max(optimalsize.y, dropDownSize.y);

		if (mLabel)
		{
			Vector2I elemOptimal = mLabel->_calculateLayoutSizeRange().optimal;

			optimalsize.x += elemOptimal.x;
			optimalsize.y = std::max(optimalsize.y, elemOptimal.y);
		}

		return optimalsize;
	}

	void GUIColorDistributionField::styleUpdated()
	{
		mDropDownButton->setStyle(getSubStyleName(DROP_DOWN_FIELD_STYLE_TYPE));

		if (mLabel)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		if (mMinColorField)
			mMinColorField->setStyle(getSubStyleName(COLOR_FIELD_STYLE_TYPE));

		if (mMaxColorField)
			mMaxColorField->setStyle(getSubStyleName(COLOR_FIELD_STYLE_TYPE));

		if (mMinGradientField)
			mMinGradientField->setStyle(getSubStyleName(COLOR_GRADIENT_FIELD_STYLE_TYPE));

		if (mMaxGradientField)
			mMaxGradientField->setStyle(getSubStyleName(COLOR_GRADIENT_FIELD_STYLE_TYPE));
	}

	void GUIColorDistributionField::rebuild()
	{
		if(mLabel)
			mLayout->removeElement(mLabel);

		mLayout->clear();
		mLayout->addElement(mLabel);

		GUILayout* valueLayout = mLayout->addNewElement<GUILayoutY>();
		switch (mPropertyType)
		{
		default:
		case PDT_Constant:
			mMinColorField = GUIColorField::create(GUIOptions(), getSubStyleName(COLOR_FIELD_STYLE_TYPE));
			mMaxColorField = nullptr;
			mMinGradientField = nullptr;
			mMaxGradientField = nullptr;
			mLabels = { nullptr, nullptr };

			mMinColorField->setValue(mMinConstant);
			mMinColorField->onClicked.connect([this]() { onMinClicked(); });

			valueLayout->addElement(mMinColorField);
			break;
		case PDT_RandomRange: 
			mMinColorField = GUIColorField::create(GUIOptions(), getSubStyleName(COLOR_FIELD_STYLE_TYPE));
			mMaxColorField = GUIColorField::create(GUIOptions(), getSubStyleName(COLOR_FIELD_STYLE_TYPE));
			mMinGradientField = nullptr;
			mMaxGradientField = nullptr;

			mMinColorField->setValue(mMinConstant);
			mMinColorField->onClicked.connect([this]() { onMinClicked(); });

			mMaxColorField->setValue(mMaxConstant);
			mMaxColorField->onClicked.connect([this]() { onMaxClicked(); });

			mLabels[0] = valueLayout->addNewElement<GUILabel>(HString("Min."), "HeaderLight");
			valueLayout->addElement(mMinColorField);
			valueLayout->addNewElement<GUIFixedSpace>(ELEM_SPACING);
			mLabels[1] = valueLayout->addNewElement<GUILabel>(HString("Max."), "HeaderLight");
			valueLayout->addElement(mMaxColorField);
			break;
		case PDT_Curve: 
			mMinColorField = nullptr;
			mMaxColorField = nullptr;
			mMinGradientField = GUIColorGradientField::create(getSubStyleName(COLOR_GRADIENT_FIELD_STYLE_TYPE));
			mMaxGradientField = nullptr;
			mLabels = { nullptr, nullptr };

			mMinGradientField->setValue(mMinGradient);
			mMinGradientField->onClicked.connect([this](){ onMinClicked(); });

			valueLayout->addElement(mMinGradientField);
			break;
		case PDT_RandomCurveRange: 
			mMinColorField = nullptr;
			mMaxColorField = nullptr;
			mMinGradientField = GUIColorGradientField::create(getSubStyleName(COLOR_GRADIENT_FIELD_STYLE_TYPE));

			mMinGradientField->setValue(mMinGradient);
			mMinGradientField->onClicked.connect([this](){ onMinClicked(); });

			mMaxGradientField = GUIColorGradientField::create(getSubStyleName(COLOR_GRADIENT_FIELD_STYLE_TYPE));

			mMaxGradientField->setValue(mMaxGradient);
			mMaxGradientField->onClicked.connect([this](){ onMaxClicked(); });

			mLabels[0] = valueLayout->addNewElement<GUILabel>(HString("Min."), "HeaderLight");
			valueLayout->addElement(mMinGradientField);
			valueLayout->addNewElement<GUIFixedSpace>(ELEM_SPACING);
			mLabels[1] = valueLayout->addNewElement<GUILabel>(HString("Max."), "HeaderLight");
			valueLayout->addElement(mMaxGradientField);
			break;
		}

		mDropDownButton = GUIButton::create(HString::dummy(), getSubStyleName(DROP_DOWN_FIELD_STYLE_TYPE));
		mDropDownButton->onClick.connect([this]()
		{
			const Rect2I bounds = mDropDownButton->getBounds(mParentWidget->getPanel());
			const Vector2I center(bounds.x + bounds.width / 2, bounds.y + bounds.height / 2);

			mContextMenu->open(center, *mParentWidget);
		});

		mLayout->addNewElement<GUIFixedSpace>(10);
		mLayout->addElement(mDropDownButton);
	}

	const String& GUIColorDistributionField::getGUITypeName()
	{
		static String typeName = "GUIColorDistributionField";
		return typeName;
	}
}
