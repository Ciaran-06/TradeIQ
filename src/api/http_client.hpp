// http_client.hpp
#pragma once

#include <string>

class HttpClient {
public:
    virtual ~HttpClient() = default;

    // Perform a GET request to the given URL.
    virtual std::string get(const std::string& url) = 0;
};
