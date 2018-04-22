#ifndef WEBSERVER_HTTP_SESSION_HPP
#define WEBSERVER_HTTP_SESSION_HPP

#include <fstream>

#include "HttpRequestHandler.hpp"

const size_t CHUNK_SIZE = 4096;

enum SessionStatus { SS_READ, SS_HEADER, SS_FILE, SS_RESPONSE, SS_CLOSE_FILE, SS_USED };

class HttpSession {
    private:
        ssize_t _desciptor;

        std::string _request;
        std::string _response_header;
        std::string _requested_path;
        std::string _response_body;
        std::ifstream _file_stream;

        SessionStatus _status;

        HttpRequestHandler& _handler;

        bool write(const std::string& message) const;

    public:
        HttpSession(ssize_t descriptor, HttpRequestHandler& handler);
        ~HttpSession();

        void read();
        void get_header();
        void get_file();
        void get_response();

        SessionStatus status() const;
        void set_status(SessionStatus status);
};

#endif
