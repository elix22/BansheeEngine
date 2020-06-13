//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Handles/BsHandleManager.h"
#include "Handles/BsHandleDrawManager.h"
#include "Handles/BsHandleSliderManager.h"
#include "Components/BsCCamera.h"
#include "Scene/BsSceneObject.h"
#include "Settings/BsEditorSettings.h"
#include "Utility/BsTime.h"

namespace bs
{
	HandleManager::HandleManager()
	{
		mSliderManager = bs_new<HandleSliderManager>();
		mDrawManager = bs_new<HandleDrawManager>();
	}

	HandleManager::~HandleManager()
	{
		bs_delete(mSliderManager);
		bs_delete(mDrawManager);
	}

	bool HandleManager::isHandleActive(const SPtr<Camera>& camera) const
	{
		return mSliderManager->isSliderActive(camera);
	}

	void HandleManager::beginInput()
	{
		triggerPreInput();
		mInputStarted = true;
	}

	bool HandleManager::updateInput(const SPtr<Camera>& camera, const Vector2I& inputPos, const Vector2I& inputDelta)
	{
		if(!mInputStarted)
		{
			BS_LOG(Warning, Editor, "Updating handle input without calling beginInput() first. Input won't be processed.");
			return false;
		}

		if (mSettings != nullptr && mSettingsHash != mSettings->getHash())
			updateFromEditorSettings();

		return mSliderManager->update(camera, inputPos, inputDelta);
	}

	void HandleManager::endInput()
	{
		triggerPostInput();
		mInputStarted = false;
	}

	void HandleManager::draw(const SPtr<Camera>& camera)
	{
		UINT64 frameIdx = gTime().getFrameIdx();
		if (frameIdx != mLastDrawFrameIdx)
		{
			mDrawManager->clear();
			queueDrawCommands();
			mLastDrawFrameIdx = frameIdx;
		}

		mDrawManager->draw(camera);
	}

	void HandleManager::setSettings(const SPtr<EditorSettings>& settings)
	{
		mSettings = settings;

		updateFromEditorSettings();
	}

	void HandleManager::updateFromEditorSettings()
	{
		setDefaultHandleSize(mSettings->getHandleSize());

		mSettingsHash = mSettings->getHash();
	}

	bool HandleManager::trySelect(const SPtr<Camera>& camera, const Vector2I& inputPos)
	{
		return mSliderManager->trySelect(camera, inputPos);
	}

	void HandleManager::clearSelection(const SPtr<Camera>& camera)
	{
		return mSliderManager->clearSelection(camera);
	}

	float HandleManager::getHandleSize(const SPtr<Camera>& camera, const Vector3& handlePos) const
	{
		if (camera->getProjectionType() == PT_PERSPECTIVE)
		{
			const Transform& tfrm = camera->getTransform();
			Vector3 cameraPos = tfrm.getPosition();

			Vector3 diff = handlePos - cameraPos;
			float distAlongViewDir = Math::abs(diff.dot(tfrm.getRotation().zAxis()));

			return distAlongViewDir * mDefaultHandleSize;
		}
		else
		{
			return camera->getOrthoWindowHeight() * mDefaultHandleSize;
		}
	}
}