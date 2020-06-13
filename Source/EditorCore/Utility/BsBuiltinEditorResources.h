//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "GUI/BsGUISkin.h"
#include "Utility/BsModule.h"
#include "GUI/BsGUIContent.h"
#include "BsApplication.h"

namespace bs
{
	/** @addtogroup Utility-Editor
	 *  @{
	 */

	/**	Types of valid icons used when viewing the project library. */
	enum BS_SCRIPT_EXPORT(api:bed) class ProjectLibraryIcon
	{
		Folder, Mesh, Font, Texture, PlainText, ScriptCode, SpriteTexture, Shader, ShaderInclude, Material, Prefab, GUISkin,
		PhysicsMaterial, PhysicsMesh, AudioClip, AnimationClip, VectorField
	};

	/**	Types of icons that may be displayed on the tool bar. */
	enum BS_SCRIPT_EXPORT(api:bed) class ToolbarIcon
	{
		NewCamera, NewRenderable, NewPointLight, NewDirLight, NewSpotLight, NewSceneObject, NewCube, NewSphere, NewCone, 
		NewQuad, NewMat, NewCSScript, NewShader, NewSpriteTex, Pause, Play, Step, Undo, Redo, OpenProject, SaveProject, 
		SaveScene
	};

	/** Types of icons that are used as 3D icons in the scene view. */
	enum BS_SCRIPT_EXPORT(api:bed) class SceneViewIcon
	{
		Camera, Light, AudioSource, AudioListener, Decal, ParticleSystem, LightProbes, ReflectionProbe
	};

	/**	Types of icons that may be displayed in the scene window. */
	enum BS_SCRIPT_EXPORT(api:bed) class SceneWindowIcon
	{
		View, Move, Rotate, Scale, Pivot, Center, Local, World, MoveSnap, RotateSnap, SceneCameraOptions
	};

	/**	Types of icons that may be displayed in the inspector window. */
	enum BS_SCRIPT_EXPORT(api:bed) class InspectorWindowIcon
	{
		Create, Clone, Clear, Resize, Delete, MoveUp, MoveDown, Edit, Apply, Add, Cancel
	};

	/**	Types of icons that may be displayed in the library window. */
	enum BS_SCRIPT_EXPORT(api:bed) class LibraryWindowIcon
	{
		Home, Up, Clear, Options
	};

	/**	Types of icons that may be displayed in the animation editor window. */
	enum BS_SCRIPT_EXPORT(api:bed) class AnimationWindowIcon
	{
		Play, Record, FrameForward, FrameBack, AddKeyframe, AddEvent, Keyframe, Event
	};

	/**	Types of icons used in various areas throughout the editor. */
	enum BS_SCRIPT_EXPORT(api:bed) class EditorIcon
	{
		X, Component, SceneObject
	};

	/**	Types of icons used in various areas throughout the editor, for toggleable elements. */
	enum BS_SCRIPT_EXPORT(api:bed) class EditorToggleIcon
	{
		AnimateProperty
	};

	/**	Types of icons to be used along with log messages depending on their severity. */
	enum BS_SCRIPT_EXPORT(api:bed) class LogMessageIcon
	{
		Info, Warning, Error
	};

	/** Various built-in sprite textures. */
	enum BS_SCRIPT_EXPORT(api:bed) class EditorSprites
	{
		Spinner
	};

	/**	Contains a set of built-in resources used by the editor. */
	class BS_ED_EXPORT BS_SCRIPT_EXPORT(n:EditorBuiltin,m:Utility-Editor,api:bed) 
	BuiltinEditorResources : public bs::Module<BuiltinEditorResources>
	{
	public:
		BuiltinEditorResources();

		/**	Returns the default editor GUI skin. */
		BS_SCRIPT_EXPORT(pr:getter,n:GUISkin)
		BS_NORREF const HGUISkin& getSkin() const { return mSkin; }

		/** Returns the default font used by the editor. */
		BS_SCRIPT_EXPORT(pr:getter,n:DefaultFont)
		BS_NORREF const HFont& getDefaultFont() const { return mDefaultFont; }

		/** Returns the default antialiased font used by the editor. */
		BS_SCRIPT_EXPORT(pr:getter,n:DefaultAAFont)
		BS_NORREF const HFont& getDefaultAAFont() const { return mDefaultAAFont; }

		/**	Creates a material used for docking drop overlay used by the editor. */
		HMaterial createDockDropOverlayMaterial() const;

		/**	Creates a material used for rendering the scene grid. */
		HMaterial createSceneGridMaterial() const;

		/**	Creates a material used for picking non-transparent objects in scene view. */
		HMaterial createPicking(CullingMode cullMode) const;

		/**	Creates a material used for picking transparent objects in scene view. */
		HMaterial createPickingAlpha(CullingMode cullMode) const;

		/**	Creates a material used for rendering line gizmos. */
		HMaterial createLineGizmoMat() const;

		/**	Creates a material used for rendering solid gizmos. */
		HMaterial createSolidGizmoMat() const;

		/**	Creates a material used for rendering wireframe gizmos. */
		HMaterial createWireGizmoMat() const;

		/**	Creates a material used for rendering icon gizmos. */
		HMaterial createIconGizmoMat() const;

		/**	Creates a material used for picking non-transparent gizmos. */
		HMaterial createGizmoPickingMat() const;

		/**	Creates a material used for picking transparent gizmos. */
		HMaterial createAlphaGizmoPickingMat() const;

		/**	Creates a material used for rendering line handles. */
		HMaterial createLineHandleMat() const;

		/**	Creates a material used for rendering solid handles. */
		HMaterial createSolidHandleMat() const;

		/** Creates a material used for rendering text for gizmos and handles. */
		HMaterial createTextGizmoMat() const;

		/** Creates a material used for clearing the alpha channel of the handle rendering texture. */
		HMaterial createHandleClearAlphaMat() const;

		/**	Creates a material used for displaying selected objects. */
		HMaterial createSelectionMat() const;

		/**
		 * Retrieves an icon that represents a specific resource type that may be displayed when viewing the project 
		 * library.
		 */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getProjectLibraryIcon(ProjectLibraryIcon icon, int size) const;

		/**	Retrieves an icon that may be displayed on the main window's toolbar. */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getToolbarIcon(ToolbarIcon icon) const;

		/**	Retrieves an icon that may be displayed on the scene window. */
		BS_SCRIPT_EXPORT()
		GUIContentImages getSceneWindowIcon(SceneWindowIcon icon) const;

		/**	Retrieves an icon that may be displayed in the 3D scene view. */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getSceneViewIcon(SceneViewIcon icon) const;

		/**	Retrieves an icon that may be displayed on the library window. */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getLibraryWindowIcon(LibraryWindowIcon icon) const;

		/**	Retrieves an icon that may be displayed on the inspector window. */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getInspectorWindowIcon(InspectorWindowIcon icon) const;

		/**	Retrieves an icon that may be displayed on the animation editor window. */
		BS_SCRIPT_EXPORT()
		GUIContentImages getAnimationWindowIcon(AnimationWindowIcon icon) const;

		/**	Retrieves an icon that represents a specific generic editor icon. */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getEditorIcon(EditorIcon icon) const;

		/**	Retrieves an icon that represents a specific generic editor icon used for toggleable elements. */
		BS_SCRIPT_EXPORT()
		GUIContentImages getEditorToggleIcon(EditorToggleIcon icon) const;

		/**	Retrieves an icon that represents a specific log message type. */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getLogMessageIcon(LogMessageIcon icon, UINT32 size, bool dark) const;

		/** Retrieves a builtin editor sprite texture. */
		BS_SCRIPT_EXPORT()
		BS_NORREF HSpriteTexture getSprite(EditorSprites sprite);

		/**	Returns text contained in the default "empty" shader. */
		BS_SCRIPT_EXPORT(pr:getter,n:EmptyShaderCode)
		String getEmptyShaderCode() const;

		/**	Returns text contained in the default "empty" C# script. */
		BS_SCRIPT_EXPORT(pr:getter,n:EmptyCSScriptCode)
		String getEmptyCSScriptCode() const;

		/**	Returns image data the Banshee Engine splash screen. */
		static SPtr<PixelData> getSplashScreen();

		/**	Returns absolute path to the default widget layout file. */
		static Path getDefaultWidgetLayoutPath();

		static const String ObjectFieldStyleName;
		static const String ObjectFieldLabelStyleName;
		static const String ObjectFieldDropBtnStyleName;
		static const String ObjectFieldClearBtnStyleName;

		static const String TextureFieldStyleName;
		static const String TextureFieldLabelStyleName;
		static const String TextureFieldDropStyleName;
		static const String TextureFieldClearBtnStyleName;

	private:
		/**	Loads a GUI icon with the specified filename. */
		HSpriteTexture getGUIIcon(const String& name) const;

		/**	Loads a GUI icon with the specified filename. */
		HSpriteTexture getGUIIcon3D(const String& name) const;

		/**	Loads an animated sprite with the specified filename. */
		HSpriteTexture getAnimatedSprite(const String& name) const;

		/**	Loads a shader with the specified filename */
		HShader getShader(const String& name) const;

		HShader mShaderDockOverlay;
		HShader mShaderSceneGrid;
		HShader mShaderPicking[3];
		HShader mShaderPickingAlpha[3];
		HShader mShaderGizmoSolid;
		HShader mShaderGizmoWire;
		HShader mShaderGizmoLine;
		HShader mShaderGizmoIcon;
		HShader mShaderGizmoPicking;
		HShader mShaderGizmoAlphaPicking;
		HShader mShaderGizmoText;
		HShader mShaderHandleSolid;
		HShader mShaderHandleLine;
		HShader mShaderHandleClearAlpha;
		HShader mShaderSelection;

		HFont mDefaultFont;
		HFont mDefaultAAFont;
		HGUISkin mSkin;

		SPtr<ResourceManifest> mResourceManifest;

		static const char* ShaderFolder;
		static const char* SkinFolder;
		static const char* IconFolder;
		static const char* Icon3DFolder;
		static const char* AnimatedSpritesFolder;
		static const char* ShaderIncludeFolder;
		static const char* SpriteSubFolder;

		Path BuiltinDataFolder;
		Path EditorSkinFolder;
		Path EditorSkinSpritesFolder;
		Path EditorIconFolder;
		Path EditorIcon3DFolder;
		Path EditorIconSpritesFolder;
		Path EditorIcon3DSpritesFolder;
		Path EditorAnimatedSpritesFolder;
		Path EditorShaderFolder;
		Path EditorShaderIncludeFolder;

		Path BuiltinRawDataFolder;
		Path EditorRawSkinFolder;
		Path EditorRawIconsFolder;
		Path EditorRawShaderIncludeFolder;
		Path EditorRawShaderFolder;

		Path ResourceManifestPath;

		static const String DefaultFontFilename;
		static const String DefaultAAFontFilename;
		static const UINT32 DefaultFontSize;
		static const UINT32 TitleFontSize;

		static const Color TextNormalColor;
		static const Color TextActiveColor;

		static const String GUISkinFile;
		static const String XButtonNormalTex;

		static const String FolderIconTex;
		static const String MeshIconTex;
		static const String TextureIconTex;
		static const String FontIconTex;
		static const String PlainTextIconTex;
		static const String ScriptCodeIconTex;
		static const String ShaderIconTex;
		static const String ShaderIncludeIconTex;
		static const String MaterialIconTex;
		static const String SpriteTextureIconTex;
		static const String PrefabIconTex;
		static const String GUISkinIconTex;
		static const String PhysicsMaterialIconTex;
		static const String PhysicsMeshIconTex;
		static const String AudioClipIconTex;
		static const String AnimationClipIconTex;

		static const String ShaderDockOverlayFile;
		static const String ShaderSceneGridFile;
		static const String ShaderPickingCullNoneFile;
		static const String ShaderPickingCullCWFile;
		static const String ShaderPickingCullCCWFile;
		static const String ShaderPickingAlphaCullNoneFile;
		static const String ShaderPickingAlphaCullCWFile;
		static const String ShaderPickingAlphaCullCCWFile;
		static const String ShaderLineGizmoFile;
		static const String ShaderSolidGizmoFile;
		static const String ShaderWireGizmoFile;
		static const String ShaderLineHandleFile;
		static const String ShaderSolidHandleFile;
		static const String ShaderHandleClearAlphaFile;
		static const String ShaderIconGizmoFile;
		static const String ShaderGizmoPickingFile;
		static const String ShaderGizmoPickingAlphaFile;
		static const String ShaderTextGizmoFile;
		static const String ShaderSelectionFile;

		static const String EmptyShaderCodeFile;
		static const String EmptyCSScriptCodeFile;

		static const char* SplashScreenName;
	};

	/** @} */
}