#pragma once

#include <windows.h>

// ประกาศฟังก์ชันพอยเตอร์สำหรับเก็บที่อยู่ของฟังก์ชันใน wsock32.dll ของจริง
extern FARPROC p_accept;
extern FARPROC p_bind;
extern FARPROC p_closesocket;
extern FARPROC p_connect;
extern FARPROC p_getpeername;
extern FARPROC p_getsockname;
extern FARPROC p_getsockopt;
extern FARPROC p_htonl;
extern FARPROC p_htons;
extern FARPROC p_ioctlsocket;
extern FARPROC p_inet_addr;
extern FARPROC p_inet_ntoa;
extern FARPROC p_listen;
extern FARPROC p_ntohl;
extern FARPROC p_ntohs;
extern FARPROC p_recv;
extern FARPROC p_recvfrom;
extern FARPROC p_select;
extern FARPROC p_send;
extern FARPROC p_sendto;
extern FARPROC p_setsockopt;
extern FARPROC p_shutdown;
extern FARPROC p_socket;
extern FARPROC p_GetHostByName;
extern FARPROC p_WSAAsyncSelect;
extern FARPROC p_WSAAsyncGetHostByName;
extern FARPROC p_WSACleanup;
extern FARPROC p_WSAStartup;
extern FARPROC p_WSAGetLastError;
extern FARPROC p_WSACancelAsyncRequest;

// ฟังก์ชันสำหรับเริ่มต้นและสิ้นสุดการทำงานของ Proxy
void LoadOriginalDll();
void ExitInstance();
