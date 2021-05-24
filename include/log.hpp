/*
 * This file is part of Crypt Underworld.
 *
 * Crypt Underworld is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * Crypt Underworld is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Crypt Underworld. If not, see
 * <https://www.gnu.org/licenses/>.
 *
 * Copyright (c) 2021 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef u5c273d49fef479e9aa0c521eda03327
#define u5c273d49fef479e9aa0c521eda03327

#include <deque>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <utility>
#include <vector>

namespace cu {

// see /The C++ Programming Language/, 4th ed., pg. 1215
//
// this should probably be changed to use std::jthread once e.g.
// debian stable switches to gcc >= 10
struct guarded_thread {
    std::thread t;

    guarded_thread& operator=(guarded_thread&& other)
    {
        std::swap(t, other.t);
        return *this;
    }

    ~guarded_thread()
    {
        if (t.joinable()) {
            t.join();
        }
    }
};

struct LoggableObjMember {
    static const inline std::string spacer = ": ";

    std::string name;
    std::string value;

    std::string str(std::string::size_type extra_spaces = 0);
};

struct LoggableObj {
    std::string name;
    std::vector<LoggableObjMember> members;

    std::string str();
};

class Log {
public:
    static const inline std::string indentation = "    ";

    Log();

    Log(Log&&) = delete;
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    ~Log() noexcept;

    void enter(std::string entry, bool newline=true) noexcept;
    void enter(std::string name, std::vector<const char*>& entries) noexcept;
    void enter_obj(LoggableObj&& obj) noexcept;
    void attempt(std::string entry) noexcept;
    void indent() noexcept;
    void indent(std::string::size_type amt) noexcept;
    void finish() noexcept;
    void brk() noexcept;
    void turn_on() noexcept;
    void turn_off() noexcept;

private:
    using dur = std::chrono::milliseconds;

    std::mutex msgs_mutex;
    std::deque<std::string> msgs;
    std::string::size_type indent_amt = 0;
    bool on = false;
    bool stopped = false;

    void wait_to_empty() noexcept;
    void empty_queue() noexcept;

    guarded_thread emptier;
};

// global log
extern Log log;

} // namespace cu

#endif