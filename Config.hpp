#ifndef WEBSERVER_CONFIG_KEYS_HPP
#define WEBSERVER_CONFIG_KEYS_HPP

#include <cstdint>
#include <string>

namespace config {
    // config file keys
    constexpr std::string ADDRESS = "address";
    constexpr std::string PORT = "port";

    constexpr std::string CPU_COUNT = "cpu_limit";
    constexpr std::string THREAD_COUNT = "thread_limit";

    constexpr std::string ROOT_FOLDER = "document_root";

    constexpr std::string QUEUE_SIZE = "queue_size";

    constexpr std::string CONFIG_FILENAME = "/httpd.conf";

    // error messages 
    constexpr std::string UNKNOWN_KEY_ERROR = "Config file parsing error: unknown key: ";

    // default configuration values
    constexpr std::string DEFAULT_CONFIG_FILE_PATH = "/etc";

    constexpr std::string DEFAULT_ADDRESS = "0.0.0.0";
    constexpr std::string DEFAULT_ROOT_FOLDER = "/var/www/html";

    constexpr std::uint16_t DEFAULT_CPU_COUNT = 4;
    constexpr std::uint16_t DEFAULT_THREAD_COUNT = 16;
    constexpr std::uint16_t DEFAULT_PORT = 80;

    constexpr size_t DEFAULT_QUEUE_SIZE = 64;

    constexpr size_t MINIMAL_THREADS_COUNT = 3;

    constexpr std::string DEFAULT_PROTOCOL = "HTTP/1.1";

    constexpr size_t EPOLL_EVENTS_POOL_LIMIT = 300;
    constexpr size_t EPOLL_CONNECTION_TIMEOUT = 50;
}

#endif