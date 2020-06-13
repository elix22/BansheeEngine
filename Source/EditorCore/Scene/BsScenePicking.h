//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Utility/BsModule.h"
#include "Math/BsMatrix4.h"
#include "RenderAPI/BsGpuParam.h"
#include "Renderer/BsParamBlocks.h"

namespace bs
{
	struct GizmoDrawSettings;

	/** @addtogroup Scene-Editor
	 *  @{
	 */

	/** Contains the data of a scene picking action. */
	struct SnapData
	{
		Vector3 normal;
		Vector3 pickPosition;
	};

	/** Contains the results of a scene picking action. */
	struct PickResults
	{
		Vector<UINT32> objects;
		Vector3 normal;
		float depth;
	};

	namespace ct { class ScenePicking; }

	/**	Handles picking of scene objects with a pointer in scene view. */
	class BS_ED_EXPORT ScenePicking : public Module<ScenePicking>
	{
		/**	Contains information about a single pickable item (mesh). */
		struct RenderablePickData
		{
			SPtr<ct::Mesh> mesh;
			UINT32 index;
			Matrix4 wvpTransform;
			bool alpha;
			CullingMode cullMode;
			HTexture mainTexture;
		};

	public:
		ScenePicking();
		~ScenePicking();

		/**
		 * Attempts to find a single nearest scene object under the provided position and area.
		 *
		 * @param[in]	cam					Camera to perform the picking from.
		 * @param[in]	gizmoDrawSettings	Settings used for drawing pickable gizmos.
		 * @param[in]	position			Pointer position relative to the camera viewport, in pixels.
		 * @param[in]	area				Width/height of the checked area in pixels. Use (1, 1) if you want the exact
		 *									position under the pointer.
		 * @param[in]	ignoreRenderables	A list of objects that should be ignored during scene picking.
		 * @param[out]	data				Picking data regarding position and normal.
		 * @return							Nearest SceneObject under the provided area, or an empty handle if no object is
		 *									found.
		 */
		HSceneObject pickClosestObject(const SPtr<Camera>& cam, const GizmoDrawSettings& gizmoDrawSettings,
			const Vector2I& position, const Vector2I& area, Vector<HSceneObject>& ignoreRenderables, SnapData* data = nullptr);

		/**
		 * Attempts to find all scene objects under the provided position and area. This does not mean objects occluded by
		 * other objects.
		 *
		 * @param[in]	cam					Camera to perform the picking from.
		 * @param[in]	gizmoDrawSettings	Settings used for drawing pickable gizmos.
		 * @param[in]	position			Pointer position relative to the camera viewport, in pixels.
		 * @param[in]	area				Width/height of the checked area in pixels. Use (1, 1) if you want the exact 
		 *									position under the pointer.
		 * @param[in]	ignoreRenderables	A list of objects that should be ignored during scene picking.
		 * @param[out]	data				Picking data regarding position and normal.
		 * @return							A list of SceneObject%s under the provided area.
		 */
		Vector<HSceneObject> pickObjects(const SPtr<Camera>& cam, const GizmoDrawSettings& gizmoDrawSettings,
			const Vector2I& position, const Vector2I& area, Vector<HSceneObject>& ignoreRenderables, 
			SnapData* data = nullptr);

	private:
		friend class ct::ScenePicking;

		typedef Set<RenderablePickData, std::function<bool(const RenderablePickData&, const RenderablePickData&)>> RenderableSet;

		/**	Encodes a pickable object identifier to a unique color. */
		static Color encodeIndex(UINT32 index);

		/** Decodes a color into a unique object identifier. Color should have initially been encoded with encodeIndex(). */
		static UINT32 decodeIndex(Color color);

		ct::ScenePicking* mCore;
	};

	/** @} */

	namespace ct
	{
	/** @addtogroup Scene-Editor-Internal
	 *  @{
	 */

	BS_PARAM_BLOCK_BEGIN(PickingParamBlockDef)
		BS_PARAM_BLOCK_ENTRY(Matrix4, gMatViewProj)
		BS_PARAM_BLOCK_ENTRY(Color, gColorIndex)
		BS_PARAM_BLOCK_ENTRY(float, gAlphaCutoff)
	BS_PARAM_BLOCK_END

	extern PickingParamBlockDef gPickingParamBlockDef;

	/** Core thread version of the ScenePicking manager. Handles actual rendering. */
	class ScenePicking
	{
	public:
		/**	Initializes the manager. Must be called right after construction. */
		void initialize();

		/**	Destroys the manager. Must be called right before destruction. */
		void destroy();

		/**
		 * Sets up the viewport, materials and their parameters as needed for picking. Also renders all the provided
		 * renderable objects. Must be followed by corePickingEnd(). You may call other methods after this one, but you must
		 * ensure they render proper unique pickable colors that can be resolved to SceneObject%s later.
		 *
		 * @param[in]	target			Render target to render to.
		 * @param[in]	viewportArea	Normalized area of the render target to render in.
		 * @param[in]	renderables		A set of pickable Renderable objects to render.
		 * @param[in]	position		Position of the pointer where to pick objects, in pixels relative to viewport.
		 * @param[in]	area			Width/height of the area to pick objects, in pixels.
		 */
		void corePickingBegin(const SPtr<RenderTarget>& target, const Rect2& viewportArea, 
			const bs::ScenePicking::RenderableSet& renderables, const Vector2I& position, const Vector2I& area);
		
		/**
		 * Ends picking operation started by corePickingBegin(). Render target is resolved and objects in the picked area
		 * are returned.
		 *
		 * @param[in]	target			Render target we're rendering to.
		 * @param[in]	viewportArea	Normalized area of the render target we're rendering in.
		 * @param[in]	position		Position of the pointer where to pick objects, in pixels relative to viewport.
		 * @param[in]	area			Width/height of the area to pick objects, in pixels.
		 * @param[in]	gatherSnapData	Determines whather normal & depth information will be recorded.
		 * @param[out]	asyncOp			Async operation handle that when complete will contain the results of the picking
		 *								operation in the form of Vector<SelectedObject>.
		 */
		void corePickingEnd(const SPtr<RenderTarget>& target, const Rect2& viewportArea, const Vector2I& position,
			const Vector2I& area, bool gatherSnapData, AsyncOp& asyncOp);

	private:
		friend class bs::ScenePicking;

		static const float ALPHA_CUTOFF;

		SPtr<RenderTexture> mPickingTexture;

		SPtr<Material> mMaterials[6];
		Vector<SPtr<GpuParamsSet>> mParamSets[6];
		Vector<SPtr<GpuParamBlockBuffer>> mParamBuffers;
	};

	/** @} */
	}
}
