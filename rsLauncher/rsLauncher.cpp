// rsLauncher.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "rsLauncher.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    
    LPCSTR DllPath = "wingman.dll";
    LPCWSTR appName = L"Rogue Squadron.exe";
    LPCWSTR titleName = L"ROGUE SQUADRON";

    CString appPath;
    LPWSTR appPathBuf = appPath.GetBufferSetLength(MAX_PATH);
    appPathBuf[0] = L'"';

    GetCurrentDirectory(MAX_PATH, appPathBuf + 1);
    appPath.ReleaseBuffer();
    appPath.Append(_T("\\Rogue Squadron.exe\""));
    appPath.Append(lpCmdLine);
   
    STARTUPINFO startup_info;
    PROCESS_INFORMATION process_info;
    memset(&startup_info, 0, sizeof(STARTUPINFO));
    startup_info.cb = sizeof(STARTUPINFO);
    memset(&process_info, 0, sizeof(PROCESS_INFORMATION));

    BOOL rv = ::CreateProcess(
        appName,          // LPCTSTR lpApplicationName
        appPath.GetBuffer(),                // LPTSTR lpCommandLine
        NULL,                // LPSECURITY_ATTRIBUTES lpProcessAttributes
        NULL,                // LPSECURITY_ATTRIBUTES lpThreadAttributes
        FALSE,               // BOOL bInheritHandles
        STARTF_TITLEISAPPID,    // DWORD dwCreationFlags
        NULL,                // LPVOID lpEnvironment
        NULL,                // LPCTSTR lpCurrentDirectory
        &startup_info,       // LPSTARTUPINFO lpStartupInfo
        &process_info        // LPPROCESS_INFORMATION lpProcessInformation
    );


    if (rv == FALSE) {
        MessageBox(NULL, L"CreateProcess() failed. Forgot to run as admin?", L"error", MB_OK);
        appPath.ReleaseBuffer();
        return 0;
    }

    LPVOID pDllPath = VirtualAllocEx(process_info.hProcess, 0, strlen(DllPath) + 1,
        MEM_COMMIT, PAGE_READWRITE);

    if (pDllPath)
    {
        WriteProcessMemory(process_info.hProcess, pDllPath, (LPVOID)DllPath,
            strlen(DllPath) + 1, 0);

        HANDLE hLoadThread = CreateRemoteThread(process_info.hProcess, 0, 0,
            (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
                "LoadLibraryA"), pDllPath, 0, 0);

        if (hLoadThread) {
            WaitForSingleObject(hLoadThread, INFINITE);
            CloseHandle(hLoadThread);
        }
        else {
            MessageBox(NULL, L"Failed to create thread to load lib", L"error", MB_OK);
        }
    }
        

    appPath.ReleaseBuffer();

    if (pDllPath)
        VirtualFreeEx(process_info.hProcess, pDllPath, 0, MEM_RELEASE);

    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    return 0;
}
