#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <zconf.h>

#include "Config.hpp"
#include "EpollRequestHandler.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpServer.hpp"
#include "ThreadPool.hpp"

const std::string SOCKET_INIT_ERROR = "socket create error: ";
const std::string NONBLOCK_ERROR = "nonblocking error: ";
const std::string BINDING_ERROR = "bind error: ";
const std::string LISTENING_ERROR = "listening error: ";

HttpServer::HttpServer(const std::string& address, const std::uint16_t& port, const size_t& queue_size,
                       const std::string& document_root, const size_t& thread_count): _stop_flag(false) {
    _thread_pool = std::move(std::make_unique<ThreadPool>(new ThreadPool(thread_count)));
    _request_handler = std::move(std::make_unique<HttpRequestHandler>(new HttpRequestHandler(document_root)));
    _request_processor = std::move(std::make_unique<EpollRequestHandler>(
        new EpollRequestHandler(config::EPOLL_EVENTS_POOL_LIMIT, config::EPOLL_CONNECTION_TIMEOUT,
        *(_request_handler.get()), *(_workers_pool.get())));

    _listening_socket_descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_listening_socket_descriptor < 0) {
        throw std::runtime_error(SOCKET_INIT_ERROR + std::string(strerror(errno)));
    }

    ssize_t flags = fcntl(()_listening_socket_descriptor, F_GETFL, 0);
    if (fcntl(_listening_socket_descriptor, F_SETFL, flags | O_NONBLOCK) < 0) {
        throw std::runtime_error(NONBLOCK_ERROR + std::string(strerror(errno)));
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, address.data(), &(server_address.sin_addr));

    if (bind(_listening_socket_descriptor, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error(BINDING_ERROR + std::string(strerror(errno)));
    }
    
    if (listen(_listening_socket_descriptor, queue_size) < 0) {
        throw std::runtime_error(LISTENING_ERROR + std::string(strerror(errno)));
    }

    _workers_pool->offer_task(std::bind(&EpollRequestHandler::on_start, &*_request_processor));
}

HttpServer::~HttpServer() {}

void HttpServer::listen() {
    sockaddr_in client_address;
    socklen_t client_size;

    while (!_stop_flag) {
        ssize_t connection_descriptor = accept (_listening_socket_descriptor,
            (struct sockaddr*)&client_address, &client_size);
        if (connection_descriptor >= 0) {
            ssize_t flags = fcntl((int)connection_descriptor, F_GETFL, 0);
            if (fcntl((int)connection_descriptor, F_SETFL, flags | O_NONBLOCK) != -1) {
                _request_processor->add_client(connection_descriptor);
            }
        }
    }
}

void HttpServer::on_stop() {
    _stop_flag = true;
}
