#include "hooks.h"
#include "menu.h"
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <MinHook.h>

HWND window = nullptr;
WNDPROC oWndProc = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;
bool g_showMenu = true;

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);
Present_t oPresent = nullptr;

LRESULT __stdcall WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (g_showMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

IDXGISwapChain* FindSwapChain() {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0, 0, GetModuleHandle(0), 0, 0, 0, 0, L"DX", 0 };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(wc.lpszClassName, L"", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, wc.hInstance, 0);
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
    UnregisterClass(wc.lpszClassName, wc.hInstance);
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
