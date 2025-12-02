#pragma once

#include "result.hpp"
#include <cstddef>
#include <cstdint>
#include <string>

class TcpConnection {
public:
    auto static connect(std::string hostname, std::uint16_t port) -> Result<TcpConnection, std::string>;
    auto read(std::string message) -> Result<size_t, std::string>;
    auto write(std::string message) -> Result<size_t, std::string>;
    auto closed() -> bool;

private:
    size_t fd;
    TcpConnection(std::size_t fd);
};

class TcpListener {
public:
    auto accept() -> Result<TcpConnection, std::string>;
};
