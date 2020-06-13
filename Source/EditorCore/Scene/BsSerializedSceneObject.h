//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Utility/BsEditorUtility.h"
#include "FileSystem/BsDataStream.h"

namespace bs
{
	/** @addtogroup Utility-Editor
	*  @{
	*/

	/**
	 * Serializes the current state of a scene object and allows that state to be restored. The advantage of using this 
	 * class versus normal serialization is that the deserialization happens into the original scene object, instead of
	 * creating a new scene object.
	 */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(m:Utility-Editor,api:bed) SerializedSceneObject final
	{
	public:
		/**
		 * Serializes the current state of the provided scene object.
		 *
		 * @param[in]	sceneObject		Object whose state to serialize.
		 * @param[in]	hierarchy		If true all children of the provided scene object will be serialized as well,
		 *								otherwise just the provided object will.
		 */
		BS_SCRIPT_EXPORT()
		SerializedSceneObject(const HSceneObject& sceneObject, bool hierarchy = false);

		/**
		 * Restores the scene object to the state as it was when this object was created. If the scene object was deleted
		 * since it will be resurrected.
		 */
		BS_SCRIPT_EXPORT()
		void restore();

	private:
		friend class UndoRedo;

		HSceneObject mSceneObject;
		EditorUtility::SceneObjProxy mSceneObjectProxy;
		bool mRecordHierarchy;

		SPtr<MemoryDataStream> mSerializedObject;
		UINT64 mSerializedObjectParentId = 0;
	};

	/** @} */
}
