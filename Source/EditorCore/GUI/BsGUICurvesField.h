//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIFieldBase.h"
#include "GUI/BsGUICurves.h"
#include "Animation/BsAnimationCurve.h"

namespace bs
{
	class GUICurves;

	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays an animation curve or a range between two animation curves.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) GUICurvesField final : public TGUIField<GUICurvesField>
	{
	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/** Style type name for the internal curve field. */
		static const String& getCurveStyleType();

		GUICurvesField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
			const String& style, const GUIDimensions& dimensions, bool withLabel, 
			CurveDrawOptions drawOptions = CurveDrawOptions());

		/** Sets an animation curve to display on the field. */
		BS_SCRIPT_EXPORT() 
		void setCurve(const TAnimationCurve<float>& curve);

		/** Sets a set of animation curves and displays the difference (range) between them. */
		BS_SCRIPT_EXPORT()
		void setCurveRange(const TAnimationCurve<float>& curveA, const TAnimationCurve<float>& curveB);

		/** 
		 * Returns the curve represented by the field. If the field represents a curve range this returns the minimal
		 * curve of that range.
		 */
		BS_SCRIPT_EXPORT(pr:getter,n:Curve)
		const TAnimationCurve<float>& getCurve() const;

		/** 
		 * Returns the minimal curve represented by the field containing a curve range. Returns the only available
		 * curve if the field doesn't represent a range.
		 */
		BS_SCRIPT_EXPORT(pr:getter,n:MinCurve)
		const TAnimationCurve<float>& getMinCurve() const;

		/** 
		 * Returns the maximal curve represented by the field containing a curve range. Returns the only available
		 * curve if the field doesn't represent a range.
		 */
		BS_SCRIPT_EXPORT(pr:getter,n:MaxCurve)
		const TAnimationCurve<float>& getMaxCurve() const;

		/** @copydoc GUICurves::setRange */
		BS_SCRIPT_EXPORT()
		void setRange(float xRange, float yRange) { mCurves->setRange(xRange, yRange); }

		/** @copydoc GUICurves::setOffset */
		BS_SCRIPT_EXPORT()
		void setOffset(const Vector2& offset) { mCurves->setOffset(offset); }

		/** @copydoc GUICurves::centerAndZoom */
		BS_SCRIPT_EXPORT()
		void centerAndZoom() { mCurves->centerAndZoom(); }

		/** @copydoc GUICurves::setPadding */
		BS_SCRIPT_EXPORT(pr:setter,n:Padding)
		void setPadding(UINT32 padding) { mCurves->setPadding(padding); }

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		BS_SCRIPT_EXPORT(in:true) 
		Event<void()> onClicked; /**< Triggered when the user clicks on the GUI element. */

		using TGUIField::create;

		/**
		 * Creates a new GUI editor field with a label.
		 *
		 * @param[in]	drawOptions		Options that control which additional curve elements to draw.
		 * @param[in]	labelContent	Content to display in the editor field label.
		 * @param[in]	labelWidth		Width of the label in pixels.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		BS_SCRIPT_EXPORT(ec:GUICurvesField)
		static GUICurvesField* create(CurveDrawOptions drawOptions, const GUIContent& labelContent, UINT32 labelWidth,
			 const String& style = StringUtil::BLANK)
		{
			const String* curStyle = &style;
			if (*curStyle == StringUtil::BLANK)
				curStyle = &GUICurvesField::getGUITypeName();

			return bs_new<GUICurvesField>(PrivatelyConstruct(), labelContent, labelWidth, *curStyle, 
				GUIDimensions::create(), true, drawOptions);
		}

		/**
		 * Creates a new GUI editor field with a label.
		 *
		 * @param[in]	drawOptions		Options that control which additional curve elements to draw.
		 * @param[in]	labelText		String to display in the editor field label.
		 * @param[in]	labelWidth		Width of the label in pixels.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		BS_SCRIPT_EXPORT(ec:GUICurvesField)
		static GUICurvesField* create(CurveDrawOptions drawOptions, const HString& labelText, UINT32 labelWidth,
			const String& style = StringUtil::BLANK)
		{
			const String* curStyle = &style;
			if (*curStyle == StringUtil::BLANK)
				curStyle = &GUICurvesField::getGUITypeName();

			return bs_new<GUICurvesField>(PrivatelyConstruct(), GUIContent(labelText), labelWidth, *curStyle,
				GUIDimensions::create(), true, drawOptions);
		}

		/**
		 * Creates a new GUI editor field without a label.
		 *
		 * @param[in]	drawOptions		Options that control which additional curve elements to draw.
		 * @param[in]	style			Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		BS_SCRIPT_EXPORT(ec:GUICurvesField)
		static GUICurvesField* create(CurveDrawOptions drawOptions, const String& style = StringUtil::BLANK)
		{
			const String* curStyle = &style;
			if (*curStyle == StringUtil::BLANK)
				curStyle = &GUICurvesField::getGUITypeName();

			return bs_new<GUICurvesField>(PrivatelyConstruct(), GUIContent(), 0, *curStyle, 
				GUIDimensions::create(), false, drawOptions);
		}

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

		GUICurves* mCurves = nullptr;
	};

	/** @} */
}
