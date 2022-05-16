#include <conio.h>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <time.h> 


#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct autogiroVar {
    char motor1;
    char motor2;
    char servo1;
    char servo2;
    char servo3;
};

clock_t t, deltaT;


int main() {
    char pythonTime[11];

    DWORD flags = 0;

    _WSABUF pythonTime2;


    string pkt_string = "Give me the data"; // This is the message to be send
    const char* pkt = &pkt_string[0]; // Message is converted co const char * type to be send
    const char* host = "127.0.0.1"; // Local host


    sockaddr_in dest;
    sockaddr_in local;
    WSAData data;

    WSAStartup(MAKEWORD(2, 2), &data);

    local.sin_family = AF_INET;
    local.sin_port = htons(0);
    inet_pton(AF_INET, host, &local.sin_addr.s_addr);

    dest.sin_family = AF_INET;
    dest.sin_port = htons(50000);
    inet_pton(AF_INET, host, &dest.sin_addr.s_addr);

    int fromlen = sizeof(dest);


    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(s, (sockaddr*)&local, sizeof(local));

    t = clock(); // time restart
    int ms = 1000; 

    while (!_kbhit()) {
        deltaT = clock() - t; 

        if (deltaT > ms) { // Esto se ejecuta cada "ms" ms
            t = clock(); 

            sendto(s, pkt, pkt_string.size(), 0, (sockaddr*)&dest, sizeof(dest));

            int result, error, addrSize;
            char message[100];

            result = recvfrom(s, message, 100, 0, (SOCKADDR*)&dest, &fromlen);
            if (result == SOCKET_ERROR) {
                error = WSAGetLastError();
                std::cout << "Receive failed. Error: " << error << std::endl;

            }
            std::cout << "Received " << result << " byte(s) from " << inet_ntoa(dest.sin_addr) << ":" << ntohs(dest.sin_port) << std::endl;
            if (result > 0) {
                std::cout.write(message, result);
                std::cout << std::endl;
            }
        }

    }

    


    return 0;
}