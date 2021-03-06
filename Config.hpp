#ifndef WEBSERVER_CONFIG_KEYS_HPP
#define WEBSERVER_CONFIG_KEYS_HPP

#include <cstdint>

namespace config {
    // config file keys
    constexpr const char* ADDRESS = "address";
    constexpr const char* PORT = "port";

    constexpr const char* CPU_COUNT = "cpu_limit";
    constexpr const char* THREAD_COUNT = "thread_limit";

    constexpr const char* ROOT_FOLDER = "document_root";

    constexpr const char* QUEUE_SIZE = "queue_size";

    constexpr const char* CONFIG_FILENAME = "/httpd.conf";

    // error messages 
    const char* UNKNOWN_KEY_ERROR = "Config file parsing error: unknown key: ";

    // default configuration values
    const char* DEFAULT_CONFIG_FILE_PATH = "/etc";

    const char* DEFAULT_ADDRESS = "0.0.0.0";
    const char* DEFAULT_ROOT_FOLDER = "/var/www/html";

    const std::uint16_t DEFAULT_CPU_COUNT = 4;
    const std::uint16_t DEFAULT_THREAD_COUNT = 16;
    const std::uint16_t DEFAULT_PORT = 80;

    const size_t DEFAULT_QUEUE_SIZE = 64;

    const size_t MINIMAL_THREADS_COUNT = 3;

    const char* DEFAULT_PROTOCOL = "HTTP/1.1";

    constexpr size_t EPOLL_EVENTS_POOL_LIMIT = 300;
    constexpr size_t EPOLL_CONNECTION_TIMEOUT = 50;
}

#endif