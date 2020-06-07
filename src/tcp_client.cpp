#include <cassert>
#include <stdexcept>

#include "../include/tcp_client.h"

namespace tcp_udp_client {

/**
 * @brief Construct a new tcp client::tcp client object
 * 
 */
TCP_Client::TCP_Client()
    : _socket(INVAL_SOCKET)
{
    _local.sin_family = AF_INET;
}

/**
 * @brief Construct a new tcp client::tcp client object
 * 
 * @param addr - IP address
 * @param port - TCP port
 */
TCP_Client::TCP_Client(const char* addr, uint16_t port)
    : TCP_Client()
{
    assert(addr != nullptr);
    assert(port != 0);
    _local.sin_addr.s_addr = ::inet_addr(addr);
    _local.sin_port = ::htons(port);
}

/**
 * @brief Destroy the tcp client::tcp client object
 * 
 */
TCP_Client::~TCP_Client()
{
    stop();
}

/**
 * @brief Set IP address
 * 
 * @param addr - IP address
 */
void TCP_Client::setAddress(const char* addr)
{
    assert(addr != nullptr);
    _local.sin_addr.s_addr = ::inet_addr(addr);
}

/**
 * @brief Set TCP port
 * 
 * @param port - TCP port
 */
void TCP_Client::setPort(uint16_t port)
{
    assert(port != 0);
    _local.sin_port = ::htons(port);
}

/**
 * @brief Create cocket
 * 
 * @param sock - cocket number, output param
 */
void TCP_Client::create(int& sock)
{
    sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        throw std::runtime_error(error_message::CREATE);
    }
}

/**
 * @brief Start client
 * 
 */
void TCP_Client::start()
{
    try {
        create(_socket);
    } catch (const std::exception& e) {
        throw e;
    }
}

/**
 * @brief Stop client, socket close
 * 
 */
void TCP_Client::stop() const
{
    ::shutdown(_socket, 1);
    close(_socket);
}

/**
 * @brief Client connecting
 * 
 */
void TCP_Client::connecting()
{
    try {
        start();
    } catch (const std::exception& e) {
        throw e;
    }

    try {
        connect();
    } catch (const std::exception& e) {
        throw e;
    }
}

/**
 * @brief Get socket
 * 
 * @return int socket
 */
int TCP_Client::getSocket() const
{
    return _socket;
}

/**
 * @brief Connect socket
 * 
 */
void TCP_Client::connect()
{
    assert(_socket != INVAL_SOCKET);
    const auto res = ::connect(_socket, (struct sockaddr*)&_local, sizeof(_local));
    if (res != 0) {
        throw std::runtime_error(error_message::CONNECT);
    }
}

/**
 * @brief Send data
 * 
 * @param data - data vector
 */
void TCP_Client::send(const std::vector<char>& data)
{
    const auto res = ::send(_socket, data.data(), data.size(), 0);
    if (res <= 0) {
        throw std::runtime_error(error_message::SEND);
    }
}

/**
 * @brief Receive data
 * 
 * @param data - data vector, output param
 * @param length - max data length
 */
void TCP_Client::receive(std::vector<char>& data, const size_t length)
{
    while (true) {
        const auto len = ::recv(_socket, data.data(), length, 0);
        if (len < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        } else if (0 == len) {
            break;
        } else {
            data.resize(len);
            return;
        }
    }

    throw std::runtime_error(error_message::RECEIVE);
}
}
