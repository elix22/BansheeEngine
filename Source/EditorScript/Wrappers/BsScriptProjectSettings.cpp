//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptProjectSettings.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "BsEditorApplication.h"
#include "Settings/BsProjectSettings.h"
#include "Serialization/BsManagedSerializableObject.h"
#include "Reflection/BsRTTIType.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Settings/BsEditorSettings.h"
#include "Serialization/BsBinarySerializer.h"
#include "FileSystem/BsDataStream.h"

namespace bs
{
	ScriptProjectSettings::ScriptProjectSettings(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptProjectSettings::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_GetLastOpenScene", (void*)&ScriptProjectSettings::internal_GetLastOpenScene);
		metaData.scriptClass->addInternalCall("Internal_SetLastOpenScene", (void*)&ScriptProjectSettings::internal_SetLastOpenScene);
		metaData.scriptClass->addInternalCall("Internal_SetFloat", (void*)&ScriptProjectSettings::internal_SetFloat);
		metaData.scriptClass->addInternalCall("Internal_SetInt", (void*)&ScriptProjectSettings::internal_SetInt);
		metaData.scriptClass->addInternalCall("Internal_SetBool", (void*)&ScriptProjectSettings::internal_SetBool);
		metaData.scriptClass->addInternalCall("Internal_SetString", (void*)&ScriptProjectSettings::internal_SetString);
		metaData.scriptClass->addInternalCall("Internal_SetObject", (void*)&ScriptProjectSettings::internal_SetObject);
		metaData.scriptClass->addInternalCall("Internal_GetFloat", (void*)&ScriptProjectSettings::internal_GetFloat);
		metaData.scriptClass->addInternalCall("Internal_GetInt", (void*)&ScriptProjectSettings::internal_GetInt);
		metaData.scriptClass->addInternalCall("Internal_GetBool", (void*)&ScriptProjectSettings::internal_GetBool);
		metaData.scriptClass->addInternalCall("Internal_GetString", (void*)&ScriptProjectSettings::internal_GetString);
		metaData.scriptClass->addInternalCall("Internal_GetObject", (void*)&ScriptProjectSettings::internal_GetObject);
		metaData.scriptClass->addInternalCall("Internal_HasKey", (void*)&ScriptProjectSettings::internal_HasKey);
		metaData.scriptClass->addInternalCall("Internal_DeleteKey", (void*)&ScriptProjectSettings::internal_DeleteKey);
		metaData.scriptClass->addInternalCall("Internal_DeleteAllKeys", (void*)&ScriptProjectSettings::internal_DeleteAllKeys);
		metaData.scriptClass->addInternalCall("Internal_GetHash", (void*)&ScriptProjectSettings::internal_GetHash);
		metaData.scriptClass->addInternalCall("Internal_Save", (void*)&ScriptProjectSettings::internal_Save);
	}

	MonoString* ScriptProjectSettings::internal_GetLastOpenScene()
	{
		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		return MonoUtil::stringToMono(settings->getLastOpenScene());
	}

	void ScriptProjectSettings::internal_SetLastOpenScene(MonoString* value)
	{
		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->setLastOpenScene(MonoUtil::monoToString(value));
	}

	void ScriptProjectSettings::internal_SetFloat(MonoString* name, float value)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->setFloat(nativeName, value);
	}

	void ScriptProjectSettings::internal_SetInt(MonoString* name, int value)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->setInt(nativeName, value);
	}

	void ScriptProjectSettings::internal_SetBool(MonoString* name, bool value)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->setBool(nativeName, value);
	}

	void ScriptProjectSettings::internal_SetString(MonoString* name, MonoString* value)
	{
		String nativeName = MonoUtil::monoToString(name);
		String nativeValue = MonoUtil::monoToString(value);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->setString(nativeName, nativeValue);
	}

	void ScriptProjectSettings::internal_SetObject(MonoString* name, MonoObject* value)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<IReflectable> nativeValue;
		if(value != nullptr)
		{
			nativeValue = ScriptAssemblyManager::instance().getReflectableFromManagedObject(value);

			if(!nativeValue)
				return;
		}

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->setObject(nativeName, nativeValue);
	}

	float ScriptProjectSettings::internal_GetFloat(MonoString* name, float defaultValue)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		return settings->getFloat(nativeName, defaultValue);
	}

	int ScriptProjectSettings::internal_GetInt(MonoString* name, int defaultValue)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		return settings->getInt(nativeName, defaultValue);
	}

	bool ScriptProjectSettings::internal_GetBool(MonoString* name, bool defaultValue)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		return settings->getBool(nativeName, defaultValue);
	}

	MonoString* ScriptProjectSettings::internal_GetString(MonoString* name, MonoString* defaultValue)
	{
		String nativeName = MonoUtil::monoToString(name);
		String nativeDefaultValue = MonoUtil::monoToString(defaultValue);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		String nativeValue = settings->getString(nativeName, nativeDefaultValue);

		return MonoUtil::stringToMono(nativeValue);
	}

	MonoObject* ScriptProjectSettings::internal_GetObject(MonoString* name)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		SPtr<IReflectable> obj = settings->getObject(nativeName);

		if(!obj)
			return nullptr;

		if(auto managedSerializableObject = rtti_cast<ManagedSerializableObject>(obj))
		{
			if (!managedSerializableObject)
				return nullptr;

			// Make a copy of the object as deserialization modifies the object in-place (in particular in can create
			// GC handles, which would then not get released on a potential assembly refresh)
			BinarySerializer bs;

			SPtr<MemoryDataStream> stream = bs_shared_ptr_new<MemoryDataStream>();
			bs.encode(managedSerializableObject.get(), stream);

			stream->seek(0);
			SPtr<ManagedSerializableObject> clone = std::static_pointer_cast<ManagedSerializableObject>(bs.decode(stream, (UINT32)stream->size()));

			return clone->deserialize();
		}
		else
		{
			UINT32 rttiId = obj->getRTTI()->getRTTIId(); 
			const ReflectableTypeInfo* reflTypeInfo = ScriptAssemblyManager::instance().getReflectableTypeInfo(rttiId);
			if(reflTypeInfo == nullptr)
			{
				BS_LOG(Error, Script,"Mapping between a reflectable object and a managed type is missing "
					"for type \"{0}\"", rttiId);
				return nullptr;
			}

			return reflTypeInfo->createCallback(obj);
		}
	}

	bool ScriptProjectSettings::internal_HasKey(MonoString* name)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		return settings->hasKey(nativeName);
	}

	void ScriptProjectSettings::internal_DeleteKey(MonoString* name)
	{
		String nativeName = MonoUtil::monoToString(name);

		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->deleteKey(nativeName);
	}

	void ScriptProjectSettings::internal_DeleteAllKeys()
	{
		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		settings->deleteAllKeys();
	}

	UINT32 ScriptProjectSettings::internal_GetHash()
	{
		SPtr<ProjectSettings> settings = gEditorApplication().getProjectSettings();
		return settings->getHash();
	}

	void ScriptProjectSettings::internal_Save()
	{
		gEditorApplication().saveProjectSettings();
	}
}
