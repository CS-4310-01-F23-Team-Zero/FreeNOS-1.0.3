/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIB_LIBSTD_LOG_H
#define __LIB_LIBSTD_LOG_H

#include "Singleton.h"
#include "Macros.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Output a log line to the system log (syslog).
 *
 * @param type Log level
 * @param typestr Log level string
 * @param msg Message to output
 */
#define MAKE_LOG(type, typestr, msg) \
    {\
     if (Log::instance() && type <= Log::instance()->getMinimumLogLevel())  \
        (*Log::instance()) << "[" typestr "] " << __FILE__ ":" <<  __LINE__ << " " << __FUNCTION__ << " -- " << msg << "\r\n"; \
    }

/**
 * Output a critical message and terminate program immediatly.
 *
 * @param msg The critical message.
 */
#define FATAL(msg) \
    { \
        MAKE_LOG(Log::Emergency, "Emergency", msg); \
        if (Log::instance()) Log::instance()->terminate(); \
    }

/**
 * Output an error message.
 *
 * @param msg The error message.
 */
#define ERROR(msg)   MAKE_LOG(Log::Error, "Error", msg)

/**
 * Output a warning message.
 *
 * @param msg The warning message.
 */
#define WARNING(msg) MAKE_LOG(Log::Warning, "Warning", msg)

/**
 * Output a notice message.
 *
 * @param msg The notice message
 */
#define NOTICE(msg)  MAKE_LOG(Log::Notice, "Notice", msg)

/**
 * Output a regular message to standard output.
 *
 * @param msg The information message
 */
#define INFO(msg)    MAKE_LOG(Log::Info, "Info", msg)

/**
 * Output a debug message to standard output.
 *
 * @param msg The debug message to output
 */
#define DEBUG(msg)   MAKE_LOG(Log::Debug, "Debug", msg)

/**
 * Logging class.
 *
 * @note This class is a singleton
 */
class Log : public WeakSingleton<Log>
{
  private:

    /** Size of the log buffer in bytes */
    static const Size LogBufferSize = 512;

  public:

    /** Logging level values */
    enum Level
    {
        Emergency,
        Alert,
        Critical,
        Error,
        Warning,
        Notice,
        Info,
        Debug
    };

  public:

    /**
     * Constructor.
     */
    Log();

    /**
     * Destructor
     */
    virtual ~Log();

    /**
     * Get the minimum logging level.
     *
     * @return Minimum LogLevel
     */
    Level getMinimumLogLevel();

    /**
     * Set the minimum logging level.
     */
    void setMinimumLogLevel(Level level);

    /**
     * Append to buffered output.
     *
     * @param str String to append to buffer
     */
    void append(const char *str);

    /**
     * Set log identity.
     *
     * @param ident Log identity
     */
    void setIdent(const char *ident);

    /**
     * Retrieve log identify.
     *
     * @return Log identity
     */
    const char * getIdent() const;

    /**
     * Terminate the program immediately.
     */
    virtual void terminate() const;

  protected:

    /**
     * Write to the actual output device.
     */
    virtual void write(const char *str) = 0;

  private:

    /**
     * Flush internal buffer.
     *
     * This function reads the contents of the internal
     * buffer and writes all available bytes to the actual
     * output device using write().
     *
     * @param force True to always flush, even without newline
     *              at the end of the buffer.
     *
     * @see write
     */
    void flush(const bool force = false);

  private:

    /** Minimum log level required to log. */
    Level m_minimumLogLevel;

    /** Identity */
    const char *m_ident;

    /** Output line is stored here until written using write() */
    char m_outputBuffer[LogBufferSize];

    /** Number of characters written in the output buffer */
    Size m_outputBufferWritten;
};

/**
 * @name Operators to output various standard types to the log
 * @{
 */

Log & operator << (Log &log, const char *str);

Log & operator << (Log &log, int number);

Log & operator << (Log &log, const char character);

Log & operator << (Log &log, unsigned number);

Log & operator << (Log &log, unsigned long number);

Log & operator << (Log &log, void *ptr);

/**
 * @}
 */

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBSTD_LOG_H */
