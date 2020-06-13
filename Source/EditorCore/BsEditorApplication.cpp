//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsEditorApplication.h"
#include "EditorWindow/BsEditorWindowManager.h"
#include "EditorWindow/BsEditorWidgetManager.h"
#include "EditorWindow/BsMainEditorWindow.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Utility/BsBuiltinEditorResources.h"
#include "UndoRedo/BsUndoRedo.h"
#include "Serialization/BsFileSerializer.h"
#include "FileSystem/BsFileSystem.h"
#include "EditorWindow/BsEditorWidgetLayout.h"
#include "Scene/BsScenePicking.h"
#include "Scene/BsSelection.h"
#include "Scene/BsGizmoManager.h"
#include "CodeEditor/BsCodeEditor.h"
#include "Build/BsBuildManager.h"
#include "Resources/BsScriptCodeImporter.h"
#include "Library/BsEditorShaderIncludeHandler.h"
#include "EditorWindow/BsDropDownWindowManager.h"
#include "Library/BsProjectLibrary.h"
#include "Settings/BsProjectSettings.h"
#include "Settings/BsEditorSettings.h"
#include "Script/BsScriptManager.h"
#include "Importer/BsImporter.h"
#include "Input/BsVirtualInput.h"
#include "Resources/BsResources.h"
#include "Scene/BsSceneManager.h"
#include "Utility/BsSplashScreen.h"
#include "Utility/BsDynLib.h"
#include "Scene/BsSceneManager.h"
#include "BsEngineConfig.h"

namespace bs
{
	constexpr UINT32 SPLASH_SCREEN_DURATION_MS = 1000;

	const Path EditorApplication::WIDGET_LAYOUT_PATH = PROJECT_INTERNAL_DIR + "Layout.asset";
	const Path EditorApplication::BUILD_DATA_PATH = PROJECT_INTERNAL_DIR + "BuildData.asset";
	const Path EditorApplication::PROJECT_SETTINGS_PATH = PROJECT_INTERNAL_DIR + "Settings.asset";

	START_UP_DESC createStartupDesc()
	{
		START_UP_DESC startUpDesc;
		startUpDesc.renderAPI = BS_RENDER_API_MODULE;
		startUpDesc.renderer = BS_RENDERER_MODULE;
		startUpDesc.audio = BS_AUDIO_MODULE;
		startUpDesc.physics = BS_PHYSICS_MODULE;
		startUpDesc.physicsCooking = true;

		startUpDesc.primaryWindowDesc.videoMode = VideoMode(1920, 1080);
		startUpDesc.primaryWindowDesc.title = "BansheeEditor";
		startUpDesc.primaryWindowDesc.fullscreen = false;
		startUpDesc.primaryWindowDesc.showTitleBar = false;
		startUpDesc.primaryWindowDesc.showBorder = false;
		startUpDesc.primaryWindowDesc.hideUntilSwap = true;
		startUpDesc.primaryWindowDesc.depthBuffer = false;
		startUpDesc.primaryWindowDesc.hidden = true;

		startUpDesc.importers.push_back("bsfFreeImgImporter");
		startUpDesc.importers.push_back("bsfFBXImporter");
		startUpDesc.importers.push_back("bsfFontImporter");
		startUpDesc.importers.push_back("bsfSL");

		return startUpDesc;
	}

	Path getEditorSettingsPath()
	{
		return Paths::getEditorDataPath() + "Settings.asset";
	}

	EditorApplication::EditorApplication()
		:Application(createStartupDesc()), mIsProjectLoaded(false)
	{

	}

	EditorApplication::~EditorApplication()
	{
		ProjectLibrary::shutDown();
		BuiltinEditorResources::shutDown();
	}

	void EditorApplication::onStartUp()
	{
		Application::onStartUp();

		// In editor we render game on a separate surface, handled in Game window
		SceneManager::instance().setMainRenderTarget(nullptr);

		loadEditorSettings();
		mProjectSettings = bs_shared_ptr_new<ProjectSettings>();

		ScriptCodeImporter* scriptCodeImporter = bs_new<ScriptCodeImporter>();
		Importer::instance()._registerAssetImporter(scriptCodeImporter);

		// Hidden dependency: Needs to be done before BuiltinEditorResources import as shader include lookup requires it
		ProjectLibrary::startUp();

		BuiltinEditorResources::startUp();

		{
			auto inputConfig = VirtualInput::instance().getConfiguration();

			inputConfig->registerButton("Copy", BC_C, ButtonModifier::Ctrl);
			inputConfig->registerButton("Cut", BC_X, ButtonModifier::Ctrl);
			inputConfig->registerButton("Paste", BC_V, ButtonModifier::Ctrl);
		}

		UndoRedo::startUp();
		EditorWindowManager::startUp();
		EditorWidgetManager::startUp();
		DropDownWindowManager::startUp();

		ScenePicking::startUp();
		Selection::startUp();
		GizmoManager::startUp();
		BuildManager::startUp();
		CodeEditorManager::startUp();

		MainEditorWindow::create(getPrimaryWindow());
		ScriptManager::startUp();
	}

	void EditorApplication::onShutDown()
	{
		unloadProject();

		CodeEditorManager::shutDown();
		BuildManager::shutDown();
		GizmoManager::shutDown();
		Selection::shutDown();
		ScenePicking::shutDown();

		saveEditorSettings();

		DropDownWindowManager::shutDown();
		EditorWidgetManager::shutDown();
		EditorWindowManager::shutDown();
		UndoRedo::shutDown();

		Application::onShutDown();
	}

	void EditorApplication::startUp()
	{
		CoreApplication::startUp<EditorApplication>();
	}

	void EditorApplication::startUpRenderer()
	{
		mSplashScreenTimer.reset();
		SplashScreen::show();
	}

	void EditorApplication::startUpScriptManager()
	{
		// Do nothing, we initialize the script manager at a later stage
	}

	void EditorApplication::updateScriptManager()
	{
		// Do nothing, we need to perform script update before editor window update, but Application::postUpdate (where this
		// method is normally called from) needs to execute after editor window update.
	}


	void EditorApplication::preUpdate()
	{
		Application::preUpdate();

		EditorWidgetManager::instance().update();
		DropDownWindowManager::instance().update();
	}

	void EditorApplication::postUpdate()
	{
		ScriptManager::instance().update();

		// Call update on editor widgets before parent's postUpdate because the parent will render the GUI and we need
		// to ensure editor widget's GUI is updated.
		EditorWindowManager::instance().update();

		Application::postUpdate();

		if(mSplashScreenShown)
		{
			UINT64 currentTime = mSplashScreenTimer.getMilliseconds();
			if (currentTime >= SPLASH_SCREEN_DURATION_MS)
			{
				SplashScreen::hide();
				EditorWindowManager::instance().showWindows();
				mSplashScreenShown = false;
			}
		}

		setFPSLimit(mEditorSettings->getFPSLimit());
	}

	void EditorApplication::quitRequested()
	{
		onQuitRequested();
	}

	void EditorApplication::saveProject()
	{
		if (!isProjectLoaded())
			return;

		Path buildDataPath = getProjectPath();
		buildDataPath.append(BUILD_DATA_PATH);

		BuildManager::instance().save(buildDataPath);
		saveWidgetLayout(EditorWidgetManager::instance().getLayout());
		saveEditorSettings();
		saveProjectSettings();

		gProjectLibrary().saveLibrary();
	}

	void EditorApplication::unloadProject()
	{
		if (!isProjectLoaded())
			return;

		saveProject();

		mProjectSettings = bs_shared_ptr_new<ProjectSettings>();
		BuildManager::instance().clear();
		UndoRedo::instance().clear();

		EditorWidgetManager::instance().closeAll();
		gProjectLibrary().unloadLibrary();
		Resources::instance().unloadAllUnused();
		gSceneManager().clearScene();

		mProjectPath = Path::BLANK;
		mProjectName = StringUtil::BLANK;
		mIsProjectLoaded = false;
	}

	void EditorApplication::loadProject(const Path& projectPath)
	{
		unloadProject();

		mProjectPath = projectPath;
		mProjectName = projectPath.getTail();
		mIsProjectLoaded = true;

		loadProjectSettings();

		Path buildDataPath = getProjectPath();
		buildDataPath.append(BUILD_DATA_PATH);

		BuildManager::instance().load(buildDataPath);
		gProjectLibrary().loadLibrary();

		// Do this before restoring windows to ensure types are loaded
		ScriptManager::instance().reload();
		
		SPtr<EditorWidgetLayout> layout = loadWidgetLayout();
		if (layout != nullptr)
			EditorWidgetManager::instance().setLayout(layout);
	}

	void EditorApplication::createProject(const Path& path)
	{
		Path resourceDir = Path::combine(path, ProjectLibrary::RESOURCES_DIR);
		Path internalResourcesDir = Path::combine(path, ProjectLibrary::INTERNAL_RESOURCES_DIR);

		if (!FileSystem::exists(resourceDir))
			FileSystem::createDir(resourceDir);

		if (!FileSystem::exists(internalResourcesDir))
			FileSystem::createDir(internalResourcesDir);

		saveDefaultWidgetLayout(path);
	}

	bool EditorApplication::isValidProjectPath(const Path& path)
	{
		if (!path.isAbsolute())
			return false;

		if (!FileSystem::isDirectory(path))
			return false;

		Path resourceDir = Path::combine(path, ProjectLibrary::RESOURCES_DIR);
		if (!FileSystem::exists(resourceDir))
			return false;

		return true;
	}

	SPtr<EditorWidgetLayout> EditorApplication::loadWidgetLayout()
	{
		Path layoutPath = getProjectPath();
		layoutPath.append(WIDGET_LAYOUT_PATH);

		if (!FileSystem::exists(layoutPath))
			saveDefaultWidgetLayout(getProjectPath());

		if(FileSystem::exists(layoutPath))
		{
			FileDecoder fs(layoutPath);
			return std::static_pointer_cast<EditorWidgetLayout>(fs.decode());
		}

		return nullptr;
	}

	void EditorApplication::saveWidgetLayout(const SPtr<EditorWidgetLayout>& layout)
	{
		Path layoutPath = getProjectPath();
		layoutPath.append(WIDGET_LAYOUT_PATH);

		FileEncoder fs(layoutPath);
		fs.encode(layout.get());
	}

	void EditorApplication::saveDefaultWidgetLayout(const Path& folder)
	{
		Path internalResourcesDir = Path::combine(folder, ProjectLibrary::INTERNAL_RESOURCES_DIR);

		if (!FileSystem::exists(internalResourcesDir))
			FileSystem::createDir(internalResourcesDir);

		Path defaultLayoutPath = BuiltinEditorResources::getDefaultWidgetLayoutPath();

		if (FileSystem::exists(defaultLayoutPath))
		{
			Path projectLayoutPath = Path::combine(folder, WIDGET_LAYOUT_PATH);
			FileSystem::copy(defaultLayoutPath, projectLayoutPath, false);
		}
	}

	void EditorApplication::loadEditorSettings()
	{
		Path settingsPath = getEditorSettingsPath();

		if (FileSystem::exists(settingsPath))
		{
			FileDecoder fs(settingsPath);
			mEditorSettings = std::static_pointer_cast<EditorSettings>(fs.decode());
		}
		
		if (mEditorSettings == nullptr)
			mEditorSettings = bs_shared_ptr_new<EditorSettings>();
	}

	void EditorApplication::saveEditorSettings()
	{
		if (mEditorSettings == nullptr)
			return;

		Path settingsPath = getEditorSettingsPath();

		FileEncoder fs(settingsPath);
		fs.encode(mEditorSettings.get());
	}

	void EditorApplication::loadProjectSettings()
	{
		if (isProjectLoaded())
		{
			Path absoluteDataPath = getProjectPath();
			absoluteDataPath.append(PROJECT_SETTINGS_PATH);

			if (FileSystem::exists(absoluteDataPath))
			{
				FileDecoder fs(absoluteDataPath);
				mProjectSettings = std::static_pointer_cast<ProjectSettings>(fs.decode());
			}
		}

		if (mProjectSettings == nullptr)
			mProjectSettings = bs_shared_ptr_new<ProjectSettings>();
	}

	void EditorApplication::saveProjectSettings()
	{
		if (mProjectSettings == nullptr || !isProjectLoaded())
			return;

		Path absoluteDataPath = getProjectPath();
		absoluteDataPath.append(PROJECT_SETTINGS_PATH);

		FileEncoder fs(absoluteDataPath);
		fs.encode(mProjectSettings.get());
	}

	SPtr<IShaderIncludeHandler> EditorApplication::getShaderIncludeHandler() const
	{
		return bs_shared_ptr_new<EditorShaderIncludeHandler>();
	}

	EditorApplication& gEditorApplication()
	{
		return static_cast<EditorApplication&>(EditorApplication::instance());
	}
}