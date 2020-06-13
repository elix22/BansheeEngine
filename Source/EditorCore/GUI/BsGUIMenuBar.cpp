//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIMenuBar.h"
#include "GUI/BsGUIPanel.h"
#include "GUI/BsGUIElement.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUITexture.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUISpace.h"
#include "GUI/BsGUIMenu.h"
#include "GUI/BsGUIManager.h"
#include "Resources/BsBuiltinResources.h"
#include "GUI/BsGUIDropDownBoxManager.h"
#include "Scene/BsSceneObject.h"
#include "Platform/BsPlatform.h"
#include "CoreThread/BsCoreThread.h"
#include "GUI/BsShortcutManager.h"
#include "GUI/BsGUIHoverHitBox.h"

namespace bs
{
	const UINT32 GUIMenuBar::NUM_ELEMENTS_AFTER_CONTENT = 8;
	const UINT32 GUIMenuBar::ELEMENT_SPACING = 10;

	const String& GUIMenuBar::getBackgroundStyleType()
	{
		static const String type = "MenuBarBg";
		return type;
	}

	const String& GUIMenuBar::getLineStyleType()
	{
		static const String type = "MenuBarLine";
		return type;
	}

	const String& GUIMenuBar::getLogoStyleType()
	{
		static const String type = "MenuBarBansheeLogo";
		return type;
	}

	const String& GUIMenuBar::getMenuItemButtonStyleType()
	{
		static const String type = "MenuBarBtn";
		return type;
	}

	const String& GUIMenuBar::getToolBarButtonStyleType()
	{
		static const String type = "ToolBarBtn";
		return type;
	}

	const String& GUIMenuBar::getToolBarSeparatorStyleType()
	{
		static const String type = "ToolBarSeparator";
		return type;
	}

	GUIMenuBar::GUIMenuBar(GUIWidget* parent, RenderWindow* parentWindow)
		: mParentWindow(parentWindow), mMainPanel(nullptr), mBgPanel(nullptr), mMenuItemLayout(nullptr)
		, mBgTexture(nullptr), mLogoTexture(nullptr), mSubMenuButton(nullptr), mSubMenuOpen(false)
	{
		mMainPanel = parent->getPanel()->addNewElement<GUIPanel>(std::numeric_limits<INT16>::min() + 15);
		mMainPanel->setWidth(1);
		mMainPanel->setHeight(50);

		mBgPanel = parent->getPanel()->addNewElement<GUIPanel>(std::numeric_limits<INT16>::min() + 25);
		mBgPanel->setWidth(1);
		mBgPanel->setHeight(50);

		mBgTexture = GUITexture::create(TextureScaleMode::StretchToFit,
			GUIOptions(GUIOption::flexibleWidth(), GUIOption::flexibleHeight()), getBackgroundStyleType());
		
		GUILayoutX* bgLayout = mBgPanel->addNewElement<GUILayoutX>();
		bgLayout->addElement(mBgTexture);

		mLogoTexture = GUITexture::create(TextureScaleMode::StretchToFit, getLogoStyleType());
		mMinBtn = GUIButton::create(HString(""), "WinMinimizeBtn");
		mMaxBtn = GUIButton::create(HString(""), "WinMaximizeBtn");
		mCloseBtn = GUIButton::create(HString(""), "WinCloseBtn");
		mSplitterLine = GUITexture::create(TextureScaleMode::StretchToFit, getLineStyleType());

		GUIElementOptions minBtnOptions = mMinBtn->getOptionFlags();
		minBtnOptions.unset(GUIElementOption::AcceptsKeyFocus);
		mMinBtn->setOptionFlags(minBtnOptions);

		GUIElementOptions maxBtnOptions = mMaxBtn->getOptionFlags();
		maxBtnOptions.unset(GUIElementOption::AcceptsKeyFocus);
		mMaxBtn->setOptionFlags(maxBtnOptions);

		GUIElementOptions closeBtnOptions = mCloseBtn->getOptionFlags();
		closeBtnOptions.unset(GUIElementOption::AcceptsKeyFocus);
		mCloseBtn->setOptionFlags(closeBtnOptions);

		GUILayout* mainLayout = mMainPanel->addNewElement<GUILayoutX>();
		mainLayout->addElement(mLogoTexture);
		mainLayout->addNewElement<GUIFixedSpace>(5);

		GUILayout* mainLayoutVert = mainLayout->addNewElement<GUILayoutY>();
		mMenuItemLayout = mainLayoutVert->addNewElement<GUILayoutX>();
		mainLayoutVert->addElement(mSplitterLine);
		mToolBarLayout = mainLayoutVert->addNewElement<GUILayoutX>();
		mainLayoutVert->addNewElement<GUIFlexibleSpace>();

		mMenuItemLayout->addNewElement<GUIFlexibleSpace>();
		mMenuItemLayout->addNewElement<GUIFixedSpace>(3);
		mMenuItemLayout->addElement(mMinBtn);
		mMenuItemLayout->addNewElement<GUIFixedSpace>(3);
		mMenuItemLayout->addElement(mMaxBtn);
		mMenuItemLayout->addNewElement<GUIFixedSpace>(3);
		mMenuItemLayout->addElement(mCloseBtn);
		mMenuItemLayout->addNewElement<GUIFixedSpace>(3);

		mToolBarLayout->addNewElement<GUIFlexibleSpace>();

		mMinBtn->onClick.connect(std::bind(&GUIMenuBar::onMinimizeClicked, this));
		mMaxBtn->onClick.connect(std::bind(&GUIMenuBar::onMaximizeClicked, this));
		mCloseBtn->onClick.connect(std::bind(&GUIMenuBar::onCloseClicked, this));

		refreshNonClientAreas();
	}

	GUIMenuBar::~GUIMenuBar()
	{
		closeSubMenu();

		for(auto& menu : mChildMenus)
		{
			bs_delete(menu.menu);
			GUIElement::destroy(menu.button);
			GUIFixedSpace::destroy(menu.space);
		}

		GUILayout::destroy(mMainPanel);
		GUILayout::destroy(mBgPanel);
	}

	void GUIMenuBar::setArea(INT32 x, INT32 y, UINT32 width, UINT32 height)
	{
		mMainPanel->setPosition(x, y);

		mMainPanel->setWidth(width);
		mMainPanel->setHeight(height);

		mBgPanel->setPosition(x, y);

		mBgPanel->setWidth(width);
		mBgPanel->setHeight(height);

		refreshNonClientAreas();
	}

	GUIMenuItem* GUIMenuBar::addMenuItem(const String& path, std::function<void()> callback, 
		INT32 priority, const ShortcutKey& shortcut)
	{
		String strippedPath = path;
		String rootName;

		if(!stripPath(strippedPath, rootName))
			return nullptr;

		const GUIMenuBarData* subMenu = getSubMenu(rootName);
		if(subMenu == nullptr)
		{
			subMenu = addNewButton(rootName, priority);

			refreshNonClientAreas();
		}

		if (shortcut.isValid() && callback != nullptr)
			registerShortcut(path, shortcut, callback);

		return subMenu->menu->addMenuItem(strippedPath, callback, priority, shortcut);
	}

	GUIMenuItem* GUIMenuBar::addMenuItemSeparator(const String& path, INT32 priority)
	{
		String strippedPath = path;
		String rootName;

		if(!stripPath(strippedPath, rootName))
			return nullptr;

		const GUIMenuBarData* subMenu = getSubMenu(rootName);
		if(subMenu == nullptr)
		{
			subMenu = addNewButton(rootName, priority);

			refreshNonClientAreas();
		}

		return subMenu->menu->addSeparator(strippedPath, priority);
	}

	GUIMenuBar::GUIMenuBarData* GUIMenuBar::addNewButton(const String& name, INT32 priority)
	{
		UINT32 numElements = (UINT32)mChildMenus.size();
		UINT32 position = numElements;
		for (UINT32 i = 0; i < numElements; i++)
		{
			if (priority > mChildMenus[i].priority)
			{
				position = i;
				break;
			}
		}

		auto iter = mChildMenus.insert(mChildMenus.begin() + position, GUIMenuBarData());

		GUIMenuBarData& newSubMenu = *iter;
		newSubMenu.name = name;
		newSubMenu.menu = bs_new<GUIMenu>();
		newSubMenu.priority = priority;

		GUIButton* newButton = GUIButton::create(HString(name), getMenuItemButtonStyleType());
		newButton->onClick.connect(std::bind(&GUIMenuBar::openSubMenu, this, name));
		newButton->onHover.connect(std::bind(&GUIMenuBar::onSubMenuHover, this, name));

		GUIElementOptions btnOptions = newButton->getOptionFlags();
		btnOptions.unset(GUIElementOption::AcceptsKeyFocus);
		newButton->setOptionFlags(btnOptions);

		UINT32 layoutPosition = mMenuItemLayout->getNumChildren() - NUM_ELEMENTS_AFTER_CONTENT - (numElements - position) * 2;

		GUIFixedSpace* space = mMenuItemLayout->insertNewElement<GUIFixedSpace>(layoutPosition, ELEMENT_SPACING);
		mMenuItemLayout->insertElement(layoutPosition, newButton);

		newSubMenu.button = newButton;
		newSubMenu.space = space;

		return &newSubMenu;
	}

	GUIMenuItem* GUIMenuBar::getMenuItem(const String& path)
	{
		String strippedPath = path;
		String rootName;

		if(!stripPath(strippedPath, rootName))
			return nullptr;

		const GUIMenuBarData* subMenu = getSubMenu(rootName);
		if(subMenu == nullptr)
			return nullptr;

		return subMenu->menu->getMenuItem(strippedPath);
	}

	void GUIMenuBar::removeMenuItem(const String& path)
	{
		String strippedPath = path;
		String rootName;

		if(!stripPath(strippedPath, rootName))
			return;

		if(strippedPath == "")
		{
			UINT32 curIdx = 0;
			GUIMenuBarData* subMenuToRemove = nullptr;
			for(auto& subMenuData : mChildMenus)
			{
				if(subMenuData.name == rootName)
				{
					subMenuToRemove = &subMenuData;
					break;
				}

				curIdx++;
			}

			if(subMenuToRemove == nullptr)
				return;

			mMenuItemLayout->removeElement(subMenuToRemove->button);
			mMenuItemLayout->removeElement(subMenuToRemove->space);
			GUIElement::destroy(subMenuToRemove->button);
			GUIFixedSpace::destroy(subMenuToRemove->space);
			bs_delete(subMenuToRemove->menu);

			mChildMenus.erase(mChildMenus.begin() + curIdx);

			refreshNonClientAreas();
			return;
		}

		const GUIMenuBarData* subMenu = getSubMenu(rootName);
		if(subMenu == nullptr)
			return;

		const GUIMenuItem* item = subMenu->menu->getMenuItem(strippedPath);
		if(item == nullptr)
			return;

		subMenu->menu->removeMenuItem(item);
	}

	void GUIMenuBar::removeMenuItem(GUIMenuItem* item)
	{
		if (item == nullptr)
			return;

		GUIMenuItem* parent = item->getParent();
		if (parent != nullptr)
		{
			parent->removeChild(item);
		}
	}

	const GUIMenuBar::GUIMenuBarData* GUIMenuBar::getSubMenu(const String& name) const
	{
		for(auto& subMenu : mChildMenus)
		{
			if(subMenu.name == name)
				return &subMenu;
		}

		return nullptr;
	}

	void GUIMenuBar::addToolBarButton(const String& name, const GUIContent& content, 
		std::function<void()> callback, INT32 priority)
	{
		removeToolBarButton(name);

		UINT32 numElements = (UINT32)mToolbarElements.size();
		UINT32 idx = 0;
		for (idx = 0; idx < numElements; idx++)
		{
			GUIToolBarData& element = mToolbarElements[idx];

			if (element.priority < priority)
				break;
		}

		if (idx >= numElements)
		{
			mToolbarElements.push_back(GUIToolBarData());
			idx = numElements;
		}
		else
		{
			mToolbarElements.insert(mToolbarElements.begin() + idx, GUIToolBarData());
		}

		GUIToolBarData& newData = mToolbarElements[idx];
		newData.name = name;
		newData.priority = priority;
		newData.button = GUIButton::create(content, getToolBarButtonStyleType());
		newData.space = GUIFixedSpace::create(5);
		newData.separator = nullptr;

		mToolBarLayout->insertElement(idx * 2, newData.button);
		mToolBarLayout->insertElement(idx * 2 + 1, newData.space);

		newData.button->onClick.connect(callback);
	}

	void GUIMenuBar::addToolBarSeparator(const String& name, INT32 priority)
	{
		removeToolBarButton(name);

		UINT32 numElements = (UINT32)mToolbarElements.size();
		UINT32 idx = 0;
		for (idx = 0; idx < numElements; idx++)
		{
			GUIToolBarData& element = mToolbarElements[idx];

			if (element.priority < priority)
				break;
		}

		if (idx >= numElements)
		{
			mToolbarElements.push_back(GUIToolBarData());
			idx = numElements;
		}
		else
		{
			mToolbarElements.insert(mToolbarElements.begin() + idx, GUIToolBarData());
		}

		GUIToolBarData& newData = mToolbarElements[idx];
		newData.name = name;
		newData.priority = priority;
		newData.space = GUIFixedSpace::create(5);
		newData.separator = GUITexture::create(getToolBarSeparatorStyleType());
		newData.button = nullptr;

		mToolBarLayout->insertElement(idx * 2, newData.separator);
		mToolBarLayout->insertElement(idx * 2 + 1, newData.space);
	}

	void GUIMenuBar::removeToolBarButton(const String& name)
	{
		for (UINT32 i = 0; i < (UINT32)mToolbarElements.size(); i++)
		{
			GUIToolBarData& element = mToolbarElements[i];

			if (element.name == name)
			{
				if (element.button != nullptr)
					GUIElement::destroy(element.button);

				if (element.separator != nullptr)
					GUIElement::destroy(element.separator);

				if (element.space != nullptr)
					GUIFixedSpace::destroy(element.space);

				mToolbarElements.erase(mToolbarElements.begin() + i);
				return;
			}
		}
	}

	void GUIMenuBar::toggleToolbarButton(const String& name, bool on)
	{
		for (UINT32 i = 0; i < (UINT32)mToolbarElements.size(); i++)
		{
			GUIToolBarData& element = mToolbarElements[i];

			if (element.name == name)
			{
				if (element.button != nullptr)
					element.button->_setOn(on);
			}
		}
	}

	void GUIMenuBar::registerShortcut(const String& path, const ShortcutKey& shortcut, std::function<void()> callback)
	{
		ShortcutManager::instance().addShortcut(shortcut, callback);

		String trimmedPath = path;
		StringUtil::trim(trimmedPath, "/\\", false, true);

		mEntryShortcuts[trimmedPath] = shortcut;
	}

	void GUIMenuBar::unregisterShortcut(const String& path)
	{
		String trimmedPath = path;
		StringUtil::trim(trimmedPath, "/\\", false, true);

		auto findIter = mEntryShortcuts.find(trimmedPath);
		if (findIter != mEntryShortcuts.end())
		{
			ShortcutManager::instance().removeShortcut(findIter->second);
			mEntryShortcuts.erase(findIter);
		}
	}

	bool GUIMenuBar::stripPath(String& path, String& pathRoot) const
	{
		Vector<String> pathElements = StringUtil::split(path, "/");
		if(pathElements.size() == 0)
			return false;

		pathRoot = pathElements[0];
		path.erase(0, pathRoot.size());

		if(path.size() > 0)
			path.erase(0, 1); // Forward slash

		return true;
	}

	void GUIMenuBar::openSubMenu(const String& name)
	{
		const GUIMenuBarData* subMenu = getSubMenu(name);
		if(subMenu == nullptr)
			return;

		if(mSubMenuOpen)
		{
			bool closingExisting = subMenu->button == mSubMenuButton;

			closeSubMenu();

			if(closingExisting)
				return;
		}

		GUIWidget* widget = subMenu->button->_getParentWidget();

		DROP_DOWN_BOX_DESC desc;
		desc.camera = widget->getCamera();
		desc.skin = widget->getSkinResource();
		desc.placement = DropDownAreaPlacement::aroundBoundsHorz(subMenu->button->_getLayoutData().area);
		desc.dropDownData = subMenu->menu->getDropDownData();

		for (auto& childMenu : mChildMenus)
		{
			Rect2I bounds = childMenu.button->getBounds();
			desc.additionalBounds.push_back(bounds);
		}
		
		GameObjectHandle<GUIDropDownMenu> dropDownBox = GUIDropDownBoxManager::instance().openDropDownBox(
			desc, GUIDropDownType::MenuBar, std::bind(&GUIMenuBar::onSubMenuClosed, this));

		subMenu->button->_setOn(true);

		mSubMenuButton = subMenu->button;
		mSubMenuOpen = true;
	}

	void GUIMenuBar::closeSubMenu()
	{
		if(mSubMenuOpen)
		{
			GUIDropDownBoxManager::instance().closeDropDownBox();

			mSubMenuButton->_setOn(false);
			mSubMenuOpen = false;
		}		
	}

	void GUIMenuBar::onSubMenuHover(const String& name)
	{
		if(mSubMenuOpen)
		{
			const GUIMenuBarData* subMenu = getSubMenu(name);

			if(subMenu == nullptr)
				return;

			if(mSubMenuButton != subMenu->button)
				openSubMenu(name);
		}
	}

	void GUIMenuBar::onSubMenuClosed()
	{
		mSubMenuButton->_setOn(false);
		mSubMenuOpen = false;
	}

	void GUIMenuBar::onMinimizeClicked()
	{
		mParentWindow->minimize();
	}

	void GUIMenuBar::onMaximizeClicked()
	{
		if(mParentWindow->getProperties().isMaximized)
			mParentWindow->restore();
		else
			mParentWindow->maximize();
	}

	void GUIMenuBar::onCloseClicked()
	{
		gCoreApplication().quitRequested();
	}

	void GUIMenuBar::refreshNonClientAreas()
	{
		Rect2I mainArea = mMenuItemLayout->getBounds();

		Vector<Rect2I> nonClientAreas;
		nonClientAreas.push_back(mLogoTexture->getBounds());

		UINT32 menuWidth = 0;
		if(mChildMenus.size() > 0)
		{
			Rect2I lastButtonBounds = mChildMenus.back().button->getBounds();
			Rect2I minButtonBounds = mMinBtn->getBounds();
			menuWidth = lastButtonBounds.x + lastButtonBounds.width;

			Rect2I emptyArea(menuWidth, mainArea.y,
				minButtonBounds.x - (lastButtonBounds.x + lastButtonBounds.width), mainArea.height);

			nonClientAreas.push_back(emptyArea);
		}

		Platform::setCaptionNonClientAreas(*mParentWindow->getCore(), nonClientAreas);
	}
}