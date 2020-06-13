# Generate the assembly info file
configure_file("${PROJECT_SOURCE_DIR}/Source/EditorManaged/Properties/AssemblyInfo.cs.in" "${PROJECT_SOURCE_DIR}/Source/EditorManaged/Properties/AssemblyInfo.cs")

# Project refs
if(MSVC)
	set(BS_CS_PROJECT_REFS " \
  <ProjectReference Include=\"${PROJECT_BINARY_DIR}\\MBansheeEngine.csproj\">\n \
    <Project>{876EB338-489E-4727-84DA-8CBBF0DA5B5E}</Project>\n \
    <Name>MBansheeEngine</Name>\n \
  </ProjectReference>\n")
	set(BS_CS_REFS " ")
else()
	set(BS_CS_PROJECT_REFS " ")
	set(BS_CS_REFS "  <Reference Include=\"MBansheeEngine.dll\"/>")
endif()

# Generate the CS project file
generate_csharp_project(
	${PROJECT_SOURCE_DIR}/Source/EditorManaged
	MBansheeEditor
	bs.Editor
	MBansheeEditor
	${BS_CS_REFS}
	${BS_CS_PROJECT_REFS})

# Include the CS project in the build
set(BS_CS_PROJ "MBansheeEditor.csproj")

if(MSVC)
	if(CMAKE_GENERATOR MATCHES "Visual Studio")
		include_external_msproject(MBansheeEditor ${PROJECT_BINARY_DIR}/${BS_CS_PROJ})
		set_property(TARGET MBansheeEditor PROPERTY FOLDER Script)
	else()
		# Generator expressions don't support 'A' else 'B', otherwise
		# this could be simplified to 'Debug' else 'Release'.
		# Another possible improvement for this would be if we support
		# all configuration types (e.g. RelWithDebInfo, etc) in MBansheeEditor
		# then we could use $<CONFIG>
		add_custom_target(MBansheeEditor
				COMMAND msbuild /p:Configuration=$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:Release> ${PROJECT_BINARY_DIR}/${BS_CS_PROJ}
				COMMENT "Building managed assembly \"MBansheeEditor\"")
	endif()
else()
	find_package(mcs)

	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(CS_PROJ_CONFIG Debug)
	else()
		set(CS_PROJ_CONFIG Release)
	endif()

	add_custom_target(MBansheeEditor
			COMMAND ${msbuild_EXECUTABLE} /p:Configuration=${CS_PROJ_CONFIG} ${PROJECT_BINARY_DIR}/${BS_CS_PROJ}
			COMMENT "Building managed assembly \"MBansheeEditor\"")
endif()

add_dependencies(MBansheeEditor MBansheeEngine)
