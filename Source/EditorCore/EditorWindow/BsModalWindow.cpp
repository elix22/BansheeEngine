//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "EditorWindow/BsModalWindow.h"
#include "EditorWindow/BsEditorWindowManager.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Platform/BsPlatform.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUISpace.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUITexture.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIPanel.h"
#include "GUI/BsCGUIWidget.h"

namespace bs
{
	ModalWindow::ModalWindow(const HString& title, bool hasCloseButton, UINT32 width, UINT32 height)
		: EditorWindowBase(true, width, height)
	{
		EditorWindowManager::instance().registerWindow(this);

		mTitleBarBgPanel = mGUI->getPanel()->addNewElement<GUIPanel>();
		mTitleBarBgPanel->setDepthRange(std::numeric_limits<INT16>::max() - 1);
		mTitleBarBgPanel->setPosition(1, 1);

		mTitleBarPanel = mGUI->getPanel()->addNewElement<GUIPanel>();
		mTitleBarPanel->setDepthRange(0);
		mTitleBarPanel->setPosition(1, 1);

		mTitleBarBg = GUITexture::create(GUIOptions(GUIOption::flexibleWidth()), "TitleBarBackground");
		mTitle = GUILabel::create(title);

		GUILayout* bgLayout = mTitleBarBgPanel->addNewElement<GUILayoutY>();
		GUILayout* bgLayoutX = bgLayout->addNewElement<GUILayoutX>();
		bgLayoutX->addElement(mTitleBarBg);
		bgLayoutX->addNewElement<GUIFixedSpace>(1);
		bgLayout->addNewElement<GUIFlexibleSpace>();
		bgLayout->addNewElement<GUIFixedSpace>(1);

		GUILayout* contentLayoutY = mTitleBarPanel->addNewElement<GUILayoutY>();
		GUILayout* contentLayoutX = contentLayoutY->addNewElement<GUILayoutX>();
		contentLayoutX->addNewElement<GUIFlexibleSpace>();
		GUILayout* titleLayout = contentLayoutX->addNewElement<GUILayoutY>();
		titleLayout->addNewElement<GUIFixedSpace>(2);
		titleLayout->addElement(mTitle);
		titleLayout->addNewElement<GUIFlexibleSpace>();
		contentLayoutX->addNewElement<GUIFlexibleSpace>();

		if (hasCloseButton)
		{
			mCloseButton = GUIButton::create(HString(""), "WinCloseBtn");
			contentLayoutX->addElement(mCloseButton);
			
			mCloseButton->onClick.connect(std::bind(&ModalWindow::close, this));

			GUIElementOptions options = mCloseButton->getOptionFlags();
			options.unset(GUIElementOption::AcceptsKeyFocus);
			mCloseButton->setOptionFlags(options);
		}

		contentLayoutX->addNewElement<GUIFixedSpace>(1);
		contentLayoutY->addNewElement<GUIFlexibleSpace>();
		contentLayoutY->addNewElement<GUIFixedSpace>(1);

		mContents = mGUI->getPanel()->addNewElement<GUIPanel>();
		mContents->setDepthRange(0);
		mContents->setPosition(1, 1 + getTitleBarHeight());

		updateSize();
	}

	Vector2I ModalWindow::screenToWindowPos(const Vector2I& screenPos) const
	{
		Vector2I renderWindowPos = getRenderWindow()->screenToWindowPos(screenPos);

		Vector2I contentsPos = renderWindowPos;
		Rect2I contentArea = getContentArea();

		contentsPos.x -= contentArea.x;
		contentsPos.y -= contentArea.y;

		return contentsPos;
	}

	Vector2I ModalWindow::windowToScreenPos(const Vector2I& windowPos) const
	{
		Vector2I contentsPos = windowPos;
		Rect2I contentArea = getContentArea();

		contentsPos.x += contentArea.x;
		contentsPos.y += contentArea.y;

		return getRenderWindow()->windowToScreenPos(contentsPos);
	}

	void ModalWindow::update()
	{

	}

	void ModalWindow::close()
	{
		EditorWindowManager::instance().destroy(this);
	}

	void ModalWindow::setTitle(const HString& title)
	{
		mTitle->setContent(GUIContent(title));
	}

	void ModalWindow::setSize(UINT32 width, UINT32 height)
	{
		EditorWindowBase::setSize(width, height);
		
		updateSize();
	}

	UINT32 ModalWindow::getContentWidth() const
	{
		return getWidth() - 2;
	}

	UINT32 ModalWindow::getContentHeight() const
	{
		return getHeight() - getTitleBarHeight() - 2;
	}

	void ModalWindow::setContentSize(UINT32 width, UINT32 height)
	{
		UINT32 actualWidth = width + 2;
		UINT32 actualHeight = height + getTitleBarHeight() + 2;

		setSize(actualWidth, actualHeight);
	}

	void ModalWindow::resized()
	{
		EditorWindowBase::resized();

		updateSize();
	}

	void ModalWindow::updateSize()
	{
		mContents->setWidth(getWidth() - 2);
		mContents->setHeight(getHeight() - 2 - getTitleBarHeight());

		UINT32 captionWidth = getWidth() - 2;
		if (mCloseButton != nullptr)
			captionWidth = mCloseButton->getGlobalBounds().x;

		Vector<Rect2I> captionAreas;
		captionAreas.push_back(Rect2I(1, 1, captionWidth, getTitleBarHeight()));

		Platform::setCaptionNonClientAreas(*mRenderWindow->getCore().get(), captionAreas);
	}

	Rect2I ModalWindow::getContentArea() const
	{
		return Rect2I(1, 1 + getTitleBarHeight(), getContentWidth(), getContentHeight());
	}

	UINT32 ModalWindow::getTitleBarHeight() const
	{
		return mTitleBarBg->getBounds().height;
	}
}