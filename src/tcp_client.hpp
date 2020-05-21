#pragma once

/* Standart lib*/
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

/* Net socket lib */
#if defined(_WIN32)
#include <winsock2.h>
#endif

#if defined(__unix__)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

/* User */
#include "vclient.hpp"

class TCP_Client : public VClient {
public:
    // Constructor
    TCP_Client(const char* addr, unsigned short port);

    // Destructor
    ~TCP_Client();

    // Start client
    virtual Result start() override;

    // Stop client
    virtual void stop() const override;

    // Send data
    virtual bool isSendData(const std::vector<char>& data) override;

    // Receive data
    virtual bool isReceiveData(std::vector<char>& data) override;

    // Restart client
    virtual bool isConnecting() override;

    // Connect to server
    bool isConnect();

private:
    TCP_Client() = default;

    int _socket; //< socket number
    struct sockaddr_in _local; //< socket info
};
