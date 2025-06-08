#pragma once

#include <string>
#include <unordered_map>

namespace network {

enum class HTTPCode : int {
    OK = 200,
    Created = 201,
    Accepted = 202,
    NoContent = 204,

    MovedPermanently = 301,
    Found = 302,
    NotModified = 304,

    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    Conflict = 409,

    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnavailable = 503
};

const std::unordered_map<HTTPCode, std::string> kHTTPCodeToString{
    {HTTPCode::OK, "200 OK"},
    {HTTPCode::Created, "201 Created"},
    {HTTPCode::Accepted, "202 Accepted"},
    {HTTPCode::NoContent, "204 No Content"},
    {HTTPCode::MovedPermanently, "301 Moved Permanently"},
    {HTTPCode::Found, "302 Found"},
    {HTTPCode::NotModified, "304 Not Modified"},
    {HTTPCode::BadRequest, "400 Bad Request"},
    {HTTPCode::Unauthorized, "401 Unauthorized"},
    {HTTPCode::Forbidden, "403 Forbidden"},
    {HTTPCode::NotFound, "404 Not Found"},
    {HTTPCode::MethodNotAllowed, "405 Method Not Allowed"},
    {HTTPCode::Conflict, "409 Conflict"},
    {HTTPCode::InternalServerError, "500 Internal Server Error"},
    {HTTPCode::NotImplemented, "501 Not Implemented"},
    {HTTPCode::BadGateway, "502 Bad Gateway"},
    {HTTPCode::ServiceUnavailable, "503 Service Unavailable"}};

inline std::string to_string(HTTPCode code) {
    auto it = kHTTPCodeToString.find(code);
    return it != kHTTPCodeToString.end() ? it->second : "Unknown";
}

inline int to_int(HTTPCode code) {
    return static_cast<int>(code);
}
}  // namespace network
