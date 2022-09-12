/*
 * MIT License
 *
 * Copyright (c) 2020 International Business Machines
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SRC_HELAYERS_ALWAYSASSERT_H
#define SRC_HELAYERS_ALWAYSASSERT_H

#include <string>
#include <stdexcept>

namespace helayers {

/// Defines a type of object to be thrown as exception when assertion
/// condition is not met.  These objects may be thrown from always_assert()
/// invocations.
///
/// The exception's message (what()) contains the source location of the
/// failed assertion.
class assert_error : public std::runtime_error
{
public:
  explicit assert_error(const std::string& what) : std::runtime_error(what) {}
  explicit assert_error(const char* what) : std::runtime_error(what) {}
  assert_error(const assert_error&) = default;
  assert_error& operator=(const assert_error&) = default;
  assert_error(assert_error&&) = default;
  assert_error& operator=(assert_error&&) = default;
};

void always_assert_fail(const char* conditionString,
                        const char* fileName,
                        unsigned int lineNum,
                        const char* funcName);

void always_assert_fail(const char* conditionString,
                        const char* fileName,
                        unsigned int lineNum,
                        const char* funcName,
                        const std::string& message);

/// always_assert(condition) runs even in Release mode; it throws assert_error
/// if the condition isn't met.
///
/// Example usage:
///
///   always_assert(dimensions == 3);
///
#define always_assert(condition)                                               \
  do {                                                                         \
    if (!(condition)) {                                                        \
      always_assert_fail(#condition, __FILE__, __LINE__, __func__);            \
    }                                                                          \
  } while (0)

/// always_assert_msg(condition, message) adds a message that will be added to
/// the assertion exception message when the condition isn't met.
///
/// Example usage:
///
///   always_assert_msg(dimensions == 3, "World is not 3D");
///
/// If the assertion fails, an assert_error exception is thrown.
#define always_assert_msg(condition, msg)                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      always_assert_fail(#condition, __FILE__, __LINE__, __func__, msg);       \
    }                                                                          \
  } while (0)

} // namespace helayers

#endif // SRC_HELAYERS_ALWAYSASSERT_H
