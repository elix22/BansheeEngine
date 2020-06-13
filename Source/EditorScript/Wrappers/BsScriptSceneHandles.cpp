//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptSceneHandles.h"
#include "Wrappers/BsScriptEditorWindow.h"
#include "Platform/BsCursor.h"
#include "RenderAPI/BsRenderWindow.h"
#include "EditorWindow/BsEditorWidget.h"
#include "EditorWindow/BsEditorWindow.h"
#include "Handles/BsHandleManager.h"
#include "BsEditorApplication.h"
#include "Components/BsCCamera.h"

#include "Generated/BsScriptCCamera.generated.h"

namespace bs
{
	ScriptSceneHandles::ScriptSceneHandles(MonoObject* object, EditorWidgetBase* parentWidget, const HCamera& camera)
		:ScriptObject(object), mParentWidget(parentWidget), mCamera(camera)
	{
		HandleManager::instance().setSettings(gEditorApplication().getEditorSettings());
	}

	ScriptSceneHandles::~ScriptSceneHandles()
	{

	}

	void ScriptSceneHandles::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_Create", (void*)&ScriptSceneHandles::internal_Create);
		metaData.scriptClass->addInternalCall("Internal_Draw", (void*)&ScriptSceneHandles::internal_Draw);
		metaData.scriptClass->addInternalCall("Internal_BeginInput", (void*)&ScriptSceneHandles::internal_BeginInput);
		metaData.scriptClass->addInternalCall("Internal_EndInput", (void*)&ScriptSceneHandles::internal_EndInput);
		metaData.scriptClass->addInternalCall("Internal_UpdateInput", (void*)&ScriptSceneHandles::internal_UpdateInput);
		metaData.scriptClass->addInternalCall("Internal_TrySelect", (void*)&ScriptSceneHandles::internal_TrySelect);
		metaData.scriptClass->addInternalCall("Internal_IsActive", (void*)&ScriptSceneHandles::internal_IsActive);
		metaData.scriptClass->addInternalCall("Internal_ClearSelection", (void*)&ScriptSceneHandles::internal_ClearSelection);
	}

	Vector2I ScriptSceneHandles::wrapCursorToWindow() const
	{
		if (mParentWidget == nullptr)
			return Vector2I();

		SPtr<RenderWindow> parentWindow = mParentWidget->getParentWindow()->getRenderWindow();

		Vector2I screenPos = Cursor::instance().getScreenPosition();
		Vector2I windowPos = parentWindow->screenToWindowPos(screenPos);
		const RenderWindowProperties& rwProps = parentWindow->getProperties();

		INT32 maxWidth = std::max(0, (INT32)rwProps.width - 1);
		INT32 maxHeight = std::max(0, (INT32)rwProps.height - 1);

		Vector2I offset;
		if (windowPos.x <= 0)
			offset.x = maxWidth;
		else if (windowPos.x >= maxWidth)
			offset.x = -maxWidth;

		if (windowPos.y <= 0)
			offset.y = maxHeight;
		else if (windowPos.y >= maxHeight)
			offset.y = -maxHeight;

		windowPos += offset;

		Vector2I wrappedScreenPos = parentWindow->windowToScreenPos(windowPos);

		// This check is important for macOS, where continuously setting the position will freeze the cursor in place
		if(wrappedScreenPos != screenPos)
			Cursor::instance().setScreenPosition(wrappedScreenPos);

		return offset;
	}

	void ScriptSceneHandles::internal_Create(MonoObject* managedInstance, ScriptEditorWindow* parentWindow, ScriptCCamera* camera)
	{
		EditorWidgetBase* widget = nullptr;

		if (parentWindow != nullptr && !parentWindow->isDestroyed())
			widget = parentWindow->getEditorWidget();

		new (bs_alloc<ScriptSceneHandles>()) ScriptSceneHandles(managedInstance, widget, camera->getHandle());
	}

	void ScriptSceneHandles::internal_Draw(ScriptSceneHandles* thisPtr)
	{
		// Make sure camera's transform is up-to-date
		const SPtr<Camera>& cameraPtr = thisPtr->mCamera->_getCamera();
		cameraPtr->_updateState(*thisPtr->mCamera->SO());

		HandleManager::instance().draw(cameraPtr);
	}

	void ScriptSceneHandles::internal_BeginInput()
	{
		HandleManager::instance().beginInput();
	}

	void ScriptSceneHandles::internal_EndInput()
	{
		HandleManager::instance().endInput();
	}

	bool ScriptSceneHandles::internal_UpdateInput(ScriptSceneHandles* thisPtr, Vector2I* inputPos, Vector2I* inputDelta)
	{
		// If mouse wrapped around last frame then we need to compensate for the jump amount
		Vector2I realDelta = *inputDelta - thisPtr->mMouseDeltaCompensate;
		thisPtr->mMouseDeltaCompensate = Vector2I::ZERO;

		if (HandleManager::instance().isHandleActive(thisPtr->mCamera->_getCamera()))
			thisPtr->mMouseDeltaCompensate = thisPtr->wrapCursorToWindow();

		return HandleManager::instance().updateInput(thisPtr->mCamera->_getCamera(), *inputPos, realDelta);
	}

	bool ScriptSceneHandles::internal_TrySelect(ScriptSceneHandles* thisPtr, Vector2I* inputPos)
	{
		return HandleManager::instance().trySelect(thisPtr->mCamera->_getCamera(), *inputPos);
	}

	bool ScriptSceneHandles::internal_IsActive(ScriptSceneHandles* thisPtr)
	{
		return HandleManager::instance().isHandleActive(thisPtr->mCamera->_getCamera());
	}

	void ScriptSceneHandles::internal_ClearSelection(ScriptSceneHandles* thisPtr)
	{
		HandleManager::instance().clearSelection(thisPtr->mCamera->_getCamera());
	}
}