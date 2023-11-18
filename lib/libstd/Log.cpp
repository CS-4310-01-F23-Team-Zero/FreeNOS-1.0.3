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

#include "Log.h"
#include "String.h"

Log::Log()
    : WeakSingleton<Log>(this)
    , m_minimumLogLevel(Notice)
    , m_ident(ZERO)
    , m_outputBufferWritten(0)
{
}

Log::~Log()
{
}

Log::Level Log::getMinimumLogLevel()
{
    return m_minimumLogLevel;
}

void Log::setMinimumLogLevel(Log::Level level)
{
    m_minimumLogLevel = level;
}

const char * Log::getIdent() const
{
    return (m_ident);
}

void Log::setIdent(const char *ident)
{
    m_ident = ident;
}

void Log::append(const char *str)
{
    // Copy input. Note that we need to reserve 1 byte for the NULL-terminator
    while (*str)
    {
        if (m_outputBufferWritten < LogBufferSize-1)
        {
            m_outputBuffer[m_outputBufferWritten++] = *str;
            str++;
        }
        else
        {
            flush(true);
        }
    }

    flush();
}

void Log::flush(const bool force)
{
    if (m_outputBufferWritten > 0 && (m_outputBuffer[m_outputBufferWritten-1] == '\n' || force))
    {
        m_outputBuffer[m_outputBufferWritten] = 0;
        write(m_outputBuffer);
        m_outputBufferWritten = 0;
    }
}

void Log::terminate() const
{
    for (;;);
}

Log & operator << (Log &log, const char *str)
{
    log.append(str);
    return log;
}

Log & operator << (Log &log, int number)
{
    String s = number;
    log.append(*s);
    return log;
}

Log & operator << (Log &log, const char character)
{
    const char tmp[2] = { character, 0 };
    log.append(tmp);
    return log;
}

Log & operator << (Log &log, unsigned number)
{
    String s = number;
    log.append(*s);
    return log;
}

Log & operator << (Log &log, unsigned long number)
{
    String s = number;
    log.append(*s);
    return log;
}

Log & operator << (Log &log, void *ptr)
{
    String s;
    s << Number::Hex << ptr;
    log.append(*s);
    return log;
}
