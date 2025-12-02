#include "tcp.hpp"
#include "result.hpp"
#include <cstddef>
#include <cstdint>

template <typename T>
using TcpResult = Result<T, std::string>;

namespace {
#include <arpa/inet.h>
#include <netinet/ip.h>

auto connection_fd_linux(std::string hostname, std::uint16_t port) -> TcpResult<size_t> {

    int socket_fd = 0;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return TcpResult<size_t>::Err("could not get socket");
    }

    struct sockaddr_in address = {
        AF_INET,
        htons(port),
        inet_addr(hostname.c_str()),
        {}
    };

    if (connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        return TcpResult<size_t>::Err({ "could not connect, is the server running?" });
    }

    return TcpResult<size_t>::Ok(socket_fd);
}

auto connection_fd(std::string hostname, std::uint16_t port) -> TcpResult<size_t> {
    return connection_fd_linux(hostname, port);
}
}

TcpConnection::TcpConnection(std::size_t fd)
    : fd(fd) { }

auto TcpConnection::connect(std::string hostname, std::uint16_t port) -> TcpResult<TcpConnection> {
    return connection_fd(hostname, port)
        .map<TcpConnection>([](auto x) { return TcpConnection(x); });
}
