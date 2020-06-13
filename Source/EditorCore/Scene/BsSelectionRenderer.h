//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Renderer/BsRendererExtension.h"
#include "Image/BsColor.h"
#include "Math/BsMatrix4.h"
#include "RenderAPI/BsGpuParam.h"

namespace bs
{
	/** @addtogroup Scene-Editor-Internal
	 *  @{
	 */

	namespace ct { class SelectionRendererCore; }

	/**	Handles rendering of the selected SceneObject%s overlay. */
	class BS_ED_EXPORT SelectionRenderer
	{
	public:
		SelectionRenderer();
		~SelectionRenderer();

		/** Called once per frame. Updates the overlay depending on current selection. */
		void update(const SPtr<Camera>& camera);

	private:
		friend class ct::SelectionRendererCore;

		SPtr<ct::SelectionRendererCore> mRenderer;
	};

	namespace ct
	{
	/** Core thread version of the selection renderer, that handles actual rendering. */
	class SelectionRendererCore : public RendererExtension
	{
		friend class bs::SelectionRenderer;

	public:
		SelectionRendererCore();

	private:
		/** @copydoc RendererExtension::initialize */
		void initialize(const Any& data) override;

		/**	@copydoc RendererExtension::check */
		RendererExtensionRequest check(const Camera& camera) override;

		/**	@copydoc RendererExtension::render */
		void render(const Camera& camera, const RendererViewContext& viewContext) override;

		/**
		 * Updates the internal data that determines what will be rendered on the next render() call.
		 *
		 * @param[in]	camera		Camera to render the selection overlay in.
		 * @param[in]	objects		A set of objects to render with the selection overlay.
		 */
		void updateData(const SPtr<Camera>& camera, const Vector<SPtr<Renderable>>& objects);

		Vector<SPtr<Renderable>> mObjects;
		SPtr<Camera> mCamera;

		// Immutable
		SPtr<Material> mMaterial;
		SPtr<GpuParamsSet> mParams[4];
		GpuParamMat4 mMatWorldViewProj[4];
		GpuParamColor mColor[4];
		GpuParamBuffer mBoneMatrices[4];

		UINT32 mTechniqueIndices[4];

		static const Color SELECTION_COLOR;
	};
	}

	/** @} */
}