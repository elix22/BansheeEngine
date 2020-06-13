//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "EditorWindow/BsEditorWindowBase.h"

namespace bs
{
	/** @addtogroup EditorWindow
	 *  @{
	 */

	/**
	 * Base implementation of a window that when open doesn't allow you to interact with other windows. Modal windows are
	 * similar to editor windows but cannot be docked, and are meant to be used for temporary operations like dialog boxes
	 * and progress bars.
	 */
	class BS_ED_EXPORT ModalWindow : public EditorWindowBase
	{
	public:
		virtual ~ModalWindow() = default;

		/** @copydoc EditorWindowBase::update */
		void update() override;

		/** @copydoc EditorWindowBase::close */
		void close() override;

		/**	Changes the text in the modal window title bar. */
		void setTitle(const HString& title);

		/** @copydoc EditorWindowBase::setSize */
		void setSize(UINT32 width, UINT32 height) override;

		/**	
		 * Returns the width of the content area of the window, in pixels. The content area represents the area of the
		 * window not including the title bar and the border.
		 */
		UINT32 getContentWidth() const;

		/**	
		 * Returns the height of the content area of the window, in pixels. The content area represents the area of the
		 * window not including the title bar and the border.
		 */
		UINT32 getContentHeight() const;

		/** 
		 * Sets the width & height of the content area of the window, in pixels. The content area represents the area of
		 * the window not including the titlebar and the border.
		 */
		void setContentSize(UINT32 width, UINT32 height);

		/** Converts screen pointer coordinates into coordinates relative to the window content's GUI panel. */
		Vector2I screenToWindowPos(const Vector2I& screenPos) const;

		/** Converts pointer coordinates relative to the window content's GUI panel into screen coordinates. */
		Vector2I windowToScreenPos(const Vector2I& windowPos) const;

	protected:
		friend class EditorWindowManager;

		ModalWindow(const HString& title, bool hasCloseButton = false, UINT32 width = 200, UINT32 height = 200);

		/**
		 * Returns the area in which the GUI contents are displayed (not including title bar and other default 
		 * elements). Area is relative to window.
		 */
		Rect2I getContentArea() const;

		/** @copydoc EditorWindowBase::resized */
		void resized() override;

	private:
		/**
		 * Updates the placement of child GUI elements and their non-client areas (used for OS move/resize operations). 
		 * Should be called after window size changes.
		 */
		void updateSize();

		/**	Returns the height in pixels taken up by the title bar. */
		UINT32 getTitleBarHeight() const;

		GUIPanel* mTitleBarPanel = nullptr;
		GUIPanel* mTitleBarBgPanel = nullptr;

		GUILabel* mTitle = nullptr;
		GUIButton* mCloseButton = nullptr;
		GUITexture* mTitleBarBg = nullptr;

	protected:
		GUIPanel* mContents = nullptr;
	};

	/** @} */
}