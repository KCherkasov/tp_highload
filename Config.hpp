#ifndef WEBSERVER_CONFIG_KEYS_HPP
#define WEBSERVER_CONFIG_KEYS_HPP

#include <cstdint>
#include <string>

namespace config {
    // config file keys
    const std::string ADDRESS = "address";
    const std::string PORT = "port";

    const std::string CPU_COUNT = "cpu_limit";
    const std::string THREAD_COUNT = "thread_limit";

    const std::string ROOT_FOLDER = "document_root";

    const std::string QUEUE_SIZE = "queue_size";

    const std::string CONFIG_FILENAME = "/httpd.conf";

    // error messages 
    const std::string UNKNOWN_KEY_ERROR = "Config file parsing error: unknown key: ";

    // default configuration values
    const std::string DEFAULT_CONFIG_FILE_PATH = "/etc";

    const std::string DEFAULT_ADDRESS = "0.0.0.0";
    const std::string DEFAULT_ROOT_FOLDER = "/var/www/html";

    const std::uint_16t DEFAULT_CPU_COUNT = 4;
    const std::uint_16t DEFAULT_THREAD_COUNT = 16;
    const std::uint_16t DEFAULT_PORT = 80;

    const size_t DEFAULT_QUEUE_SIZE = 64;

    const size_t MINIMAL_THREADS_COUNT = 3;

    const std::string DEFAULT_PROTOCOL = "HTTP/1.1";

    const size_t EPOLL_EVENTS_POOL_LIMIT = 300;
    const size_t EPOLL_CONNECTION_TIMEOUT = 50;
}

#endif