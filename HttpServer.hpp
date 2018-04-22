#ifndef TP_HIGHLOAD_HTTP_SERVER_HPP
#define TP_HIGHLOAD_HTTP_SERVER_HPP

#include <glob.h>
#include <memory>
#include <string>

class EpollRequestHandler;
class HttpRequestHandler;
class ThreadPool;

class HttpServer {
    private:
        ssize_t _listening_socket_descriptor;
        bool _stop_flag;

        std::unique_ptr<EpollRequestHandler> _request_processor;
        std::unique_ptr<HttpRequestHandler> _request_handler;
        std::unique_ptr<ThreadPool> _workers_pool;

    public:
        HttpServer(const std::string& address, const std::uint16_t& port, const size_t& queue_size,
                   const std::string& document_root, const size_t& thread_count);
        ~HttpServer();

        void listen();
        void on_stop();
};

#endif
