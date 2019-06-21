#pragma once 

#include <string>

inline std::string GetDirname(const std::string& path)
{
    size_t pivot = path.find_last_of('/');
    return (pivot == std::string::npos
        ? "./"
        : path.substr(0, pivot));
}

inline std::string GetBasename(const std::string& path)
{
    size_t pivot = path.find_last_of('/');
    return (pivot == std::string::npos
        ? std::string()
        : path.substr(pivot + 1));
}

inline std::string GetExtension(const std::string& path)
{
    size_t pivot = path.find_last_of('.');
    return (pivot == std::string::npos
        ? std::string()
        : path.substr(pivot + 1));
}
