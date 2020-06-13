//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "Build/BsBuildManager.h"
#include "Private/RTTI/BsBuildDataRTTI.h"
#include "Serialization/BsFileSerializer.h"
#include "FileSystem/BsFileSystem.h"
#include "BsEditorApplication.h"

namespace bs
{
	BuildData::BuildData()
		:activePlatform(PlatformType::Windows)
	{
		platformData.resize((UINT32)PlatformType::Count);
		platformData[0] = bs_shared_ptr_new<WinPlatformInfo>();
	}

	RTTITypeBase* BuildData::getRTTIStatic()
	{
		return BuildDataRTTI::instance();
	}

	RTTITypeBase* BuildData::getRTTI() const
	{
		return BuildData::getRTTIStatic();
	}

	const char* BuildManager::BUILD_FOLDER_NAME = "Builds/";

	BuildManager::BuildManager()
	{
		mBuildData = bs_shared_ptr_new<BuildData>();
	}

	const Vector<PlatformType>& BuildManager::getAvailablePlatforms() const
	{
		static const Vector<PlatformType> PLATFORMS = { PlatformType::Windows };

		return PLATFORMS;
	}

	PlatformType BuildManager::getActivePlatform() const
	{
		return mBuildData->activePlatform;
	}

	void BuildManager::setActivePlatform(PlatformType type)
	{
		if ((UINT32)type < (UINT32)PlatformType::Count)
			mBuildData->activePlatform = type;
	}

	SPtr<PlatformInfo> BuildManager::getActivePlatformInfo() const
	{
		return mBuildData->platformData[(UINT32)mBuildData->activePlatform];
	}

	SPtr<PlatformInfo> BuildManager::getPlatformInfo(PlatformType type) const
	{
		if ((UINT32)type < (UINT32)mBuildData->platformData.size())
			return mBuildData->platformData[(UINT32)type];

		return nullptr;
	}

	Vector<String> BuildManager::getFrameworkAssemblies(PlatformType type) const
	{
		switch (type)
		{
		case PlatformType::Windows:
		default:
			return { u8"mscorlib", u8"System", u8"System.Core" };
		}
	}

	Vector<Path> BuildManager::getNativeBinaries(PlatformType type) const
	{
		Vector<Path> libs = { u8"bsfEngine", u8"bsfCore", u8"bsfUtility",
			u8"bsfD3D11RenderAPI", u8"bsfGLRenderAPI", u8"RenderBeast",  u8"mono-2.0-sgen", u8"nvtt" };

		switch (type)
		{
		case PlatformType::Windows:
		{
			for (auto& lib : libs)
				lib.setExtension(lib.getExtension() + ".dll");
		}
		default:
			break;
		}

		return libs;
	}

	Path BuildManager::getBuildFolder(BuildFolder folder, PlatformType platform) const
	{
		// Use Data folder as an anchor to find where the root is
		Path sourceRoot = Paths::getDataPath();

		UINT32 numDataDirs = Paths::FRAMEWORK_DATA_PATH.getNumDirectories();
		if (Paths::FRAMEWORK_DATA_PATH.isFile())
			numDataDirs++;

		for (UINT32 i = 0; i < numDataDirs; i++)
			sourceRoot.makeParent();

		switch (folder)
		{
		case BuildFolder::SourceRoot:
			return sourceRoot;
		case BuildFolder::DestinationRoot:
		{
			switch (platform)
			{
			case PlatformType::Windows:
				return gEditorApplication().getProjectPath() + BUILD_FOLDER_NAME + u8"Windows";
			default:
				break;
			}

			return gEditorApplication().getProjectPath() + BUILD_FOLDER_NAME;
		}
		case BuildFolder::NativeBinaries:
		{
			Path binariesPath = Paths::getBinariesPath();

			return binariesPath.makeRelative(sourceRoot);
		}
		case BuildFolder::BansheeDebugAssemblies:
			return Paths::DEBUG_ASSEMBLY_PATH;
		case BuildFolder::BansheeReleaseAssemblies:
			return Paths::RELEASE_ASSEMBLY_PATH;
		case BuildFolder::Data:
			return Paths::FRAMEWORK_DATA_PATH;
		}

		return Path::BLANK;
	}

	Path BuildManager::getMainExecutable(PlatformType type) const
	{
		switch (type)
		{
		case PlatformType::Windows:
		{
			Path output = Paths::getEditorDataPath() + "Binaries/Win64/Game.exe";
			return output;
		}
		default:
			break;
		}

		return Path::BLANK;
	}

	String BuildManager::getDefines(PlatformType type) const
	{
		return getPlatformInfo(type)->defines;
	}

	void BuildManager::clear()
	{
		mBuildData = nullptr;
	}

	void BuildManager::save(const Path& outFile)
	{
		FileEncoder fe(outFile);
		fe.encode(mBuildData.get());
	}

	void BuildManager::load(const Path& inFile)
	{
		if (FileSystem::exists(inFile))
		{
			FileDecoder fd(inFile);
			mBuildData = std::static_pointer_cast<BuildData>(fd.decode());
		}

		if (mBuildData == nullptr)
			mBuildData = bs_shared_ptr_new<BuildData>();
	}
}