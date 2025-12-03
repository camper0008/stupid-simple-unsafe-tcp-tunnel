#pragma once
#include "result.hpp"
#include <cstdint>
#include <memory>
#include <string>

class TcpConnection {
public:
    auto virtual read(std::string message) -> Result<size_t, std::string> = 0;
    auto virtual write(std::string message) -> Result<size_t, std::string> = 0;
    auto virtual closed() -> bool = 0;

protected:
    TcpConnection();
};

auto connect(std::string hostname, std::uint16_t port) -> Result<std::unique_ptr<TcpConnection>, std::string>;
