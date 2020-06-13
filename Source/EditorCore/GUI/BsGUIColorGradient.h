//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIElement.h"
#include "2D/BsImageSprite.h"
#include "GUI/BsGUIContent.h"
#include "Image/BsColorGradient.h"

namespace bs
{
	/** 
	 * @addtogroup Implementation
	 * @{
	 */

	/** Templated class used for representing both normal and HDR variations of the color gradient GUI element. */
	template<class T, class SELF>
	class BS_ED_EXPORT TGUIColorGradient : public GUIElement
	{
	public:
		TGUIColorGradient(const String& styleName, const GUIDimensions& dimensions);

		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/**
		 * Creates a new GUI color gradient element.
		 *
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		BS_SCRIPT_EXPORT(ec:T)
		static SELF* create(const String& styleName = StringUtil::BLANK);

		/**
		 * Creates a new GUI color gradient element.
		 *
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		static SELF* create(const GUIOptions& options, const String& styleName = StringUtil::BLANK);

		/**	Color gradient to display. */
		BS_SCRIPT_EXPORT(pr:setter,n:Gradient)
		void setGradient(const T& colorGradient);

		/** @copydoc setGradient */
		BS_SCRIPT_EXPORT(pr:getter,n:Gradient)
		T getGradient() const { return mValue; }

		BS_SCRIPT_EXPORT(in:true)
		Event<void()> onClicked; /**< Triggered when the user clicks on the GUI element. */

		/** @name Internal
		 *  @{
		 */

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */
	protected:
		virtual ~TGUIColorGradient();

		/** @copydoc GUIElement::_fillBuffer() */
		void _fillBuffer(UINT8* vertices, UINT32* indices, UINT32 vertexOffset, UINT32 indexOffset, 
			const Vector2I& offset, UINT32 maxNumVerts, UINT32 maxNumIndices, UINT32 renderElementIdx) const override;

		/** @copydoc GUIElement::updateRenderElementsInternal() */
		void updateRenderElementsInternal() override;

		/** @copydoc GUIElement::_mouseEvent() */
		bool _mouseEvent(const GUIMouseEvent& ev) override;

		/** 
		 * Generates a texture from the provided color gradient value. 
		 * 
		 * @param[in]	gradient	Gradient to generate the texture from.
		 * @param[in]	width		Width of the texture in pixels. Height is always 1.
		 * @param[in]	alpha		If true generate a texture representing the alpha values of the gradient (grayscale),
		 *							otherwise generate a color texture.
		 * @return					Generated texture.
		 */
		static HTexture generateGradientTexture(const T& gradient, UINT32 width, bool alpha = false);

	private:
		static const float ALPHA_SPLIT_POSITION;

		ImageSprite* mColorSprite = nullptr;
		ImageSprite* mAlphaSprite = nullptr;

		IMAGE_SPRITE_DESC mColorImageDesc;
		IMAGE_SPRITE_DESC mAlphaImageDesc;

		T mValue;
	};

	/** @} */

	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/** GUI element that displays a color gradient. */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) GUIColorGradient : public TGUIColorGradient<ColorGradient, GUIColorGradient>
	{
		using TGUIColorGradient::TGUIColorGradient;
	};

	/** GUI element that displays a color gradient with HDR color support. */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) GUIColorGradientHDR : public TGUIColorGradient<ColorGradientHDR, GUIColorGradientHDR>
	{
		using TGUIColorGradient::TGUIColorGradient;
	};

	/** @} */
}