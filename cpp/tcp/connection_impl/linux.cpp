#ifdef __linux
template <typename T>
using TcpResult = Result<T, std::string>;

namespace {

auto connection_fd(std::string hostname, std::uint16_t port) -> TcpResult<size_t> {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return TcpResult::Err("unable to initialize winsock!");
    }

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
}

auto TcpConnection::connect(std::string hostname, std::uint16_t port) -> TcpResult<TcpConnection> {
    return connection_fd(hostname, port)
        .map<TcpConnection>([](auto x) { return TcpConnection(x); });
}
#endif