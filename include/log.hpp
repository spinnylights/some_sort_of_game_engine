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

/*!
 * \brief A wrapper class around a std::thread that joins the
 * thread automatically on destruction.
 *
 * (based on a class in _The C++ Programming Language_, Bjarne
 * Stroustrup, 4th ed., pg. 1215)
 */
struct GuardedThread {
    std::thread t;

    /*!
     * \brief (move assignment operator)
     */
    GuardedThread& operator=(GuardedThread&& other)
    {
        std::swap(t, other.t);
        return *this;
    }

    /*!
     * \brief Joins the thread, if it is joinable.
     */
    void join()
    {
        if (t.joinable()) {
            t.join();
        }
    }

    /*!
     * \brief (destructor)
     *
     * \copydoc join()
     */
    ~GuardedThread()
    {
        join();
    }
private:
    // this class should probably be substituted for std::jthread
    // once e.g. debian stable switches to gcc >= 10
};

/*!
 * \brief A single entry in the LoggableObj output format.
 *
 * Corresponds to one "member" of the "loggable object". As a
 * general rule, this class is not intended to be instantiated
 * directly, but rather to be used in the composition of
 * LoggableObj.
 */
struct LoggableObjMember {
    /*!
     * \brief The string to insert between the name and the
     * value.
     */
    static const inline std::string spacer = ": ";

    /*!
     * \brief The name of the member.
     */
    std::string name;
    /*!
     * \brief The value of the member.
     */
    std::string value;

    /*!
     * \brief The whole string to output.
     *
     * \param opening_chars The desired number of characters for
     * the "opening" (name + spacer) to take up. Uses spaces
     * between the spacer and name to make up the difference if
     * needed. See Log for an example.
     *
     * \param pre_spaces The number of spaces to insert before
     * the "opening" (i.e. for indentation).
     */
    std::string str(std::string::size_type opening_chars = 0,
                    std::string::size_type pre_spaces = 0);

    /*!
     * \brief (constructor) A single name-value pair.
     *
     * \param nme \copydoc name
     * \param vle \copydoc value
     */
    LoggableObjMember(std::string nme, std::string vle)
        :name{nme},
         value{vle}
    {}

    /*!
     * \brief (constructor) A single name-value pair, C-string
     * version.
     *
     * \param nme \copydoc name
     * \param vle \copydoc value
     */
    LoggableObjMember(std::string nme, const char* vle)
        :name{nme},
         value{vle}
    {}

    /*!
     * \brief (constructor) A name and a list of associated
     * values.
     */
    LoggableObjMember(std::string nme, std::vector<std::string> const& values)
        :name{nme}
    {
        for (std::string::size_type i = 0;
             i < values.size() - 1;
             ++i) {
            value += values.at(i) + "\n";
        }
        value += values.back();
    }

    /*!
     * \brief (constructor) "Duck-typed" value logging over
     * `std::to_string()`.
     */
    template <typename T> LoggableObjMember(std::string nme, T vle)
        :name{nme},
         value{std::to_string(vle)}
    {}
};

/*!
 * \brief A way to log an "object", i.e. a name and a list of
 * "members". See Log for an example of the output format.
 */
struct LoggableObj {
    /*!
     * \brief The name of the object.
     */
    std::string name;
    /*!
     * \brief A list of the object's members, which can assume a
     * variety of formats (see LoggableObjMember for more).
     */
    std::vector<LoggableObjMember> members;

    /*!
     * \brief The formatted string.
     */
    std::string str();
};

/*!
 * \brief A logging class. Has a variety of output formats,
 * easily-controlled indentation and line breaks, and can be
 * switched between synchronous and asynchronous operation at
 * runtime.
 *
 * For the time being, this class is not designed to be copied or
 * moved, as with some of the game engine classes. An instance
 * called `log` is available globally by including the header
 * `log.hpp`.
 *
 * It should be safe to call the various logging functions from
 * within destructors. The one thing you should not do from a
 * destructor is sync/async switching.
 *
 * This class's functions are not designed to be called from
 * multiple threads. This will be changed in the future if
 * necessary.
 *
 * For the time being, it just writes to stdout (unless it enters
 * an error state internally). This may be changed in the future
 * if it seems desirable.
 *
 * Some examples of use:
 *
 * ```
 * log.enter("meowing begins");
 *
 * // meowing begins
 *
 * log.attempt("Meower", "meowing");
 *
 * uint32_t meow_cnt = 3;
 * meower.meow(meow_cnt);
 *
 * log.finish();
 * log.indent();
 * log.enter("times meowed", meow_cnt);
 * log.indent(2);
 * log.enter("meow sounds", { "mrrr", "mrrrau", "maaaaau" });
 * log.brk();
 * log.enter("line breaks reset indentation");
 *
 * // Meower: meowing...OK
 * //     times meowed: 3
 * //         meow sounds: mrrr, mrrrau, maaaaau
 * //
 * // line breaks reset indentation
 *
 * log.enter({
 *     .name = "Meowers",
 *     .members = {
 *         { "colors",              "many" },
 *         { "legs",                4      },
 *         { "body temp",           101.5 },
 *         { "some i have known", { "Big Boy",
 *                                  "Lyla",
 *                                  "Fly",
 *                                  "Maxine",
 *                                  "Fifi",
 *                                  "Cerise" }}
 *     }
 * });
 *
 * // Meowers { colors:            many,
 * //           legs:              4,
 * //           body temp:         101.500000,
 * //           some i have known: Big Boy
 * //                              Lyla
 * //                              Fly
 * //                              Maxine
 * //                              Fifi
 * //                              Cerise }
 * ```
 */
class Log {
public:
    static const inline std::string indentation = "    ";

    /*!
     * \brief (constructor)
     */
    Log();

    Log(Log&&) = delete;
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    ~Log() noexcept;

    /*!
     * \brief Write a string.
     *
     * \param entry The string to write.
     *
     * \param newline Whether to write a newline after the entry
     * or not.
     */
    void enter(std::string entry, bool newline=true) noexcept;

    /*!
     * \brief Write a name, a separator, and a list of entries.
     *
     * \param name The name to write, followed by a separator.
     *
     * \param entries A list of entries. Written comma-separated.
     */
    void enter(std::string name,
               std::vector<const char*> const& entries) noexcept;

    /*!
     * \brief "Duck-typed" name-entry logging.
     *
     * \param obj The object name to write.
     *
     * \param attr The value of the object. Can be anything
     * supported by std::to_string().
     */
    template <typename T> void enter(std::string obj, T attr) noexcept
    {
        enter(obj, std::to_string(attr));
    }

    /*!
     * \brief Name-entry logging where both are strings.
     *
     * \param obj The object name to write.
     *
     * \param attr The value to write.
     */
    void enter(std::string obj, std::string attr) noexcept;

    /*!
     * \brief A convenient way to display a complex object. See
     * Log for an example.
     *
     * \param obj The object to display.
     */
    void enter(LoggableObj&& obj) noexcept;

    /*!
     * \brief Prints the entry followed by an ellipse and no
     * newline. Designed to be used in concert with finish() or
     * the like for operations that may fail.
     *
     * \param entry A description of what is being attempted.
     */
    void attempt(std::string entry) noexcept;

    /*!
     * \brief Like attempt(std::string), but using the
     * name-entry format.
     *
     * \param domain The domain or category the operation falls under.
     *
     * \copydetails attempt(std::string)
     */
    void attempt(std::string domain, std::string entry) noexcept;

    /*!
     * \brief Prints "OK" and a newline. Designed to be used with
     * attempt(); you can sandwich an operation that may fail
     * between them.
     */
    void finish() noexcept;

    /*!
     * \brief Turns on indentation. Everything after this will be
     * indented until indent(0) or brk() is called.
     */
    void indent() noexcept;

    /*!
     * \brief Like indent(), but allows the degree of indentation
     * to be set.
     *
     * \param amt How many indentations to precede the string
     * with.
     */
    void indent(std::string::size_type amt) noexcept;

    /*!
     * \brief Inserts a line break and turns off indentation.
     */
    void brk() noexcept;

    /*!
     * \brief Enables logging.
     */
    void turn_on() noexcept;

    /*!
     * \brief Disables logging.
     */
    void turn_off() noexcept;

    /*!
     * \brief Starts asynchronous logging (off by default).
     */
    void async_on();

    /*!
     * \brief Stops asynchronous logging.
     */
    void async_off();

private:
    std::mutex msgs_mutex;
    std::deque<std::string> msgs;
    std::string::size_type indent_amt = 0;
    bool on = false;
    bool stopped = false;
    bool async = false;

    void wait_to_empty() noexcept;
    void empty_queue() noexcept;

    GuardedThread emptier;

    bool append_newline(std::string& entry, bool newline) noexcept;
    bool indent_entry(std::string& entry) noexcept;
    bool format_entry(std::string& entry, bool newline) noexcept;
    void enter_sync(std::string entry, bool newline) noexcept;
    void enter_async(std::string entry, bool newline) noexcept;
    void write_entry(std::string entry) noexcept;

    void safe_err(const char* oper) noexcept
    {
        fprintf(stderr, "*** could not %s! discarding entry...\n", oper);
    }
};

// global log
extern Log log;

} // namespace cu

#endif
