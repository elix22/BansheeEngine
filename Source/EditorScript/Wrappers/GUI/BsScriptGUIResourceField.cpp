//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/GUI/BsScriptGUIResourceField.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "Image/BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "BsGUIResourceField.h"
#include "GUI/BsGUIOptions.h"
#include "GUI/BsGUIContent.h"
#include "Wrappers/BsScriptResource.h"
#include "BsScriptResourceManager.h"
#include "Resources/BsResources.h"

#include "Generated/BsScriptGUIContent.generated.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUIResourceField::OnChangedThunkDef ScriptGUIResourceField::onChangedThunk;

	ScriptGUIResourceField::ScriptGUIResourceField(MonoObject* instance, GUIResourceField* resourceField)
		:TScriptGUIElement(instance, resourceField)
	{

	}

	void ScriptGUIResourceField::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptGUIResourceField::internal_createInstance);
		metaData.scriptClass->addInternalCall("Internal_GetValue", (void*)&ScriptGUIResourceField::internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_SetValue", (void*)&ScriptGUIResourceField::internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_GetValueRef", (void*)&ScriptGUIResourceField::internal_getValueRef);
		metaData.scriptClass->addInternalCall("Internal_SetValueRef", (void*)&ScriptGUIResourceField::internal_setValueRef);
		metaData.scriptClass->addInternalCall("Internal_SetTint", (void*)&ScriptGUIResourceField::internal_setTint);

		onChangedThunk = (OnChangedThunkDef)metaData.scriptClass->getMethod("DoOnChanged", 1)->getThunk();
	}

	void ScriptGUIResourceField::internal_createInstance(MonoObject* instance, MonoReflectionType* type, 
		__GUIContentInterop* title, UINT32 titleWidth, MonoString* style, MonoArray* guiOptions, bool withTitle)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		String styleName = MonoUtil::monoToString(style);
		::MonoClass* monoClass = MonoUtil::getClass(type);

		String typeNamespace;
		String typeName;
		MonoUtil::getClassName(monoClass, typeNamespace, typeName);

		GUIResourceField* guiResourceField = nullptr;
		if (withTitle)
		{
			GUIContent nativeContent = ScriptGUIContent::fromInterop(*title);
			guiResourceField = GUIResourceField::create(typeNamespace, typeName, 
				GUIFieldOptions(nativeContent, titleWidth, options, styleName));
		}
		else
		{
			guiResourceField = GUIResourceField::create(typeNamespace, typeName, GUIFieldOptions(options, styleName));
		}

		auto nativeInstance = new (bs_alloc<ScriptGUIResourceField>()) ScriptGUIResourceField(instance, guiResourceField);

		guiResourceField->onValueChanged.connect(std::bind(&ScriptGUIResourceField::onChanged, nativeInstance, _1));
	}

	void ScriptGUIResourceField::internal_getValue(ScriptGUIResourceField* nativeInstance, MonoObject** output)
	{
		GUIResourceField* resourceField = static_cast<GUIResourceField*>(nativeInstance->getGUIElement());

		HResource resource = resourceField->getValue();
		
		if(resource)
		{
			const ResourceLoadFlags loadFlags = ResourceLoadFlag::Default | ResourceLoadFlag::KeepSourceData;
			resource = gResources().loadFromUUID(resource.getUUID(), false, loadFlags);
		}

		MonoUtil::referenceCopy(output, nativeToManagedResource(resource));
	}

	void ScriptGUIResourceField::internal_setValue(ScriptGUIResourceField* nativeInstance, MonoObject* value)
	{
		GUIResourceField* resourceField = static_cast<GUIResourceField*>(nativeInstance->getGUIElement());

		if (value == nullptr)
			resourceField->setValue(HResource());
		else
		{
			ScriptResourceBase* scriptResource = ScriptResource::toNative(value);
			resourceField->setValue(scriptResource->getGenericHandle());
		}
	}

	void ScriptGUIResourceField::internal_getValueRef(ScriptGUIResourceField* nativeInstance, MonoObject** output)
	{
		GUIResourceField* resourceField = static_cast<GUIResourceField*>(nativeInstance->getGUIElement());

		HResource resource = resourceField->getValue();
		ScriptRRefBase* scriptRRef = ScriptResourceManager::instance().getScriptRRef(resource);

		if(scriptRRef)
			MonoUtil::referenceCopy(output, scriptRRef->getManagedInstance());
		else
			MonoUtil::referenceCopy(output, nullptr);
	}

	void ScriptGUIResourceField::internal_setValueRef(ScriptGUIResourceField* nativeInstance, MonoObject* value)
	{
		GUIResourceField* resourceField = static_cast<GUIResourceField*>(nativeInstance->getGUIElement());

		if (value == nullptr)
			resourceField->setValue(HResource());
		else
		{
			ScriptRRefBase* scriptResource = ScriptRRefBase::toNative(value);
			resourceField->setValue(scriptResource->getHandle());
		}
	}

	void ScriptGUIResourceField::internal_setTint(ScriptGUIResourceField* nativeInstance, Color* color)
	{
		GUIResourceField* resourceField = (GUIResourceField*)nativeInstance->getGUIElement();
		resourceField->setTint(*color);
	}

	void ScriptGUIResourceField::onChanged(const HResource& newHandle)
	{
		ScriptRRefBase* scriptRRef = ScriptResourceManager::instance().getScriptRRef(newHandle);
		MonoObject* managedObj = nullptr;
		if(scriptRRef)
			managedObj = scriptRRef->getManagedInstance();

		MonoUtil::invokeThunk(onChangedThunk, getManagedInstance(), managedObj);
	}

	MonoObject* ScriptGUIResourceField::nativeToManagedResource(const HResource& instance)
	{
		if (instance == nullptr)
			return nullptr;
		else
		{
			ScriptResourceBase* scriptResource = ScriptResourceManager::instance().getScriptResource(instance, true);
			return scriptResource->getManagedInstance();
		}
	}
}