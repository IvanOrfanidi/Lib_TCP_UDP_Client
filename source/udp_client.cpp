#include <cassert>
#include <stdexcept>

#include <include/udp_client.h>

namespace tcp_udp_client {

/**
 * @brief Construct a new udp client::udp client object
 */
UDP_Client::UDP_Client()
    : _socket(INVAL_SOCKET)
{
    _server.sin_family = AF_INET;
}

/**
 * @brief Construct a new udp client::udp client object
 * @param addr - IP address
 * @param port - UDP port
 */
UDP_Client::UDP_Client(const char* addr, uint16_t port)
    : UDP_Client()
{
    assert(addr != nullptr);
    assert(port != 0);
    _server.sin_addr.s_addr = ::inet_addr(addr);
    _server.sin_port = ::htons(port);
}

/**
 * @brief Destroy the udp client::udp client object
 */
UDP_Client::~UDP_Client()
{
    stop();
}

/**
 * @brief Set IP address
 * @param addr - IP address
 */
void UDP_Client::setAddress(const char* addr)
{
    assert(addr != nullptr);
    _server.sin_addr.s_addr = ::inet_addr(addr);
}

/**
 * @brief Set UDP port
 * @param port - UDP port
 */
void UDP_Client::setPort(uint16_t port)
{
    assert(port != 0);
    _server.sin_port = ::htons(port);
}

/**
 * @brief Create cocket
 * @param sock - cocket number, output param
 */
int UDP_Client::create(int sock) const
{
    sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        throw std::runtime_error(error_message::CREATE);
    }
    return sock;
}

/**
 * @brief Start client
 */
void UDP_Client::start()
{
    try {
        _socket = create(_socket);
    } catch (const std::exception& e) {
        throw e;
    }
}

/**
 * @brief Stop client, socket close
 */
void UDP_Client::stop() const
{
    ::shutdown(_socket, 1);
    close(_socket);
}

/**
 * @brief Send data
 * @param data - data vector
 */
void UDP_Client::send(const std::vector<char>& data)
{
    const auto res = ::sendto(_socket, data.data(), data.size(), 0, (const struct sockaddr*)&_server, sizeof(struct sockaddr_in));
    if (res <= 0) {
        throw std::runtime_error(error_message::SEND);
    }
}

/**
 * @brief Receive data
 * @param data - data vector, output param
 * @param length - max data length
 */
void UDP_Client::receive(std::vector<char>& data, const size_t length)
{
    int localLen = sizeof(struct sockaddr_in);
    const auto len = ::recvfrom(_socket, data.data(), length, MSG_WAITALL, (struct sockaddr*)&_server, &localLen);
    if (len < 0) {
        throw std::runtime_error(error_message::RECEIVE);
    }
    data.resize(len);
}

/**
 * @brief Client connecting
 */
void UDP_Client::connecting()
{
    try {
        start();
    } catch (const std::exception& e) {
        throw e;
    }
}

/**
 * @brief Get socket
 * @return int socket
 */
int UDP_Client::getSocket() const
{
    return _socket;
}

}
