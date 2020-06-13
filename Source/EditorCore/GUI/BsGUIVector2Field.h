//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIFieldBase.h"
#include "Math/BsVector2.h"

namespace bs
{
	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays a Vector2 input field.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed)
	GUIVector2Field final : public TGUIField<GUIVector2Field>
	{
	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/** Style type name for the internal X input box. */
		static constexpr const char* X_FIELD_STYLE_TYPE = "XFloatField";

		/** Style type name for the internal Y input box. */
		static constexpr const char* Y_FIELD_STYLE_TYPE = "YFloatField";

		GUIVector2Field(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
			const String& style, const GUIDimensions& dimensions, bool withLabel);

		/**	Returns the value of the input field. */
		BS_SCRIPT_EXPORT(pr:getter,n:Value)
		Vector2 getValue() const;

		/**	Sets a new value in the input field. */
		BS_SCRIPT_EXPORT(pr:setter,n:Value)
		void setValue(const Vector2& value);

		/**	Checks is the input field currently active. */
		BS_SCRIPT_EXPORT(pr:getter,n:HasInputFocus)
		bool hasInputFocus() const;

		/** Sets input focus to a specific component's input box. */
		BS_SCRIPT_EXPORT()
		void setInputFocus(VectorComponent component, bool focus);

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		/** Reports the new value of the vector when the user changes the value of any of the vector components. */
		BS_SCRIPT_EXPORT()
		Event<void(const Vector2&)> onValueChanged;

		/** Reports the new value of an individual vector component when the user changes it. */
		BS_SCRIPT_EXPORT()
		Event<void(float, VectorComponent)> onComponentChanged;

		/**	Triggered when an individual component loses or gains focus. */
		BS_SCRIPT_EXPORT()
		Event<void(bool, VectorComponent)> onComponentFocusChanged;

		/** Triggered when the user hits the Enter key with any of the component input boxes in focus. */
		BS_SCRIPT_EXPORT()
		Event<void(VectorComponent)> onConfirm;
	protected:
		static const UINT32 ELEMENT_LABEL_WIDTH;

		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		/**	Triggered when the values in either of the input boxes change. */
		void valueChanged(float newValue, VectorComponent component);

		/**	Triggered when the users confirms input in the input box. */
		void inputConfirmed(VectorComponent component);

		GUIFloatField* mFieldX = nullptr;
		GUIFloatField* mFieldY = nullptr;
	};

	/** @} */
}