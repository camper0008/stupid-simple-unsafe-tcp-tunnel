#pragma once

#include <cstdint>
#include <string>

class TcpConnection {
public:
    auto static connect(std::string hostname, std::uint16_t port) -> TcpConnection;
    auto write(std::string message) -> size_t;
    auto read(std::string message) -> size_t;
    auto closed() -> bool;

private:
    TcpConnection() { }
};

class TcpListener {
public:
    auto accept() -> TcpConnection;
};