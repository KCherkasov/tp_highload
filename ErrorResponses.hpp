#ifndef ERROR_RESPONSES_HPP
#define ERROR_RESPONSES_HPP

#include <string>

#include "ResponseCodes.hpp"

const std::string ERROR_403 = "<html><head><title>Forbidden</title></head>"
    + "<body><h1>" + FORBIDDEN_CODE + "</h1></body></html>";
    
const std::string ERROR_404_NOT_FOUND = "<html><head><title>Not Found</title></head>"
    + "<body><h1>" + NOT_FOUND_CODE + "</h1></body></html>";

const std::string ERROR_405_NOT_ALLOWED = "<html><head><title>Not Allowed</title></head>"
    + "<body><h1>" + NOT_ALLOWED_LONG + "</h1></body></html>";

const std::string ERROR_501 = "<html><title>Not Implemented</title><head></head>"
    + "<body><h1>" + NOT_IMPLEMENTED_LONG + "</h1></body></html>";

#endif