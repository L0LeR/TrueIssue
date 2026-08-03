#include "esp.h"
#include "offsets.h"
#include "memory.h"
#include <imgui.h>
#include <string>
#include <cmath>

// offsets for esp
extern bool bESP, bESP_Box, bESP_Health, bESP_Name, bESP_Skeleton;
extern float esp_maxDist;
extern ImColor esp_color_enemy, esp_color_teammate, esp_color_bot;
extern uintptr_t g_player_manager;
extern uintptr_t g_main_camera;

struct Vector3 { float x, y, z; };
struct Vector2 { float x, y; };

bool WorldToScreen(const Vector3& world, Vector2& screen, float* viewMatrix) {
    float w = viewMatrix[12] * world.x + viewMatrix[13] * world.y + viewMatrix[14] * world.z + viewMatrix[15];
    if (w < 0.01f) return false;
    float invW = 1.0f / w;
    screen.x = (viewMatrix[0] * world.x + viewMatrix[1] * world.y + viewMatrix[2] * world.z + viewMatrix[3]) * invW;
    screen.y = (viewMatrix[4] * world.x + viewMatrix[5] * world.y + viewMatrix[6] * world.z + viewMatrix[7]) * invW;
    return true;
}

void GetViewMatrix(float* matrix) {

// tired getting offset of ViewMatrix with CE so i made it empty sorry :(
  
}

void RunESP() {
    if (!bESP || !g_player_manager) return;
    float viewMatrix[16] = {0};
    GetViewMatrix(viewMatrix);
    if (viewMatrix[0] == 0 && viewMatrix[5] == 0) return;

    ImDrawList* draw = ImGui::GetForegroundDrawList();
    uintptr_t localPlayer = read<uintptr_t>(g_player_manager + player_manager_local_player);
    if (!localPlayer) return;

    uintptr_t dict = read<uintptr_t>(g_player_manager + player_manager_all_players);
    if (!dict) return;
    int count = read<int>(dict + 0x40);
    uintptr_t entries = read<uintptr_t>(dict + 0x18);
    if (!entries) return;

    for (int i = 0; i < count; ++i) {
        uintptr_t player = read<uintptr_t>(entries + i * 0x20 + 0x18);
        if (!player || player == localPlayer) continue;

        uintptr_t moveCtrl = read<uintptr_t>(player + player_controller_movement);
        if (!moveCtrl) continue;
        uintptr_t snap = read<uintptr_t>(moveCtrl + movement_controller_snapshot);
        if (!snap) continue;
        Vector3 pos = read<Vector3>(snap + movement_snapshot_position);

        Vector2 screenPos;
        if (!WorldToScreen(pos, screenPos, viewMatrix)) continue;

        uint8_t team = read<uint8_t>(player + player_controller_team_byte);
        bool isBot = read<uintptr_t>(player + player_controller_bot_controller) != 0;
        ImColor color = esp_color_enemy;
        if (isBot) color = esp_color_bot;
        else if (team == read<uint8_t>(localPlayer + player_controller_team_byte))
            color = esp_color_teammate;

        if (bESP_Name) {
            uintptr_t namePtr = read<uintptr_t>(player + player_controller_nickname);
            if (namePtr) {
                std::string name = (const char*)namePtr;
                draw->AddText(ImVec2(screenPos.x, screenPos.y - 20), color, name.c_str());
            }
        }
        if (bESP_Box) {
            float height = 60;
            draw->AddRect(ImVec2(screenPos.x - 20, screenPos.y - height/2), ImVec2(screenPos.x + 20, screenPos.y + height/2), color);
        }
        if (bESP_Health) {
            uintptr_t hitCtrl = read<uintptr_t>(player + player_controller_hit);
            if (hitCtrl) {
                float health = read<float>(hitCtrl + hit_controller_health);
                float barHeight = 40;
                float barWidth = 4;
                float healthHeight = barHeight * (health / 100.0f);
                draw->AddRectFilled(ImVec2(screenPos.x - 25, screenPos.y - barHeight/2), ImVec2(screenPos.x - 21, screenPos.y + barHeight/2), ImColor(0,0,0,180));
                draw->AddRectFilled(ImVec2(screenPos.x - 25, screenPos.y + barHeight/2 - healthHeight), ImVec2(screenPos.x - 21, screenPos.y + barHeight/2), ImColor(0,255,0,200));
            }
        }
    }
}
