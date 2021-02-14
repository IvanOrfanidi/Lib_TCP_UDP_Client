#pragma once

#pragma once

#include <string>
#include <vector>

#if defined(_WIN32)
#define _WINSOCK_DEPRECATED_NO_WARNINGS // disable warnings
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#if defined(__unix__)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace tcp_udp_client {

namespace error_message {
    const std::string CREATE = "client socket not create";
    const std::string CONNECT = "client socket connect failed";
    const std::string SEND = "client socket sending data failed";
    const std::string RECEIVE = "client socket receve data failed";
}

class VClient {
public:
    // Invalid socket
    static constexpr int INVAL_SOCKET = -1;

    VClient();
    virtual ~VClient();

    /**
     * @brief Set IP address
     * @param addr - IP address
     */
    virtual void setAddress(const char* addr) = 0;

    /**
     * @brief Get the IP Address
     * @return std::string IP address
     */
    virtual std::string getAddress() const = 0;

    /**
     * @brief Set port
     * @param port - port
     */
    virtual void setPort(uint16_t port) = 0;

    /**
     * @brief Get the Port
     * @return uint16_t port
     */
    virtual uint16_t getPort() const = 0;

    /**
     * @brief Create socket
     * @param sock - socket number, output param
     * @return int socket
     */
    virtual int createSocket(int sock) const = 0;

    /** Start client */
    virtual void start() = 0;

    /** Stop client, socket close */
    virtual void stop() const = 0;

    /** Client connecting */
    virtual void connecting() = 0;

    /**
     * @brief Send data
     * @param data - data vector
     */
    virtual void sendData(const std::vector<char>& data) = 0;

    /**
     * @brief Receive data
     * @param data - data vector, output param
     * @param length - max data length
     */
    virtual void receiveData(std::vector<char>& data, const size_t length) = 0;

    /**
     * @brief Get socket
     * @return int socket
     */
    virtual int getSocket() const = 0;

protected:
    /**
     * @brief Close socket
     * @param sock - socket
     */
    void closeSocket(int sock) const;

private:
    VClient(const VClient&) = delete;
    VClient& operator=(const VClient&) = delete;

#if defined(_WIN32)
    static int _countClients; //< counter of children
#endif
};

}
