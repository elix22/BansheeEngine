//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIOptions.h"
#include "GUI/BsGUIContent.h"

namespace bs
{
	/** @addtogroup GUI-Editor
	*  @{
	*/

	/** Provides common options for initializing a GUI element used for editor fields. */
	struct BS_ED_EXPORT GUIFieldOptions
	{
		/**
		 * Constructor for field options with a label.
		 *
		 * @param[in]	labelContent	Content to display in the editor field label.
		 * @param[in]	labelWidth		Width of the label in pixels.
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const GUIContent& labelContent, UINT32 labelWidth, const GUIOptions& options,
			const String& style = StringUtil::BLANK)
			:labelContent(labelContent), labelWidth(labelWidth), options(options), style(style)
		{ }

		/**
		 * Constructor for field options with a label.
		 *
		 * @param[in]	labelContent	Content to display in the editor field label.
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const GUIContent& labelContent, const GUIOptions& options,
			const String& style = StringUtil::BLANK)
			:labelContent(labelContent), options(options), style(style)
		{ }


		/**
		 * Constructor for field options with a label.
		 *
		 * @param[in]	labelText		Text to display in the editor field label.
		 * @param[in]	labelWidth		Width of the label in pixels.
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const HString& labelText, UINT32 labelWidth, const GUIOptions& options,
			const String& style = StringUtil::BLANK)
			:labelContent(labelText), labelWidth(labelWidth), options(options), style(style)
		{ }


		/**
		 * Constructor for field options with a label.
		 *
		 * @param[in]	labelText		Text to display in the editor field label.
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const HString& labelText, const GUIOptions& options,
			const String& style = StringUtil::BLANK)
			:labelContent(labelText), options(options), style(style)
		{ }


		/**
		 * Constructor for field options without a label.
		 *
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const GUIOptions& options, const String& style = StringUtil::BLANK)
			:labelWidth(0), options(options), style(style)
		{ }

		/**
		 * Constructor.
		 *
		 * @param[in]	labelContent	Content to display in the editor field label.
		 * @param[in]	labelWidth		Width of the label in pixels.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const GUIContent& labelContent, UINT32 labelWidth, const String& style = StringUtil::BLANK)
			:labelContent(labelContent), labelWidth(labelWidth), style(style)
		{ }

		/**
		 * Constructor for field options with a label.
		 *
		 * @param[in]	labelContent	Content to display in the editor field label.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const GUIContent& labelContent, const String& style = StringUtil::BLANK)
			:labelContent(labelContent), style(style)
		{ }

		/**
		 * Constructor for field options with a label.
		 *
		 * @param[in]	labelText		Text to display in the editor field label.
		 * @param[in]	labelWidth		Width of the label in pixels.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const HString& labelText, UINT32 labelWidth, const String& style = StringUtil::BLANK)
			:labelContent(labelText), labelWidth(labelWidth), style(style)
		{ }

		/**
		 * Constructor for field options with a label.
		 *
		 * @param[in]	labelText		Text to display in the editor field label.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const HString& labelText, const String& style = StringUtil::BLANK)
			:labelContent(labelText), style(style)
		{ }

		/**
		 * Constructor for field options without a label.
		 *
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		GUIFieldOptions(const String& style = StringUtil::BLANK)
			:labelWidth(0), style(style)
		{ }

		GUIContent labelContent;
		UINT32 labelWidth = 100;
		GUIOptions options;
		String style;
	};

	/** @} */
}
