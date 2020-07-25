#include "wcap.h"


namespace wcap
{

int GetProcessID(const TCHAR* exeFileName) {
    HANDLE snapHandle;
    PROCESSENTRY32 processEntry = {0};

    if ((snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE) {
        return 0;
    }

    processEntry.dwSize = sizeof(PROCESSENTRY32);
    Process32First(snapHandle, &processEntry);
    do {
        if (_tcscmp(processEntry.szExeFile, exeFileName) == 0 ) {
            return processEntry.th32ProcessID;
        }
    } while (Process32Next(snapHandle, &processEntry));

    if (snapHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(snapHandle);
    }

    return 0;
}

bool InjectDll(int pid, const TCHAR* dll_filename) {

    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);

    int dll_filename_length = ::wcslen(dll_filename) + 2;
    void* dll_filename_remote = ::VirtualAllocEx(hProcess, 0, dll_filename_length, MEM_COMMIT, PAGE_READWRITE );

    if (!dll_filename_remote)
        return false;

    if(!::WriteProcessMemory(hProcess, dll_filename_remote, dll_filename, dll_filename_length, NULL))
        return false;

    HMODULE hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));
    PTHREAD_START_ROUTINE load_library = (PTHREAD_START_ROUTINE)::GetProcAddress(hKernel32, "LoadLibrary");

    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0,
                    load_library, dll_filename_remote, 0, NULL);

    if (hThread == NULL )
    {
        ::VirtualFreeEx(hProcess, dll_filename_remote, dll_filename_length, MEM_RELEASE);
        return false;
    }

    //if (hThread) CloseHandle(hThread);
    //if (hProcess) CloseHandle(hProcess);

    return true;
}

}//namespace wcap
