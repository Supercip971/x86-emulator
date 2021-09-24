#pragma once

#include <fmt/color.h>
#include <fmt/core.h>
class location
{
    int _line;
    const char *_file;
    const char *_func;

public:
    constexpr location(){};
    constexpr location(const char *file, const char *func, int line) : _line(line), _file(file), _func(func){};

    constexpr const char *file() const { return _file; };
    constexpr const char *func() const { return _func; };
    constexpr int line() const { return _line; };
};

#define LOC_CURRENT location(__FILE__, __FUNCTION__, __LINE__)
#define log_impl(ll, format_col, format_str, ...)                                        \
    (                                                                                    \
        {                                                                                \
            fmt::print(fg(format_col) | fmt::emphasis::bold, format_str);                \
            fmt::print("{}: {}: {} \n", ll.file(), ll.line(), fmt::format(__VA_ARGS__)); \
        })

#define log$(...) \
    log_impl(LOC_CURRENT, fmt::color::steel_blue, "[log] ", __VA_ARGS__)
#define error$(...) \
    log_impl(LOC_CURRENT, fmt::color::crimson, "[err] ", __VA_ARGS__)
#define debug$(...) \
    log_impl(LOC_CURRENT, fmt::color::white, "[dbg] ", __VA_ARGS__)
