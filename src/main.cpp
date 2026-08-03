#include <Windows.h>
#include <thread>
#include "hooks.h"
#include "memory.h"
#include "offsets.h"
#include "aimbot.h"
#include "esp.h"
#include "misc.h"

uintptr_t g_game_assembly_base = 0;
uintptr_t g_player_manager = 0;
uintptr_t g_main_camera = 0;

typedef void* Il2CppDomain;
typedef void* Il2CppImage;
typedef void* Il2CppClass;
typedef void* Il2CppObject;
typedef struct { const char* name; int32_t offset; void* type; } FieldInfo;

typedef Il2CppDomain (*il2cpp_domain_get_t)();
typedef Il2CppClass  (*il2cpp_class_from_name_t)(Il2CppImage* image, const char* ns, const char* name);
typedef FieldInfo*   (*il2cpp_class_get_field_from_name_t)(Il2CppClass* klass, const char* name);
typedef void         (*il2cpp_field_static_get_value_t)(FieldInfo* field, void* value);

bool InitIL2CPP() {
    HMODULE gameAssembly = GetModuleHandleA("GameAssembly.dll");
    if (!gameAssembly) return false;

    auto p_domain_get = (il2cpp_domain_get_t)GetProcAddress(gameAssembly, "il2cpp_domain_get");
    auto p_class_from_name = (il2cpp_class_from_name_t)GetProcAddress(gameAssembly, "il2cpp_class_from_name");
    auto p_get_field = (il2cpp_class_get_field_from_name_t)GetProcAddress(gameAssembly, "il2cpp_class_get_field_from_name");
    auto p_static_get = (il2cpp_field_static_get_value_t)GetProcAddress(gameAssembly, "il2cpp_field_static_get_value");

    if (!p_domain_get || !p_class_from_name || !p_get_field || !p_static_get)
        return false;

    Il2CppClass* pmClass = (Il2CppClass*)p_class_from_name(nullptr, "Chillow.StandChillow.Player", "PlayerManager");
    if (pmClass) {
        FieldInfo* instanceField = p_get_field(pmClass, "_instance");
        if (instanceField) {
            uintptr_t instancePtr = 0;
            p_static_get(instanceField, &instancePtr);
            if (instancePtr)
                g_player_manager = *(uintptr_t*)instancePtr;
        }
    }

    Il2CppClass* camClass = (Il2CppClass*)p_class_from_name(nullptr, "Chillow.StandChillow.Player", "PlayerMainCamera");
    if (camClass) {
        FieldInfo* camInstanceField = p_get_field(camClass, "_instance");
        if (camInstanceField) {
            uintptr_t camInstancePtr = 0;
            p_static_get(camInstanceField, &camInstancePtr);
            if (camInstancePtr)
                g_main_camera = *(uintptr_t*)camInstancePtr;
        }
    }

    return g_player_manager != 0;
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    Sleep(3000);
    while (!InitIL2CPP())
        Sleep(1000);
    SetupHooks();

    while (true) {
        RunAimbot();
        RunESP();
        RunMisc();
        Sleep(10);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
    }
    return TRUE;
}
