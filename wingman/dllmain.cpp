// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MinHook/include/MinHook.h"

bool ApplyPatches();
void calcProbeDroidBounce(float*, float);

DWORD bCamFix = 0;
DWORD bCalcProbeDroidBounce = 0;
DWORD bFighterSwayFix1 = 0;
DWORD bFighterSwayFix2 = 0;
DWORD bSmokeFix1 = 0;
DWORD bSmokeFix2 = 0;

float* pFrametime = (float*)0x6D01F8;
float frametimeTemp = 0;
float Decrease50_toScale = 0.02f;
float Decrease48_toScale = 0.11f;
float Increase50_toScale = 0.1f;

//BULLSHIT
float LowerLimit48 = 0.050000001f;
float LowerLimit4C = 0.0f;
float zeroFloatLol = 0.0f;
float UpperLimit48BeforeDamp = 0.5f;
float frametime30FPS = 0.033333335f;
float oneFloatLol = 1.0f;

char lodPatch1[6] = { 0xE9, 0xD8, 0x01, 0x00, 0x00, 0x90 };
char lodPatch2[2] = { 0xEB, 0x71 };
char camPatch[6] = { 0xE9, 0xE7, 0x00, 0x00, 0x00, 0x90 };

int currIdx = 0;
float shitfuck[1000];

char lodPatchOrigBytes[6] = { 0x0F, 0x85, 0xD7, 0x01, 0, 0 };


float smokeScale = 0.1f;
__declspec(naked) void tSmokeFix1() {
    __asm 
    {
        fld dword ptr [smokeScale]
        mov eax, 6D01F8h
        fmul dword ptr [eax]
        fdiv dword ptr [frametime30FPS]
        fadd dword ptr [oneFloatLol]
        fmul dword ptr [ecx+20h]
        jmp [bSmokeFix1]
    }
    
}

__declspec(naked) void tSmokeFix2() {
    __asm
    {
        fld dword ptr [smokeScale]
        mov ecx, 6D01F8h
        fmul dword ptr [ecx]
        fdiv dword ptr [frametime30FPS]
        fadd dword ptr [oneFloatLol]
        fmul dword ptr [eax+24h]
        jmp [bSmokeFix2]
    }

}

__declspec(naked) void tFighterSwayFix1() {
    __asm
    {
        fmul dword ptr [edx+74h]
        fmul dword ptr [ebp+14h]
        fdiv dword ptr [frametime30FPS]
        mov eax, [ebp+0Ch]
        jmp [bFighterSwayFix1]
    }
}

__declspec(naked) void tFighterSwayFix2() {
    __asm
    {
        fld dword ptr [ecx+78h]
        fmul dword ptr [ebp+14h]
        fdiv dword ptr [frametime30FPS]
        fadd dword ptr [eax+0A4h]
        ; mov edx, [ebp+0Ch]
        jmp [bFighterSwayFix2]
    }
}

__declspec(naked) void tCamFix()
{
    __asm
    {
        mov dword ptr [ebp-80h], 00000000
        mov dword ptr [ebp+0Ch], 3C888889h
        jmp [bCamFix]
    }
}

__declspec(naked) void tCalcProbeDroidBounce()
{
    __asm
    {
        call calcProbeDroidBounce
        jmp [bCalcProbeDroidBounce]
    }
}

void calcProbeDroidBounce(float* values, float dt) {

    float* multi = &values[20];
    const float lowerLimit = 0.0f;
    const float upperSoftLimit = 0.5f;
    const float upperHardLimit = 1.0f;
    const float minClamp = 0.050000001f;
    const float maxClamp = 1.0f;

    //go down
    if (values[19] <= 0.0f) {
        float decr = 1.0f - 0.1f * (dt / frametime30FPS);
        values[18] = values[18] * decr;
        
        if (values[18] < minClamp) {
            values[19] = minClamp;
            *multi = 1.0f + 0.1f * (dt / frametime30FPS);
        }
    }
    //go up
    else {
        if (values[18] > upperSoftLimit) {
            //dampen
            float dampen = 1.0f - 0.02f * pow(dt / frametime30FPS, 2);
            *multi = *multi * dampen;

            if (*multi < 1.0f) {
                *multi = 1.0f;
            }
        }

        values[19] = values[19] * *multi;
        values[18] = values[19];

        //no longer changing or reached limit
        if (*multi == 1.0f || values[18] >= upperHardLimit) {
            values[19] = 0.0f;
        }
    }
}

bool ApplyPatches() {
    DWORD oldProtect;
    DWORD hookAddress = 0x45E39C;
    SIZE_T hookLength = 6;
    bool result = false;

    if (!VirtualProtect((LPVOID)hookAddress, 6, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    memcpy((void*)0x45E39C, camPatch, 6);

    //bCamFix = hookAddress + hookLength;
    //result = MH_CreateHook((LPVOID)hookAddress, (LPVOID)tCamFix, 0);
    //if (result != 0) MessageBox(NULL, L"ha", L"HEsss", MB_OK);
    //result = MH_EnableHook((LPVOID)hookAddress);
    //if (result != 0) MessageBox(NULL, L"ha", L"HEsss", MB_OK);
    //VirtualProtect((LPVOID)0x45E046, 7, oldProtect, &oldProtect);

    hookAddress = 0x4A4ADA;
    hookLength = 5;

    if (!VirtualProtect((LPVOID)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    bCalcProbeDroidBounce = hookAddress + hookLength;
    result = MH_CreateHook((LPVOID)hookAddress, (LPVOID)tCalcProbeDroidBounce, 0);
    result = MH_EnableHook((LPVOID)hookAddress);

    hookAddress = 0x405A5D;
    hookLength = 6;

    if (!VirtualProtect((LPVOID)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    bFighterSwayFix1 = hookAddress + hookLength;
    result = MH_CreateHook((LPVOID)hookAddress, (LPVOID)tFighterSwayFix1, 0);
    result = MH_EnableHook((LPVOID)hookAddress);

    hookAddress = 0x405D0C;
    hookLength = 9;

    if (!VirtualProtect((LPVOID)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    bFighterSwayFix2 = hookAddress + hookLength;
    result = MH_CreateHook((LPVOID)hookAddress, (LPVOID)tFighterSwayFix2, 0);
    result = MH_EnableHook((LPVOID)hookAddress);

    hookAddress = 0x4C3786;
    hookLength = 9;

    if (!VirtualProtect((LPVOID)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    bSmokeFix1 = hookAddress + hookLength;
    result = MH_CreateHook((LPVOID)hookAddress, (LPVOID)tSmokeFix1, 0);
    result = MH_EnableHook((LPVOID)hookAddress);

    hookAddress = 0x4C3798;
    hookLength = 9;

    if (!VirtualProtect((LPVOID)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    bSmokeFix2 = hookAddress + hookLength;
    result = MH_CreateHook((LPVOID)hookAddress, (LPVOID)tSmokeFix2, 0);
    result = MH_EnableHook((LPVOID)hookAddress);

    if (!VirtualProtect((LPVOID)0x471619, 6, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    memcpy((void*)0x471619, (void*)lodPatch1, 6);

    if (!VirtualProtect((LPVOID)0x471A5C, 2, PAGE_EXECUTE_READWRITE, &oldProtect))
        return true;

    memcpy((void*)0x471A5C, (void*)lodPatch2, 2);

    return false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    FILE* fp = NULL;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MH_Initialize();
        /*AllocConsole();
        freopen_s(&fp, "CONIN$", "r", stdin);
        freopen_s(&fp, "CONOUT$", "w", stdout);*/
        ApplyPatches();

        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        MH_Uninitialize();
        /*FreeConsole();

        if (fp)
            fclose(fp);*/
        break;
    }
    return TRUE;
}

