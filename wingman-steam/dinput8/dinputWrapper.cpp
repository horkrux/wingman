#include "dinputWrapper.h"

HRESULT WINAPI DirectInput8Create(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk)
{
    return oDirectInput8Create(inst_handle, version, r_iid, out_wrapper, p_unk);
}

HRESULT WINAPI DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter)
{
    return oDirectInputCreateA(hinst, dwVersion, ppDI, punkOuter);
}

HRESULT WINAPI DirectInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTW* ppDI, LPUNKNOWN punkOuter)
{
    return oDirectInputCreateW(hinst, dwVersion, ppDI, punkOuter);
}