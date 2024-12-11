#include "pch.h"
#include <intrin.h>

void DummyFn()
{
	Utils::LogDebug(std::format("ReturnAddress: {:#010x}", reinterpret_cast<uintptr_t>(_ReturnAddress())));
}


DWORD __stdcall FrameworkInit(LPVOID lpParam)
{
	// Why must we resort to hacks when we have simple issues.
	AppendFeatures();

	{
		char szModuleName[1024]{};
		if (!GetModuleFileNameA(Framework::hModule, szModuleName, sizeof(szModuleName)))
			return false;

		Framework::iModuleNameHash = CRC64::hash(szModuleName);
	}
	
#ifdef _DEBUG
	Framework::console->SetVisibility(true); // Set the console to be visible by default if the framework is in debug mode
#endif
	// Initalize MinHook
	if (MH_Initialize() != MH_STATUS::MH_OK)
		return false;

	if (!Framework::wndproc.get()->Setup())
		return false;

	if (!Framework::renderer.get()->Setup())
		return false;

	Utils::LogDebug(std::format("PrivilegedHandle: {:#010x}", reinterpret_cast<uintptr_t>(Memory::GetPrivilegedHandleToProcess())));
	if (!Memory::ResetTrampolineCollection())
		return false;

	DummyFn();
	for (size_t i = 0; i < 5; ++i)
		Memory::CallSpoofed(DummyFn);

#if FRAMEWORK_UNREAL
	if (!FrameworkUnrealInit())
		return false;
#endif

#if FRAMEWORK_UNREAL
	Utils::LogDebug("Initializing FNames, this can take a bit.");
	Utils::LogDebug(std::format("Unreal: 0x{:x}", Framework::unreal.get()));
	FNames::Initialize();
	Utils::LogDebug("FNames initialized!");
#endif

	try {
		for (auto& pFeature : g_vecFeatures) {
			if (pFeature->SetupMenu() && pFeature->Setup())
				continue;
			
			Utils::LogError(std::format("Feature \"{}\" failed SetupMenu or Setup", pFeature->GetName()));
			return false;
		}
	}
	catch (const std::exception& e) {
		Utils::LogError(e.what());
		return false;
	}

	Framework::config = std::make_unique<Config>(); // Initalize the config class

	Framework::lua = std::make_unique<Lua>();

	Framework::bInitalized = true;
	Utils::LogDebug(std::format("{}: Initialized", Framework::Title));

	Framework::lua.get()->AutoRun();

	while (Framework::bShouldRun)
	{
		#if FRAMEWORK_UNREAL
			Framework::unreal.get()->RefreshActorList();
		#endif

		for (auto& pFeature : g_vecFeatures)
			pFeature->Run();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	Framework::console->SetVisibility(true); // Set the console to be visible when the cheat is unloading
	Utils::LogDebug(std::format("{}: Unloading...", Framework::Title)); // Log that the cheat is unloading

	delete Framework::lua.release();

	Framework::wndproc.get()->Destroy();
	Framework::renderer.get()->Destroy();

	// Destroy features
	for (auto& pFeature : g_vecFeatures)
		pFeature->Destroy();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	Framework::console->Destroy();

	// Uninitialize MinHook, will also disable any lurking hooks still active as a safety net
	MH_Uninitialize();

	// Unload the module and exit the thread
	FreeLibraryAndExitThread(Framework::hModule, EXIT_SUCCESS);
	return true; // Return true if the initalization was successful
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	Framework::hModule = hModule;

	if (ulReasonForCall != DLL_PROCESS_ATTACH)
		return TRUE;

	Memory::SpoofThreadAddress(FrameworkInit, hModule);

	return TRUE;
}