//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIIntField.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIInputBox.h"
#include "GUI/BsGUIWidget.h"
#include "GUI/BsGUIMouseEvent.h"
#include "Platform/BsCursor.h"
#include "RenderAPI/BsViewport.h"
#include <regex>

using namespace std::placeholders;

namespace bs
{
	const INT32 GUIIntField::DRAG_SPEED = 5;

	GUIIntField::GUIIntField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
		const String& style, const GUIDimensions& dimensions, bool withLabel)
		: TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel), mInputBox(nullptr), mValue(0)
		, mLastDragPos(0), mMinValue(std::numeric_limits<INT32>::lowest()), mMaxValue(std::numeric_limits<INT32>::max())
		, mStep(1), mIsDragging(false), mIsDragCursorSet(false), mHasInputFocus(false)
	{
		mInputBox = GUIInputBox::create(false, GUIOptions(GUIOption::flexibleWidth()), getSubStyleName(getInputStyleType()));
		mInputBox->setFilter(&GUIIntField::intFilter);

		mInputBox->onValueChanged.connect(std::bind((void(GUIIntField::*)(const String&))&GUIIntField::valueChanged, this, _1));
		mInputBox->onFocusChanged.connect(std::bind(&GUIIntField::focusChanged, this, _1));
		mInputBox->onConfirm.connect(std::bind(&GUIIntField::inputConfirmed, this));

		mLayout->addElement(mInputBox);

		setValue(0);
		mInputBox->setText("0");
		mFocusElement = mInputBox;
	}

	bool GUIIntField::_hasCustomCursor(const Vector2I position, CursorType& type) const
	{
		if (!_isDisabled())
		{
			Rect2I draggableArea;

			if (mLabel != nullptr)
				draggableArea = mLabel->_getLayoutData().area;

			if (draggableArea.contains(position))
			{
				type = CursorType::ArrowLeftRight;
				return true;
			}
		}

		return false;
	}

	bool GUIIntField::_mouseEvent(const GUIMouseEvent& event)
	{
		GUIElementContainer::_mouseEvent(event);

		Rect2I draggableArea;

		if(mLabel != nullptr)
			draggableArea = mLabel->_getLayoutData().area;

		if(event.getType() == GUIMouseEventType::MouseDragStart)
		{
			if (!_isDisabled())
			{
				if (draggableArea.contains(event.getDragStartPosition()))
				{
					mLastDragPos = event.getPosition().x;
					mIsDragging = true;
				}
			}

			return true;
		}
		else if(event.getType() == GUIMouseEventType::MouseDrag)
		{
			if (!_isDisabled())
			{
				if (mIsDragging)
				{
					INT32 xDiff = event.getPosition().x - mLastDragPos;

					INT32 jumpAmount = 0;
					Rect2I viewArea = _getParentWidget()->getTarget()->getPixelArea();
					if (event.getPosition().x <= 0)
					{
						Vector2I cursorScreenPos = Cursor::instance().getScreenPosition();

						jumpAmount = viewArea.width - event.getPosition().x - 1;
						cursorScreenPos.x += jumpAmount;

						Cursor::instance().setScreenPosition(cursorScreenPos);
					}
					else if (event.getPosition().x >= (INT32)viewArea.width)
					{
						Vector2I cursorScreenPos = Cursor::instance().getScreenPosition();

						jumpAmount = -(INT32)viewArea.width - (event.getPosition().x - (INT32)viewArea.width) + 1;
						cursorScreenPos.x += jumpAmount;

						Cursor::instance().setScreenPosition(cursorScreenPos);
					}

					INT32 oldValue = getValue();
					INT32 newValue = oldValue;

					if (xDiff >= DRAG_SPEED)
					{
						while (xDiff >= DRAG_SPEED)
						{
							newValue++;
							xDiff -= DRAG_SPEED;
						}
					}
					else if (xDiff <= -DRAG_SPEED)
					{
						while (xDiff <= -DRAG_SPEED)
						{
							newValue--;
							xDiff += DRAG_SPEED;
						}
					}

					mLastDragPos += (newValue - oldValue) * DRAG_SPEED + jumpAmount;

					if (oldValue != newValue)
						_setValue(newValue, true);
				}
			}

			return true;
		}
		else if(event.getType() == GUIMouseEventType::MouseDragEnd)
		{
			if (!_isDisabled())
				mIsDragging = false;

			return true;
		}

		return false;
	}

	void GUIIntField::styleUpdated()
	{
		if (mLabel != nullptr)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		mInputBox->setStyle(getSubStyleName(getInputStyleType()));
	}

	INT32 GUIIntField::getValue() const
	{
		return applyRangeAndStep(mValue);
	}

	INT32 GUIIntField::setValue(INT32 value)
	{
		if (mValue == value)
			return value;

		mValue = value;

		value = applyRangeAndStep(value);
		setText(value);

		return value;
	}

	void GUIIntField::setRange(INT32 min, INT32 max)
	{
		mMinValue = min;
		mMaxValue = max;
	}

	void GUIIntField::setStep(INT32 step)
	{
		mStep = step;
	}

	void GUIIntField::setTint(const Color& color)
	{
		if (mLabel != nullptr)
			mLabel->setTint(color);

		mInputBox->setTint(color);
	}

	void GUIIntField::_setValue(INT32 value, bool triggerEvent)
	{
		mValue = value;
		setText(value);

		if (triggerEvent)
			onValueChanged(mValue);
	}

	const String& GUIIntField::getGUITypeName()
	{
		static String typeName = "GUIIntField";
		return typeName;
	}

	const String& GUIIntField::getInputStyleType()
	{
		static String LABEL_STYLE_TYPE = "EditorFieldInput";
		return LABEL_STYLE_TYPE;
	}

	void GUIIntField::valueChanged(const String& newValue)
	{
		_setValue(parseINT32(newValue), true);
	}

	void GUIIntField::focusChanged(bool focus)
	{
		if (focus)
		{
			mHasInputFocus = true;
			onFocusChanged(true);
		}
		else
		{
			setText(applyRangeAndStep(mValue));

			mHasInputFocus = false;
			onFocusChanged(false);
		}
	}

	void GUIIntField::inputConfirmed()
	{
		onConfirm();
	}

	void GUIIntField::setText(INT32 value)
	{
		// Only update with new value if it actually changed, otherwise
		// problems can occur when user types in "0." and the field
		// updates back to "0" effectively making "." unusable
		float curValue = parseFloat(mInputBox->getText());
		if (value != curValue)
			mInputBox->setText(toString(value));
	}

	INT32 GUIIntField::applyRangeAndStep(INT32 value) const
	{
		if (mStep != 0)
			value = value - value % mStep;

		return Math::clamp(value, mMinValue, mMaxValue);
	}

	bool GUIIntField::intFilter(const String& str)
	{
		return std::regex_match(str, std::regex("-?(\\d+)?"));
	}
}