#include "tcp.hpp"

int connection_fd(void) {
    int socket_fd = 0;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("could not get socket");
    }

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(8080),
    };

    if (connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        error("could not connect, is the server running?");
    }

    return socket_fd;
}

auto TcpConnection::connect(std::string hostname, std::uint16_t port) -> TcpConnection {

    return TcpConnection {
    }
}
