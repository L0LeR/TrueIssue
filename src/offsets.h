#pragma once
#include <cstdint>

// i bored with CE to find GameAssembly needs so i deleted it

// PlayerManager
constexpr uintptr_t player_manager_all_players = 0x28;
constexpr uintptr_t player_manager_local_player = 0x40;
constexpr uintptr_t player_manager_players_by_name = 0x50;
constexpr uintptr_t player_manager_snapshot_delegate = 0x68;

// PlayerController
constexpr uintptr_t player_controller_bot_controller = 0xC0;
constexpr uintptr_t player_controller_aim = 0xC8;
constexpr uintptr_t player_controller_weaponry = 0xD0;
constexpr uintptr_t player_controller_movement = 0xE0;
constexpr uintptr_t player_controller_hit = 0xF0;
constexpr uintptr_t player_controller_network = 0x108;
constexpr uintptr_t player_controller_biped = 0x118;
constexpr uintptr_t player_controller_team_byte = 0x79;
constexpr uintptr_t player_controller_nickname = 0x158;

// HitController
constexpr uintptr_t hit_controller_health = 0x20;

// MovementController
constexpr uintptr_t movement_controller_transform = 0x90;
constexpr uintptr_t movement_controller_snapshot = 0x88;

// MovementSnapshot
constexpr uintptr_t movement_snapshot_position = 0x40;
constexpr uintptr_t movement_snapshot_velocity = 0x4C;
constexpr uintptr_t movement_snapshot_euler = 0x98;

// AimController
constexpr uintptr_t aim_controller_head_angles = 0xB8;

// GunController
constexpr uintptr_t gun_controller_current_ammo = 0xE4;
constexpr uintptr_t gun_controller_reserve_ammo = 0xE6;
constexpr uintptr_t gun_controller_fire_rate = 0x100;
constexpr uintptr_t gun_controller_accuracy = 0x140;

// WeaponryController
constexpr uintptr_t weaponry_controller_current_weapon = 0x98;

// PlayerMainCamera
constexpr uintptr_t player_main_camera_camera = 0x20;

// PlayerCheatParameters
constexpr uintptr_t cheat_params_god_mode = 0x22;
constexpr uintptr_t cheat_params_health = 0x24;
constexpr uintptr_t cheat_params_speed = 0x10;

// WeaponCheatParameters
constexpr uintptr_t cheat_weapon_inf_ammo = 0x10;
constexpr uintptr_t cheat_weapon_anti_recoil = 0x12;
constexpr uintptr_t cheat_weapon_wallshot = 0x13;

// BipedMap
constexpr uintptr_t biped_head = 0x20;
constexpr uintptr_t biped_neck = 0x28;
constexpr uintptr_t biped_spine = 0x30;
constexpr uintptr_t biped_spine1 = 0x38;
constexpr uintptr_t biped_spine2 = 0x40;
constexpr uintptr_t biped_left_shoulder = 0x48;
constexpr uintptr_t biped_left_upperarm = 0x50;
constexpr uintptr_t biped_left_forearm = 0x58;
constexpr uintptr_t biped_left_hand = 0x60;
constexpr uintptr_t biped_right_shoulder = 0x68;
constexpr uintptr_t biped_right_upperarm = 0x70;
constexpr uintptr_t biped_right_forearm = 0x78;
constexpr uintptr_t biped_right_hand = 0x80;
constexpr uintptr_t biped_hip = 0x88;
constexpr uintptr_t biped_left_upleg = 0x90;
constexpr uintptr_t biped_left_leg = 0x98;
constexpr uintptr_t biped_left_foot = 0xA0;
constexpr uintptr_t biped_left_toe = 0xA8;
constexpr uintptr_t biped_right_upleg = 0xB0;
constexpr uintptr_t biped_right_leg = 0xB8;
constexpr uintptr_t biped_right_foot = 0xC0;
constexpr uintptr_t biped_right_toe = 0xC8;
constexpr uintptr_t biped_weapon_container = 0x250;
