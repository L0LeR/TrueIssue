#include "aimbot.h"
#include "offsets.h"
#include "memory.h"
#include <cmath>

// offsets for aimbot
extern bool bAimbot, bAimbot_BotsOnly, bAimbot_TeamCheck;
extern float aimbot_fov, aimbot_smooth;
extern int aimbot_bone;
extern uintptr_t g_player_manager;

struct Vector3 { float x, y, z; };

Vector3 CalcAngle(const Vector3& src, const Vector3& dst) {
    Vector3 angle;
    float delta[3] = { dst.x - src.x, dst.y - src.y, dst.z - src.z };
    float hyp = sqrt(delta[0]*delta[0] + delta[2]*delta[2]);
    angle.x = -atan2f(delta[1], hyp) * (180.0f / 3.14159265358979323846f);
    angle.y = atan2f(delta[0], delta[2]) * (180.0f / 3.14159265358979323846f);
    angle.z = 0;
    return angle;
}

float GetFov(const Vector3& curAngle, const Vector3& targetAngle) {
    float dx = targetAngle.x - curAngle.x;
    float dy = targetAngle.y - curAngle.y;
    return sqrt(dx*dx + dy*dy);
}

void RunAimbot() {
    if (!bAimbot || !g_player_manager) return;
    uintptr_t localPlayer = read<uintptr_t>(g_player_manager + player_manager_local_player);
    if (!localPlayer) return;
    uintptr_t localAim = read<uintptr_t>(localPlayer + player_controller_aim);
    if (!localAim) return;
    uint8_t localTeam = read<uint8_t>(localPlayer + player_controller_team_byte);
    uintptr_t localMoveCtrl = read<uintptr_t>(localPlayer + player_controller_movement);
    if (!localMoveCtrl) return;
    uintptr_t localSnap = read<uintptr_t>(localMoveCtrl + movement_controller_snapshot);
    if (!localSnap) return;
    Vector3 localPos = read<Vector3>(localSnap + movement_snapshot_position);
    Vector3 currentAngle = read<Vector3>(localAim + aim_controller_head_angles);

    float bestFov = aimbot_fov;
    Vector3 bestAngle = {};

    uintptr_t dict = read<uintptr_t>(g_player_manager + player_manager_all_players);
    if (!dict) return;
    int count = read<int>(dict + 0x40);
    uintptr_t entries = read<uintptr_t>(dict + 0x18);
    if (!entries) return;

    for (int i = 0; i < count; ++i) {
        uintptr_t player = read<uintptr_t>(entries + i * 0x20 + 0x18);
        if (!player || player == localPlayer) continue;
        if (bAimbot_TeamCheck) {
            if (read<uint8_t>(player + player_controller_team_byte) == localTeam) continue;
        }
        if (bAimbot_BotsOnly) {
            if (!read<uintptr_t>(player + player_controller_bot_controller)) continue;
        }
        uintptr_t moveCtrl = read<uintptr_t>(player + player_controller_movement);
        if (!moveCtrl) continue;
        uintptr_t snap = read<uintptr_t>(moveCtrl + movement_controller_snapshot);
        if (!snap) continue;
        Vector3 targetPos = read<Vector3>(snap + movement_snapshot_position);
        Vector3 angle = CalcAngle(localPos, targetPos);
        float fov = GetFov(currentAngle, angle);
        if (fov < bestFov) {
            bestFov = fov;
            bestAngle = angle;
        }
    }

    if (bestFov < aimbot_fov) {
        Vector3 delta = { bestAngle.x - currentAngle.x, bestAngle.y - currentAngle.y, 0 };
        delta.x /= aimbot_smooth;
        delta.y /= aimbot_smooth;
        Vector3 newAngle = { currentAngle.x + delta.x, currentAngle.y + delta.y, 0 };
        write<Vector3>(localAim + aim_controller_head_angles, newAngle);
    }
}
