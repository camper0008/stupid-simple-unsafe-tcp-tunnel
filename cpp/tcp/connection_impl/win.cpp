#include <memory>
#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include "../connection.hpp"
#include <windows.h>
#include "result.hpp"
#include <format>
#include <iphlpapi.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

template <typename T>
using TcpResult = Result<T, std::string>;

namespace {
auto connection_fd(std::string hostname, std::uint16_t port) -> TcpResult<SOCKET> {
    WSADATA wsaData;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (startupResult != NO_ERROR) {
        return TcpResult<size_t>::Err(std::format("WSAStartup function failed with error: '{}'", startupResult));
    }

    SOCKET connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connection == INVALID_SOCKET) {
        return TcpResult<size_t>::Err(std::format("socket function failed with error: '{}'", WSAGetLastError()));
    }
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(port);
    if (inet_pton(AF_INET, hostname.c_str(), &clientService.sin_addr.s_addr) != 1) {
        return TcpResult<size_t>::Err(std::format("invalid hostname '{}'", hostname));
    }

    // Connect to server.
    startupResult = connect(connection, (SOCKADDR*)&clientService, sizeof(clientService));
    if (startupResult == SOCKET_ERROR) {
        auto startupError = WSAGetLastError();
        auto close = closesocket(connection);
        if (close == SOCKET_ERROR) {
            return TcpResult<size_t>::Err(std::format("connect & close failed with error '{}' & '{}'", startupError, WSAGetLastError()));
        }
        return TcpResult<size_t>::Err(std::format("connect failed with error '{}'", startupError));
    }

    wprintf(L"Connected to server.\n");

    return TcpResult<SOCKET>::Ok(connection);
}

class WinTcpConnection : public TcpConnection {
public:
    static auto connect(std::string hostname, std::uint16_t port) -> Result<std::unique_ptr<TcpConnection>, std::string> {
        auto socket = connection_fd(hostname, port);
        return socket.map<std::unique_ptr<WinTcpConnection>>([](auto socket) {
            return std::make_unique(WinTcpConnection(socket));
        });
    };
auto

    private : SOCKET socket;
    WinTcpConnection(SOCKET socket)
        : socket(socket) { }
};

}

auto connect(std::string hostname, std::uint16_t port) -> TcpResult<std::unique_ptr<TcpConnection>> {
    return WinTcpConnection::connect(hostname, port);
}

#endif