#pragma once

#include <include/vclient.h>

namespace tcp_udp_client {

class UDP_Client : public VClient {
public:
    UDP_Client();
    virtual ~UDP_Client();

    /**
     * @brief Construct a new udp client::udp client object
     * @param addr - IP address
     * @param port - UDP port
     */
    explicit UDP_Client(const char* addr, uint16_t port);

    /**
     * @brief Set IP address
     * @param addr - IP address
     */
    virtual void setAddress(const char* addr) override;

    /**
     * @brief Set UDP port
     * @param port - UDP port
     */
    virtual void setPort(uint16_t port) override;

    /**
     * @brief Create cocket
     * @param sock - cocket number, output param
     */
    virtual int create(int sock) const override;

    /** Start client */
    virtual void start() override;

    /** Stop client, socket close */
    virtual void stop() const override;

    /**
     * @brief Send data
     * @param data - data vector
     */
    virtual void send(const std::vector<char>& data) override;

    /**
     * @brief Receive data
     * @param data - data vector, output param
     * @param length - max data length
     */
    virtual void receive(std::vector<char>& data, const size_t length) override;

    /** Client connecting */
    virtual void connecting() override;

    /**
     * @brief Get socket
     * @return int socket
     */
    virtual int getSocket() const override;

private:
    int _socket; //< socket number
    struct sockaddr_in _server; //< socket info
};

}