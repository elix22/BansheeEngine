# Source files and their filters
include(CMakeSources.cmake)

# Find packages
if(LINUX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
endif()

# Target
add_library(EditorCore SHARED ${BS_BANSHEEEDITOR_SRC})

# Common flags
add_common_flags(EditorCore)

# Includes
target_include_directories(EditorCore PUBLIC "./")

# Defines
target_compile_definitions(EditorCore PRIVATE -DBS_ED_EXPORTS)

# Libraries
## Local libs
target_link_libraries(EditorCore PUBLIC bsf)
	
if(WIN32)
	## OS libs
	target_link_libraries(EditorCore PRIVATE Advapi32)
elseif(LINUX)
	## External lib: GTK+ 3.0
	include_directories(${GTK3_INCLUDE_DIRS})
	target_link_libraries(EditorCore PRIVATE ${GTK3_LIBRARIES})
elseif(APPLE) # MacOS
	target_link_framework(EditorCore AppKit)
endif()

# IDE specific
set_property(TARGET EditorCore PROPERTY FOLDER Foundation)

# Install
install_bsf_target(EditorCore)