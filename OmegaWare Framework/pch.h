// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

// ^That was automatically generated by Visual Studio

// I use Pascal case for almost everything, if you don't like it 1v1 me in the parking lot :smile:

// Disabled warnings for this file
#pragma warning(push)
#pragma warning(disable : 5056) // Operator '!=': deprecated for array types
#pragma warning(disable : 4244) // Disable the warning for the conversion from 'float' to 'int', possible loss of data

// Framework defines
#pragma execution_character_set("utf-8")

#define FRAMEWORK_MAJOR_VERSION 6
#define FRAMEWORK_MINOR_VERSION 8
#define FRAMEWORK_REWORK_VERSION 4
#define FRAMEWORK_VERSION FRAMEWORK_MAJOR_VERSION.FRAMEWORK_MINOR_VERSION.FRAMEWORK_REWORK_VERSION

#define FRAMEWORK_CODENAME "OmegaWare"
#define FRAMEWORK_TARGET_GAME "ye"
#define FRAMEWORK_TARGET_PROCESS "ye"
static_assert(FRAMEWORK_TARGET_GAME != "", "Target game not set."); // Make sure the target game title is set
static_assert(FRAMEWORK_TARGET_PROCESS != "", "Target process name not set."); // Make sure the target process name is set

#ifndef FRAMEWORK_INJECTOR

#define IGNORE_32BIT_WARNING 1 // Ignore the 32 bit warning for the framework because Dx12 will complain when using x86

#define FRAMEWORK_OTHER 1
#define FRAMEWORK_UNREAL 0
#define FRAMEWORK_UNITY 0

static_assert((FRAMEWORK_OTHER + FRAMEWORK_UNREAL + FRAMEWORK_UNITY) == 1, "Must use exactly one framework type"); // Don't allow both frameworks to be used)

// Make sure a rendering API is selected and only one rendering API is selected
#define FRAMEWORK_RENDER_DYNAMIC 0
#define FRAMEWORK_RENDER_D3D11 1
#define FRAMEWORK_RENDER_D3D12 0
static_assert((FRAMEWORK_RENDER_DYNAMIC + FRAMEWORK_RENDER_D3D11 + FRAMEWORK_RENDER_D3D12) == 1, "Must use exactly one rendering API");
static_assert(!(FRAMEWORK_RENDER_DYNAMIC || FRAMEWORK_RENDER_D3D12), "This does NOT work right now, please dont use ;3");

#include "Libs/MinHook/include/MinHook.h"

#endif

// Start the precompiled headers
#ifndef PCH_H
#define PCH_H

#ifndef FRAMEWORK_INJECTOR

#if FRAMEWORK_UNREAL // If the framework set is Unreal include the SDK.h file that includes all the SDK headers made by an SDK generator
#include "SDK.h"

// Reminder remember to incude these files in the project
// BasicTypes_Package.cpp
// CoreUObject_Package.cpp
// Engine_Package.cpp
#endif

#endif

// Include standard libraries that are used in the project
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>
#include <format>
#include <string>
#include <cmath>
#include <sstream>
#include <cstdio>
#include <vector>
#include <functional>
#include <algorithm>
#include <span>
#include <eh.h> // I dont remember what this was for, but I think it was for a scuffed try catch block to stop crashes on memory access violations

#ifndef FRAMEWORK_INJECTOR

#define _USE_MATH_DEFINES // Define math constants for things like M_PI and M_SQRT2
#include <math.h>

#include "Memory/Memory.h"
#include "Utils/Utils.h" // Include the Utils.h file that contains various utility functions for the framework

#if FRAMEWORK_UNREAL // If the framework set is Unreal include the Unreal.h file that contains the Unreal interface class that is used to interact with the Unreal Engine
#include "Interfaces/FNames.h"
#include "Interfaces/Unreal.h"
#endif

#if FRAMEWORK_UNITY
#include "Interfaces/Mono.h"
#endif

// Include ImGui and the ImGui implementation for Win32
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Libs/ImGui/imgui.h"
#include "Libs/ImGui/imgui_impl_win32.h"

// Include the ImGui implementation for the rendering API that is being used
#if FRAMEWORK_RENDER_D3D11 || FRAMEWORK_RENDER_DYNAMIC
#pragma comment(lib, "d3d11.lib") // WHY DO I NEED THIS WTF
#pragma comment(lib, "dxgi.lib")
#include <d3d11.h>
#include <dxgi1_2.h>
#include "Libs/ImGui/imgui_impl_dx11.h"
#endif

#if FRAMEWORK_RENDER_D3D12 || FRAMEWORK_RENDER_DYNAMIC
#pragma comment(lib, "d3d12.lib") // WHY DO I NEED THIS WTF
#pragma comment(lib, "dxgi.lib")
#include <D3D12.h>
#include <dxgi1_4.h>
#include "Libs/ImGui/imgui_impl_dx12.h"
#endif

#include "GUI/Styles.h" // Include the Styles.h file that contains the ImGui styles for the framework

#include "GUI/Custom.h" // Include the Custom.h file that contains the custom ImGui widgets for the framework
#include "GUI/GUI.h" // Include the GUI.h file that contains the GUI class that is used to create the framework's menu

#include "Hooks/WndProc/WndProcHooks.h"
#include "Hooks/Renderer/RendererHooks.h"

#define DEG2RAD(deg) deg * M_PI / 180 // A macro to convert degrees to radians
#define RAD2DEG(rad) rad * 180.0 / M_PI; // A macro to convert radians to degrees

#include "Config/Config.h"
#include "Localization/Localization.h"

#endif

namespace Cheat
{
	inline const std::string Framework = FRAMEWORK_CODENAME; // Set the framework name to the codename
	inline const std::string Game = FRAMEWORK_TARGET_GAME; // Set the game name to the target game
	inline const std::string Title = Framework + " (" + Game + ")"; // Set the title to the framework name and the game name

#ifndef FRAMEWORK_INJECTOR

#ifdef _WIN64
	constexpr bool bIs64Bit = true;
#else
	constexpr bool bIs64Bit = true;
#endif

	inline bool bInitalized = false;
	inline bool bShouldRun = true; // A boolean to check if the cheat should run or exit
	inline DWORD dwThreadID = NULL; // A DWORD to store the thread ID of the cheat thread
	inline HMODULE hModule = NULL; // A HMODULE to store the module handle of the cheat used for unloading the module

	constexpr ImGuiKey keyMenuKey = ImGuiKey_Insert; // A DWORD to store the key that opens and closes the menu
	constexpr ImGuiKey keyUnloadKey1 = ImGuiKey_End; // A DWORD to store the key that unloads the cheat (Cool one)
	constexpr ImGuiKey keyUnloadKey2 = ImGuiKey_Delete; // A DWORD to store the key that unloads the cheat (Lame one)
	constexpr ImGuiKey keyConsoleKey = ImGuiKey_Home; // A DWORD to store the key that opens and closes the console

	inline std::unique_ptr<Console> console = std::make_unique<Console>(false, Title);  // A unique pointer to the console class that is used to create the console for the framework

	inline std::unique_ptr<Menu> menu = std::make_unique<Menu>(Cheat::Title, ElementBase::Style_t({
		.vec2Size = { GUI::WIDTH, GUI::HEIGHT },
		.iFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse,
	}));
		
	inline std::unique_ptr<WndProcHooks> wndproc = std::make_unique<WndProcHooks>();
	inline std::unique_ptr<RendererHooks> renderer = std::make_unique<RendererHooks>();

#if FRAMEWORK_UNREAL // If the framework set is Unreal create a unique pointer to the Unreal interface class
	inline std::unique_ptr<Unreal> unreal = std::make_unique<Unreal>();
#endif

#if FRAMEWORK_UNITY // If the framework set is Unity create a unique pointer to the Mono interface class
	inline Mono mono = Mono::Instance(); // I would use a unique pointer but the class is already setup as a singlton and I need to call the destructor to clean up the mono domain
#endif

	inline std::unique_ptr<Config> config;
	inline std::vector<ConfigEntry> Entries;

	inline std::vector<LocalizationData> Locales;
	inline LocalizationData CurrentLocale;
	inline std::unique_ptr<Localization> localization;
#endif
}

#ifndef FRAMEWORK_INJECTOR

#include "Features/Feature.h" // Include the Feature.h file that contains the Feature class that is used to create the features for the framework
#include "Features/ExampleFeature/ExampleFeature.h"

// https://stackoverflow.com/questions/13048301/pointer-to-array-of-base-class-populate-with-derived-class
inline std::vector<std::unique_ptr<Feature>> Features; // A vector of unique pointers to the Feature class that is used to store the features for the framework

#endif

#endif //PCH_H



#pragma warning(pop)