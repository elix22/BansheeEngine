//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUIFieldBase.h"
#include "Image/BsColorGradient.h"
#include "Particles/BsParticleDistribution.h"

namespace bs
{
	class GUICurves;

	/** @addtogroup GUI-Editor
	 *  @{
	 */

	/**
	 * A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field.
	 * Label is optional. This specific implementation displays an input field for a color distribution.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:GUIEditor,api:bed) 
	GUIColorDistributionField final : public TGUIField<GUIColorDistributionField>
	{
	public:
		/** Returns type name of the GUI element used for finding GUI element styles. */
		static const String& getGUITypeName();

		GUIColorDistributionField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, UINT32 labelWidth,
			const String& style, const GUIDimensions& dimensions, bool withLabel);

		/**	Returns the value of the field. */
		BS_SCRIPT_EXPORT(pr:getter,n:Value)
		ColorDistribution getValue() const { return mValue; }

		/**	Changes the value of the field. */
		BS_SCRIPT_EXPORT(pr:setter,n:Value)
		void setValue(const ColorDistribution& value);

		/** Returns the type of the currently selected distribution. */
		BS_SCRIPT_EXPORT(pr:getter,n:DistributionType)
		PropertyDistributionType getType() const { return mPropertyType; }

		/** @copydoc GUIElement::setTint */
		void setTint(const Color& color) override;

		/** Triggered when the user clicks on the minimum value display for the distribution. */
		BS_SCRIPT_EXPORT(in:true)
		Event<void()> onMinClicked;

		/** Triggered when the user clicks on the maximum value display for the distribution. */
		BS_SCRIPT_EXPORT(in:true)
		Event<void()> onMaxClicked;

		/** @name Internal 
		 *  @{
		 */

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */

	protected:
		/** @copydoc GUIElement::styleUpdated */
		void styleUpdated() override;

		/** Rebuilds the internal GUI components for the current property type. */
		void rebuild();

		ColorDistribution mValue = Color::White;
		GUIButton* mDropDownButton = nullptr;
		GUIColorField* mMinColorField = nullptr;
		GUIColorField* mMaxColorField = nullptr;
		GUIColorGradientField* mMinGradientField = nullptr;
		GUIColorGradientField* mMaxGradientField = nullptr;
		std::array<GUILabel*, 2> mLabels = { nullptr, nullptr };
		PropertyDistributionType mPropertyType = PDT_Constant;

		Color mMinConstant = Color::White;
		Color mMaxConstant = Color::White;
		ColorGradient mMinGradient;
		ColorGradient mMaxGradient;

		SPtr<GUIContextMenu> mContextMenu;
	};

	/** @} */
}
