#ifndef LMS_LOGGING_CONSOLE_SINK_H
#define LMS_LOGGING_CONSOLE_SINK_H

#include <iostream>
#include <mutex>

#include "lms/logging/sink.h"

namespace lms {
namespace logging {

class Event;

/**
 * @brief A console sink is a sink implementation that
 * writes all log messages into an std::ostream.
 *
 * The log messages will be prefixed with the time, the logging
 * level and the logging tag.
 *
 * @author Hans Kirchner
 */
class ConsoleSink : public Sink {
public:
    /**
     * @brief Create a new console sink that will write into the given ostream.
     * @param out an ostream instance, e.g. std::cout or std::cerr
     */
    explicit ConsoleSink(std::ostream& out = std::cout)
        : m_out(out), m_colored(true), m_time(true) {}

    /**
     * @brief Create a new console sink that will write into the given ostream.
     * @param out an ostream instance
     * @param colored set to true if the output should be ASCII colored
     * depending on the log level
     * @param time set to true if each logging message should be prepended by
     * the current time
     */
    ConsoleSink(std::ostream& out, bool colored, bool time)
        : m_out(out), m_colored(colored), m_time(time) {}

    /**
     * @brief Log the given message to the ostream.
     *
     * Format: TIME LEVEL TAG: MESSAGE
     *
     * @param message a log message instance
     */
    void sink(const Event &message) override;

    /**
     * @brief Set to true if the time should be logged.
     */
    void printTime(bool time);

    /**
     * @brief Set to true if the output should be colored.
     */
    void printColored(bool colored);
private:
    std::ostream &m_out;
    bool m_colored;
    bool m_time;
    std::mutex mtx;
};

} // namespace logging
} // namespace lms

#endif /* LMS_LOGGING_CONSOLE_SINK_H */

