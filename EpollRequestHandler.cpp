#include <memory>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

#include "EpollRequestHandler.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpSession.hpp"
#include "ThreadPool.hpp"

const std::string READ_LOG_ENTRY = "read\n";

EpollRequestHandler::EpollRequestHandler(size_t epoll_events_limit, size_t connection_timeout,
                                         HttpRequestHandler& request_handler, ThreadPool& workers):
                                             _epoll_events_limit(epoll_events_limit),
                                             _connection_timeout(connection_timeout),
                                             _request_handler(request_handler), _workers(workers) {
    _epoll_descriptor = epoll_create(_epoll_events_limit);
}

EpollRequestHandler::~EpollRequestHandler() {}

void EpollRequestHandler::on_start() {
    epoll_event events_pool[_epoll_events_limit];
    while (!_stop_flag) {
        ssize_t descriptors_count = epoll_wait(_epoll_descriptor, events_pool,
            _epoll_events_limit, _connection_timeout);
        for (size_t i = 0; i < descriptors_count; ++i) {
            std::cout << READ_LOG_ENTRY;
            ssize_t descriptor = events_pool[i].data.fd;
            HttpSession& session = _sessions_pool.at(descriptor);
            std::function<void()>* task = make_task(session, events_pool[i]);
            if (task != nullptr) {
                _workers.offer_task(*task);
            }
        }
    }
}

void EpollRequestHandler::on_stop() {
    _stop_flag = false;
}

std::function<void()>* EpollRequestHandler::make_task(HttpSession& session, const epoll_event& event) {
    if (event.events & EPOLLIN) {
        return std::make_unique<std::function<void()> >(
            std::bind(&HttpSession::read, &session)).get();
    }

    return nullptr;
}