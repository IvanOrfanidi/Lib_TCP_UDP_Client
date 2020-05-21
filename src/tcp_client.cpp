#include "tcp_client.hpp"

/*
 * @brief Constructor
 * @param addr - IP address
 * @param port - TCP port
 * @param size - max size for data
 */
TCP_Client::TCP_Client(const char* addr, unsigned short port)
    : _socket(INVAL_SOCKET)
{
    _local.sin_family = AF_INET;
    _local.sin_addr.s_addr = ::inet_addr(addr);
    _local.sin_port = ::htons(port);
}

/*
 * @brief Destructor
 */
TCP_Client::~TCP_Client()
{
    stop();
}

/*
 * @brief Start client
 */
TCP_Client::Result TCP_Client::start()
{
    // Create socket
    if (!isCreateSocket(TypeConnect::TCP, _socket)) {
        return Result::ERR_CREATE;
    }

    return Result::OK;
}

/*
 * @brief Restart client
 */
bool TCP_Client::isConnecting()
{
    // Starting server
    if (start() != TCP_Client::Result::OK) {
        return false;
    }
    return isConnect();
}

/*
 * @brief Stop client
 */
void TCP_Client::stop() const
{
    ::shutdown(_socket, 1);
    closeSocket(_socket);
}

/*
 * @brief Connect to server
 * @retval true - success, false - fail
 */
bool TCP_Client::isConnect()
{
    const int res = ::connect(_socket, (struct sockaddr*)&_local, sizeof(_local));
    if (res != 0) {
        return false;
    }
    return true;
}

/*
 * @brief Send data
 * @retval true - success, false - fail
 */
bool TCP_Client::isSendData(const std::vector<char>& data)
{
    // First comes the total length of data in formate host to network
    const uint32_t size = ::htonl(data.size());
    int res = ::send(_socket, reinterpret_cast<const char*>(&size), sizeof(size), 0);
    if (res <= 0) {
        return false;
    }

    // Second, are data
    res = ::send(_socket, data.data(), data.size(), 0);
    if (res <= 0) {
        return false;
    }
    return true;
}

/*
 * @brief Receive data
 * @retval true - success, false - fail
 */
bool TCP_Client::isReceiveData(std::vector<char>& data)
{
    bool isSize = false;
    uint32_t size = 0;
    while (!isSize) {
        const int len = ::recv(_socket, reinterpret_cast<char*>(&size), sizeof(size), 0);
        if (len < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        } else if (0 == len) {
            break;
        } else {
            // Converting data in format network to host
            size = ::ntohl(size);
            data.resize(size);
            isSize = true;
        }
    }

    if (isSize) {
        size_t index = 0;
        while (true) {
            const int len = ::recv(_socket, data.data(), (size - index), 0);
            if (len < 0) {
                if (errno == EINTR) {
                    continue;
                }
                break;
            } else if (0 == len) {
                break;
            } else {
                index += (size - (size - len));
                if (index >= size) {
                    return true;
                }
            }
        }
    }

    return false;
}
