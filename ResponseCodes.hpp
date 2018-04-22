#ifndef RESPONSE_CODES_HPP
#define RESPONSE_CODES_HPP

#include <string>

const std::string OK_CODE = "200 OK";
const std::string FORBIDDEN_CODE = "403 Forbidden";
const std::string NOT_FOUND_CODE = "404 Not Found";
const std::string NOT_ALLOWED_CODE = "405 Not Allowed";
const std::string NOT_IMPLEMENTED_CODE = "501 Not Implemented";

const std::string NOT_ALLOWED_LONG = "405 Method Not Allowed";
const std::string NOT_IMPLEMENTED_LONG = "501 Method Not Implemented";

const std::string UNKNOWN_CODE = "";

const size_t OK_DIGITAL = 200;
const size_t FORBIDDEN_DIGITAL = 403;
const size_t NOT_FOUND_DIGITAL = 404;
const size_t NOT_ALLOWED_DIGITAL = 405;
const size_t NOT_IMPLEMENTED_DIGITAL = 501;

#endif
