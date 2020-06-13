//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIFieldBase.h"
#include "Image/BsColor.h"

namespace bs
{
	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays a color input field.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed)
	GUIColorField : public TGUIField<GUIColorField>
	{
	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/** Style type name for the internal color field. */
		static const String& getColorInputStyleType();

		GUIColorField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
			const String& style, const GUIDimensions& dimensions, bool withLabel);

		/**	Returns the value of the field. */
		BS_SCRIPT_EXPORT(pr:getter,n:Value)
		Color getValue() const { return mValue; }

		/**	Changes the value of the field. */
		BS_SCRIPT_EXPORT(pr:setter,n:Value)
		void setValue(const Color& value);

		/** @copydoc setAllowHDR */
		BS_SCRIPT_EXPORT(pr:getter,n:AllowHDR)
		bool getAllowHDR() const { return mAllowHDR; }

		/** Determines if the color assigned to the field is allowed to have values outside of [0-1] range. */
		BS_SCRIPT_EXPORT(pr:setter,n:AllowHDR)
		void setAllowHDR(bool allow) { mAllowHDR = allow; }

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		BS_SCRIPT_EXPORT(in:true)
		Event<void()> onClicked; /**< Triggered when the user clicks on the GUI element. */

		/** @name Internal 
		 *  @{
		 */

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */

	protected:
		virtual ~GUIColorField() = default;

		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		/**	Triggered when the child color input field is clicked on. */
		void clicked();

		UINT32 mLabelWidth = 100;
		Color mValue;
		GUILabel* mLabel = nullptr;
		GUIColor* mColor = nullptr;
		bool mAllowHDR = false;
	};

	/** @} */
}