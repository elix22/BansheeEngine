//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Utility/BsBuiltinEditorResources.h"
#include "Resources/BsBuiltinResources.h"
#include "GUI/BsGUIElementStyle.h"
#include "Image/BsSpriteTexture.h"
#include "Text/BsFont.h"
#include "Image/BsTexture.h"
#include "Material/BsShader.h"
#include "Material/BsMaterial.h"
#include "Resources/BsResources.h"
#include "FileSystem/BsFileSystem.h"
#include "Resources/BsResourceManifest.h"
#include "FileSystem/BsDataStream.h"
#include "GUI/BsGUITooltip.h"
#include "Resources/BsBuiltinResourcesHelper.h"
#include "Importer/BsImporter.h"
#include "Importer/BsTextureImportOptions.h"
#include "Serialization/BsFileSerializer.h"
#include "BsEditorApplication.h"

using json = nlohmann::json;

namespace bs
{
	const String BuiltinEditorResources::ObjectFieldStyleName = "GUIObjectField";
	const String BuiltinEditorResources::ObjectFieldLabelStyleName = "EditorFieldLabel";
	const String BuiltinEditorResources::ObjectFieldDropBtnStyleName = "DropButton";
	const String BuiltinEditorResources::ObjectFieldClearBtnStyleName = "ObjectClearButton";

	const String BuiltinEditorResources::TextureFieldStyleName = "GUITextureField";
	const String BuiltinEditorResources::TextureFieldLabelStyleName = "TextureFieldLabel";
	const String BuiltinEditorResources::TextureFieldDropStyleName = "TextureDrop";
	const String BuiltinEditorResources::TextureFieldClearBtnStyleName = "TextureClearButton";

	const String BuiltinEditorResources::DefaultFontFilename = "arial.ttf";
	const String BuiltinEditorResources::DefaultAAFontFilename = "arialAA.ttf";
	const UINT32 BuiltinEditorResources::DefaultFontSize = 8;
	const UINT32 BuiltinEditorResources::TitleFontSize = 16;

	const Color BuiltinEditorResources::TextNormalColor = Color(0.7f, 0.7f, 0.7f);
	const Color BuiltinEditorResources::TextActiveColor = Color(0.0f, 0.0f, 0.0f);

	const String BuiltinEditorResources::GUISkinFile = "GUISkin";

	const char* BuiltinEditorResources::ShaderFolder = "Shaders/";
	const char* BuiltinEditorResources::SkinFolder = "Skin/";
	const char* BuiltinEditorResources::IconFolder = "Icons/";
	const char* BuiltinEditorResources::Icon3DFolder = "Icons3D/";
	const char* BuiltinEditorResources::AnimatedSpritesFolder = "AnimatedSprites/";
	const char* BuiltinEditorResources::ShaderIncludeFolder = "Shaders/Includes/";
	const char* BuiltinEditorResources::SpriteSubFolder = "Sprites/";

	const String BuiltinEditorResources::FolderIconTex = u8"FolderIcon.psd";
	const String BuiltinEditorResources::MeshIconTex = u8"MeshIcon.psd";
	const String BuiltinEditorResources::TextureIconTex = u8"TextureIcon.psd";
	const String BuiltinEditorResources::FontIconTex = u8"FontIcon.psd";
	const String BuiltinEditorResources::PlainTextIconTex = u8"TextIcon.psd";
	const String BuiltinEditorResources::ScriptCodeIconTex = u8"CSharpIcon.psd";
	const String BuiltinEditorResources::ShaderIconTex = u8"ShaderIcon.psd";
	const String BuiltinEditorResources::ShaderIncludeIconTex = u8"ShaderIncludeIcon.psd";
	const String BuiltinEditorResources::MaterialIconTex = u8"MaterialIcon.psd";
	const String BuiltinEditorResources::SpriteTextureIconTex = u8"SpriteIcon.psd";
	const String BuiltinEditorResources::PrefabIconTex = u8"PrefabIcon.psd";
	const String BuiltinEditorResources::GUISkinIconTex = u8"GUISkinIcon.psd";
	const String BuiltinEditorResources::PhysicsMaterialIconTex = u8"PhysicsMaterialIcon.psd";
	const String BuiltinEditorResources::PhysicsMeshIconTex = u8"PhysicsMeshIcon.psd";
	const String BuiltinEditorResources::AudioClipIconTex = u8"AudioClipIcon.psd";
	const String BuiltinEditorResources::AnimationClipIconTex = u8"AnimationClipIcon.psd";

	const char* BuiltinEditorResources::SplashScreenName = u8"SplashScreen.png";
	const String BuiltinEditorResources::XButtonNormalTex = u8"XBtnNormal.png";

	/************************************************************************/
	/* 									SHADERS                      		*/
	/************************************************************************/

	const String BuiltinEditorResources::ShaderDockOverlayFile = u8"DockDropOverlay.bsl";
	const String BuiltinEditorResources::ShaderSceneGridFile = u8"SceneGrid.bsl";
	const String BuiltinEditorResources::ShaderPickingCullNoneFile = u8"PickingCullNone.bsl";
	const String BuiltinEditorResources::ShaderPickingCullCWFile = u8"PickingCullCW.bsl";
	const String BuiltinEditorResources::ShaderPickingCullCCWFile = u8"PickingCullCCW.bsl";
	const String BuiltinEditorResources::ShaderPickingAlphaCullNoneFile = u8"PickingAlphaCullNone.bsl";
	const String BuiltinEditorResources::ShaderPickingAlphaCullCWFile = u8"PickingAlphaCullCW.bsl";
	const String BuiltinEditorResources::ShaderPickingAlphaCullCCWFile = u8"PickingAlphaCullCCW.bsl";
	const String BuiltinEditorResources::ShaderLineGizmoFile = u8"LineGizmo.bsl";
	const String BuiltinEditorResources::ShaderWireGizmoFile = u8"WireGizmo.bsl";
	const String BuiltinEditorResources::ShaderSolidGizmoFile = u8"SolidGizmo.bsl";
	const String BuiltinEditorResources::ShaderLineHandleFile = u8"LineHandle.bsl";
	const String BuiltinEditorResources::ShaderSolidHandleFile = u8"SolidHandle.bsl";
	const String BuiltinEditorResources::ShaderHandleClearAlphaFile = u8"ClearHandleAlpha.bsl";
	const String BuiltinEditorResources::ShaderIconGizmoFile = u8"IconGizmo.bsl";
	const String BuiltinEditorResources::ShaderGizmoPickingFile = u8"GizmoPicking.bsl";
	const String BuiltinEditorResources::ShaderGizmoPickingAlphaFile = u8"GizmoPickingAlpha.bsl";
	const String BuiltinEditorResources::ShaderTextGizmoFile = u8"TextGizmo.bsl";
	const String BuiltinEditorResources::ShaderSelectionFile = u8"Selection.bsl";

	/************************************************************************/
	/* 								OTHER							  		*/
	/************************************************************************/
	const String BuiltinEditorResources::EmptyShaderCodeFile = u8"EmptyShaderText.txt";
	const String BuiltinEditorResources::EmptyCSScriptCodeFile = u8"EmptyCSScriptText.txt";

	BuiltinEditorResources::BuiltinEditorResources()
	{
		// Set up paths
		BuiltinRawDataFolder = Paths::getEditorDataPath() + "Raw/";
		EditorRawSkinFolder = BuiltinRawDataFolder + SkinFolder;
		EditorRawIconsFolder = BuiltinRawDataFolder + IconFolder;
		EditorRawShaderFolder = BuiltinRawDataFolder + ShaderFolder;
		EditorRawShaderIncludeFolder = BuiltinRawDataFolder + ShaderIncludeFolder;

		BuiltinDataFolder = Paths::getEditorDataPath();
		EditorSkinFolder = BuiltinDataFolder + SkinFolder;
		EditorSkinSpritesFolder = EditorSkinFolder + SpriteSubFolder;
		EditorIconFolder = BuiltinDataFolder + IconFolder;
		EditorIcon3DFolder = BuiltinDataFolder + Icon3DFolder;
		EditorIconSpritesFolder = EditorIconFolder + SpriteSubFolder;
		EditorIcon3DSpritesFolder = EditorIcon3DFolder + SpriteSubFolder;
		EditorAnimatedSpritesFolder = BuiltinDataFolder + AnimatedSpritesFolder + SpriteSubFolder;
		EditorShaderFolder = BuiltinDataFolder + ShaderFolder;
		EditorShaderIncludeFolder = BuiltinDataFolder + ShaderIncludeFolder;

		ResourceManifestPath = BuiltinDataFolder + "ResourceManifest.asset";

		// Load manifest
		if (FileSystem::exists(ResourceManifestPath))
			mResourceManifest = ResourceManifest::load(ResourceManifestPath, BuiltinDataFolder);

		if (mResourceManifest == nullptr)
			mResourceManifest = ResourceManifest::create("BuiltinResources");

		gResources().registerResourceManifest(mResourceManifest);

		// Load basic resources
		mShaderDockOverlay = getShader(ShaderDockOverlayFile);
		mShaderSceneGrid = getShader(ShaderSceneGridFile);
		mShaderPicking[(int)CULL_NONE] = getShader(ShaderPickingCullNoneFile);
		mShaderPicking[(int)CULL_CLOCKWISE] = getShader(ShaderPickingCullCWFile);
		mShaderPicking[(int)CULL_COUNTERCLOCKWISE] = getShader(ShaderPickingCullCCWFile);
		mShaderPickingAlpha[(int)CULL_NONE] = getShader(ShaderPickingAlphaCullNoneFile);
		mShaderPickingAlpha[(int)CULL_CLOCKWISE] = getShader(ShaderPickingAlphaCullCWFile);
		mShaderPickingAlpha[(int)CULL_COUNTERCLOCKWISE] = getShader(ShaderPickingAlphaCullCCWFile);
		mShaderGizmoSolid = getShader(ShaderSolidGizmoFile);
		mShaderGizmoLine = getShader(ShaderLineGizmoFile);
		mShaderGizmoWire = getShader(ShaderWireGizmoFile);
		mShaderGizmoIcon = getShader(ShaderIconGizmoFile);
		mShaderGizmoPicking = getShader(ShaderGizmoPickingFile);
		mShaderGizmoAlphaPicking = getShader(ShaderGizmoPickingAlphaFile);
		mShaderGizmoText = getShader(ShaderTextGizmoFile);
		mShaderHandleSolid = getShader(ShaderSolidHandleFile);
		mShaderHandleClearAlpha = getShader(ShaderHandleClearAlphaFile);
		mShaderHandleLine = getShader(ShaderLineHandleFile);
		mShaderSelection = getShader(ShaderSelectionFile);

		mDefaultFont = gResources().load<Font>(BuiltinDataFolder + (DefaultFontFilename + ".asset"));
		mDefaultAAFont = gResources().load<Font>(BuiltinDataFolder + (DefaultAAFontFilename + ".asset"));
		mSkin = gResources().load<GUISkin>(BuiltinDataFolder + (GUISkinFile + ".json.asset"));
	}

	HSpriteTexture BuiltinEditorResources::getGUIIcon(const String& name) const
	{
		Path texturePath = EditorIconSpritesFolder;
		texturePath.append("sprite_" + name + ".asset");

		return gResources().load<SpriteTexture>(texturePath);
	}

	HSpriteTexture BuiltinEditorResources::getGUIIcon3D(const String& name) const
	{
		Path texturePath = EditorIcon3DSpritesFolder;
		texturePath.append("sprite_" + name + ".asset");

		return gResources().load<SpriteTexture>(texturePath);
	}

	HSpriteTexture BuiltinEditorResources::getAnimatedSprite(const String& name) const
	{
		Path texturePath = EditorAnimatedSpritesFolder;
		texturePath.append("sprite_" + name + ".asset");

		return gResources().load<SpriteTexture>(texturePath);
	}

	HShader BuiltinEditorResources::getShader(const String& name) const
	{
		Path programPath = EditorShaderFolder;
		programPath.append(name + ".asset");

		HShader shader = gResources().load<Shader>(programPath);

#if BS_DEBUG_MODE
		BuiltinResourcesHelper::verifyAndReportShader(shader);
#endif

		return shader;
	}

	HMaterial BuiltinEditorResources::createDockDropOverlayMaterial() const
	{
		return Material::create(mShaderDockOverlay);
	}

	HMaterial BuiltinEditorResources::createSceneGridMaterial() const
	{
		return Material::create(mShaderSceneGrid);
	}

	HMaterial BuiltinEditorResources::createPicking(CullingMode cullMode) const
	{
		UINT32 modeIdx = (UINT32)cullMode;

		return Material::create(mShaderPicking[modeIdx]);
	}

	HMaterial BuiltinEditorResources::createPickingAlpha(CullingMode cullMode) const
	{
		UINT32 modeIdx = (UINT32)cullMode;

		return Material::create(mShaderPickingAlpha[modeIdx]);
	}

	HMaterial BuiltinEditorResources::createLineGizmoMat() const
	{
		return Material::create(mShaderGizmoLine);
	}

	HMaterial BuiltinEditorResources::createSolidGizmoMat() const
	{
		return Material::create(mShaderGizmoSolid);
	}

	HMaterial BuiltinEditorResources::createWireGizmoMat() const
	{
		return Material::create(mShaderGizmoWire);
	}

	HMaterial BuiltinEditorResources::createIconGizmoMat() const
	{
		return Material::create(mShaderGizmoIcon);
	}

	HMaterial BuiltinEditorResources::createGizmoPickingMat() const
	{
		return Material::create(mShaderGizmoPicking);
	}

	HMaterial BuiltinEditorResources::createAlphaGizmoPickingMat() const
	{
		return Material::create(mShaderGizmoAlphaPicking);
	}

	HMaterial BuiltinEditorResources::createLineHandleMat() const
	{
		return Material::create(mShaderHandleLine);
	}

	HMaterial BuiltinEditorResources::createTextGizmoMat() const
	{
		return Material::create(mShaderGizmoText);
	}

	HMaterial BuiltinEditorResources::createSolidHandleMat() const
	{
		return Material::create(mShaderHandleSolid);
	}

	HMaterial BuiltinEditorResources::createHandleClearAlphaMat() const
	{
		return Material::create(mShaderHandleClearAlpha);
	}

	HMaterial BuiltinEditorResources::createSelectionMat() const
	{
		return Material::create(mShaderSelection);
	}

	HSpriteTexture BuiltinEditorResources::getProjectLibraryIcon(ProjectLibraryIcon icon, int size) const
	{
		String iconName;

		switch (icon)
		{
		case ProjectLibraryIcon::Folder:
			iconName = FolderIconTex;
			break;
		case ProjectLibraryIcon::Font:
			iconName = FontIconTex;
			break;
		case ProjectLibraryIcon::Mesh:
			iconName = MeshIconTex;
			break;
		case ProjectLibraryIcon::Texture:
			iconName = TextureIconTex;
			break;
		case ProjectLibraryIcon::PlainText:
			iconName = PlainTextIconTex;
			break;
		case ProjectLibraryIcon::ScriptCode:
			iconName = ScriptCodeIconTex;
			break;
		case ProjectLibraryIcon::Shader:
			iconName = ShaderIconTex;
			break;
		case ProjectLibraryIcon::ShaderInclude:
			iconName = ShaderIncludeIconTex;
			break;
		case ProjectLibraryIcon::Material:
			iconName = MaterialIconTex;
			break;
		case ProjectLibraryIcon::SpriteTexture:
			iconName = SpriteTextureIconTex;
			break;
		case ProjectLibraryIcon::Prefab:
			iconName = PrefabIconTex;
			break;
		case ProjectLibraryIcon::GUISkin:
			iconName = GUISkinIconTex;
			break;
		case ProjectLibraryIcon::PhysicsMaterial:
			iconName = PhysicsMaterialIconTex;
			break;
		case ProjectLibraryIcon::PhysicsMesh:
			iconName = PhysicsMeshIconTex;
			break;
		case ProjectLibraryIcon::AudioClip:
			iconName = AudioClipIconTex;
			break;
		case ProjectLibraryIcon::AnimationClip:
			iconName = AnimationClipIconTex;
			break;
		case ProjectLibraryIcon::VectorField:
			iconName = SpriteTextureIconTex;
			break;
		}

		if (iconName.empty())
			return HSpriteTexture();

		if (size <= 16)
			iconName += "16";
		else if (size <= 32)
			iconName += "32";
		else if (size <= 48)
			iconName += "48";

		return getGUIIcon(iconName);
	}

	HSpriteTexture BuiltinEditorResources::getToolbarIcon(ToolbarIcon icon) const
	{
		switch (icon)
		{
		case ToolbarIcon::NewCamera:
			return getGUIIcon("ToolbarNewCameraIcon.png");
		case ToolbarIcon::NewRenderable:
			return getGUIIcon("ToolbarNewRenderableIcon.png");
		case ToolbarIcon::NewPointLight:
			return getGUIIcon("ToolbarNewPointLightIcon.png");
		case ToolbarIcon::NewDirLight:
			return getGUIIcon("ToolbarNewDirectionalLightIcon.png");
		case ToolbarIcon::NewSpotLight:
			return getGUIIcon("ToolbarNewSpotLightIcon.png");
		case ToolbarIcon::NewSceneObject:
			return getGUIIcon("ToolbarNewSceneObjectIcon.png");
		case ToolbarIcon::NewCube:
			return getGUIIcon("ToolbarNewCubeIcon.png");
		case ToolbarIcon::NewSphere:
			return getGUIIcon("ToolbarNewSphereIcon.png");
		case ToolbarIcon::NewCone:
			return getGUIIcon("ToolbarNewConeIcon.png");
		case ToolbarIcon::NewQuad:
			return getGUIIcon("ToolbarNewQuadIcon.png");
		case ToolbarIcon::NewMat:
			return getGUIIcon("ToolbarNewMaterialIcon.png");
		case ToolbarIcon::NewCSScript:
			return getGUIIcon("ToolbarNewCSScriptIcon.png");
		case ToolbarIcon::NewShader:
			return getGUIIcon("ToolbarNewShaderIcon.png");
		case ToolbarIcon::NewSpriteTex:
			return getGUIIcon("ToolbarNewSpriteTextureIcon.png");
		case ToolbarIcon::Pause:
			return getGUIIcon("ToolbarPauseIcon.png");
		case ToolbarIcon::Play:
			return getGUIIcon("ToolbarPlayIcon.png");
		case ToolbarIcon::Step:
			return getGUIIcon("ToolbarStepIcon.png");
		case ToolbarIcon::Undo:
			return getGUIIcon("ToolbarUndoIcon.png");
		case ToolbarIcon::Redo:
			return getGUIIcon("ToolbarRedoIcon.png");
		case ToolbarIcon::OpenProject:
			return getGUIIcon("ToolbarOpenProjectIcon.png");
		case ToolbarIcon::SaveProject:
			return getGUIIcon("ToolbarSaveProjectIcon.png");
		case ToolbarIcon::SaveScene:
			return getGUIIcon("ToolbarSaveSceneIcon.png");
		}

		return HSpriteTexture();
	}

	GUIContentImages BuiltinEditorResources::getSceneWindowIcon(SceneWindowIcon icon) const
	{
		HSpriteTexture off;
		HSpriteTexture on;

		switch (icon)
		{
		case SceneWindowIcon::View:
			off = getGUIIcon("SceneViewIcon.png");
			on = getGUIIcon("SceneViewIconOn.png");
			break;
		case SceneWindowIcon::Move:
			off = getGUIIcon("SceneMoveIcon.png");
			on = getGUIIcon("SceneMoveIconOn.png");
			break;
		case SceneWindowIcon::Rotate:
			off = getGUIIcon("SceneRotateIcon.png");
			on = getGUIIcon("SceneRotateIconOn.png");
			break;
		case SceneWindowIcon::Scale:
			off = getGUIIcon("SceneScaleIcon.png");
			on = getGUIIcon("SceneScaleIconOn.png");
			break;
		case SceneWindowIcon::Pivot:
			off = getGUIIcon("ScenePivotIcon.png");
			on = getGUIIcon("ScenePivotIconOn.png");
			break;
		case SceneWindowIcon::Center:
			off = getGUIIcon("SceneCenterIcon.png");
			on = getGUIIcon("SceneCenterIconOn.png");
			break;
		case SceneWindowIcon::Local:
			off = getGUIIcon("SceneLocalIcon.png");
			on = getGUIIcon("SceneLocalIconOn.png");
			break;
		case SceneWindowIcon::World:
			off = getGUIIcon("SceneWorldIcon.png");
			on = getGUIIcon("SceneWorldIconOn.png");
			break;
		case SceneWindowIcon::MoveSnap:
			off = getGUIIcon("SceneMoveSnapIcon.png");
			on = getGUIIcon("SceneMoveSnapIconOn.png");
			break;
		case SceneWindowIcon::RotateSnap:
			off = getGUIIcon("SceneRotateSnapIcon.png");
			on = getGUIIcon("SceneRotateSnapIconOn.png");
			break;
		case SceneWindowIcon::SceneCameraOptions:
			off = getGUIIcon("SceneCameraOptionsIcon.png");
			on = getGUIIcon("SceneCameraOptionsIcon.png");
			break;
		}

		GUIContentImages output;
		output.normal = off;
		output.hover = off;
		output.active = on;
		output.focused = off;
		output.normalOn = on;
		output.hoverOn = on;
		output.activeOn = on;
		output.focusedOn = on;

		return output;
	}

	HSpriteTexture BuiltinEditorResources::getSceneViewIcon(SceneViewIcon icon) const
	{
		switch (icon)
		{
		case SceneViewIcon::AudioListener:
			return getGUIIcon3D("SceneAudioListener.png");
		case SceneViewIcon::AudioSource:
			return getGUIIcon3D("SceneAudioSource.png");
		case SceneViewIcon::Camera:
			return getGUIIcon3D("SceneCamera.png");
		case SceneViewIcon::Decal:
			return getGUIIcon3D("SceneDecal.png");
		case SceneViewIcon::Light:
			return getGUIIcon3D("SceneLight.png");
		case SceneViewIcon::LightProbes:
			return getGUIIcon3D("SceneLightProbes.png");
		case SceneViewIcon::ParticleSystem:
			return getGUIIcon3D("SceneParticleSystem.png");
		case SceneViewIcon::ReflectionProbe:
			return getGUIIcon3D("SceneReflectionProbe.png");
		}

		return HSpriteTexture();
	}

	HSpriteTexture BuiltinEditorResources::getLibraryWindowIcon(LibraryWindowIcon icon) const
	{
		switch (icon)
		{
		case LibraryWindowIcon::Home:
			return getGUIIcon("LibraryHomeIcon.png");
		case LibraryWindowIcon::Up:
			return getGUIIcon("LibraryUpIcon.png");
		case LibraryWindowIcon::Clear:
			return getGUIIcon("LibraryClearSearchIcon.png");
		case LibraryWindowIcon::Options:
			return getGUIIcon("LibraryOptionsIcon.png");
		}

		return HSpriteTexture();
	}

	HSpriteTexture BuiltinEditorResources::getInspectorWindowIcon(InspectorWindowIcon icon) const
	{
		switch (icon)
		{
		case InspectorWindowIcon::Create:
			return getGUIIcon("InspectorCreateIcon.png");
		case InspectorWindowIcon::Clone:
			return getGUIIcon("InspectorCloneIcon.png");
		case InspectorWindowIcon::Clear:
			return getGUIIcon("InspectorClearIcon.png");
		case InspectorWindowIcon::Resize:
			return getGUIIcon("InspectorResizeIcon.png");
		case InspectorWindowIcon::Delete:
			return getGUIIcon("InspectorDeleteIcon.png");
		case InspectorWindowIcon::MoveUp:
			return getGUIIcon("InspectorMoveUpIcon.png");
		case InspectorWindowIcon::MoveDown:
			return getGUIIcon("InspectorMoveDownIcon.png");
		case InspectorWindowIcon::Edit:
			return getGUIIcon("InspectorEditIcon.png");
		case InspectorWindowIcon::Apply:
			return getGUIIcon("InspectorApplyIcon.png");
		case InspectorWindowIcon::Add:
			return getGUIIcon("InspectorAddIcon.png");
		case InspectorWindowIcon::Cancel:
			return getGUIIcon("InspectorCancelIcon.png");
		}

		return HSpriteTexture();
	}

	GUIContentImages BuiltinEditorResources::getAnimationWindowIcon(AnimationWindowIcon icon) const
	{
		HSpriteTexture off;
		HSpriteTexture on;

		switch (icon)
		{
		case AnimationWindowIcon::Play:
			off = getGUIIcon("AnimationPlayIcon.png");
			on = getGUIIcon("AnimationPlayIconOn.png");
			break;
		case AnimationWindowIcon::Record:
			off = getGUIIcon("AnimationRecordIcon.png");
			on = getGUIIcon("AnimationRecordIconOn.png");
			break;
		case AnimationWindowIcon::FrameForward:
			off = getGUIIcon("AnimationFrameForwardIcon.png");
			on = getGUIIcon("AnimationFrameForwardIconOn.png");
			break;
		case AnimationWindowIcon::FrameBack:
			off = getGUIIcon("AnimationFrameBackIcon.png");
			on = getGUIIcon("AnimationFrameBackIconOn.png");
			break;
		case AnimationWindowIcon::AddEvent:
			off = getGUIIcon("AnimationAddEventIcon.png");
			on = getGUIIcon("AnimationAddEventIconOn.png");
			break;
		case AnimationWindowIcon::AddKeyframe:
			off = getGUIIcon("AnimationAddKeyframeIcon.png");
			on = getGUIIcon("AnimationAddKeyframeIconOn.png");
			break;
		case AnimationWindowIcon::Event:
			off = getGUIIcon("AnimationEventIcon.png");
			on = off;
			break;
		case AnimationWindowIcon::Keyframe:
			off = getGUIIcon("AnimationKeyframeIcon.png");
			on = off;
			break;
		}

		GUIContentImages output;
		output.normal = off;
		output.hover = off;
		output.active = on;
		output.focused = off;
		output.normalOn = on;
		output.hoverOn = on;
		output.activeOn = on;
		output.focusedOn = on;

		return output;
	}

	HSpriteTexture BuiltinEditorResources::getEditorIcon(EditorIcon icon) const
	{
		switch (icon)
		{
		case EditorIcon::X:
			return getGUIIcon(XButtonNormalTex);
		case EditorIcon::Component:
			return getGUIIcon("IconComponent.png");
		case EditorIcon::SceneObject:
			return getGUIIcon("IconSceneObject.png");
		}

		return HSpriteTexture();
	}

	GUIContentImages BuiltinEditorResources::getEditorToggleIcon(EditorToggleIcon icon) const
	{
		HSpriteTexture off;
		HSpriteTexture on;

		switch (icon)
		{
		case EditorToggleIcon::AnimateProperty:
			off = getGUIIcon("AnimPropertyIcon.png");
			on = getGUIIcon("AnimPropertyIconOn.png");
			break;
		}

		GUIContentImages output;
		output.normal = off;
		output.hover = off;
		output.active = on;
		output.focused = off;
		output.normalOn = on;
		output.hoverOn = on;
		output.activeOn = on;
		output.focusedOn = on;

		return output;
	}

	HSpriteTexture BuiltinEditorResources::getLogMessageIcon(LogMessageIcon icon, UINT32 size, bool dark) const
	{
		if (size < 24) // Round to 16
		{
			if (dark)
			{
				switch (icon)
				{
				case LogMessageIcon::Info:
					return getGUIIcon("IconInfoDark.png");
				case LogMessageIcon::Warning:
					return getGUIIcon("IconWarningDark.png");
				case LogMessageIcon::Error:
					return getGUIIcon("IconErrorDark.png");
				}
			}
			else
			{
				switch (icon)
				{
				case LogMessageIcon::Info:
					return getGUIIcon("IconInfo.png");
				case LogMessageIcon::Warning:
					return getGUIIcon("IconWarning.png");
				case LogMessageIcon::Error:
					return getGUIIcon("IconError.png");
				}
			}
		}
		else // Round to 32
		{
			switch (icon)
			{
			case LogMessageIcon::Info:
				return getGUIIcon("IconInfo32.png");
			case LogMessageIcon::Warning:
				return getGUIIcon("IconWarning32.png");
			case LogMessageIcon::Error:
				return getGUIIcon("IconError32.png");
			}
		}

		return HSpriteTexture();
	}

	HSpriteTexture BuiltinEditorResources::getSprite(EditorSprites sprite)
	{
		switch(sprite)
		{
		case EditorSprites::Spinner:
			return getAnimatedSprite("Spinner.png");
		}

		return HSpriteTexture();
	}

	SPtr<PixelData> BuiltinEditorResources::getSplashScreen()
	{
		Path splashScreenPath = Paths::getEditorDataPath() + (String(SplashScreenName) + ".asset");
		FileDecoder fd(splashScreenPath);

		return std::static_pointer_cast<PixelData>(fd.decode());
	}

	String BuiltinEditorResources::getEmptyShaderCode() const
	{
		Path filePath = BuiltinDataFolder;
		filePath.append(EmptyShaderCodeFile);

		SPtr<DataStream> fileStream = FileSystem::openFile(filePath);
		if (fileStream != nullptr)
			return fileStream->getAsString();

		return StringUtil::BLANK;
	}

	String BuiltinEditorResources::getEmptyCSScriptCode() const
	{
		Path filePath = BuiltinDataFolder;
		filePath.append(EmptyCSScriptCodeFile);

		SPtr<DataStream> fileStream = FileSystem::openFile(filePath);
		if (fileStream != nullptr)
			return fileStream->getAsString();

		return StringUtil::BLANK;
	}

	Path BuiltinEditorResources::getDefaultWidgetLayoutPath()
	{
		return Paths::getEditorDataPath() + "Layout.asset";
	}
}