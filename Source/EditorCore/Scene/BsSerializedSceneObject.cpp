//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Scene/BsSerializedSceneObject.h"
#include "Scene/BsSceneObject.h"
#include "Utility/BsUtility.h"
#include "Serialization/BsBinarySerializer.h"
#include "FileSystem/BsDataStream.h"

namespace bs
{
	class BinarySerializer;

	SerializedSceneObject::SerializedSceneObject(const HSceneObject& sceneObject, bool hierarchy)
		:mSceneObject(sceneObject), mRecordHierarchy(hierarchy), mSerializedObject(bs_shared_ptr_new<MemoryDataStream>())
	{
		if(mSceneObject.isDestroyed())
			return;

		UINT32 numChildren = mSceneObject->getNumChildren();
		HSceneObject* children = nullptr;

		if (!mRecordHierarchy)
		{
			children = bs_stack_new<HSceneObject>(numChildren);
			for (UINT32 i = 0; i < numChildren; i++)
			{
				HSceneObject child = mSceneObject->getChild(i);
				children[i] = child;

				child->setParent(HSceneObject());
			}
		}

		bool isInstantiated = !mSceneObject->hasFlag(SOF_DontInstantiate);
		mSceneObject->_setFlags(SOF_DontInstantiate);

		BinarySerializer serializer;
		serializer.encode(mSceneObject.get(), mSerializedObject);
		
		if (isInstantiated)
			mSceneObject->_unsetFlags(SOF_DontInstantiate);

		mSceneObjectProxy = EditorUtility::createProxy(mSceneObject);

		HSceneObject parent = sceneObject->getParent();
		if (parent != nullptr)
			mSerializedObjectParentId = parent->getInstanceId();

		if (!mRecordHierarchy)
		{
			for (UINT32 i = 0; i < numChildren; i++)
				children[i]->setParent(sceneObject->getHandle());

			bs_stack_delete(children, numChildren);
		}
	}

	void SerializedSceneObject::restore()
	{
		HSceneObject parent;
		if (mSerializedObjectParentId != 0)
			parent = static_object_cast<SceneObject>(GameObjectManager::instance().getObject(mSerializedObjectParentId));

		HSceneObject* children = nullptr;
		UINT32 numChildren = 0;
		if (!mSceneObject.isDestroyed())
		{
			numChildren = mSceneObject->getNumChildren();
			if (!mRecordHierarchy)
			{
				children = bs_stack_new<HSceneObject>(numChildren);
				for (UINT32 i = 0; i < numChildren; i++)
				{
					HSceneObject child = mSceneObject->getChild(i);
					children[i] = child;

					child->setParent(HSceneObject());
				}
			}

			mSceneObject->destroy(true);
		}

		CoreSerializationContext serzContext;
		serzContext.goState = bs_shared_ptr_new<GameObjectDeserializationState>(GODM_RestoreExternal | GODM_UseNewIds);

		BinarySerializer serializer;
		mSerializedObject->seek(0);
		SPtr<SceneObject> restored = std::static_pointer_cast<SceneObject>(
			serializer.decode(mSerializedObject, (UINT32)mSerializedObject->size(), 
			BinarySerializerFlag::None, &serzContext));

		EditorUtility::restoreIds(restored->getHandle(), mSceneObjectProxy);
		restored->setParent(parent);

		if (children)
		{
			for (UINT32 i = 0; i < numChildren; i++)
				children[i]->setParent(restored->getHandle());

			bs_stack_delete(children, numChildren);
		}

		restored->_instantiate();
	}
}
