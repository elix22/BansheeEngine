//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Handles/BsHandleSliderLine.h"
#include "Handles/BsHandleManager.h"
#include "Handles/BsHandleSliderManager.h"
#include "Math/BsCapsule.h"
#include "Math/BsLineSegment3.h"
#include "Math/BsSphere.h"
#include "Math/BsRay.h"
#include "Renderer/BsCamera.h"

namespace bs
{
	const float HandleSliderLine::CAPSULE_RADIUS = 0.05f;
	const float HandleSliderLine::SPHERE_RADIUS = 0.2f;

	HandleSliderLine::HandleSliderLine(const Vector3& direction, float length, bool fixedScale, UINT64 layer)
		: HandleSlider(fixedScale, layer), mDirection(Vector3::normalize(direction)), mLength(length), mDelta(0.0f)
		, mStartPosition(BsZero)
	{
		Vector3 start = Vector3::ZERO;
		Vector3 end = start + mDirection * length;

		Vector3 sphereCenter = start + mDirection * std::max(0.0f, length - SPHERE_RADIUS);

		mCapsuleCollider = Capsule(LineSegment3(start, end), CAPSULE_RADIUS);
		mSphereCollider = Sphere(sphereCenter, SPHERE_RADIUS);

		HandleSliderManager& sliderManager = HandleManager::instance().getSliderManager();
		sliderManager._registerSlider(this);
	}

	HandleSliderLine::~HandleSliderLine()
	{
		HandleSliderManager& sliderManager = HandleManager::instance().getSliderManager();
		sliderManager._unregisterSlider(this);
	}

	bool HandleSliderLine::intersects(const Vector2I& screenPos, const Ray& ray, float& t) const
	{
		Ray localRay = ray;
		localRay.transformAffine(getTransformInv());

		auto capsuleIntersect = mCapsuleCollider.intersects(localRay);
		auto sphereIntersect = mSphereCollider.intersects(localRay);

		t = std::numeric_limits<float>::max();
		bool gotIntersect = false;

		if (capsuleIntersect.first)
		{
			t = capsuleIntersect.second;
			gotIntersect = true;
		}

		if (sphereIntersect.first)
		{
			if (sphereIntersect.second < t)
			{
				t = sphereIntersect.second;
				gotIntersect = true;
			}
		}

		if (gotIntersect)
		{
			Vector3 intrPoint = localRay.getPoint(t);
			intrPoint = getTransform().multiplyAffine(intrPoint);
			t = (intrPoint - ray.getOrigin()).length(); // Get distance in world space
		}

		return gotIntersect;
	}

	void HandleSliderLine::handleInput(const SPtr<Camera>& camera, const Vector2I& inputDelta)
	{
		assert(getState() == State::Active);

		mCurrentPointerPos += inputDelta;

		Vector3 worldDir = getRotation().rotate(mDirection);
		mDelta = calcDelta(camera, mStartPosition, worldDir, mStartPointerPos, mCurrentPointerPos);

		// Movement in screen space will cover larger range in world space if the camera is further away
		float dist = (camera->getTransform().getPosition() - mStartPosition).length();

		float arbitraryScale = 1.0f / 5.0f;
		mDelta *= dist * arbitraryScale;
	}
}
