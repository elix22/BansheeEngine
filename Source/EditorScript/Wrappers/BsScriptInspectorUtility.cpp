//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptInspectorUtility.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoAssembly.h"
#include "BsMonoUtil.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsScriptObjectManager.h"
#include "Wrappers/BsScriptResource.h"
#include "Wrappers/BsScriptComponent.h"

namespace bs
{
	MonoClass* ScriptInspectorUtility::mCustomInspectorAtribute = nullptr;
	MonoField* ScriptInspectorUtility::mTypeField = nullptr;

	UnorderedMap<MonoClass*, MonoClass*> ScriptInspectorUtility::mInspectorTypes;
	UnorderedMap<MonoClass*, MonoClass*> ScriptInspectorUtility::mInspectableFieldTypes;

	HEvent ScriptInspectorUtility::mDomainLoadedConn;

	ScriptInspectorUtility::ScriptInspectorUtility(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptInspectorUtility::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GetCustomInspector", (void*)&ScriptInspectorUtility::internal_GetCustomInspector);
		metaData.scriptClass->addInternalCall("Internal_GetCustomInspectable", (void*)&ScriptInspectorUtility::internal_GetCustomInspectable);
	}

	void ScriptInspectorUtility::startUp()
	{
		mDomainLoadedConn = ScriptObjectManager::instance().onRefreshDomainLoaded.connect(&ScriptInspectorUtility::reloadAssemblyData);
		reloadAssemblyData();
	}

	void ScriptInspectorUtility::shutDown()
	{
		mDomainLoadedConn.disconnect();
	}

	void ScriptInspectorUtility::reloadAssemblyData()
	{
		mInspectorTypes.clear();
		mInspectableFieldTypes.clear();

		// Reload MenuItem attribute from editor assembly
		MonoAssembly* editorAssembly = MonoManager::instance().getAssembly(EDITOR_ASSEMBLY);
		mCustomInspectorAtribute = editorAssembly->getClass(EDITOR_NS, "CustomInspector");
		if (mCustomInspectorAtribute == nullptr)
			BS_EXCEPT(InternalErrorException, "Cannot find CustomInspector managed class.");

		MonoClass* inspectorClass = editorAssembly->getClass(EDITOR_NS, "Inspector");
		if (inspectorClass == nullptr)
			BS_EXCEPT(InternalErrorException, "Cannot find Inspector managed class.");

		MonoClass* inspectableFieldClass = editorAssembly->getClass(EDITOR_NS, "InspectableField");
		if (inspectableFieldClass == nullptr)
			BS_EXCEPT(InternalErrorException, "Cannot find InspectableField managed class.");

		MonoAssembly* corlib = MonoManager::instance().getAssembly("corlib");
		if(corlib == nullptr)
			BS_EXCEPT(InternalErrorException, "corlib assembly is not loaded.");

		MonoClass* systemAttributeClass = corlib->getClass("System", "Attribute");
		if(systemAttributeClass == nullptr)
			BS_EXCEPT(InternalErrorException, "Cannot find System.Attribute managed class.");

		mTypeField = mCustomInspectorAtribute->getField("type");

		ScriptAssemblyManager& sam = ScriptAssemblyManager::instance();

		Vector<String> scriptAssemblyNames = sam.getScriptAssemblies();
		for (auto& assemblyName : scriptAssemblyNames)
		{
			MonoAssembly* assembly = MonoManager::instance().getAssembly(assemblyName);

			// Find new classes/structs with the custom inspector attribute
			const Vector<MonoClass*>& allClasses = assembly->getAllClasses();
			for (auto curClass : allClasses)
			{
				MonoObject* attrib = curClass->getAttribute(mCustomInspectorAtribute);
				if (attrib == nullptr)
					continue;

				// Check if the attribute references a valid class
				MonoReflectionType* referencedReflType = nullptr;
				mTypeField->get(attrib, &referencedReflType);

				::MonoClass* referencedMonoClass = MonoUtil::getClass(referencedReflType);

				MonoClass* referencedClass = MonoManager::instance().findClass(referencedMonoClass);
				if (referencedClass == nullptr)
					continue;

				if (curClass->isSubClassOf(inspectorClass))
				{
					bool isValidInspectorType = referencedClass->isSubClassOf(ScriptResource::getMetaData()->scriptClass) ||
						referencedClass->isSubClassOf(ScriptComponent::getMetaData()->scriptClass);

					if (!isValidInspectorType)
						continue;

					mInspectorTypes[referencedClass] = curClass;
				}
				else if (curClass->isSubClassOf(inspectableFieldClass) || curClass->isSubClassOf(systemAttributeClass))
				{
					mInspectableFieldTypes[referencedClass] = curClass;
				}
			}
		}
	}

	MonoObject* ScriptInspectorUtility::internal_GetCustomInspector(MonoReflectionType* reflType)
	{
		if (reflType == nullptr)
			return nullptr;

		::MonoClass* monoClass = MonoUtil::getClass(reflType);

		MonoClass* klass = MonoManager::instance().findClass(monoClass);
		if (klass == nullptr)
			return nullptr;

		auto iterFind = mInspectorTypes.find(klass);
		if (iterFind != mInspectorTypes.end())
			return iterFind->second->createInstance();

		return nullptr;
	}

	MonoReflectionType* ScriptInspectorUtility::internal_GetCustomInspectable(MonoReflectionType* reflType)
	{
		if (reflType == nullptr)
			return nullptr;

		::MonoClass* monoClass = MonoUtil::getClass(reflType);

		MonoClass* klass = MonoManager::instance().findClass(monoClass);
		if (klass == nullptr)
			return nullptr;

		// Try to find an inspectable field implementation directly referencing the class
		auto iterFind = mInspectableFieldTypes.find(klass);
		if (iterFind != mInspectableFieldTypes.end())
			return MonoUtil::getType(iterFind->second->_getInternalClass());

		// Try to find an inspectable field implementation referencing any of the class' attributes
		Vector<MonoClass*> attribs = klass->getAllAttributes();
		for (auto attrib : attribs)
		{
			auto iterFind = mInspectableFieldTypes.find(attrib);
			if (iterFind != mInspectableFieldTypes.end())
				return MonoUtil::getType(iterFind->second->_getInternalClass());
		}

		return nullptr;
	}
}