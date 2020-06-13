//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "GUI/BsGUIElementContainer.h"
#include "GUI/BsGUIFieldOptions.h"

namespace bs
{
	/** @addtogroup EditorScript
	 *  @{
	 */

	/**
	 * GUI object that displays a field in which a Resource can be dragged and dropped. The field accepts a Resource of a
	 * specific type and displays an optional label.
	 */
	class BS_SCR_BED_EXPORT GUIResourceField : public GUIElementContainer
	{
		struct PrivatelyConstruct {};

	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/**
		 * Creates a new resource GUI editor field.
		 *
		 * @param[in]	typeNamespace	Namespace of the type this field accepts. 
		 * @param[in]	type			Type name of the type this field accepts. Must derive from Resource.
		 * @param[in]	options			Options controlling the label, style and layout of the field.
		 */
		static GUIResourceField* create(const String& typeNamespace, const String& type, const GUIFieldOptions& options); 

		GUIResourceField(const PrivatelyConstruct& dummy, const String& typeNamespace, const String& type, 
			const GUIContent& labelContent, UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, 
			bool withLabel);

		/**	Returns the resource referenced by the field, if any. */
		HResource getValue() const;

		/**	Sets the resource referenced by the field. */
		void setValue(const HResource& value);

		/** Returns the resource referenced by the field. Returns empty string with no resource is referenced. */
		UUID getUUID() const { return mUUID; }

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		/**
		 * Triggered whenever the referenced resource changes. Provides	a weak handle of the resource, or empty handle if
		 * no resource is referenced.
		 */
		Event<void(const HResource&)> onValueChanged;

		/** @name Internal 
		 *  @{
		 */

		/** @copydoc GUIElement::_updateLayoutInternal */
		void _updateLayoutInternal(const GUILayoutData& data) override;

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */
	private:
		virtual ~GUIResourceField() = default;

		/**
		 * Sets the resource referenced by the field by finding the resource with the provided UUID.
		 *			
		 * @param[in]	uuid			Unique resource identifier of the resource to show, or empty string if no resource.
		 * @param[in]	triggerEvent	Determines should the onValueChanged() event be triggered if the new UUID is
		 *								different from the previous one.
		 */
		void setUUID(const UUID& uuid, bool triggerEvent = true);

		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		/**	Triggered when a drag and drop operation finishes over this element. */
		void dataDropped(void* data);

		/**	Triggered when the drop button that displays the game object label is clicked. */
		void onDropButtonClicked();

		/**	Triggered when the clear button is clicked. */
		void onClearButtonClicked();

	private:
		static const UINT32 DEFAULT_LABEL_WIDTH;

		GUILayout* mLayout;
		GUILabel* mLabel;
		GUIDropButton* mDropButton;
		GUIButton* mClearButton;
		String mNamespace;
		String mType;
		UUID mUUID;
	};

	/** @} */
}
