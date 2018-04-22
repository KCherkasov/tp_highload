#ifndef WEBSERVER_EPOLL_REQUEST_HANDLER_HPP
#define WEBSERVER_EPOLL_REQUEST_HANDLER_HPP

#include <atomic>
#include <map>

class HttpRequestHandler;
class HttpSession;
class ThreadPool;

class EpollRequestHandler {
    private:
        ssize_t _epoll_descriptor;
        size_t _epoll_events_limit;
        size_t _connection_timeout;

        std::atomic_bool _stop_flag;

        HttpRequestHandler& _request_handler;
        ThreadPool& _workers;

        std::map<ssize_t, HttpSession&> _sessions_pool;

        std::function<void()> make_task(HttpSession& session, const epoll_event& event);

    public:
        EpollRequestHandler(size_t epoll_events_limit, size_t connection_timeout,
                            HttpRequestHandler& request_handler, ThreadPool& workers);
        ~EpollRequestHandler();

        void on_start();
        void add_client(ssize_t client_descriptor);
        void on_stop();
};

#endif
