//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptSceneSelection.h"
#include "Scene/BsSelectionRenderer.h"
#include "Wrappers/BsScriptSceneObject.h"
#include "BsScriptGameObjectManager.h"
#include "Scene/BsScenePicking.h"
#include "Scene/BsSelection.h"
#include "BsMonoPrerequisites.h"
#include "BsMonoArray.h"
#include "Components/BsCCamera.h"

#include "BsScriptCCamera.generated.h"

namespace bs
{
	ScriptSceneSelection::ScriptSceneSelection(MonoObject* object, const SPtr<Camera>& camera, 
		const GizmoDrawSettings& gizmoDrawSettings)
		:ScriptObject(object), mCamera(camera), mGizmoDrawSettings(gizmoDrawSettings)
	{
		mSelectionRenderer = bs_new<SelectionRenderer>();
	}

	ScriptSceneSelection::~ScriptSceneSelection()
	{
		bs_delete(mSelectionRenderer);
	}

	void ScriptSceneSelection::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_Create", (void*)&ScriptSceneSelection::internal_Create);
		metaData.scriptClass->addInternalCall("Internal_Draw", (void*)&ScriptSceneSelection::internal_Draw);
		metaData.scriptClass->addInternalCall("Internal_PickObject", (void*)&ScriptSceneSelection::internal_PickObject);
		metaData.scriptClass->addInternalCall("Internal_PickObjects", (void*)&ScriptSceneSelection::internal_PickObjects);
		metaData.scriptClass->addInternalCall("Internal_Snap", (void*)&ScriptSceneSelection::internal_Snap);
		metaData.scriptClass->addInternalCall("Internal_SetGizmoDrawSettings", (void*)&ScriptSceneSelection::internal_SetGizmoDrawSettings);
		metaData.scriptClass->addInternalCall("Internal_GetGizmoDrawSettings", (void*)&ScriptSceneSelection::internal_GetGizmoDrawSettings);
	}

	void ScriptSceneSelection::internal_Create(MonoObject* managedInstance, ScriptCCamera* camera, GizmoDrawSettings* gizmoDrawSettings)
	{
		new (bs_alloc<ScriptSceneSelection>()) ScriptSceneSelection(managedInstance, camera->getHandle()->_getCamera(), 
			*gizmoDrawSettings);
	}

	void ScriptSceneSelection::internal_Draw(ScriptSceneSelection* thisPtr)
	{
		thisPtr->mSelectionRenderer->update(thisPtr->mCamera);
	}

	void ScriptSceneSelection::internal_PickObject(ScriptSceneSelection* thisPtr, Vector2I* inputPos, bool additive, MonoArray* ignoreRenderables)
	{
		Vector<HSceneObject> ignoredSceneObjects;

		if (ignoreRenderables != nullptr)
		{
			ScriptArray scriptArray(ignoreRenderables);

			UINT32 arrayLen = scriptArray.size();
			for (UINT32 i = 0; i < arrayLen; i++)
			{
				MonoObject* monoSO = scriptArray.get<MonoObject*>(i);
				ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(monoSO);

				if (scriptSO == nullptr)
					continue;

				HSceneObject so = static_object_cast<SceneObject>(scriptSO->getNativeHandle());
				ignoredSceneObjects.push_back(so);
			}
		}

		HSceneObject pickedObject = ScenePicking::instance().pickClosestObject(thisPtr->mCamera, 
			thisPtr->mGizmoDrawSettings, *inputPos, Vector2I(1, 1), ignoredSceneObjects);
		if (pickedObject)
		{
			if (additive) // Append to existing selection
			{
				Vector<HSceneObject> selectedSOs = Selection::instance().getSceneObjects();

				auto iterFind = std::find_if(selectedSOs.begin(), selectedSOs.end(),
					[&](const HSceneObject& obj) { return obj == pickedObject; }
				);

				if (iterFind == selectedSOs.end())
					selectedSOs.push_back(pickedObject);

				Selection::instance().setSceneObjects(selectedSOs);
			}
			else
			{
				Vector<HSceneObject> selectedSOs = { pickedObject };
				Selection::instance().setSceneObjects(selectedSOs);
			}
		}
		else if (!additive)
		{
			Selection::instance().clearSceneSelection();
		}
	}

	void ScriptSceneSelection::internal_PickObjects(ScriptSceneSelection* thisPtr, Vector2I* inputPos, Vector2I* area, 
		bool additive, MonoArray* ignoreRenderables)
	{
		Vector<HSceneObject> ignoredSceneObjects;

		if (ignoreRenderables != nullptr)
		{
			ScriptArray scriptArray(ignoreRenderables);

			UINT32 arrayLen = scriptArray.size();
			for (UINT32 i = 0; i < arrayLen; i++)
			{
				MonoObject* monoSO = scriptArray.get<MonoObject*>(i);
				ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(monoSO);

				if (scriptSO == nullptr)
					continue;

				HSceneObject so = static_object_cast<SceneObject>(scriptSO->getNativeHandle());
				ignoredSceneObjects.push_back(so);
			}
		}

		Vector<HSceneObject> pickedObjects = ScenePicking::instance().pickObjects(thisPtr->mCamera, 
			thisPtr->mGizmoDrawSettings, *inputPos, *area, ignoredSceneObjects);

		if (!pickedObjects.empty())
		{
			if (additive) // Append to existing selection
			{
				Vector<HSceneObject> selectedSOs = Selection::instance().getSceneObjects();

				for (int i = 0; i < (int)pickedObjects.size(); i++)
				{
					bool found = false;
					for (int j = 0; j < (int)selectedSOs.size(); j++)
					{
						if (selectedSOs[j] == pickedObjects[i])
						{
							found = true;
							break;
						}
					}

					if (!found)
						selectedSOs.push_back(pickedObjects[i]);
				}

				Selection::instance().setSceneObjects(selectedSOs);
			}
			else
				Selection::instance().setSceneObjects(pickedObjects);
		}
		else if (!additive)
		{
			Selection::instance().clearSceneSelection();
		}
	}

	MonoObject* ScriptSceneSelection::internal_Snap(ScriptSceneSelection* thisPtr, Vector2I* inputPos, SnapData* data, 
		MonoArray* ignoreRenderables)
	{
		Vector<HSceneObject> ignoredSceneObjects;

		if (ignoreRenderables != nullptr)
		{
			ScriptArray scriptArray(ignoreRenderables);

			UINT32 arrayLen = scriptArray.size();
			for (UINT32 i = 0; i < arrayLen; i++)
			{
				MonoObject* monoSO = scriptArray.get<MonoObject*>(i);
				ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(monoSO);

				if (scriptSO == nullptr)
					continue;

				HSceneObject so = static_object_cast<SceneObject>(scriptSO->getNativeHandle());
				ignoredSceneObjects.push_back(so);
			}
		}

		HSceneObject instance = ScenePicking::instance().pickClosestObject(thisPtr->mCamera, thisPtr->mGizmoDrawSettings,
			*inputPos, Vector2I(1, 1), ignoredSceneObjects, data);

		if (instance == nullptr)
			return nullptr;

		ScriptSceneObject* scriptSO = ScriptGameObjectManager::instance().getOrCreateScriptSceneObject(instance);
		return scriptSO->getManagedInstance();
	}

	void ScriptSceneSelection::internal_GetGizmoDrawSettings(ScriptSceneSelection* thisPtr, GizmoDrawSettings* settings)
	{
		*settings = thisPtr->mGizmoDrawSettings;
	}

	void ScriptSceneSelection::internal_SetGizmoDrawSettings(ScriptSceneSelection* thisPtr, GizmoDrawSettings* settings)
	{
		thisPtr->mGizmoDrawSettings = *settings;;
	}
}
