//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
#include "BsScriptBuiltinEditorResources.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../EditorCore/Utility/BsBuiltinEditorResources.h"
#include "BsScriptResourceManager.h"
#include "BsScriptGUISkin.generated.h"
#include "BsScriptFont.generated.h"
#include "BsScriptSpriteTexture.generated.h"
#include "BsScriptGUIContentImages.generated.h"

namespace bs
{
	ScriptBuiltinEditorResources::ScriptBuiltinEditorResources(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{
	}

	void ScriptBuiltinEditorResources::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getSkin", (void*)&ScriptBuiltinEditorResources::Internal_getSkin);
		metaData.scriptClass->addInternalCall("Internal_getDefaultFont", (void*)&ScriptBuiltinEditorResources::Internal_getDefaultFont);
		metaData.scriptClass->addInternalCall("Internal_getDefaultAAFont", (void*)&ScriptBuiltinEditorResources::Internal_getDefaultAAFont);
		metaData.scriptClass->addInternalCall("Internal_getProjectLibraryIcon", (void*)&ScriptBuiltinEditorResources::Internal_getProjectLibraryIcon);
		metaData.scriptClass->addInternalCall("Internal_getToolbarIcon", (void*)&ScriptBuiltinEditorResources::Internal_getToolbarIcon);
		metaData.scriptClass->addInternalCall("Internal_getSceneWindowIcon", (void*)&ScriptBuiltinEditorResources::Internal_getSceneWindowIcon);
		metaData.scriptClass->addInternalCall("Internal_getSceneViewIcon", (void*)&ScriptBuiltinEditorResources::Internal_getSceneViewIcon);
		metaData.scriptClass->addInternalCall("Internal_getLibraryWindowIcon", (void*)&ScriptBuiltinEditorResources::Internal_getLibraryWindowIcon);
		metaData.scriptClass->addInternalCall("Internal_getInspectorWindowIcon", (void*)&ScriptBuiltinEditorResources::Internal_getInspectorWindowIcon);
		metaData.scriptClass->addInternalCall("Internal_getAnimationWindowIcon", (void*)&ScriptBuiltinEditorResources::Internal_getAnimationWindowIcon);
		metaData.scriptClass->addInternalCall("Internal_getEditorIcon", (void*)&ScriptBuiltinEditorResources::Internal_getEditorIcon);
		metaData.scriptClass->addInternalCall("Internal_getEditorToggleIcon", (void*)&ScriptBuiltinEditorResources::Internal_getEditorToggleIcon);
		metaData.scriptClass->addInternalCall("Internal_getLogMessageIcon", (void*)&ScriptBuiltinEditorResources::Internal_getLogMessageIcon);
		metaData.scriptClass->addInternalCall("Internal_getSprite", (void*)&ScriptBuiltinEditorResources::Internal_getSprite);
		metaData.scriptClass->addInternalCall("Internal_getEmptyShaderCode", (void*)&ScriptBuiltinEditorResources::Internal_getEmptyShaderCode);
		metaData.scriptClass->addInternalCall("Internal_getEmptyCSScriptCode", (void*)&ScriptBuiltinEditorResources::Internal_getEmptyCSScriptCode);

	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getSkin()
	{
		ResourceHandle<GUISkin> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getSkin();

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getDefaultFont()
	{
		ResourceHandle<Font> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getDefaultFont();

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getDefaultAAFont()
	{
		ResourceHandle<Font> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getDefaultAAFont();

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getProjectLibraryIcon(ProjectLibraryIcon icon, int32_t size)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getProjectLibraryIcon(icon, size);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getToolbarIcon(ToolbarIcon icon)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getToolbarIcon(icon);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptBuiltinEditorResources::Internal_getSceneWindowIcon(SceneWindowIcon icon, __GUIContentImagesInterop* __output)
	{
		GUIContentImages tmp__output;
		tmp__output = BuiltinEditorResources::instance().getSceneWindowIcon(icon);

		__GUIContentImagesInterop interop__output;
		interop__output = ScriptGUIContentImages::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIContentImages::getMetaData()->scriptClass->_getInternalClass());
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getSceneViewIcon(SceneViewIcon icon)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getSceneViewIcon(icon);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getLibraryWindowIcon(LibraryWindowIcon icon)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getLibraryWindowIcon(icon);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getInspectorWindowIcon(InspectorWindowIcon icon)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getInspectorWindowIcon(icon);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptBuiltinEditorResources::Internal_getAnimationWindowIcon(AnimationWindowIcon icon, __GUIContentImagesInterop* __output)
	{
		GUIContentImages tmp__output;
		tmp__output = BuiltinEditorResources::instance().getAnimationWindowIcon(icon);

		__GUIContentImagesInterop interop__output;
		interop__output = ScriptGUIContentImages::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIContentImages::getMetaData()->scriptClass->_getInternalClass());
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getEditorIcon(EditorIcon icon)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getEditorIcon(icon);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptBuiltinEditorResources::Internal_getEditorToggleIcon(EditorToggleIcon icon, __GUIContentImagesInterop* __output)
	{
		GUIContentImages tmp__output;
		tmp__output = BuiltinEditorResources::instance().getEditorToggleIcon(icon);

		__GUIContentImagesInterop interop__output;
		interop__output = ScriptGUIContentImages::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIContentImages::getMetaData()->scriptClass->_getInternalClass());
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getLogMessageIcon(LogMessageIcon icon, uint32_t size, bool dark)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getLogMessageIcon(icon, size, dark);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptBuiltinEditorResources::Internal_getSprite(EditorSprites sprite)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		tmp__output = BuiltinEditorResources::instance().getSprite(sprite);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoString* ScriptBuiltinEditorResources::Internal_getEmptyShaderCode()
	{
		String tmp__output;
		tmp__output = BuiltinEditorResources::instance().getEmptyShaderCode();

		MonoString* __output;
		__output = MonoUtil::stringToMono(tmp__output);

		return __output;
	}

	MonoString* ScriptBuiltinEditorResources::Internal_getEmptyCSScriptCode()
	{
		String tmp__output;
		tmp__output = BuiltinEditorResources::instance().getEmptyCSScriptCode();

		MonoString* __output;
		__output = MonoUtil::stringToMono(tmp__output);

		return __output;
	}
}
