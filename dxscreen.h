#include <functional>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9tex.h>

#include "hook.h"


class DX9Capture
{
public:

    DX9Capture()
    {

    }

    HMODULE GetSystemModule(const char* module)
    {
        static std::string systemPath;
        if (systemPath.empty()) {
            systemPath.resize(2048);
            uint32_t res = GetSystemDirectoryA(&systemPath[0], systemPath.size());
            systemPath.resize(res);
        }

        std::string basePath = systemPath + "\\" + module;
        return GetModuleHandleA(basePath.c_str());
    }

    static void GetDX9Screenshot(IDirect3DDevice9* device)
    {
        HRESULT hr;

        IDirect3DSurface9* backbuffer;
        hr = device->GetRenderTarget(0, &backbuffer);
        if (FAILED(hr))
        {
            return;
        }

        D3DSURFACE_DESC desc;
        hr = backbuffer->GetDesc(&desc);
        if (FAILED(hr))
        {
            return;
        }

        IDirect3DSurface9* buffer;
        hr = device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format,
                                                 D3DPOOL_SYSTEMMEM, &buffer, nullptr);
        if (FAILED(hr))
        {
            return;
        }

        hr = device->GetRenderTargetData(backbuffer, buffer);
        if (FAILED(hr))
        {
            return;
        }

        backbuffer->Release();

        D3DLOCKED_RECT rect;
        hr = buffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
        if (FAILED(hr))
        {
            return;
        }

        //Save surface to file
        //D3DXSaveSurfaceToFile(L"capture.bmp", D3DXIFF_BMP, buffer, NULL, NULL);
        //Save surface to file

        buffer->Release();
    }

    static HRESULT STDMETHODCALLTYPE HookPresent(IDirect3DDevice9* device,
                                                 CONST RECT* srcRect, CONST RECT* dstRect,
                                                 HWND overrideWindow, CONST RGNDATA* dirtyRegion)
    {
        UnHook(PresentFun);
        UnHook(PresentExFun);
        GetDX9Screenshot(device);
        return device->Present(srcRect, dstRect, overrideWindow, dirtyRegion);
    }

    static HRESULT STDMETHODCALLTYPE HookPresentEx(IDirect3DDevice9Ex* device,
                                                   CONST RECT* srcRect, CONST RECT* dstRect,
                                                   HWND overrideWindow, CONST RGNDATA* dirtyRegion,
                                                   DWORD flags)
    {
        UnHook(PresentFun);
        UnHook(PresentExFun);
        GetDX9Screenshot(device);
        return device->PresentEx(srcRect, dstRect, overrideWindow, dirtyRegion, flags);
    }

    bool MakeDX9Screen(const std::function<void(const char*)>& callback,
                       uint64_t presentOffset,
                       uint64_t presentExOffset)
    {
        HMODULE dx9module = GetSystemModule("d3d9.dll");
        if (!dx9module) {
            return false;
        }
        Callback = callback;
        PresentFun = (void*)((uintptr_t)dx9module + (uintptr_t)presentOffset);
        Hook(PresentFun, HookPresent);
        PresentExFun = (void*)((uintptr_t)dx9module + (uintptr_t)presentExOffset);
        Hook(PresentFun, HookPresentEx);

        return true;
    }

private:

    static inline void* PresentFun = nullptr;
    static inline void* PresentExFun = nullptr;
    std::function<void(const char*)> Callback;

};//class DX9Capture
