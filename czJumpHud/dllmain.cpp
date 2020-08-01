// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "czApi.h"
#include "czJumpHud.h"

bool HudElement(std::string title, bool force_title)
{
    ImGui::SetNextWindowPos(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    static bool czVelocity = true;
    static float titlebar_height;
    if (czJumpHudMenu || force_title)
    {
        if (!ImGui::Begin(title.c_str(), &czVelocity, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::End();
            return false;
        }
    }
    else {
        if (!ImGui::Begin(title.c_str(), &czVelocity, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration))
        {
            ImGui::End();
            return false;
        }
    }
    return true;
}

static refdef_t* refDef = reinterpret_cast<refdef_t*>(0x00797600);
static Vec3* ViewOrg = (Vec3*)0xcee2814;
static CViewAngles* viewAngles = reinterpret_cast<CViewAngles*>(0xC84FD8);
void hudPosition()
{
    static int* ConnectionState = (int*)0xC5F900;
    ImGui::SetNextWindowBgAlpha(hud::pos->opacity);
    bool open_popup = false;
    
    
    /*if (*ConnectionState != 9 && czJumpHudMenu)
        ImGui::Text("Position");*/
    if ((*ConnectionState == 9 || czJumpHudMenu) && hud::pos->enabled && HudElement("Hud Position", false))
    {
        ImGui::SetWindowFontScale(hud::pos->fontSize);
        std::ostringstream ref;
        ref.str(std::string());
        ref.clear();
        ImGui::Spacing();
        ImGui::TextColored(hud::pos->tColor, "Position:");
        ImGui::Separator();
        ref.precision(6);
        ref << "X: " << std::fixed << ViewOrg->x << std::endl << "Y: " << ViewOrg->y << std::endl << "Z: " << ViewOrg->z << std::endl << "V: " << refDef->refdefViewAngles.y;
        ImGui::TextColored(hud::pos->tColor, ref.str().c_str());
        if (ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowHovered())
            open_popup = true;
        ImGui::End();
        if (open_popup)
            ImGui::OpenPopup("posPopup");
    }
    if (ImGui::BeginPopup("posPopup"))
    {
        ImGui::SliderFloat("Font Size", &hud::pos->fontSize, 0, 8.0, "%f", 1.0f);
        ImGui::SliderFloat("Background Opacity", &hud::pos->opacity, 0, 1.0, "%f", 1.0f);
        ImGui::ColorEdit4("Font Color", (float*)&hud::pos->tColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview);
        ImGui::EndPopup();
    }

}

void hudVelocity()
{
    static int prev_speed = 0;
    static ULONGLONG next_update = GetTickCount64();
    static int* ConnectionState = (int*)0xC5F900;
    ImGui::SetNextWindowBgAlpha(hud::vel->opacity);
    Vec3* Velocity = (Vec3*)0x79449c;
    float x_vel = pow(fabsf(Velocity->x), 2);
    float y_vel = pow(fabsf(Velocity->y), 2);
    float speed = sqrt(x_vel + y_vel);
    bool open_popup = false;
    if ((*ConnectionState == 9 || czJumpHudMenu) && hud::vel->enabled && HudElement("Hud Velocity", false))
    {
        ImGui::SetWindowFontScale(hud::vel->fontSize);

        std::ostringstream str;
        str << (int)speed;
        std::string	 val = str.str();
        ImVec4 speed_color;
        if (speed > prev_speed)
            speed_color = ImVec4(0.0f, 1.0f, 0.0f, 0.8f);
        if (speed == prev_speed)
            speed_color = ImVec4(1.0f, 1.0f, 1.0f, 0.8f);
        if (speed < prev_speed)
            speed_color = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
        if (next_update <= GetTickCount64())
        {
            prev_speed = speed;
            next_update = GetTickCount64() + 50;
        }

        float x1 = ImGui::GetWindowSize().x / 2;
        float x2 = ImGui::CalcTextSize(val.c_str()).x / 2;
        float y1 = ImGui::GetWindowSize().y / 2;
        float y2 = ImGui::CalcTextSize(val.c_str()).y / 2;
        ImGui::SetCursorPos(ImVec2(x1 - x2, y1 - y2));
        ImGui::TextColored(speed_color, val.c_str());

        if (ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowHovered())
            open_popup = true;
        ImGui::End();
        if (open_popup)
            ImGui::OpenPopup("velPopup");
    }
    if (ImGui::BeginPopup("velPopup"))
    {
        ImGui::SliderFloat("Font Size", &hud::vel->fontSize, 0, 8.0, "%f", 1.0f);
        ImGui::SliderFloat("Background Opacity", &hud::vel->opacity, 0, 1.0, "%f", 1.0f);
        //  ImGui::ColorEdit4("Font Color", (float*)&hud::veltColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview);
        ImGui::EndPopup();
    }

}

ex_czApi void draw(LPDIRECT3DDEVICE9 dev, DWORD* imgui_context)
{

    if (GImGui == nullptr)
        GImGui = (ImGuiContext*)imgui_context;
    static ULONGLONG lastUpdate = GetTickCount64();
    if (GetTickCount64() - lastUpdate >= 1000)
    {
        czConfig::saveSettings();
        lastUpdate = GetTickCount64();
    }

    hudVelocity();
    hudPosition();

    if (!czJumpHudMenu)
        return;
    ImGui::SetNextWindowPos(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(785, 265), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Jump Hud", &czJumpHudMenu, ImGuiWindowFlags_None))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Checkbox("Velocity Hud", &hud::vel->enabled))
    {
        if (hud::vel->enabled)
            czApi::addHudWindows(1);
        else
            czApi::removeHudWindows(1);

    }
    if (ImGui::Checkbox("Position Hud", &hud::pos->enabled))
    {
        if (hud::pos->enabled)
            czApi::addHudWindows(1);
        else
            czApi::removeHudWindows(1);

    }
    if (ImGui::GetIO().KeysDown[VK_ESCAPE])
    {
        ImGui::GetIO().KeysDown[VK_ESCAPE] = false;
        czJumpHudMenu = false;
    }
    ImGui::End();

}

ex_czApi void init(HMODULE mainMod)
{

    czConfig::loadSettings();

  if (hud::vel->enabled)
       czApi::addHudWindows(1);
   if (hud::pos->enabled)
       czApi::addHudWindows(1);

    czApi::mainMod = mainMod;
    czApi::writeConsole("czJumpHud: Enabled");
    //czJumpHudMenu = true;
}
ex_czApi void shutDown()
{
    if (hud::vel->enabled)
        czApi::removeHudWindows(1);
    if (hud::pos->enabled)
        czApi::removeHudWindows(1);
    czConfig::saveSettings();
    czApi::writeConsole("czJumpHud: Disabled");
}

ex_czApi void loadFonts()
{

}
ex_czApi void getInfo(pluginInfo* pl_info)
{
    pl_info->Author = "Human/Terry";
    pl_info->Version = "1.0";
    pl_info->Description = "Hud for jumping";
    pl_info->UIBool = (uint64_t)&czJumpHudMenu;
}

ex_czApi bool keyboardEvent(WPARAM Keycode, UINT WindowsMessage) //return true if handled (blocked input from going further), false if not handled and needs passed on
{
    static auto lastchange = GetTickCount64() - 500;
    if (GetTickCount64() - lastchange > 50 && WindowsMessage == WM_KEYDOWN) //no spam
    {
        if (static_cast<UINT_PTR>(Keycode) == VK_INSERT)
        {
            lastchange = GetTickCount64();
            //do stuff
            float view_angle = refDef->refdefViewAngles[1];
            for (int i = 0; i < 4; i++)
            {
                float cur_angle = i * 90;
                float prev_half_angle = fmodf((cur_angle - 45 + 360), 360);
                float next_half_angle = fmodf((cur_angle + 45 + 360), 360);
                if ((i != 0 && view_angle > prev_half_angle && view_angle < next_half_angle) || (i == 0 && view_angle > prev_half_angle || view_angle < next_half_angle))
                {
                    viewAngles->x += cur_angle - view_angle;
                    break;
                }
            }
            return true;
        }
    }
    return false;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        thisMod = hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

