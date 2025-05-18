#pragma once
#include "dinput.h"

typedef HRESULT(WINAPI *tDirectInput8Create)(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk);
typedef HRESULT(WINAPI* tDirectInputCreateA)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter);
typedef HRESULT(WINAPI* tDirectInputCreateW)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTW* ppDI, LPUNKNOWN punkOuter);

extern tDirectInput8Create oDirectInput8Create;
extern tDirectInputCreateA oDirectInputCreateA;
extern tDirectInputCreateW oDirectInputCreateW;