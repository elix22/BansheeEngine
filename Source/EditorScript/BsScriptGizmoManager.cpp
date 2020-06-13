//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsScriptGizmoManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsMonoAssembly.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoField.h"
#include "BsMonoManager.h"
#include "Scene/BsSceneManager.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsComponent.h"
#include "BsManagedComponent.h"
#include "Scene/BsGizmoManager.h"
#include "Scene/BsSelection.h"
#include "BsScriptObjectManager.h"
#include "BsScriptGameObjectManager.h"
#include "Wrappers/BsScriptComponent.h"
#include "Wrappers/BsScriptSelection.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGizmoManager::ScriptGizmoManager(ScriptAssemblyManager& scriptObjectManager)
		:mScriptObjectManager(scriptObjectManager)
	{
		mDomainLoadedConn = ScriptObjectManager::instance().onRefreshDomainLoaded.connect(std::bind(&ScriptGizmoManager::reloadAssemblyData, this));
		mSelectionSOAddedConn = Selection::instance().onSceneObjectsAdded.connect(
			[this](const Vector<HSceneObject>& objects) { onSOSelectionChanged(objects, true); });
		mSelectionSORemovedConn = Selection::instance().onSceneObjectsRemoved.connect(
			[this](const Vector<HSceneObject>& objects) { onSOSelectionChanged(objects, false); });

		reloadAssemblyData();
	}

	ScriptGizmoManager::~ScriptGizmoManager()
	{
		mSelectionSOAddedConn.disconnect();
		mSelectionSORemovedConn.disconnect();
		mDomainLoadedConn.disconnect();
	}

	void ScriptGizmoManager::update()
	{
		GizmoManager::instance().clearGizmos();

		HSceneObject rootSO = SceneManager::instance().getMainScene()->getRoot();

		Stack<HSceneObject> todo;
		todo.push(rootSO);

		bool isParentSelected = false;
		UINT32 parentSelectedPopIdx = 0;
		
		Vector<HSceneObject> selectedObjects = Selection::instance().getSceneObjects();

		while (!todo.empty())
		{
			if (isParentSelected && parentSelectedPopIdx == (UINT32)todo.size())
			{
				isParentSelected = false;
			}

			HSceneObject curSO = todo.top();
			todo.pop();

			if(curSO->hasFlag(SOF_Internal))
				continue;

			bool isSelected = std::count(selectedObjects.begin(), selectedObjects.end(), curSO) > 0;
			if (isSelected && !isParentSelected)
			{
				isParentSelected = true;
				parentSelectedPopIdx = (UINT32)todo.size();
			}

			const Vector<HComponent>& components = curSO->getComponents();
			for (auto& component : components)
			{
				String componentName;
				MonoObject* managedInstance = nullptr;
				if (rtti_is_of_type<ManagedComponent>(component.get()))
				{
					ManagedComponent* managedComponent = static_cast<ManagedComponent*>(component.get());
					componentName = managedComponent->getManagedFullTypeName();
					managedInstance = managedComponent->getManagedInstance();
				}
				else
				{
					ScriptGameObjectManager& sgoManager = ScriptGameObjectManager::instance();
					ScriptComponentBase* scriptComponent = sgoManager.getBuiltinScriptComponent(component);

					if (scriptComponent)
					{
						managedInstance = scriptComponent->getManagedInstance();

						String ns, typeName;
						MonoUtil::getClassName(managedInstance, ns, typeName);

						componentName = ns + "." + typeName;
					}
				}

				if (componentName.empty())
					continue;

				auto iterFind = mGizmoDrawers.find(componentName);
				if (iterFind != mGizmoDrawers.end())
				{
					SmallVector<GizmoData, 2>& entries = iterFind->second;

					for(auto& entry : entries)
					{
						UINT32 flags = entry.flags;

						bool drawGizmo = false;
						if (((flags & (UINT32)DrawGizmoFlags::Selected) != 0) && isSelected)
							drawGizmo = true;

						if (((flags & (UINT32)DrawGizmoFlags::ParentSelected) != 0) && isParentSelected)
							drawGizmo = true;

						if (((flags & (UINT32)DrawGizmoFlags::NotSelected) != 0) && !isSelected && !isParentSelected)
							drawGizmo = true;

						if (drawGizmo)
						{
							bool pickable = (flags & (UINT32)DrawGizmoFlags::Pickable) != 0;
							GizmoManager::instance().startGizmo(curSO);
							GizmoManager::instance().setPickable(pickable);

							void* params[1] = { managedInstance };
							entry.method->invoke(nullptr, params);

							GizmoManager::instance().endGizmo();
						}
					}
				}
			}

			for (UINT32 i = 0; i < curSO->getNumChildren(); i++)
				todo.push(curSO->getChild(i));
		}
	}

	void ScriptGizmoManager::reloadAssemblyData()
	{
		// Reload DrawGizmo attribute from editor assembly
		MonoAssembly* editorAssembly = MonoManager::instance().getAssembly(EDITOR_ASSEMBLY);
		mDrawGizmoAttribute = editorAssembly->getClass(EDITOR_NS, "DrawGizmo");
		if (mDrawGizmoAttribute == nullptr)
			BS_EXCEPT(InvalidStateException, "Cannot find DrawGizmo managed class.");

		mFlagsField = mDrawGizmoAttribute->getField("flags");

		mOnSelectionChangedAttribute = editorAssembly->getClass(EDITOR_NS, "OnSelectionChanged");
		if (mOnSelectionChangedAttribute == nullptr)
			BS_EXCEPT(InvalidStateException, "Cannot find OnSelectionChanged managed class.");

		mGizmoDrawers.clear();

		Vector<String> scriptAssemblyNames = mScriptObjectManager.getScriptAssemblies();
		for (auto& assemblyName : scriptAssemblyNames)
		{
			MonoAssembly* assembly = MonoManager::instance().getAssembly(assemblyName);

			// Find new gizmo drawer & selection changed methods
			const Vector<MonoClass*>& allClasses = assembly->getAllClasses();
			for (auto curClass : allClasses)
			{
				const Vector<MonoMethod*>& methods = curClass->getAllMethods();
				for (auto& curMethod : methods)
				{
					UINT32 drawGizmoFlags = 0;
					MonoClass* componentType = nullptr;
					if (isValidDrawGizmoMethod(curMethod, componentType, drawGizmoFlags))
					{
						String fullComponentName = componentType->getFullName();
						SmallVector<GizmoData, 2>& entries = mGizmoDrawers[fullComponentName];

						GizmoData data{componentType, curMethod, drawGizmoFlags};
						entries.add(data);
					}
					else if(isValidOnSelectionChangedMethod(curMethod, componentType))
					{
						String fullComponentName = componentType->getFullName();
						SelectionChangedData& data = mSelectionChangedCallbacks[fullComponentName];

						data.type = componentType;
						data.method = curMethod;
					}
				}
			}
		}
	}

	void ScriptGizmoManager::onSOSelectionChanged(const Vector<HSceneObject>& sceneObjects, bool added)
	{
		for(auto& sceneObject : sceneObjects)
		{
			if(sceneObject.isDestroyed())
				continue;

			Vector<HComponent> components = sceneObject->getComponents();
			for(auto& component : components)
			{
				String componentName;
				MonoObject* managedInstance = nullptr;
				if (rtti_is_of_type<ManagedComponent>(component.get()))
				{
					ManagedComponent* managedComponent = static_cast<ManagedComponent*>(component.get());
					componentName = managedComponent->getManagedFullTypeName();
					managedInstance = managedComponent->getManagedInstance();
				}
				else
				{
					ScriptGameObjectManager& sgoManager = ScriptGameObjectManager::instance();
					ScriptComponentBase* scriptComponent = sgoManager.getBuiltinScriptComponent(component, false);

					if (scriptComponent)
					{
						managedInstance = scriptComponent->getManagedInstance();

						String ns, typeName;
						MonoUtil::getClassName(managedInstance, ns, typeName);

						componentName = ns + "." + typeName;
					}
				}

				if (componentName.empty())
					continue;

				auto iterFind = mSelectionChangedCallbacks.find(componentName);
				if (iterFind != mSelectionChangedCallbacks.end())
				{
					void* params[2] = { managedInstance, &added };
					iterFind->second.method->invoke(nullptr, params);
				}
			}
		}
	}

	bool ScriptGizmoManager::isValidDrawGizmoMethod(MonoMethod* method, MonoClass*& componentType, UINT32& drawGizmoFlags)
	{
		componentType = nullptr;
		drawGizmoFlags = 0;

		if (!method->hasAttribute(mDrawGizmoAttribute))
			return false;

		if (method->getNumParameters() != 1)
			return false;

		if (!method->isStatic())
			return false;

		MonoClass* paramType = method->getParameterType(0);
		MonoClass* componentClass = mScriptObjectManager.getBuiltinClasses().componentClass;

		if (!paramType->isSubClassOf(componentClass))
			return false;

		componentType = paramType;

		MonoObject* drawGizmoAttrib = method->getAttribute(mDrawGizmoAttribute);
		mFlagsField->get(drawGizmoAttrib, &drawGizmoFlags);

		return true;
	}

	bool ScriptGizmoManager::isValidOnSelectionChangedMethod(class MonoMethod* method, MonoClass*& componentType)
	{
		componentType = nullptr;

		if (!method->hasAttribute(mOnSelectionChangedAttribute))
			return false;

		if (method->getNumParameters() != 2)
			return false;

		if (!method->isStatic())
			return false;

		MonoClass* param0Type = method->getParameterType(0);
		MonoClass* componentClass = mScriptObjectManager.getBuiltinClasses().componentClass;

		if (!param0Type->isSubClassOf(componentClass))
			return false;

		MonoClass* param1Type = method->getParameterType(1);
		if(param1Type->_getInternalClass() != MonoUtil::getBoolClass())
			return false;

		componentType = param0Type;

		return true;
	}
}