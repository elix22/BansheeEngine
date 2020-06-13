//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsScriptEditorPrerequisites.h"
#include "Utility/BsModule.h"

namespace bs
{
	/** @addtogroup EditorScript
	 *  @{
	 */

	/**	Available flags to be used when defining gizmos. */
	enum class DrawGizmoFlags // Note: Must match the C# enum DrawGizmoFlags
	{
		Selected = 0x01, /**< Gizmo is only displayed when its scene object is selected. */
		ParentSelected = 0x02, /**< Gizmo is only displayed when its parent scene object is selected. */
		NotSelected = 0x04, /**< Gizmo is only displayed when its scene object is not selected. */
		Pickable = 0x08 /**< Gizmo can be clicked on in scene view, which will select its scene object. */
	};

	/** 
	 * Manages all active managed gizmo methods. Finds all gizmos methods in loaded assemblies, and calls them every frame. 
	 */
	class BS_SCR_BED_EXPORT ScriptGizmoManager : public Module<ScriptGizmoManager>
	{
		/**	Data about a managed gizmo drawing method. */
		struct GizmoData
		{
			MonoClass* type; /**< Component the gizmo method belongs to. */
			MonoMethod* method; /**< Method that displays the gizmo. */
			UINT32 flags; /**< Gizmo flags of type DrawGizmoFlags that control gizmo properties. */
		};

		/** Data about a managed selection changed callback method. */
		struct SelectionChangedData
		{
			MonoClass* type; /**< Component or resource the selection method should trigger on. */
			MonoMethod* method; /**< Method that receives the selection changed callback. */
		};

	public:
		ScriptGizmoManager(ScriptAssemblyManager& scriptObjectManager);
		~ScriptGizmoManager();

		/**
		 * Iterates over all managed gizmos, calls their draw methods and registers the gizmos with the native GizmoManager.
		 */
		void update();

	private:
		/**	Finds all gizmo methods (marked with the DrawGizmo attribute). Clears any previously found methods. */
		void reloadAssemblyData();

		/**
		 * Triggered when entries are added or removed from the selection.
		 *
		 * @param[in]	sceneObjects	Newly selected or deselected scene objects. 
		 * @param[in]	added			If true, the provided objects were added to the selection. If false, the provided
		 *								objects were removed from the selection.
		 */
		void onSOSelectionChanged(const Vector<HSceneObject>& sceneObjects, bool added);

		/**
		 * Checks is the provided method a valid gizmo draw method and if it is, returns properties of that method.
		 *
		 * @param[in]	method			Method to check.
		 * @param[in]	componentType	Output parameter containing the component the method is part of. Only valid if this
		 *								method returns true.
		 * @param[in]	drawGizmoFlags	Output parameters containing optional flags that control gizmo properties. Only
		 *								valid if this method returns true.
		 * @return						True if the method is a valid draw gizmo method.
		 */
		bool isValidDrawGizmoMethod(MonoMethod* method, MonoClass*& componentType, UINT32& drawGizmoFlags);

		/**
		 * Checks is the provided method a valid selection changed callback method.
		 *
		 * @param[in]	method			Method to check.
		 * @param[in]	componentType	Output parameter containing the component the method is part of. Only valid if this
		 *								method returns true.
		 * @return						True if the method is a valid selection changed callback method.
		 */
		bool isValidOnSelectionChangedMethod(MonoMethod* method, MonoClass*& componentType);

		ScriptAssemblyManager& mScriptObjectManager;
		HEvent mDomainLoadedConn;
		HEvent mSelectionSOAddedConn;
		HEvent mSelectionSORemovedConn;

		MonoClass* mDrawGizmoAttribute = nullptr;
		MonoField* mFlagsField = nullptr;
		MonoClass* mOnSelectionChangedAttribute = nullptr;
		UnorderedMap<String, SmallVector<GizmoData, 2>> mGizmoDrawers;
		Map<String, SelectionChangedData> mSelectionChangedCallbacks;
	};

	/** @} */
}