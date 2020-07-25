#include "wcap.h"
#include "dxoffsets.h"
#include "dummy_window.h"

#include <d3d9.h>
#include <windows.h>

#include <iostream>

namespace wcap
{

typedef IDirect3D9 *(WINAPI *DX9CreateFunc)(UINT);

class TDirect3D9Ctx {
public:
    TDirect3D9Ctx()
        : m_hwnd(_T("dx8 test window"))
        , m_module(0)
        , m_dx9(nullptr)
        , m_device(nullptr)
    {
        if (!m_hwnd) {
            return;
        }
        m_module = LoadLibrary(_T("d3d9.dll"));
        if (!m_module) {
            return;
        }
        DX9CreateFunc create = (DX9CreateFunc)GetProcAddress(m_module, "Direct3DCreate9");
        if (!create)
            return;

        m_dx9 = create(D3D_SDK_VERSION);
        if (!m_dx9) {
            return;
        }
        D3DPRESENT_PARAMETERS presentParams = {};
        presentParams.Windowed = true;
        presentParams.SwapEffect = D3DSWAPEFFECT_FLIP;
        presentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentParams.BackBufferWidth = 4;
        presentParams.BackBufferHeight = 4;
        presentParams.BackBufferCount = 1;
        presentParams.hDeviceWindow = m_hwnd;
        LPDIRECT3DDEVICE9 _device = nullptr;

        m_dx9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
                          &presentParams, &_device);

        m_device = _device;
    }
    ~TDirect3D9Ctx() {
        if (m_device) {
            m_device->Release();
        }
        if (m_dx9) {
            m_dx9->Release();
        }
    }
    HMODULE GetModule() {
        return m_module;
    }
    LPDIRECT3DDEVICE9 GetDevice() {
        return m_device;
    }
private:
    TDummyWindow m_hwnd;
    HMODULE m_module;
    IDirect3D9* m_dx9;
    LPDIRECT3DDEVICE9 m_device;
};


void GetDX9Offsets(uint64_t& present, uint64_t& presentEx) {
    TDirect3D9Ctx dx9;
    present = 0;
    presentEx = 0;
    if (dx9.GetModule() && dx9.GetDevice()) {
        present = wcap::GetVtableOffset((uint64_t)dx9.GetModule(), dx9.GetDevice(), 17);
        presentEx = wcap::GetVtableOffset((uint64_t)dx9.GetModule(), dx9.GetDevice(), 121);
    }
}

}//namespace wcap
