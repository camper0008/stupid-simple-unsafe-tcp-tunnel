#pragma once

#include <string>
#include <vector>

struct Connection {
    std::string hostname;
    std::uint16_t port;
    std::string secret;
};

struct Config {
    std::vector<Connection> connections;
};

static Config global_conf { { { "localhost", 4200, "abcd" } } };