#pragma once
#pragma execution_character_set("utf-8")

// Universal includes
#define NOMINMAX
#include <Windows.h>

#include <iostream>
#include <cstdio>

#include <string>
#include <sstream>
#include <format>

#include <thread>
#include <chrono>

#include <memory>
#include <mutex>

#include <functional>
#include <algorithm>

#include <span>
#include <vector>

#include <random>

#define _USE_MATH_DEFINES // Define math constants for things like M_PI and M_SQRT2
#include <math.h>
#include <cmath>

#include "../Libs/MinHook/include/MinHook.h"

#include "../Memory/Memory.h"

#include "../FrameworkConfig.h"

#include "../Config/Config.h"
#include "../Localization/Localization.h"

#include "../Lua/LuaAPI.h"

// Framework Globals
namespace Framework
{
	inline const std::string Framework = FRAMEWORK_CODENAME;
	inline const std::string Game = TARGET_GAME_NAME;
	inline const std::string Title = Framework + " (" + Game + ")";

	inline std::unique_ptr<Console> console = std::make_unique<Console>(false, Title);

	inline bool bInitalized = false;
	inline bool bShouldRun = true;
	inline HMODULE hModule = NULL;
	inline size_t iModuleNameHash = {};

	constexpr ImGuiKey keyMenuKey = ImGuiKey_Insert;
	constexpr ImGuiKey keyUnloadKey1 = ImGuiKey_End; // A DWORD to store the key that unloads the cheat (Cool one)
	constexpr ImGuiKey keyUnloadKey2 = ImGuiKey_Delete; // A DWORD to store the key that unloads the cheat (Lame one)
	constexpr ImGuiKey keyConsoleKey = ImGuiKey_Home;

	inline bool bWatermark = true;
	inline bool bWatermarkFPS = true;

	inline std::unique_ptr<Config> config;
	inline std::unique_ptr<Lua> lua;

	inline std::unique_ptr<Menu> menu = std::make_unique<Menu>(Framework::Title, Framework::Title, ElementBase::Style_t({
	.vec2Size = ImVec2{ 800.f, 600.f },
	.iFlags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize }));
	inline std::unique_ptr<WndProcHooks> wndproc = std::make_unique<WndProcHooks>();
	inline std::unique_ptr<RendererHooks> renderer = std::make_unique<RendererHooks>();

#if ENGINE_UNREAL
	inline std::unique_ptr<Unreal> unreal = std::make_unique<Unreal>();
#endif

#if ENGINE_UNITY
	inline Mono mono = Mono::Instance();
#endif

}

class BaseFeature; //Forward declaration
inline std::vector<BaseFeature*> g_vecFeatures{};

// Framework Features
#include "../Features/Feature.h"
