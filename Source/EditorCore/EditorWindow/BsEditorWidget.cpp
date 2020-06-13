//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "EditorWindow/BsEditorWidget.h"
#include "GUI/BsGUIManager.h"
#include "GUI/BsGUIWidget.h"
#include "GUI/BsGUITexture.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUIPanel.h"
#include "EditorWindow/BsEditorWidgetContainer.h"
#include "EditorWindow/BsEditorWidgetManager.h"
#include "EditorWindow/BsEditorWindow.h"
#include "RenderAPI/BsRenderWindow.h"

namespace bs
{
	EditorWidgetBase::EditorWidgetBase(const HString& displayName, const String& name, UINT32 defaultWidth,
		UINT32 defaultHeight, EditorWidgetContainer& parentContainer)
		: mName(name), mDisplayName(displayName), mDefaultWidth(defaultWidth), mDefaultHeight(defaultHeight)
	{
		parentContainer.add(*this);
	}

	EditorWidgetBase::~EditorWidgetBase()
	{
		if (mContent != nullptr)
			GUILayout::destroy(mContent);
	}

	EditorWindowBase* EditorWidgetBase::getParentWindow() const
	{
		if (mParent != nullptr)
			return mParent->getParentWindow();
		else
			return nullptr;
	}

	void EditorWidgetBase::setHasFocus(bool focus)
	{
		EditorWidgetContainer* parentContainer = _getParent();
		if (parentContainer == nullptr)
			return;

		EditorWindowBase* parentWindow = parentContainer->getParentWindow();
		SPtr<RenderWindow> parentRenderWindow = parentWindow->getRenderWindow();
		const RenderWindowProperties& props = parentRenderWindow->getProperties();

		if (!props.hasFocus)
			return;

		if (parentContainer->getActiveWidget() != this)
			return;

		_setHasFocus(focus);
	}

	void EditorWidgetBase::setActive()
	{
		EditorWidgetContainer* parentContainer = _getParent();
		if (parentContainer == nullptr)
			return;

		parentContainer->setActiveWidget(mIndex);
	}

	void EditorWidgetBase::close()
	{
		EditorWidgetManager::instance().close(this);
	}

	void EditorWidgetBase::destroy(EditorWidgetBase* widget)
	{
		widget->~EditorWidgetBase();
		bs_free(widget);
	}

	void EditorWidgetBase::_setPosition(INT32 x, INT32 y)
	{
		if (mX == x && mY == y)
			return;

		mX = x;
		mY = y;

		if(mContent != nullptr)
			mContent->setPosition(x, y);

		doOnMoved(x, y);
	}

	void EditorWidgetBase::_setSize(UINT32 width, UINT32 height)
	{
		if (mWidth == width && mHeight == height)
			return;

		mWidth = width;
		mHeight = height;

		if (mContent != nullptr)
		{
			mContent->setWidth(width);
			mContent->setHeight(height);
		}

		doOnResized(width, height);
	}

	void EditorWidgetBase::_setHasFocus(bool focus) 
	{ 
		if (mHasFocus != focus)
		{
			mHasFocus = focus;

			onFocusChanged(focus);
		}
	}

	Vector2I EditorWidgetBase::screenToWidgetPos(const Vector2I& screenPos) const
	{
		if (mParent == nullptr)
			return screenPos;

		EditorWindowBase* parentEditorWindow = mParent->getParentWindow();
		SPtr<RenderWindow> parentRenderWindow = parentEditorWindow->getRenderWindow();

		Vector2I windowPos = parentRenderWindow->screenToWindowPos(screenPos);
		windowPos.x -= mX;
		windowPos.y -= mY;

		return windowPos;
	}

	Vector2I EditorWidgetBase::widgetToScreenPos(const Vector2I& widgetPos) const
	{
		if (mParent == nullptr)
			return widgetPos;

		EditorWindowBase* parentEditorWindow = mParent->getParentWindow();
		SPtr<RenderWindow> parentRenderWindow = parentEditorWindow->getRenderWindow();

		Vector2I windowPos = widgetPos;
		windowPos.x += mX;
		windowPos.y += mY;

		return parentRenderWindow->windowToScreenPos(windowPos);
	}

	void EditorWidgetBase::doOnMoved(INT32 x, INT32 y)
	{
		onMoved(x, y);
	}

	void EditorWidgetBase::doOnResized(UINT32 width, UINT32 height)
	{
		onResized(width, height);
	}

	void EditorWidgetBase::doOnParentChanged()
	{

	}

	// Note: Must not be virtual as parent container uses it in constructor
	void EditorWidgetBase::_changeParent(EditorWidgetContainer* parent, UINT32 indx)
	{
		if(mParent != parent) 
		{
			if(parent != nullptr)
			{
				if (mContent == nullptr)
					mContent = parent->getParentWidget().getPanel()->addNewElement<GUIPanel>();
				else
					parent->getParentWidget().getPanel()->addElement(mContent);
			}
			else
			{
				if (mContent != nullptr)
					mParent->getParentWidget().getPanel()->removeElement(mContent);
			}

			mParent = parent;
			mIndex = indx;

			doOnParentChanged();

			if(!onParentChanged.empty())
				onParentChanged(mParent);
		}
	}

	void EditorWidgetBase::_disable()
	{
		mContent->setVisible(false);
		mIsActive = false;
	}

	void EditorWidgetBase::_enable()
	{
		mContent->setVisible(true);
		mIsActive = true;
	}

	GUIWidget& EditorWidgetBase::getParentWidget() const
	{
		return mParent->getParentWidget();
	}
}