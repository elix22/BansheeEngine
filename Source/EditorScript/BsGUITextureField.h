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

	/** Type that control what type of texture does a GUITexture field accept. */
	enum class BS_SCRIPT_EXPORT(m:GUI-Editor,api:bed) GUITextureFieldType
	{
		Texture = 1,
		SpriteTexture = 2,
		TextureOrSpriteTexture = 3,
	};

	/**
	 * GUI object that displays a field in which a Texture or a SpriteTexture can be dragged and dropped. It also displays
	 * an optional label field. When a texture is referenced its image is displayed in the field.
	 */
	class BS_SCR_BED_EXPORT GUITextureField : public GUIElementContainer
	{
		struct PrivatelyConstruct {};

	public:
		GUITextureField(const PrivatelyConstruct& dummy, GUITextureFieldType type, const GUIContent& labelContent,
			UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel);

		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		/**
		 * Creates a new texture GUI editor field.
		 *
		 * @param[in]	type			Determines the type of texture the field should accept.
		 * @param[in]	options			Options controlling the label, style and layout of the field.
		 */
		static GUITextureField* create(GUITextureFieldType type, const GUIFieldOptions& options = GUIFieldOptions());

		/**
		 * Creates a new texture GUI editor field that accepts a Texture.
		 *
		 * @param[in]	options			Options controlling the label, style and layout of the field.
		 */
		static GUITextureField* create(const GUIFieldOptions& options = GUIFieldOptions());

		/** 
		 * Returns the texture referenced by the field, if any. Returns null if no texture is assigned, or if sprite 
		 * texture is assigned.
		 */
		HTexture getTexture() const;

		/**	Sets the texture referenced by the field. */
		void setTexture(const HTexture& value);

		/** 
		 * Returns the sprite texture referenced by the field, if any. Returns null if no sprite texture is assigned, or
		 * if non-sprite texture is assigned.
		 */
		HSpriteTexture getSpriteTexture() const;

		/**	Sets the sprite texture referenced by the field. */
		void setSpriteTexture(const HSpriteTexture& value);

		/** Returns the normal or sprite texture referenced by the field, as a generic resource handle. */
		HResource getValue() const;

		/**	Sets the resource referenced by the field. */
		void setValue(const HResource& value);

		/**	Returns the UUID of the resource referenced by the field. */
		UUID getUUID() const { return mUUID; }

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		/** 
		 * @name Internal
		 * @{
		 */

		/** @copydoc GUIElement::_updateLayoutInternal */
		void _updateLayoutInternal(const GUILayoutData& data) override;

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */

		/** Triggered whenever the referenced texture changes. */
		Event<void(const HResource&)> onValueChanged;
	private:
		virtual ~GUITextureField() = default;

		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		/**
		 * Sets the texture referenced by the field by finding the texture with the provided UUID.
		 *			
		 * @param[in]	uuid			Unique resource identifier of the texture to show, or empty string if no texture.
		 * @param[in]	triggerEvent	Determines should the onValueChanged() event be triggered if the new UUID is
		 *								different from the previous one.
		 */
		void setUUID(const UUID& uuid, bool triggerEvent = true);

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
		UUID mUUID;
	};

	/** @} */
}
