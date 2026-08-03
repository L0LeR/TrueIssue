#include "menu.h"
#include <imgui.h>

// i tired to make this hack

extern bool g_showMenu;
extern bool bAimbot, bAimbot_BotsOnly, bAimbot_TeamCheck;
extern float aimbot_fov, aimbot_smooth;
extern int aimbot_bone;
extern bool bESP, bESP_Box, bESP_Health, bESP_Name, bESP_Skeleton;
extern float esp_maxDist;
extern ImColor esp_color_enemy, esp_color_teammate, esp_color_bot;
extern bool bGodMode, bInfAmmo, bRapidFire, bNoRecoil, bSpeedHack;
extern float speedHackValue;

void DrawMenu() {
    if (!g_showMenu) return;
    ImGui::Begin("TrueIssue | made by t.me/ZeroxSlop <3", &g_showMenu, ImGuiWindowFlags_NoCollapse);
    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("Aimbot")) {
            ImGui::Checkbox("Enable", &bAimbot);
            ImGui::SliderFloat("FOV", &aimbot_fov, 0.1f, 45.0f);
            ImGui::SliderFloat("Smooth", &aimbot_smooth, 0.1f, 10.0f);
            ImGui::Combo("Bone", &aimbot_bone, "Head\0Neck\0Chest\0\0");
            ImGui::Checkbox("Bots only", &bAimbot_BotsOnly);
            ImGui::Checkbox("Team check", &bAimbot_TeamCheck);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ESP")) {
            ImGui::Checkbox("Enable", &bESP);
            ImGui::Checkbox("Box", &bESP_Box);
            ImGui::Checkbox("Health", &bESP_Health);
            ImGui::Checkbox("Name", &bESP_Name);
            ImGui::Checkbox("Skeleton", &bESP_Skeleton);
            ImGui::SliderFloat("Max Distance", &esp_maxDist, 0.0f, 1000.0f);
            ImGui::ColorEdit3("Enemy", (float*)&esp_color_enemy);
            ImGui::ColorEdit3("Teammate", (float*)&esp_color_teammate);
            ImGui::ColorEdit3("Bot", (float*)&esp_color_bot);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Misc")) {
            ImGui::Checkbox("God Mode", &bGodMode);
            ImGui::Checkbox("Infinite Ammo", &bInfAmmo);
            ImGui::Checkbox("Rapid Fire", &bRapidFire);
            ImGui::Checkbox("No Recoil", &bNoRecoil);
            ImGui::Checkbox("Speed Hack", &bSpeedHack);
            ImGui::SliderFloat("Speed", &speedHackValue, 0.5f, 5.0f);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Config")) {
            if (ImGui::Button("Save")) {}
            ImGui::SameLine();
            if (ImGui::Button("Load")) {}
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}
