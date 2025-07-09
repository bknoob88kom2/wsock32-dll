#include "proxy.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // เมื่อ DLL ถูกโหลดโดยเกม ให้เรียกฟังก์ชันเพื่อโหลด DLL ของจริง
        LoadOriginalDll();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // เมื่อเกมปิด และ DLL ถูกถอดออก ให้ปล่อย Library ของจริง
        ExitInstance();
        break;
    }
    return TRUE;
}
