# Compiling

## Quick start
Use the guide below to get Banshee up and running as quickly as possible. Scroll further below for advanced build options that allow for more customization.

- Install git (https://git-scm.com) and CMake 3.12.0 or higher (https://cmake.org)
  - Ensure they are added to your *PATH* environment variable
- Install other dependencies
  - See [here](#otherDeps)
- Run the following commands in the terminal/command line:
  - `git clone https://github.com/BearishSun/BansheeEngine.git`
  - `cd BansheeEngine`
  - `mkdir Build`
  - `cd Build`
  - `cmake -G "$generator$" ../`
    - Where *$generator$* should be replaced with any of the supported generators. Some common ones:
	  - `Visual Studio 15 2017 Win64` - Visual Studio 2017 (64-bit build)
	  - `Unix Makefiles`
	  - `Ninja`
	  - `Xcode`
	- See all valid generators: [cmake-generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
- Build the project using your chosen tool
  - Build files will be placed in `BansheeEngine/Build` folder (e.g. Visual Studio or XCode solution, Makefiles)
- Run the editor
  - Editor executable and libraries will be placed in `BansheeEngine/build/bin` folder
	 
## Customizing the build

By default the process above will fetch the *master* branch. If required switch to a different branch before building:
 - **preview** - This branch always contains latest "stable" pre-release version. This might be a few months behind master.
 - **master** - This branch contains the latest changes, might be quite unstable and might not even compile. Use this if you absolutely need the bleeding edge changes.
 
Variety of *CMake* variables are provided that allow you to customize the build. They allow you to pick between the render API (Vulkan, DirectX, OpenGL), audio module (FMOD, OpenAudio) among other options. Run *CMake* to see all options. Note that non-default *CMake* options might require additional dependencies to be installed, see [here](#otherDeps).
 
You can choose to use a different *CMake* generator than those specified above, as long as the platform/compiler is supported:  
  - Supported platforms:
    - Windows, Linux
    - (macOS coming soon)
  - Supported compilers:
	- MSVC++ 15.0 (Visual Studio 2017)
    - GCC 7.0 (or newer)
    - Clang 5.0 (or newer)

### <a name="dependencies"></a>Third party dependencies
Banshee relies on a variety of third party dependencies. A set of pre-compiled dependencies are provided for every supported platform/compiler. These will be fetched automatically by the build process, but you may also compile them manually - see [this guide](dependencies.md). Note that prebuilt dependencies only come as 64-bit, if you wish to build as 32-bit you will need to compile the dependencies manually.

## <a name="otherDeps"></a>Other dependencies
The following dependencies will need to be installed manually. Which ones are required depend on the selected *CMake* options and your usage of Banshee (check text of each entry below).

**Windows**
  - **DirectX SDK** (Required by default on Windows 7 or earlier)
	- Optional if you have choosen a different RenderAPI in *CMake* options
    - Set up DXSDK_DIR environment variable pointing to the DirectX instalation
  - **Windows SDK** (Required by default on Windows 8 or later)
	- Optional if you have choosen a different RenderAPI in *CMake* options
  - **Visual C++ ATL for x86 and x64**
    - Make sure this is selected in your Visual Studio Installer, or install it via some other means
  - **DirectX Debug Layer** (Required by default on Windows 10)
    - Optional if you have choosen a different RenderAPI in *CMake* options
    - Go to Settings panel (type "Settings" in Start)->System->Apps & features->Manage optional Features->Add a feature->Select "Graphics Tools"
  - **Mono 4.2** (Optional)
    - If you wish to compile managed assemblies using a Microsoft compiler (e.g. using Visual Studio) yet still be able to debug the generated assemblies, you must install Mono 4.2. and set up an environment variable MONO_INSTALL_DIR pointing to the Mono installation directory. When this is set up "pdb2mdb" script will trigger on next compile generating the needed debug symbols.
    - http://www.mono-project.com/download/
 
**Linux**
  - **OpenGL**
    - Required by default, but optional if you have choosen a different RenderAPI in *CMake* options
    - Debian/Ubuntu: *apt-get install libgl1-mesa-dev libglu1-mesa-dev mesa-common-dev*
  - **X11**
    - Debian/Ubuntu: *apt-get install libx11-dev libxcursor-dev libxrandr-dev*
  - **LibUUID**
    - Debian/Ubuntu: *apt-get install uuid-dev*
  - **GTK+ 3.0** (Editor only)
    - Optional if not building the editor
    - Debian/Ubuntu: *apt-get install libgtk-3-dev*
  - **Mono** (Editor only)
    - Required for compilation of C# code. Optional if not building the editor.
    - Debian/Ubuntu: *apt-get install mono-complete*
  - **Bison & Flex**
    - Debian/Ubuntu: *apt-get install bison flex*
  - (Or equivalent packages for your distribution)

**All OS**
  - **Vulkan SDK 1.0.65.1** (Optional) 
    - Only needed if you selected the Vulkan render API during build configuration
    - https://lunarg.com/vulkan-sdk/
    - Set up VULKAN_SDK environment variable pointing to your instalation
  - **Python 3.5** (Optional)
    - Only needed if you plan on running Python scripts in the /Scripts folder
    - https://www.python.org/downloads/
  - **FMOD Low Level Programmer API 1.08.02** (Optional)
    - Only needed if you selected the FMOD audio module during build configuration
    - http://www.fmod.org/download/
    - If CMake complains it cannot find FMOD, manually set the FMOD_INSTALL_DIRS to your installation directory 
    - Copy the dynamic libraries (.dll) from {INSTALLDIR}/api/lowlevel/lib into /bin folder in Banshee source code folder
      - Use logging libraries for the Debug builds, and non-logging for OptimizedDebug and Release builds
