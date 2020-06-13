//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptProjectResourceIcons.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../bsf/Source/Foundation/bsfCore/Image/BsTexture.h"
#include "BsScriptTexture.generated.h"

namespace bs
{
	ScriptProjectResourceIcons::ScriptProjectResourceIcons(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptProjectResourceIcons::initRuntimeData()
	{ }

	MonoObject*ScriptProjectResourceIcons::box(const __ProjectResourceIconsInterop& value)
	{
		return MonoUtil::box(metaData.scriptClass->_getInternalClass(), (void*)&value);
	}

	__ProjectResourceIconsInterop ScriptProjectResourceIcons::unbox(MonoObject* value)
	{
		return *(__ProjectResourceIconsInterop*)MonoUtil::unbox(value);
	}

	ProjectResourceIcons ScriptProjectResourceIcons::fromInterop(const __ProjectResourceIconsInterop& value)
	{
		ProjectResourceIcons output;
		ResourceHandle<Texture> tmpicon16;
		ScriptRRefBase* scripticon16;
		scripticon16 = ScriptRRefBase::toNative(value.icon16);
		if(scripticon16 != nullptr)
			tmpicon16 = static_resource_cast<Texture>(scripticon16->getHandle());
		output.icon16 = tmpicon16;
		ResourceHandle<Texture> tmpicon32;
		ScriptRRefBase* scripticon32;
		scripticon32 = ScriptRRefBase::toNative(value.icon32);
		if(scripticon32 != nullptr)
			tmpicon32 = static_resource_cast<Texture>(scripticon32->getHandle());
		output.icon32 = tmpicon32;
		ResourceHandle<Texture> tmpicon48;
		ScriptRRefBase* scripticon48;
		scripticon48 = ScriptRRefBase::toNative(value.icon48);
		if(scripticon48 != nullptr)
			tmpicon48 = static_resource_cast<Texture>(scripticon48->getHandle());
		output.icon48 = tmpicon48;
		ResourceHandle<Texture> tmpicon64;
		ScriptRRefBase* scripticon64;
		scripticon64 = ScriptRRefBase::toNative(value.icon64);
		if(scripticon64 != nullptr)
			tmpicon64 = static_resource_cast<Texture>(scripticon64->getHandle());
		output.icon64 = tmpicon64;
		ResourceHandle<Texture> tmpicon96;
		ScriptRRefBase* scripticon96;
		scripticon96 = ScriptRRefBase::toNative(value.icon96);
		if(scripticon96 != nullptr)
			tmpicon96 = static_resource_cast<Texture>(scripticon96->getHandle());
		output.icon96 = tmpicon96;
		ResourceHandle<Texture> tmpicon128;
		ScriptRRefBase* scripticon128;
		scripticon128 = ScriptRRefBase::toNative(value.icon128);
		if(scripticon128 != nullptr)
			tmpicon128 = static_resource_cast<Texture>(scripticon128->getHandle());
		output.icon128 = tmpicon128;
		ResourceHandle<Texture> tmpicon192;
		ScriptRRefBase* scripticon192;
		scripticon192 = ScriptRRefBase::toNative(value.icon192);
		if(scripticon192 != nullptr)
			tmpicon192 = static_resource_cast<Texture>(scripticon192->getHandle());
		output.icon192 = tmpicon192;
		ResourceHandle<Texture> tmpicon256;
		ScriptRRefBase* scripticon256;
		scripticon256 = ScriptRRefBase::toNative(value.icon256);
		if(scripticon256 != nullptr)
			tmpicon256 = static_resource_cast<Texture>(scripticon256->getHandle());
		output.icon256 = tmpicon256;

		return output;
	}

	__ProjectResourceIconsInterop ScriptProjectResourceIcons::toInterop(const ProjectResourceIcons& value)
	{
		__ProjectResourceIconsInterop output;
		ScriptRRefBase* scripticon16;
		scripticon16 = ScriptResourceManager::instance().getScriptRRef(value.icon16);
		MonoObject* tmpicon16;
		if(scripticon16 != nullptr)
			tmpicon16 = scripticon16->getManagedInstance();
		else
			tmpicon16 = nullptr;
		output.icon16 = tmpicon16;
		ScriptRRefBase* scripticon32;
		scripticon32 = ScriptResourceManager::instance().getScriptRRef(value.icon32);
		MonoObject* tmpicon32;
		if(scripticon32 != nullptr)
			tmpicon32 = scripticon32->getManagedInstance();
		else
			tmpicon32 = nullptr;
		output.icon32 = tmpicon32;
		ScriptRRefBase* scripticon48;
		scripticon48 = ScriptResourceManager::instance().getScriptRRef(value.icon48);
		MonoObject* tmpicon48;
		if(scripticon48 != nullptr)
			tmpicon48 = scripticon48->getManagedInstance();
		else
			tmpicon48 = nullptr;
		output.icon48 = tmpicon48;
		ScriptRRefBase* scripticon64;
		scripticon64 = ScriptResourceManager::instance().getScriptRRef(value.icon64);
		MonoObject* tmpicon64;
		if(scripticon64 != nullptr)
			tmpicon64 = scripticon64->getManagedInstance();
		else
			tmpicon64 = nullptr;
		output.icon64 = tmpicon64;
		ScriptRRefBase* scripticon96;
		scripticon96 = ScriptResourceManager::instance().getScriptRRef(value.icon96);
		MonoObject* tmpicon96;
		if(scripticon96 != nullptr)
			tmpicon96 = scripticon96->getManagedInstance();
		else
			tmpicon96 = nullptr;
		output.icon96 = tmpicon96;
		ScriptRRefBase* scripticon128;
		scripticon128 = ScriptResourceManager::instance().getScriptRRef(value.icon128);
		MonoObject* tmpicon128;
		if(scripticon128 != nullptr)
			tmpicon128 = scripticon128->getManagedInstance();
		else
			tmpicon128 = nullptr;
		output.icon128 = tmpicon128;
		ScriptRRefBase* scripticon192;
		scripticon192 = ScriptResourceManager::instance().getScriptRRef(value.icon192);
		MonoObject* tmpicon192;
		if(scripticon192 != nullptr)
			tmpicon192 = scripticon192->getManagedInstance();
		else
			tmpicon192 = nullptr;
		output.icon192 = tmpicon192;
		ScriptRRefBase* scripticon256;
		scripticon256 = ScriptResourceManager::instance().getScriptRRef(value.icon256);
		MonoObject* tmpicon256;
		if(scripticon256 != nullptr)
			tmpicon256 = scripticon256->getManagedInstance();
		else
			tmpicon256 = nullptr;
		output.icon256 = tmpicon256;

		return output;
	}

}
