cmake_minimum_required (VERSION 3.12.0)
project (Banshee3D)

set (BSF_DIRECTORY ${PROJECT_SOURCE_DIR}/Source/bsf)
set (BSF_SOURCE_DIR ${BSF_DIRECTORY}/Source)
set (APP_ROOT_DIR ${PROJECT_SOURCE_DIR})
set (BS_IS_BANSHEE3D 1)

# Version
set (BS_B3D_VERSION_MAJOR 0)
set (BS_B3D_VERSION_MINOR 5)
set (BS_B3D_VERSION_PATCH 0) 
set (BS_B3D_CURRENT_RELEASE_YEAR 2019)

# Grab BSF
find_path(SUBMODULE_SOURCES "Source/Foundation/bsfEngine/BsApplication.h" "Source/bsf/")
if(NOT SUBMODULE_SOURCES)
	execute_process(COMMAND git submodule update 
						--init 
						-- Source/bsf
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
else()
	execute_process(COMMAND git submodule update
						-- Source/bsf
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
mark_as_advanced(SUBMODULE_SOURCES)

include(${BSF_SOURCE_DIR}/CMake/Properties.cmake)
include(${BSF_SOURCE_DIR}/CMake/FindPackageOrBuild.cmake)
include(${BSF_SOURCE_DIR}/CMake/HelperMethods.cmake)

add_subdirectory(${BSF_SOURCE_DIR})

set (BS_PREBUILT_DEPENDENCIES_VERSION 24)
set (BS_SRC_DEPENDENCIES_VERSION 15)
set (BS_BUILTIN_ASSETS_VERSION 17)

# Options
set(SCRIPT_BINDING_GENERATION OFF CACHE BOOL "If true, script binding generation will be supported through a specialized build target. Enable this if you plan on modifying the scripting API. Requires the SBGen tool dependency.")

# Ensure dependencies are up to date
## Check data dependencies
check_and_update_builtin_assets(Banshee ${PROJECT_SOURCE_DIR}/Data/Raw Raw ${BS_BUILTIN_ASSETS_VERSION} NO)
		
## Import built-in assets
if(${INCLUDE_ASSET_PACKAGING_SCRIPTS})
	add_run_asset_import_target(EditorBuiltinAssets ${PROJECT_SOURCE_DIR}/Data ${BSF_SOURCE_DIR}/.. "--editor")
	add_upload_assets_target(EditorBuiltinAssets "BansheeCompiledData_Master" ${PROJECT_SOURCE_DIR}/Data Raw/DataPackageContents.txt)
endif()

## Fetch built-in assets
set(BANSHEE_COMPILED_ASSETS_VERSION_FILE ${PROJECT_SOURCE_DIR}/Data/.reqversion)
file (STRINGS ${BANSHEE_COMPILED_ASSETS_VERSION_FILE} BANSHEE_COMPILED_ASSETS_VERSION)
check_and_update_builtin_assets(BansheeCompiled ${PROJECT_SOURCE_DIR}/Data Data ${BANSHEE_COMPILED_ASSETS_VERSION} NO)
		
# Sub-directories
add_subdirectory(Source/EditorCore)

## Script interop
add_subdirectory(Source/EditorScript)
add_subdirectory(Source/EditorManaged)

## Executables
add_subdirectory(Source/Banshee3D)
add_subdirectory(Source/Game)

## Managed project dependencies
add_dependencies(Banshee3D MBansheeEngine MBansheeEditor)
add_dependencies(Game MBansheeEngine)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Banshee3D)
endif()

## Install
install(
	DIRECTORY Data/
	DESTINATION EditorData
	PATTERN "Data/Raw" EXCLUDE
	PATTERN ".version" EXCLUDE
	PATTERN ".reqversion" EXCLUDE
	PATTERN "Settings.asset" EXCLUDE
)