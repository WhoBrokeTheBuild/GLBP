#pragma once

#define JSON_NOEXCEPTION
#include <nlohmann/json.hpp>

typedef nlohmann::basic_json<
    std::map,
    std::vector,
    std::string,
    bool,
    std::int64_t,
    std::uint64_t,
    float,
    std::allocator> json;