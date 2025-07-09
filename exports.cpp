#include "proxy.h"
#include <winsock2.h>
#include <vector>
#include <string>

// --- Global variables for our DLL ---
struct PeerInfo {
    unsigned long ip;
    unsigned short port;
};
std::vector<PeerInfo> g_peers;
bool g_isVip = false;
SOCKET g_udpSocket = INVALID_SOCKET;

// --- ฟังก์ชันสำหรับอ่าน Shared Memory ---
void ReadSharedMemory() {
    HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "THW-LAN-P2P-Key");
    if (hMapFile == NULL) return;

    LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pBuf == NULL) {
        CloseHandle(hMapFile);
        return;
    }

    char* pData = static_cast<char*>(pBuf);
    
    // อ่านสถานะ VIP (1 byte)
    unsigned char vipStatus = 0;
    memcpy(&vipStatus, pData, sizeof(vipStatus));
    g_isVip = (vipStatus == 1);
    pData += sizeof(vipStatus);

    // อ่านจำนวน Peer
    unsigned int peerCount = 0;
    memcpy(&peerCount, pData, sizeof(peerCount));
    pData += sizeof(peerCount);

    // อ่านข้อมูลของแต่ละ Peer
    g_peers.clear();
    for (unsigned int i = 0; i < peerCount; ++i) {
        PeerInfo peer;
        memcpy(&peer.ip, pData, sizeof(peer.ip));
        pData += sizeof(peer.ip);
        memcpy(&peer.port, pData, sizeof(peer.port));
        pData += sizeof(peer.port);
        g_peers.push_back(peer);
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}


// --- ฟังก์ชันดักจับ sendto ---
int __stdcall sendto_hook(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
{
    // อ่านข้อมูลล่าสุดจาก Shared Memory
    ReadSharedMemory();

    // สร้าง Socket ของเราเอง (ถ้ายังไม่มี)
    if (g_udpSocket == INVALID_SOCKET) {
        g_udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    // ตรวจสอบว่าเป็น Packet สร้างห้องหรือไม่
    bool isGameCreationPacket = (len > 4 && (unsigned char)buf[0] == 0xF7 && (unsigned char)buf[1] == 0x2F);

    // ถ้าเป็น Packet สร้างห้อง และผู้ใช้ไม่ใช่ VIP ให้ทิ้ง Packet นี้ไป
    if (isGameCreationPacket && !g_isVip) {
        // ไม่ทำอะไรเลย = ทิ้ง packet
        // แต่ยังคงต้องเรียก sendto ของจริงเพื่อให้เกมทำงานต่อได้ (แม้จะส่งไปหาแค่ broadcast address ก็ตาม)
    } else {
        // ถ้าเป็น VIP หรือเป็น Packet ประเภทอื่น ให้ส่งต่อไปยัง Peer ทุกคน
        for (const auto& peer : g_peers) {
            sockaddr_in peer_addr;
            peer_addr.sin_family = AF_INET;
            peer_addr.sin_addr.s_addr = peer.ip;
            peer_addr.sin_port = peer.port;
            
            sendto(g_udpSocket, buf, len, flags, (const sockaddr*)&peer_addr, sizeof(peer_addr));
        }
    }
    
    // เรียกใช้ฟังก์ชัน sendto ของจริงเสมอ เพื่อให้เกมทำงานได้ปกติ
    typedef int (__stdcall *p_sendto_t)(SOCKET, const char*, int, int, const struct sockaddr*, int);
    return ((p_sendto_t)p_sendto)(s, buf, len, flags, to, tolen);
}

// ... (โค้ดส่วนที่เหลือของ exports.cpp เหมือนเดิม) ...

#pragma comment(linker, "/export:accept=p_accept,@1")
#pragma comment(linker, "/export:bind=p_bind,@2")
#pragma comment(linker, "/export:closesocket=p_closesocket,@3")
#pragma comment(linker, "/export:connect=p_connect,@4")
#pragma comment(linker, "/export:getpeername=p_getpeername,@5")
#pragma comment(linker, "/export:getsockname=p_getsockname,@6")
#pragma comment(linker, "/export:getsockopt=p_getsockopt,@7")
#pragma comment(linker, "/export:htonl=p_htonl,@8")
#pragma comment(linker, "/export:htons=p_htons,@9")
#pragma comment(linker, "/export:ioctlsocket=p_ioctlsocket,@10")
#pragma comment(linker, "/export:inet_addr=p_inet_addr,@11")
#pragma comment(linker, "/export:inet_ntoa=p_inet_ntoa,@12")
#pragma comment(linker, "/export:listen=p_listen,@13")
#pragma comment(linker, "/export:ntohl=p_ntohl,@14")
#pragma comment(linker, "/export:ntohs=p_ntohs,@15")
#pragma comment(linker, "/export:recv=p_recv,@16")
#pragma comment(linker, "/export:recvfrom=p_recvfrom,@17")
#pragma comment(linker, "/export:select=p_select,@18")
#pragma comment(linker, "/export:send=p_send,@19")
#pragma comment(linker, "/export:shutdown=p_shutdown,@22")
#pragma comment(linker, "/export:socket=p_socket,@23")
#pragma comment(linker, "/export:GetHostByName=p_GetHostByName,@52")
#pragma comment(linker, "/export:WSAAsyncSelect=p_WSAAsyncSelect,@101")
#pragma comment(linker, "/export:WSAAsyncGetHostByName=p_WSAAsyncGetHostByName,@103")
#pragma comment(linker, "/export:WSACleanup=p_WSACleanup,@116")
#pragma comment(linker, "/export:WSAStartup=p_WSAStartup,@115")
#pragma comment(linker, "/export:WSAGetLastError=p_WSAGetLastError,@111")
#pragma comment(linker, "/export:WSACancelAsyncRequest=p_WSACancelAsyncRequest,@108")

#pragma comment(linker, "/export:sendto=sendto_hook,@20")

int __stdcall setsockopt_hook(SOCKET s, int level, int optname, const char* optval, int optlen)
{
    typedef int (__stdcall *p_setsockopt_t)(SOCKET, int, int, const char*, int);
    return ((p_setsockopt_t)p_setsockopt)(s, level, optname, optval, optlen);
}

#pragma comment(linker, "/export:setsockopt=setsockopt_hook,@21")
