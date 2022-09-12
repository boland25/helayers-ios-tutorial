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

#ifndef SRC_HELAYERS_PRINTABLE_H
#define SRC_HELAYERS_PRINTABLE_H

#include <iostream>

namespace helayers {

///@brief For controlling verbosity of Printable objects printouts.
///
enum Verbosity
{
  ///@brief Print nothing
  VERBOSITY_NONE,
  ///@brief Minimal, only some outline meta data
  VERBOSITY_LOW,
  ///@brief Regular, main part of object.
  VERBOSITY_REGULAR,
  ///@brief High detail
  VERBOSITY_DETAILED,
  ///@brief Extra high detail
  VERBOSITY_EXTRA_DETAILED
};

/// An abstract class for objects that can be printed to text in a human
/// readable way. Used mainly for debug purposes.
///
/// Simplest way to print a Printable obj:
///     cout << obj << endl;
///
/// To convert to a string, use:
///     string res=obj.toString()
/// with optional argument: verbosity.
///
/// To write to a stream with custom verbosity use:
///     cout << obj.toString(verbosity);
/// or for large objects this is more efficient:
///     obj.debugPrint(title,verbosity);
/// which writes to cout by default, but has an additonal optional ostream
/// argument.
///
class Printable
{

protected:
public:
  virtual ~Printable() {}

  ///@brief Prints the content of this object.
  ///
  ///@param title Text to add to the print
  ///@param verbosity Verbosity level
  ///@param out Output stream
  virtual void debugPrint(const std::string& title = "",
                          Verbosity verbosity = VERBOSITY_REGULAR,
                          std::ostream& out = std::cout) const = 0;

  ///@brief Convert object to string.
  ///
  ///@param verbosity Verbosity level.
  std::string toString(Verbosity verbosity = VERBOSITY_REGULAR) const;
};

///@brief Prints object to output stream in readable format.
/// Uses verbosity VERBOSITY_REGULAR.
///
///@param out Output stream.
///@param p Object to print.
std::ostream& operator<<(std::ostream& out, const Printable& p);

} // namespace helayers

#endif /* SRC_HELAYERS_CTILE_H */
