#pragma once
#include "api/http_client.hpp"
#include <string>

class DefaultHttpClient : public HttpClient {
public:
    DefaultHttpClient() = default;
    std::string get(const std::string& url) override; // declared here
};
