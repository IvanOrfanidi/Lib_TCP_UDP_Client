#include "udp_client.hpp"

/*
 * @brief Constructor
 * @param addr - IP address
 * @param port - TCP port
 * @param size - max size for data
 */
UDP_Client::UDP_Client(const char* addr, unsigned short port)
    : _socket(INVAL_SOCKET)
{
    _local.sin_family = AF_INET;
    _local.sin_addr.s_addr = inet_addr(addr);
    _local.sin_port = htons(port);
}

/*
 * @brief Destructor
 */
UDP_Client::~UDP_Client()
{
    stop();
}

/*
 * @brief Start client
 */
UDP_Client::Result UDP_Client::start()
{
    // Create socket
    if (!isCreateSocket(TypeConnect::UDP, _socket)) {
        return Result::ERR_CREATE;
    }

    return Result::OK;
}

/*
 * @brief Restart client
 */
bool UDP_Client::isConnecting()
{
    stop();

    if (start() != UDP_Client::Result::OK) {
        return false;
    }

    return true;
}

/*
 * @brief Stop client
 */
void UDP_Client::stop() const
{
    closeSocket(_socket);
}

/*
 * @brief Send data
 * @retval true - success, false - fail
 */
bool UDP_Client::isSendData(const std::vector<char>& data)
{
    const int res = sendto(_socket, data.data(), data.size(), 0, (struct sockaddr*)&_local,
        sizeof(struct sockaddr_in));

    if (res < 0) {
        return false;
    }
    return true;
}

/*
 * @brief Receive data
 * @retval true - success, false - fail
 */
bool UDP_Client::isReceiveData(std::vector<char>& data)
{
    data.resize(data.capacity());
    int localLen = sizeof(struct sockaddr_in);

#if defined(_WIN32)
    const auto len = recvfrom(_socket, data.data(), data.capacity(), 0, (struct sockaddr*)&_local, &localLen);
#endif
#if defined(__unix__)
    const auto len = recvfrom(_socket, data.data(), data.capacity(), 0, (struct sockaddr*)&_local, (socklen_t*)&localLen);
#endif

    if (len < 0) {
        return false;
    }
    data.resize(len);

    return true;
}
