#include "misc.h"
#include "offsets.h"
#include "memory.h"

// offsets for misc section lol
extern bool bGodMode, bInfAmmo, bRapidFire, bNoRecoil, bSpeedHack;
extern float speedHackValue;
extern uintptr_t g_player_manager;

void RunNoRecoil() {
    if (!g_player_manager) return;
    uintptr_t localPlayer = read<uintptr_t>(g_player_manager + player_manager_local_player);
    if (!localPlayer) return;
    uintptr_t weaponry = read<uintptr_t>(localPlayer + player_controller_weaponry);
    if (!weaponry) return;
    uintptr_t currentWeapon = read<uintptr_t>(weaponry + weaponry_controller_current_weapon);
    if (!currentWeapon) return;
    uintptr_t gunParams = read<uintptr_t>(currentWeapon + 0xA8);
    if (!gunParams) return;
    write<int>(gunParams + 0xD8, 0);
    write<float>(gunParams + 0x100, 0.0f);
    write<float>(gunParams + 0x108, 0.0f);
    write<float>(gunParams + 0x110, 0.0f);
}

void RunSpeedHack() {
    if (!bSpeedHack) return;
    if (!g_player_manager) return;
    uintptr_t localPlayer = read<uintptr_t>(g_player_manager + player_manager_local_player);
    if (!localPlayer) return;
    uintptr_t moveCtrl = read<uintptr_t>(localPlayer + player_controller_movement);
    if (!moveCtrl) return;
    uintptr_t xoParams = read<uintptr_t>(moveCtrl + 0xE8);
    if (!xoParams) return;
    float curSpeed = read<float>(xoParams + 0x10);
    write<float>(xoParams + 0x10, curSpeed * speedHackValue);
    float curAccel = read<float>(xoParams + 0x14);
    write<float>(xoParams + 0x14, curAccel * speedHackValue);
    float curJump = read<float>(xoParams + 0x18);
    write<float>(xoParams + 0x18, curJump * speedHackValue);
}

void RunMisc() {
    if (!g_player_manager) return;
    uintptr_t localPlayer = read<uintptr_t>(g_player_manager + player_manager_local_player);
    if (!localPlayer) return;

    if (bGodMode) {
        uintptr_t hitCtrl = read<uintptr_t>(localPlayer + player_controller_hit);
        if (hitCtrl) {
            write<float>(hitCtrl + hit_controller_health, 1000.0f);
        }
    }
    if (bInfAmmo) {
        uintptr_t weaponry = read<uintptr_t>(localPlayer + player_controller_weaponry);
        if (weaponry) {
            uintptr_t gun = read<uintptr_t>(weaponry + weaponry_controller_current_weapon);
            if (gun) {
                write<short>(gun + gun_controller_current_ammo, 999);
                write<short>(gun + gun_controller_reserve_ammo, 999);
            }
        }
    }
    if (bRapidFire) {
        uintptr_t weaponry = read<uintptr_t>(localPlayer + player_controller_weaponry);
        if (weaponry) {
            uintptr_t gun = read<uintptr_t>(weaponry + weaponry_controller_current_weapon);
            if (gun) {
                write<float>(gun + gun_controller_fire_rate, 0.01f);
            }
        }
    }
    if (bNoRecoil) {
        RunNoRecoil();
    }
    if (bSpeedHack) {
        RunSpeedHack();
    }
}
