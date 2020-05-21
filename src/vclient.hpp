#pragma once

/* Standart lib*/
#include <cstdio>
#include <iostream>
#include <vector>

/* Net socket lib */
#if defined(_WIN32)
#include <winsock2.h>
#endif

#if defined(__unix__)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

class VClient {
public:
    // Type Connect(TCP or UDP)
    enum class TypeConnect {
        TCP,
        UDP
    };

    // Result start
    enum class Result {
        OK,
        ERR_CREATE,
        ERR_CONNECT
    };

    // Invalid socket
    static constexpr int INVAL_SOCKET = -1;

    // Constructor
    VClient();

    // Destructor
    virtual ~VClient();

    // Start client
    virtual Result start() = 0;

    // Stop client
    virtual void stop() const = 0;

    // Send data
    virtual bool isSendData(const std::vector<char>& data) = 0;

    // Receive data
    virtual bool isReceiveData(std::vector<char>& data) = 0;

    // Restart client
    virtual bool isConnecting() = 0;

protected:
    // Close socket
    void closeSocket(int sock) const;

    // Create socket
    bool isCreateSocket(TypeConnect type, int& sock);

private:
    VClient(const VClient&) = delete;
    VClient& operator=(const VClient&) = delete;

#if defined(_WIN32)
    static int _countClients; //< counter of children
#endif
};
