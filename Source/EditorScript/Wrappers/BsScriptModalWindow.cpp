//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Wrappers/BsScriptModalWindow.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoManager.h"
#include "BsMonoUtil.h"
#include "BsMonoAssembly.h"
#include "BsScriptObjectManager.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"

#include "Generated/BsScriptHString.generated.h"

using namespace std::placeholders;

namespace bs
{
	MonoField* ScriptModalWindow::guiPanelField = nullptr;

	ScriptModalWindow::ScriptModalWindow(ManagedModalWindow* window)
		:ScriptObject(window->getManagedInstance()), mModalWindow(window)
	{
		mOnAssemblyRefreshStartedConn = ScriptObjectManager::instance().onRefreshStarted.connect(std::bind(&ScriptModalWindow::onAssemblyRefreshStarted, this));
	}

	ScriptModalWindow::~ScriptModalWindow()
	{
		mOnAssemblyRefreshStartedConn.disconnect();
	}

	void ScriptModalWindow::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptModalWindow::internal_createInstance);
		metaData.scriptClass->addInternalCall("Internal_Close", (void*)&ScriptModalWindow::internal_close);
		metaData.scriptClass->addInternalCall("Internal_GetWidth", (void*)&ScriptModalWindow::internal_getWidth);
		metaData.scriptClass->addInternalCall("Internal_GetHeight", (void*)&ScriptModalWindow::internal_getHeight);
		metaData.scriptClass->addInternalCall("Internal_SetWidth", (void*)&ScriptModalWindow::internal_setWidth);
		metaData.scriptClass->addInternalCall("Internal_SetHeight", (void*)&ScriptModalWindow::internal_setHeight);
		metaData.scriptClass->addInternalCall("Internal_GetContentWidth", (void*)&ScriptModalWindow::internal_getContentWidth);
		metaData.scriptClass->addInternalCall("Internal_GetContentHeight", (void*)&ScriptModalWindow::internal_getContentHeight);
		metaData.scriptClass->addInternalCall("Internal_SetContentWidth", (void*)&ScriptModalWindow::internal_setContentWidth);
		metaData.scriptClass->addInternalCall("Internal_SetContentHeight", (void*)&ScriptModalWindow::internal_setContentHeight);
		metaData.scriptClass->addInternalCall("Internal_SetTitle", (void*)&ScriptModalWindow::internal_setTitle);
		metaData.scriptClass->addInternalCall("Internal_ScreenToWindowPos", (void*)&ScriptModalWindow::internal_screenToWindowPos);
		metaData.scriptClass->addInternalCall("Internal_WindowToScreenPos", (void*)&ScriptModalWindow::internal_windowToScreenPos);

		guiPanelField = metaData.scriptClass->getField("GUI");
	}

	void ScriptModalWindow::internal_createInstance(MonoObject* instance, bool allowCloseButton)
	{
		::MonoClass* rawMonoClass = MonoUtil::getClass(instance);
		MonoClass* monoClass = MonoManager::instance().findClass(rawMonoClass);

		MonoAssembly* assembly = MonoManager::instance().getAssembly(EDITOR_ASSEMBLY);

		MonoClass* defaultSizeAttrib = assembly->getClass(EDITOR_NS, "DefaultSize");
		if (defaultSizeAttrib == nullptr)
			BS_EXCEPT(InternalErrorException, "Cannot find DefaultSize managed class.");

		MonoField* defaultWidthField = defaultSizeAttrib->getField("width");
		MonoField* defaultHeightField = defaultSizeAttrib->getField("height");

		int width = 200;
		int height = 200;

		MonoObject* defaultSizeObj = monoClass->getAttribute(defaultSizeAttrib);
		if (defaultSizeObj != nullptr)
		{
			defaultWidthField->get(defaultSizeObj, &width);
			defaultHeightField->get(defaultSizeObj, &height);
		}

		ManagedModalWindow* modalWindow = bs_new<ManagedModalWindow>(allowCloseButton, width, height, instance);
		ScriptModalWindow* nativeInstance = new (bs_alloc<ScriptModalWindow>()) ScriptModalWindow(modalWindow);
		modalWindow->setParent(nativeInstance);
	}

	void ScriptModalWindow::internal_close(ScriptModalWindow* thisPtr)
	{
		if (thisPtr->mModalWindow != nullptr)
			thisPtr->mModalWindow->close();
	}

	void ScriptModalWindow::internal_setTitle(ScriptModalWindow* thisPtr, MonoObject* title)
	{
		HString titleStr = HString::dummy();
		if (title != nullptr)
		{
			ScriptHString* textScript = ScriptHString::toNative(title);
			titleStr = *textScript->getInternal();
		}

		thisPtr->mModalWindow->setTitle(titleStr);
	}

	void ScriptModalWindow::notifyWindowDestroyed()
	{
		mModalWindow = nullptr;
	}

	void ScriptModalWindow::onAssemblyRefreshStarted()
	{
		if (mModalWindow != nullptr)
			mModalWindow->close();
	}

	UINT32 ScriptModalWindow::internal_getWidth(ScriptModalWindow* thisPtr)
	{
		if (thisPtr->mModalWindow != nullptr)
			return thisPtr->mModalWindow->getWidth();

		return 0;
	}

	UINT32 ScriptModalWindow::internal_getHeight(ScriptModalWindow* thisPtr)
	{
		if (thisPtr->mModalWindow != nullptr)
			return thisPtr->mModalWindow->getHeight();

		return 0;
	}

	void ScriptModalWindow::internal_setWidth(ScriptModalWindow* thisPtr, UINT32 value)
	{
		if (thisPtr->mModalWindow != nullptr)
			thisPtr->mModalWindow->setSize(value, thisPtr->mModalWindow->getHeight());
	}

	void ScriptModalWindow::internal_setHeight(ScriptModalWindow* thisPtr, UINT32 value)
	{
		if (thisPtr->mModalWindow != nullptr)
			thisPtr->mModalWindow->setSize(thisPtr->mModalWindow->getWidth(), value);
	}

	UINT32 ScriptModalWindow::internal_getContentWidth(ScriptModalWindow* thisPtr)
	{
		if (thisPtr->mModalWindow != nullptr)
			return thisPtr->mModalWindow->getContentWidth();

		return 0;
	}

	UINT32 ScriptModalWindow::internal_getContentHeight(ScriptModalWindow* thisPtr)
	{
		if (thisPtr->mModalWindow != nullptr)
			return thisPtr->mModalWindow->getContentHeight();

		return 0;
	}

	void ScriptModalWindow::internal_setContentWidth(ScriptModalWindow* thisPtr, UINT32 value)
	{
		if (thisPtr->mModalWindow != nullptr)
			thisPtr->mModalWindow->setSize(value, thisPtr->mModalWindow->getHeight());
	}

	void ScriptModalWindow::internal_setContentHeight(ScriptModalWindow* thisPtr, UINT32 value)
	{
		if (thisPtr->mModalWindow != nullptr)
			thisPtr->mModalWindow->setSize(thisPtr->mModalWindow->getWidth(), value);
	}

	void ScriptModalWindow::internal_screenToWindowPos(ScriptModalWindow* thisPtr, Vector2I* screenPos, Vector2I* windowPos)
	{
		if (thisPtr->mModalWindow != nullptr)
			*windowPos = thisPtr->mModalWindow->screenToWindowPos(*screenPos);
		else
			*windowPos = *screenPos;
	}

	void ScriptModalWindow::internal_windowToScreenPos(ScriptModalWindow* thisPtr, Vector2I* windowPos, Vector2I* screenPos)
	{
		if (thisPtr->mModalWindow != nullptr)
			*screenPos = thisPtr->mModalWindow->windowToScreenPos(*windowPos);
		else
			*screenPos = *windowPos;
	}

	ManagedModalWindow::ManagedModalWindow(bool allowCloseButton, UINT32 width, UINT32 height, MonoObject* managedInstance)
		: ModalWindow(HString::dummy(), allowCloseButton, width, height), mManagedInstance(managedInstance)
	{
		mGCHandle = MonoUtil::newGCHandle(mManagedInstance);
		mManagedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);

		MonoObject* guiPanel = ScriptGUIPanel::createFromExisting(mContents);
		mContentsPanel = ScriptGUILayout::toNative(guiPanel);
		ScriptModalWindow::guiPanelField->set(mManagedInstance, guiPanel);

		::MonoClass* rawMonoClass = MonoUtil::getClass(mManagedInstance);
		MonoClass* monoClass = MonoManager::instance().findClass(rawMonoClass);

		mNamespace = monoClass->getNamespace();
		mTypename = monoClass->getTypeName();

		reloadMonoTypes(monoClass);
	}

	ManagedModalWindow::~ManagedModalWindow()
	{
		if (mGCHandle != 0)
			close();
	}

	bool ManagedModalWindow::createManagedInstance()
	{
		MonoAssembly* assembly = MonoManager::instance().getAssembly(EDITOR_ASSEMBLY);

		if (assembly != nullptr)
		{
			MonoClass* editorWindowClass = assembly->getClass(mNamespace, mTypename);

			if (editorWindowClass != nullptr)
			{
				mManagedInstance = editorWindowClass->createInstance(false);
				mGCHandle = MonoUtil::newGCHandle(mManagedInstance);
				mManagedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);

				MonoObject* guiPanel = ScriptGUIPanel::createFromExisting(mContents);
				mContentsPanel = ScriptGUILayout::toNative(guiPanel);
				ScriptModalWindow::guiPanelField->set(mManagedInstance, guiPanel);

				reloadMonoTypes(editorWindowClass);
				return true;
			}
		}

		return false;
	}

	void ManagedModalWindow::releaseManagedInstance()
	{
		MonoUtil::freeGCHandle(mGCHandle);
		mGCHandle = 0;
	}

	void ManagedModalWindow::triggerOnInitialize()
	{
		if (mOnInitializeThunk != nullptr && mManagedInstance != nullptr)
		{
			// Note: Not calling virtual methods. Can be easily done if needed but for now doing this
			// for some extra speed.
			MonoUtil::invokeThunk(mOnInitializeThunk, mManagedInstance);
		}
	}

	void ManagedModalWindow::triggerOnDestroy()
	{
		if (mOnDestroyThunk != nullptr && mManagedInstance != nullptr)
		{
			// Note: Not calling virtual methods. Can be easily done if needed but for now doing this
			// for some extra speed.
			MonoUtil::invokeThunk(mOnDestroyThunk, mManagedInstance);
		}
	}

	void ManagedModalWindow::setParent(ScriptModalWindow* parent)
	{
		mScriptParent = parent;
	}

	void ManagedModalWindow::update()
	{
		if (!mIsInitialized)
		{
			triggerOnInitialize();
			mIsInitialized = true;
		}

		if (mUpdateThunk != nullptr && mManagedInstance != nullptr)
		{
			// Note: Not calling virtual methods. Can be easily done if needed but for now doing this
			// for some extra speed.
			MonoUtil::invokeThunk(mUpdateThunk, mManagedInstance);
		}
	}

	void ManagedModalWindow::resized()
	{
		UINT32 width = getWidth();
		UINT32 height = getHeight();

		if (mOnWindowResizedMethod != nullptr && mManagedInstance != nullptr)
		{
			void* params[] = { &width, &height };
			mOnWindowResizedMethod->invokeVirtual(mManagedInstance, params);
		}

		ModalWindow::resized();
	}

	void ManagedModalWindow::close()
	{
		triggerOnDestroy();

		mContentsPanel->destroy();
		mContentsPanel = nullptr;

		releaseManagedInstance();

		mScriptParent->notifyWindowDestroyed();

		ModalWindow::close();
	}

	void ManagedModalWindow::reloadMonoTypes(MonoClass* windowClass)
	{
		MonoMethod* updateMethod = windowClass->getMethod("OnEditorUpdate", 0);

		if (updateMethod != nullptr)
			mUpdateThunk = (UpdateThunkDef)updateMethod->getThunk();

		MonoMethod* onInitializeMethod = windowClass->getMethod("OnInitialize", 0);

		if (onInitializeMethod != nullptr)
			mOnInitializeThunk = (OnInitializeThunkDef)onInitializeMethod->getThunk();

		MonoMethod* onDestroyMethod = windowClass->getMethod("OnDestroy", 0);

		if (onDestroyMethod != nullptr)
			mOnDestroyThunk = (OnDestroyThunkDef)onDestroyMethod->getThunk();

		MonoClass* modalWindowClass = windowClass->getBaseClass();
		mOnWindowResizedMethod = modalWindowClass->getMethod("OnWindowResized", 2);
	}
}