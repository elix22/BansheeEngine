# Source files and their filters
include(CMakeSources.cmake)
	
# Target
if(WIN32)
	add_executable(Game WIN32 ${BS_GAME_SRC} Game.rc)
else()
	add_executable(Game ${BS_GAME_SRC})
endif()

# Common flags
add_common_flags(Game)

# Includes
target_include_directories(Game PRIVATE "./")

# Post-build step
# TODO: Use CMAKE_SYSTEM_NAME and BS_64BIT?
if(WIN32)
	if(BS_64BIT)
		set(BS_COPY_FOLDER Win64)
	else()
		set(BS_COPY_FOLDER Win32)
	endif()
elseif(LINUX)
	if(BS_64BIT)
		set(BS_COPY_FOLDER Linux64)
	else()
		set(BS_COPY_FOLDER Linux32)
	endif()
else()
	# TODO_OTHER_PLATFORMS_GO_HERE	
endif()

if (BS_COPY_FOLDER)
	set(DST_FOLDER ${PROJECT_SOURCE_DIR}/Data/Binaries/${BS_COPY_FOLDER})
	add_custom_command(
		TARGET Game POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${DST_FOLDER}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Game>" "${DST_FOLDER}")
endif()

# Libraries
## Local libs
target_link_libraries(Game bsfScript bsfMono bsf)

# IDE specific
set_property(TARGET Game PROPERTY FOLDER Executable)

# Plugin dependencies
add_engine_dependencies(Game)

# Compiler flags
if(MSVC)
	target_compile_options(Game PUBLIC /wd4509)
endif()

# Install
set(PLATFORM_PATH_PREFIX "")
if(WIN32)
	set(PLATFORM_PATH_PREFIX "Win")
elseif(LINUX)
	set(PLATFORM_PATH_PREFIX "Linux")
elseif(APPLE)
	set(PLATFORM_PATH_PREFIX "macOS")
else()
	message(FATAL "Unsupported platform.")
endif()

if(BS_64BIT)
	set(PLATFORM_PATH_PREFIX "${PLATFORM_PATH_PREFIX}64")
endif()
	
install(
	TARGETS Game
	RUNTIME DESTINATION EditorData/Binaries/${PLATFORM_PATH_PREFIX}
)
