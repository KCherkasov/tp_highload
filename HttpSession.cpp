#include <bits/ios_base.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include "HttpSession.hpp"

const std::string SEND_ERROR = "send error: ";
const std::string READ_ERROR = "read error: ";

HttpSession::HttpSession(ssize_t descriptor, HttpRequestHandler& handler):
    _desciptor(descriptor),
    _handler(handler) {}

HttpSession::~HttpSession() {}

void HttpSession::read() {
    char read_buffer[CHUNK_SIZE];
    ssize_t read_bytes_count = recv(_desciptor, read_buffer, sizeof(read_buffer), 0);
    if (read_bytes_count < 0 && errno != EAGAIN) {
        throw std::runtime_error(READ_ERROR + std::string(strerror(errno)));
    }
    _request.append(read_buffer, (size_t)read_bytes_count);
    try {
        _handle.handle(_request, _reponse_header, _requested_path, _response_body);
    } catch (std::runtime_error) {
        return;
    }
}

void HttpSession::get_header() {
    try {
        if (write(_response_header)) {
            if (!_requested_path.empty()) {
                _file_stream.open(_requested_path, std::ifstream::in);
                _status = SS_FILE;
                get_file();
            } else {
                _status == SS_RESPONSE;
                get_response();
            }
        }
    } catch (std::runtime_error) {
        throw std::runtime_error(SEND_ERROR + std::string(strerror(errno)));
    }
}

void HttpSession::get_file() {
    char read_buffer[CHUNK_SIZE];
    ssize_t read_bytes_count = 0;
    if (_file_stream.is_open()) {
        while ((read_bytes_count = _file.readsome(read_buffer, CHUNK_SIZE)) > 0) {
            write(std::string(read_buffer, (size_t)read_bytes_count));
        }
        _file_stream.close();
        _status = SS_CLOSE_FILE;
    }
}

void HttpSession::get_response() {
    try {
        if (write(_response_body)) {
            _status = SS_CLOSE_FILE;
        }
    } catch (std::runtime_error) {
        throw std::runtime_error(SEND_ERROR + std::string(strerror(errno)));
    }
}

SessionStatus HttpSession::status() const {
    return _status;
}

void HttpSession::set_status(SessionStatus status) {
    _status = status;
}

bool HttpSession::write(const std::string& message) const {
    ssize_t left_to_send = message.size();
    ssize_t sent_size = 0;

    while (left_to_send > 0) {
        sent_size = send(_desciptor, message.data() + sent_size, message.size() - sent_size, 0);
        if (sent_size < 0) {
            throw std::runtime_error(SEND_ERROR + std::string(strerror(errno)));
        }
        left_to_send -= sent_size;
    }

    return true;
}
