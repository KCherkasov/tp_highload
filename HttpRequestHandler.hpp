#ifndef WEBSERVER_HTTP_REQUEST_HANDLER_HPP
#define WEBSERVER_HTTP_REQUEST_HANDLER_HPP

#include <functional>
#include <iostream>
#include <unordered_map>

#include "MimeTypeMapper.hpp"

const std::string BAD_REQUEST_ERROR = "bad request";

class HttpRequestHandler {
    private:
        std::string _document_root;

        MimeTypeMapper _supported_mime_types;

        bool file_exists(const std::string& path) const;

        bool parse_request_parameter(std::string& request_parameter, std::istringstream& source) const;

        char remove_percent_coding(const std::string& source, size_t pos) const;
        std::string decode_url(const std::string& source) const;
        std::string get_file_extension(const std::string& source) const;
        std::string make_response_header(size_t content_length, const std::string& file_extension,
                                         const std::string& protocol, const std::string& response_code) const;
        std::string make_url_path(const std::string& source, bool is_folder) const;

        void make_get_response(const std::string& request_url, const std::string& protocol,
                               std::string& response_header, std::string& path, std::string& response_body) const;
        void make_head_response(const std::string& request_url, const std::string& protocol,
                                std::string& response_header, std::string& response_body) const;

        void make_not_allowed_response(const std::string& protocol, std::string& response_header,
                                       std::string& response_body) const;
        void make_not_implemented_response(const std::string& protocol, std::string& response_header,
                                       std::string& response_body) const;

    public:
        HttpRequestHandler(const std::string& document_root);
        ~HttpRequestHandler();

        void handle(const std::string& request, std::string& response_header,
                    std::string& response_path, std::string& response_body) const;
};

#endif
