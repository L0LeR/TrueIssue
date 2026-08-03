#include <imgui.h>

bool bAimbot = true;
bool bAimbot_BotsOnly = true;
bool bAimbot_TeamCheck = true;
float aimbot_fov = 5.0f;
float aimbot_smooth = 2.0f;
int aimbot_bone = 0;

bool bESP = true;
bool bESP_Box = true;
bool bESP_Health = true;
bool bESP_Name = true;
bool bESP_Skeleton = false;
float esp_maxDist = 500.0f;
ImColor esp_color_enemy = ImColor(255, 0, 0, 255);
ImColor esp_color_teammate = ImColor(0, 255, 0, 255);
ImColor esp_color_bot = ImColor(255, 255, 0, 255);

bool bGodMode = false;
bool bInfAmmo = false;
bool bRapidFire = false;
bool bNoRecoil = false;
bool bSpeedHack = false;
float speedHackValue = 1.5f;
