//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Utility/BsModule.h"

namespace bs
{
	/** @addtogroup Scene-Editor
	 *  @{
	 */

	/**
	 * Handles scene object and resource selection. Triggeres events when selection changes and allows the user to query
	 * current selection state.
	 */
	class BS_ED_EXPORT Selection : public Module<Selection>
	{
	public:
		Selection();
		~Selection();

		/**	Returns a currently selected set of scene objects. */
		const Vector<HSceneObject>& getSceneObjects() const;

		/**	Sets a new set of scene objects to select, replacing the old ones. */
		void setSceneObjects(const Vector<HSceneObject>& sceneObjects);

		/**	Returns a currently selected set of resource paths. */
		const Vector<Path>& getResourcePaths() const;

		/**	Sets a new set of resource paths to select, replacing the old ones. */
		void setResourcePaths(const Vector<Path>& paths);

		/**	Returns a currently selected set of resource UUIDs. */
		Vector<UUID> getResourceUUIDs() const;

		/**	Sets a new set of resource UUIDs to select, replacing the old ones. */
		void setResourceUUIDs(const Vector<UUID>& UUIDs);

		/**	Deselects all currently selected scene objects. */
		void clearSceneSelection();

		/**	Deselects all currently selected resources. */
		void clearResourceSelection();

		/**	Pings the scene object, highlighting it in its respective editors. */
		void ping(const HSceneObject& sceneObject);

		/**
		 * Pings the resource, highlighting it in its respective editors.
		 * 		
		 * @param[in]	resourcePath	Resource path relative to the project library resources folder.
		 */
		void ping(const Path& resourcePath);

		/** Triggered when one or multiple scene objects is being added to the selection. */
		Event<void(const Vector<HSceneObject>&)> onSceneObjectsAdded;

		/** Triggered when one or multiple resources are being added to the selection. */
		Event<void(const Vector<Path>&)> onResourcesAdded;

		/** Triggered when one or multiple scene objects is being removed from the selection. */
		Event<void(const Vector<HSceneObject>&)> onSceneObjectsRemoved;

		/** Triggered when one or multiple resources are being removed from the selection. */
		Event<void(const Vector<Path>&)> onResourcesRemoved;

		/**
		 * Triggered whenever scene object or resource selection changes. The provided parameters will contain the newly
		 * selected objects/resource paths.
		 */
		Event<void(const Vector<HSceneObject>&, const Vector<Path>&)> onSelectionChanged; 

		/**
		 * Triggered when a scene object ping is requested. Ping usually means the object will be highlighted in its
		 * respective editors.
		 */
		Event<void(const HSceneObject&)> onSceneObjectPing; 

		/**
		 * Triggered when a resource ping is requested. Ping usually means the object will be highlighted in its respective
		 * editors.
		 */
		Event<void(const Path&)> onResourcePing; 
	private:
		/**	Triggered when the scene object selection in the scene tree view changes. */
		void sceneSelectionChanged();

		/**	Triggered when the resource selection in the resource tree view changes. */
		void resourceSelectionChanged();

		/**	Updates scene and resource tree views with new selection. */
		void updateTreeViews();

		/** Removes any destroyed scene objects from the provided scene object list. */
		void pruneDestroyedSceneObjects(Vector<HSceneObject>& sceneObjects) const;

		mutable Vector<HSceneObject> mSelectedSceneObjects;
		Vector<Path> mSelectedResourcePaths;

		HMessage mSceneSelectionChangedConn;
		HMessage mResourceSelectionChangedConn;

		mutable Vector<HSceneObject> mTempPrune;
		mutable Vector<HSceneObject> mTempSceneObjects;
		mutable Vector<Path> mTempResources;
	};

	/** @} */
}