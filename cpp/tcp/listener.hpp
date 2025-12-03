#pragma once
#include "result.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include "connection.hpp"

class TcpListener {
public:
    auto virtual accept() -> Result<TcpConnection, std::string> = 0;
};

auto listen(std::string hostname, std::uint16_t port) -> Result<std::unique_ptr<TcpListener>, std::string>;
