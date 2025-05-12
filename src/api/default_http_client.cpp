#include <cpr/cpr.h>
#include <stdexcept>

#include "api/default_http_client.hpp"

std::string DefaultHttpClient::get(const std::string& url) {
    auto response = cpr::Get(cpr::Url{url}, cpr::Timeout{5000});
    if (response.status_code != 200) {
        throw std::runtime_error("HTTP request failed: " + std::to_string(response.status_code));
    }
    return response.text;
}
