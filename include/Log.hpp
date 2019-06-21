#pragma once

#include <Util.hpp>
#include <depend/JSON.hpp>

#include <cstdio> // for printf, vsnprintf

enum class LogLevel {
    INFO,
    WARN,
    ERROR,
    PERF,
    VERBOSE,
    LOAD,
};

template <class T>
static auto LogWrap(const T& v) {
    return v;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

template <> 
auto LogWrap<std::string>(const std::string& v) {
    return v.c_str();
}

template <> 
auto LogWrap<json>(const json& v) {
    return v.get<std::string>().c_str();
}

#pragma clang diagnostic pop

#pragma GCC diagnostic pop


template <class ...Args>
static inline void Log(LogLevel level, const char * format, Args... args)
{
#if defined(WIN32)

    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    const int DEFAULT = 7;

    int color = DEFAULT;

    switch (level)
    {
    case LogLevel::INFO:
        color = 7; // White
        break;
    case LogLevel::WARN:
        color = 6; // Yellow
        break;
    case LogLevel::ERROR:
        color = 4; // Red
        break;
    case LogLevel::PERF:
        color = 5; // Magenta
        break;
    case LogLevel::VERBOSE:
        color = 8; // Grey
        break;
    case LogLevel::LOAD:
        color = 2; // Green
        break;
    }

    SetConsoleTextAttribute(hConsole, color);

#else 

    static const char * TERM = getenv("TERM");
    static bool hasColor = (TERM && (
        strncmp(TERM, "xterm", 5)   == 0 || 
        strncmp(TERM, "rxvt", 4)    == 0 ||
        strncmp(TERM, "konsole", 8) == 0
    ));
    
    const short FG_DEFAULT = 39;
    const short BG_DEFAULT = 49;

    short fgColor = FG_DEFAULT;
    short bgColor = BG_DEFAULT;

    switch (level)
    {
    case LogLevel::INFO:
        fgColor = 97; // White
        break;
    case LogLevel::WARN:
        fgColor = 33; // Yellow
        break;
    case LogLevel::ERROR:
        fgColor = 31; // Red
        break;
    case LogLevel::PERF:
        fgColor = 35; // Magenta
        break;
    case LogLevel::VERBOSE:
        fgColor = 37; // Grey
        break;
    case LogLevel::LOAD:
        fgColor = 32; // Green
        break;
    }

    if (hasColor) {
        printf("\033[%dm\033[%dm", fgColor, bgColor);
    }

#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
    
    printf(format, LogWrap(args)...);

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

#if defined(WIN32)
    
    SetConsoleTextAttribute(hConsole, DEFAULT);

#else

    if (hasColor) {
        printf("\033[%dm\033[%dm", FG_DEFAULT, BG_DEFAULT);
    }

#endif
}

#define LogInfo(M, ...) \
    do { Log(LogLevel::INFO, "[INFO](%s:%d) " M "\n", GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogWarn(M, ...) \
    do { Log(LogLevel::WARN, "[WARN](%s:%d) " M "\n", GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogError(M, ...) \
    do { Log(LogLevel::ERROR, "[ERRO](%s:%d) " M "\n", GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogPerf(M, ...) \
    do { Log(LogLevel::PERF, "[PERF](%s:%d) " M "\n", GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogVerbose(M, ...) \
    do { Log(LogLevel::VERBOSE, "[VERB](%s:%d) " M "\n", GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogLoad(M, ...) \
    do { Log(LogLevel::LOAD, "[LOAD](%s:%d) " M "\n", GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)
