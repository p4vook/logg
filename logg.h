#ifndef _LOGG_H
#define _LOGG_H

#include <iostream>
#include <iomanip>
#include <chrono>

// debug vyvod by pavelthebest
// 2022

namespace logg {

enum class LogLevel { NONE, INFO, DEBUG, TRACE };

std::ostream &operator<<(std::ostream &out, const LogLevel &x) {
        switch (x) {
                case LogLevel::INFO:
                        return out << "INFO";
                case LogLevel::DEBUG:
                        return out << "DEBUG";
                case LogLevel::TRACE:
                        return out << "TRACE";
                default:
                        return out;
        }
}

template <bool exists> struct StreamWrap {
};

template <> struct StreamWrap<true> {
        std::ostream out{std::ostream{std::cerr.rdbuf()}};
};

template <class T>
StreamWrap<true> &operator<<(StreamWrap<true> &stream, const T &x)
{
        stream.out << x;
        return stream;
};

template <class T>
StreamWrap<false> &operator<<(StreamWrap<false> &stream, const T &)
{
        return stream;
}

template <LogLevel max_level> struct Logger {
        const std::chrono::time_point<std::chrono::steady_clock> constructed = std::chrono::steady_clock::now();

        StreamWrap<false> none;
        StreamWrap<max_level >= LogLevel::INFO> info;
        StreamWrap<max_level >= LogLevel::DEBUG> debug;
        StreamWrap<max_level >= LogLevel::TRACE> trace;

        std::chrono::duration<float> delta() const {
                return std::chrono::duration<float>(std::chrono::steady_clock::now() - constructed);
        }
};

template<LogLevel level, LogLevel max_level>
StreamWrap<true> &extract_stream(Logger<max_level> &logger) requires (level == LogLevel::INFO) {
        return logger.info;
}

template<LogLevel level, LogLevel max_level>
StreamWrap<true> &extract_stream(Logger<max_level> &logger) requires (level == LogLevel::DEBUG) {
        return logger.debug;
}

template<LogLevel level, LogLevel max_level>
StreamWrap<true> &extract_stream(Logger<max_level> &logger) requires (level == LogLevel::TRACE) {
        return logger.trace;
}

template <LogLevel level> struct lvl {
};

template <LogLevel max_level, LogLevel cur_level>
StreamWrap<true> &
operator<<(Logger<max_level> &logger, const lvl<cur_level> &) requires (cur_level <= max_level && cur_level != LogLevel::NONE) {
        return extract_stream<cur_level>(logger) << cur_level << " T+" << std::fixed << std::setprecision(5) << logger.delta().count() << "s ";
}

template <LogLevel max_level, LogLevel cur_level>
StreamWrap<false> &
operator<<(Logger<max_level> &logger, const lvl<cur_level> &)
{
        return logger.none;
}

lvl<LogLevel::INFO> INFO;
lvl<LogLevel::DEBUG> DEBUG;
lvl<LogLevel::TRACE> TRACE;

}

#endif // _LOGG_H
