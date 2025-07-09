#include "proxy.h"

// ตัวแปรสำหรับเก็บ instance ของ DLL ของจริง
static HINSTANCE hL = 0;

// กำหนดค่าเริ่มต้นให้ฟังก์ชันพอยเตอร์ทั้งหมดเป็น NULL
FARPROC p_accept = NULL;
FARPROC p_bind = NULL;
FARPROC p_closesocket = NULL;
FARPROC p_connect = NULL;
FARPROC p_getpeername = NULL;
FARPROC p_getsockname = NULL;
FARPROC p_getsockopt = NULL;
FARPROC p_htonl = NULL;
FARPROC p_htons = NULL;
FARPROC p_ioctlsocket = NULL;
FARPROC p_inet_addr = NULL;
FARPROC p_inet_ntoa = NULL;
FARPROC p_listen = NULL;
FARPROC p_ntohl = NULL;
FARPROC p_ntohs = NULL;
FARPROC p_recv = NULL;
FARPROC p_recvfrom = NULL;
FARPROC p_select = NULL;
FARPROC p_send = NULL;
FARPROC p_sendto = NULL;
FARPROC p_setsockopt = NULL;
FARPROC p_shutdown = NULL;
FARPROC p_socket = NULL;
FARPROC p_GetHostByName = NULL;
FARPROC p_WSAAsyncSelect = NULL;
FARPROC p_WSAAsyncGetHostByName = NULL;
FARPROC p_WSACleanup = NULL;
FARPROC p_WSAStartup = NULL;
FARPROC p_WSAGetLastError = NULL;
FARPROC p_WSACancelAsyncRequest = NULL;

void LoadOriginalDll(void)
{
    char cSysDir[255];
    // หา Path ของโฟลเดอร์ System32
    GetSystemDirectoryA(cSysDir, 255);

    // สร้าง Path เต็มไปยัง wsock32.dll ของจริง
    strcat_s(cSysDir, "\\wsock32.dll");

    // โหลด DLL ของจริงขึ้นมา
    hL = LoadLibraryA(cSysDir);
    if (!hL) return;

    // ดึงที่อยู่ของฟังก์ชันจริงๆ ทั้งหมดมาเก็บไว้ในพอยเตอร์
    p_accept = GetProcAddress(hL, "accept");
    p_bind = GetProcAddress(hL, "bind");
    p_closesocket = GetProcAddress(hL, "closesocket");
    p_connect = GetProcAddress(hL, "connect");
    p_getpeername = GetProcAddress(hL, "getpeername");
    p_getsockname = GetProcAddress(hL, "getsockname");
    p_getsockopt = GetProcAddress(hL, "getsockopt");
    p_htonl = GetProcAddress(hL, "htonl");
    p_htons = GetProcAddress(hL, "htons");
    p_ioctlsocket = GetProcAddress(hL, "ioctlsocket");
    p_inet_addr = GetProcAddress(hL, "inet_addr");
    p_inet_ntoa = GetProcAddress(hL, "inet_ntoa");
    p_listen = GetProcAddress(hL, "listen");
    p_ntohl = GetProcAddress(hL, "ntohl");
    p_ntohs = GetProcAddress(hL, "ntohs");
    p_recv = GetProcAddress(hL, "recv");
    p_recvfrom = GetProcAddress(hL, "recvfrom");
    p_select = GetProcAddress(hL, "select");
    p_send = GetProcAddress(hL, "send");
    p_sendto = GetProcAddress(hL, "sendto");
    p_setsockopt = GetProcAddress(hL, "setsockopt");
    p_shutdown = GetProcAddress(hL, "shutdown");
    p_socket = GetProcAddress(hL, "socket");
    p_GetHostByName = GetProcAddress(hL, "GetHostByName");
    p_WSAAsyncSelect = GetProcAddress(hL, "WSAAsyncSelect");
    p_WSAAsyncGetHostByName = GetProcAddress(hL, "WSAAsyncGetHostByName");
    p_WSACleanup = GetProcAddress(hL, "WSACleanup");
    p_WSAStartup = GetProcAddress(hL, "WSAStartup");
    p_WSAGetLastError = GetProcAddress(hL, "WSAGetLastError");
    p_WSACancelAsyncRequest = GetProcAddress(hL, "WSACancelAsyncRequest");
}

void ExitInstance(void)
{
    if (hL)
    {
        FreeLibrary(hL);
        hL = 0;
    }
}
