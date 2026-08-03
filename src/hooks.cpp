#include "hooks.h"
#include "menu.h"
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <MinHook.h>
#include <cstdint>

HWND window = nullptr;
WNDPROC oWndProc = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;
bool g_showMenu = true;

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);
Present_t oPresent = nullptr;

// Ручная передача событий в ImGui без вызова ImGui_ImplWin32_WndProcHandler
void ProcessInputEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    ImGuiIO& io = ImGui::GetIO();
    switch (uMsg) {
        case WM_MOUSEMOVE:
            io.AddMousePosEvent((float)LOWORD(lParam), (float)HIWORD(lParam));
            break;
        case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
            io.AddMouseButtonEvent(0, true);
            break;
        case WM_LBUTTONUP:
            io.AddMouseButtonEvent(0, false);
            break;
        case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
            io.AddMouseButtonEvent(1, true);
            break;
        case WM_RBUTTONUP:
            io.AddMouseButtonEvent(1, false);
            break;
        case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
            io.AddMouseButtonEvent(2, true);
            break;
        case WM_MBUTTONUP:
            io.AddMouseButtonEvent(2, false);
            break;
        case WM_MOUSEWHEEL:
            io.AddMouseWheelEvent(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
            break;
        case WM_KEYDOWN: case WM_SYSKEYDOWN:
            if (wParam < 256)
                io.AddKeyEvent(ImGuiKey_KeysToKey[wParam], true);
            break;
        case WM_KEYUP: case WM_SYSKEYUP:
            if (wParam < 256)
                io.AddKeyEvent(ImGuiKey_KeysToKey[wParam], false);
            break;
        case WM_CHAR:
            if (wParam > 0 && wParam < 0x10000)
                io.AddInputCharacter((unsigned short)wParam);
            break;
        default: break;
    }
}

LRESULT __stdcall WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (g_showMenu) {
        ProcessInputEvents(hWnd, uMsg, wParam, lParam);
        // Не позволяем ImGui перехватывать управление, если меню открыто
        if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN)
            return true;
    }
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

IDXGISwapChain* FindSwapChain() {
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, DefWindowProc, 0, 0, GetModuleHandle(0), 0, 0, 0, 0, L"DX", 0 };
    RegisterClassExW(&wc);
    HWND hWnd = CreateWindowExW(0, wc.lpszClassName, L"", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, wc.hInstance, 0);
    D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
    DXGI_SWAP_CHAIN_DESC sd = { 0 };
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    ID3D11Device* pTempDevice = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, levels, 1, D3D11_SDK_VERSION, &sd, &pSwapChain, &pTempDevice, 0, 0);
    pTempDevice->Release();
    DestroyWindow(hWnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return pSwapChain;
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (!pDevice) {
        pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
        pDevice->GetImmediateContext(&pContext);
        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);
        window = sd.OutputWindow;
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(pDevice, pContext);
        ImGui::StyleColorsDark();
        oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProcHook);
    }
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    DrawMenu();
    ImGui::Render();
    pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

void SetupHooks() {
    MH_Initialize();
    IDXGISwapChain* pSwapChain = FindSwapChain();
    void** vtable = *(void***)pSwapChain;
    oPresent = (Present_t)vtable[8];
    pSwapChain->Release();
    MH_CreateHook((LPVOID)oPresent, &hkPresent, (LPVOID*)&oPresent);
    MH_EnableHook(MH_ALL_HOOKS);
}
