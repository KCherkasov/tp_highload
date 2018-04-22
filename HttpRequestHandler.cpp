#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sys/stat.h>
#include <zconf.h>

#include "AllowedMethods.hpp"
#include "Config.hpp"
#include "ErrorResponses.hpp"
#include "HttpRequestHandler.hpp"
#include "ProhibitedMethods.hpp"
#include "SupportedFileExtensions.hpp"
#include "ResponseCodes.hpp"

const std::string DIR_LEVEL_UP_SEQUENCE = "/../";
const char ARGUMENTS_START = '?';

const char SEPARATOR_SPACE = ' ';
const char SEPARATOR_RETURN = '\r';
const char SEPARATOR_NEW_LINE = '\n';

const size_t PERCENT_ENCODING_LENGTH = 2;
const char ENCODING_START_CHAR = '%';
const char SPACE_ESCAPE_CHAR = '+';
const char DOT_CHAR = '.';
const char SLASH_CHAR = '/';

const std::string SERVER_HEADER = "Server: Webserver ";
const std::string DATE_HEADER = "Date: ";
const std::string CONNECTION_HEADER = "Connection: close";
const std::string CONTENT_LENGTH_HEADER = "Content-Length: ";
const std::string CONTENT_TYPE_HEADER = "Content-Type: ";

const std::string INDEX_FILE = "index.html"

HttpRequestHandler::HttpRequestHandler(const std::string& document_root): _document_root(document_root) {}

void HttpRequestHandler::handle(const std::string& request, std::string& response_header,
                                std::string& response_path, std::string& response_body) const {
    std::istringstream request_stream(request);
    std::string method, url = "";
    std::string protocol = config::DEFAULT_PROTOCOL;

    if (!parse_request_parameter(method, request_stream) || !parse_request_parameter(url, request_stream)
        || !parse_request_parameter(protocol, request_stream)) {
        std::runtime_error(BAD_REQUEST_ERROR);
    }

    url = decode_url(url);
    size_t pos;
    while ((pos = url.find(DIR_LEVEL_UP_SEQUENCE)) != std::string::npos) {
        url.erase(pos, DIR_LEVEL_UP_SEQUENCE.size());
    }

    pos = url.find(ARGUMENTS_START);
    url = pos != std::string::npos ? url.substr(0, pos) : url;

    switch (method) {
        case GET:
            make_get_response();
            break;
        case HEAD:
            make_head_response();
            break;
        case POST: case PUT: case PATCH:
        case DELETE: case TRACE:
        case CONNECT: case OPTIONS:
            make_not_allowed_response();
            break;
        default:
            make_not_implemented_response();
    }
}

bool HttpRequestHandler::file_exists(const std::string& path) const {
    return access(path.c_str(), 0) != -1;
}

bool HttpRequestHandler::parse_request_parameter(std::string& request_parameter, 
     std::istringstream& source) const {
    while (source.peek() != EOF) {
        char read = (char)source.get();
        if (read == SEPARATOR_NEW_LINE
            || read == SEPARATOR_RETURN
            || read == SEPARATOR_SPACE) {
            return true;
        } else {
            source.append(read);
        }
    }
    return false;
}

char HttpRequestHandler::remove_percent_coding(const std::string& source, size_t pos) const {
    int char_code = -1;
    sscanf(source.substr(pos + 1, PERCENT_ENCODING_LENGTH), "%x", &char_code);
    return static_cast<char>(char_code);
}

std::string  HttpRequestHandler::decode_url(const std::string& source) const {
    std::string parsed_url = "";
    for (size_t i = 0; i < url.size(); ++i) {
        switch (source[i]) {
            case ENCODING_START_CHAR:
                parsed_url.append(remove_percent_coding(source, i));
                i += PERCENT_ENCODING_LENGTH;
                break;
            case SPACE_ESCAPE_CHAR:
                parsed_url.append(SEPARATOR_SPACE);
                break;
            default:
                parsed_url.append(source[i]);
        }
    }
    return parsed_url;
}

std::string HttpRequestHandler::get_file_extension(const std::string& source) const {
    size_t dot_pos = source.find_last_or(DOT_CHAR);
    return dot_pos == std::string::npos ? TXT : source.substr(dot_pos + 1);
}

std::string HttpRequestHandler::make_response_header(size_t content_length,
                                                     const std::string& file_extension,
                                                     const std::string& protocol,
                                                     const std::string& response_code) const {
    std::ostringstream header_stream;
    std::chrono::system_clock::time_point timepoint_stamp = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(timepoint_stamp);
    
    header_stream << protocol << SEPARATOR_SPACE << response_code
        << SEPARATOR_RETURN << SEPARATOR_NEW_LINE << SERVER_HEADER << SEPARATOR_RETURN
        << SEPARATOR_NEW_LINE << DATE_HEADER << std::ctime(&timestamp) << CONNECTION_HEADER
        << SEPARATOR_RETURN << SEPARATOR_NEW_LINE << CONTENT_LENGTH_HEADER << content_length
        << SEPARATOR_RETURN << SEPARATOR_NEW_LINE << CONTENT_TYPE_HEADER
        << _mime_map.get_mime_type_for_extension(file_extension) << SEPARATOR_RETURN << SEPARATOR_NEW_LINE;

    return response_stream.str();
}std::string HttpRequestHandler::make_url_path(const std::string& source, bool is_folder) const {
    return !is_folder ? source += INDEX_FILE : source.back() != SLASH_CHAR ? source += SLASH_CHAR : source;
}
                            
void HttpRequestHandler::make_get_response(const std::string request_url, const std::string& protocol,
                                           std::string& response_header, std::string& path,
                                           std::string& response_body) const {
    std::string root_path = _document_root + request_url;
    struct stat request_stat;
    bool is_folder = (stat(root_path.c_str(), request_stat) == 0) && (stat.st_mode & S_IFDIR);
    root_path = make_url_path(root_path, is_folder);

    if (file_exists(root_path)) {
        std::string content_type = is_folder ? HTML : get_file_extension(request_url);
        struct stat file;
        stat(root_path.c_str(), &file);
        response_header = make_response_header((size_t)file.st_size, content_type, protocol, OK_CODE);
        response_header.append(SEPARATOR_RETURN);
        response_header.append(SEPARATOR_NEW_LINE);
        path = root_path;
        std::ifstream in(path);
    } else {
        std::string response_code = is_folder ? FORBIDDEN_CODE : NOT_FOUND_CODE;
        response_header = make_response_header(strlen(is_folder ? ERROR_403 : ERROR_404_NOT_FOUND),
                                               HTML, protocol, code);                                       
        response_header.append(SEPARATOR_RETURN);
        response_header.append(SEPARATOR_NEW_LINE);
        response_body = is_folder ? ERROR_403 : ERROR_404_NOT_FOUND;
    }
}

void HttpRequestHandler::make_head_response(const std::string& request_url, const std::string& protocol,
                                            std::string& response_header, std::string& response_body) const {
    std::string root_path = _document_root + request_url;
    struct stat request_stat;
    bool is_folder = (stat(root_path.c_str(), &request_stat) == 0) && (request_stat.st_mode & S_IFDIR);
    root_path = make_url_path(root_path, is_folder);

    if (file_exists(root_path)) {
        std::string content_type = is_folder ? HTML : get_file_extension(request_url);
        struct stat file;
        stat(root_path.c_str(), &file);
        response_header = make_response_header((size_t)file.st_size, content_type, protocol, OK_CODE);
    } else {
        std::string code = is_folder ? FORBIDDEN_CODE : NOT_FOUND_CODE;
        response_header = make_response_header(strlen(is_folder ? ERROR_403 : ERROR_404_NOT_FOUND),
                                               HTML, protocol, code);
    }
}

void HttpRequestHandler::make_not_allowed_response(const std::string& protocol,
                                                   std::string& response_header,
                                                   std::string& response_body) const {
    response_header = make_response_header(strlen(ERROR_405_NOT_ALLOWED), HTML, protocol, NOT_ALLOWED_CODE);
    response_header.append(SEPARATOR_RETURN);
    response_header.append(SEPARATOR_NEW_LINE);
    response_body = ERROR_405_NOT_ALLOWED;
}

void HttpRequestHandler::make_not_implemented_response(const std::string& protocol,
                                                       std::string& response_header,
                                                       std::string& response_body) const {
    response_header = make_response_header(strlen(ERROR_501), HTML, protocol, NOT_IMPLEMENTED_CODE);
    response_header.append(SEPARATOR_RETURN);
    response_header.append(SEPARATOR_NEW_LINE);
    response_body = ERROR_501;
}
