//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIFieldBase.h"
#include "Image/BsColorGradient.h"

namespace bs
{
	class GUIColorGradient;
	class GUIColorGradientHDR;

	/**
	 * @addtogroup Implementation
	 * @{
	 */

	/** Templated class for creating GUI fields for both normal and HDR color gradients. */
	template<class T, class TGUI, class TSELF>
	class BS_ED_EXPORT TGUIColorGradientField : public TGUIField<TSELF>
	{
   protected:
      using PrivatelyConstruct = typename TGUIField<TSELF>::PrivatelyConstruct;
	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/** Style type name for the internal color gradient field. */
		static constexpr const char* GRADIENT_FIELD_STYLE_TYPE = "GradientField";

      TGUIColorGradientField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
			const String& style, const GUIDimensions& dimensions, bool withLabel);

		/**	Returns the value of the field. */
		BS_SCRIPT_EXPORT(pr:getter,n:Value)
		T getValue() const { return mValue; }

		/**	Changes the value of the field. */
		BS_SCRIPT_EXPORT(pr:setter,n:Value)
		void setValue(const T& value);

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		BS_SCRIPT_EXPORT(in:true)
		Event<void()> onClicked; /**< Triggered when the user clicks on the gradient field. */

		/** @name Internal
		 *  @{
		 */

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */

	protected:
		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		/**	Triggered when the child color input field is clicked on. */
		void clicked();

		UINT32 mLabelWidth = 100;
		T mValue;
		GUILabel* mLabel = nullptr;
		TGUI* mGradient = nullptr;
	};

	/** @} */

	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays a color gradient input field.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) 
	GUIColorGradientField final : public TGUIColorGradientField<ColorGradient, GUIColorGradient, GUIColorGradientField>
	{
	public:
		using TGUIColorGradientField<ColorGradient, GUIColorGradient, GUIColorGradientField>::TGUIColorGradientField;
	};

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays a color gradient input field with support for HDR colors.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) 
	GUIColorGradientHDRField final : public TGUIColorGradientField<ColorGradientHDR, GUIColorGradientHDR, GUIColorGradientHDRField>
	{
	public:
      using TGUIColorGradientField<ColorGradientHDR, GUIColorGradientHDR, GUIColorGradientHDRField>::TGUIColorGradientField;
	};

	/** @} */
}
