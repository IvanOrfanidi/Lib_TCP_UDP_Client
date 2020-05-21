#include "vclient.hpp"

#if defined(_WIN32)
int VClient::_countClients = 0;
#endif

/*
 * @brief Constructor
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

/*
 * @brief Destructor
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

/*
 * @brief Close socket
 */
void VClient::closeSocket(int sock) const
{
#if defined(_WIN32)
    ::closesocket(sock);
#endif

#if defined(__unix__)
    ::close(sock);
#endif
}

/*
 * @brief Create socket
 * @retval true - socket is created, false - socket is not created
 */
bool VClient::isCreateSocket(const TypeConnect type, int& sock)
{
    // Create socket TCP or UDP
    if (type == TypeConnect::TCP) {
        sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    } else {
        sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    if (sock < 0) {
        return false;
    }
    return true;
}
