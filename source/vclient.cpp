#include <include/vclient.h>

namespace tcp_udp_client {

#if defined(_WIN32)
int VClient::_countClients = 0;
#endif

/**
 * @brief Construct a new VClient::VClient object
 * 
 */
VClient::VClient()
{
#if defined(_WIN32)
    if (_countClients <= 0) {
        WSADATA wsaData;
        ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    _countClients++;
#endif
}

/**
 * @brief Destroy the VClient::VClient object
 * 
 */
VClient::~VClient()
{
#if defined(_WIN32)
    _countClients--;
    if (_countClients <= 0) {
        ::WSACleanup();
    }
#endif
}

/**
 * @brief Close socket
 * 
 * @param sock - socket
 */
void VClient::close(int sock) const
{
#if defined(_WIN32)
    ::closesocket(sock);
#endif

#if defined(__unix__)
    ::close(sock);
#endif
}
}
