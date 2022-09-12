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

#ifndef SRC_HELAYERS_PRINTUTILS_H
#define SRC_HELAYERS_PRINTUTILS_H

#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>
#include "Printable.h"

namespace helayers {

///@brief Options effecting printing of helayers object for debug purposes.
/// A global settings can be obtained by getPrintOptions()
struct PrintOptions
{
  /// When printing an array, how many elements will be printed.
  int numPrintedElements = 4;

  /// When printing an array, additional elements printed at the end
  int numPrintedLastElements = 1;

  /// When set to false, even when printing complex numbers the imaginary part
  /// is discarded. This is useful when working with schemes like
  /// CKKS, and not using the imaginary part
  bool printComplexNumbers = true;

  /// When set to false, numbers will be printed in conventional decimal form
  /// (e.g., 1.234)
  bool useScientificNotation = true;

  /// Precision setting when printing floating point numbers
  int precision = 3;

  /// When set to a value other than one, some ciphertext prints
  /// will include the values multiplied by this factor, in addition
  /// to the original ones.
  double factor = 1;

  /// When set to true, some ciphertext prints will print
  /// The ciphertext itself, e.g., the CRT residues of the
  /// ciphertext's polynomials.
  bool printEncryptedContent = false;
};

/// A helper class for printing numbers, tensors, etc...
class PrintUtils
{

public:
  ///@brief Prints a title for debug prints
  ///
  ///@param out Output stream
  ///@param objectType Type of object being printed
  ///@param title Optional title of this print
  static void printTitle(std::ostream& out,
                         const std::string& objectType,
                         const std::string& title);

  ///@brief Same as printTitle, but formatted as a section title with a line
  /// separator
  static void printLargeTitle(std::ostream& out,
                              const std::string& objectType,
                              const std::string& title);

  ///@brief Prints a separator line (filled with '=' chars)
  ///
  ///@param out Output stream
  static void printSeparator(std::ostream& out);

  ///@brief Prints a double formatted according to the print options.
  ///
  ///@param out Output stream
  static void printDouble(std::ostream& out, double val);

  ///@brief Converts a boolean value to a 'true' or 'false' string
  ///
  ///@param val Boolean value
  static const char* boolToString(bool val);

  ///@brief Prints a complex number formatted according to the print options.
  ///
  ///@param out Output stream
  static void printComplex(std::ostream& out, std::complex<double> val);

  ///@brief Prints an array of type T. The printed elements are according to the
  /// print options. The T type elements are printed using the << operator
  ///
  ///@param out Output stream
  ///@param vals Values to print
  ///@param withIndices Whether to include indices in print
  template <typename T>
  static void printArray(std::ostream& out,
                         const std::vector<T>& vals,
                         bool withIndices = false);

  template <typename T>
  static void printArrayWithTitle(std::ostream& out,
                                  const std::string title,
                                  const std::vector<T>& vals,
                                  bool withIndices = false);

  ///@brief Returns a string representing the given vector of elements.
  ///
  ///@param vals The given vector of elements.
  ///@param withIndices Whether to include indices in string.
  template <typename T>
  static std::string toString(const std::vector<T>& vals,
                              bool withIndices = false);

  ///@brief Prints a complex array according to the print options.
  ///
  ///@param out Output stream
  ///@param vals Values to print
  static void printComplexArray(std::ostream& out,
                                const std::vector<std::complex<double>>& vals);

  ///@brief Prints a complex array, multiplied by a factor, according to the
  /// print options.
  ///
  ///@param out Output stream
  ///@param vals Values to print
  ///@param factor Scaling factor.
  static void printFactoredComplexArray(
      std::ostream& out,
      const std::vector<std::complex<double>>& vals,
      double factor = 1);

  ///@brief Returns a verbosity one notch lower than input, if exists.
  ///
  ///@param val Input verbosity
  static Verbosity lowerVerbosity(Verbosity val);

  ///@brief Backward compatability. Do not use unless required.
  /// Returns old code for verbosity.
  ///@param val Input verbosity
  static int verbosityToOldCode(Verbosity val);

  ///@brief Prints general statistics on an array of doubles.
  ///
  ///@param title Title for print
  ///@param vals Array of doubles
  static void printDoubleArrayProfile(std::ostream& out,
                                      const std::string& title,
                                      const std::vector<double>& vals);

  ///@brief Backward compatability. Do not use unless required.
  /// Returns new verbosity from old code
  ///@param val Input verbosity code
  static Verbosity verbosityFromOldCode(int val);
};

/// Returns global print options.
/// Forward decleration from HelayersConfig
PrintOptions& getPrintOptions();

template <typename T>
void PrintUtils::printArray(std::ostream& out,
                            const std::vector<T>& vals,
                            bool withIndices)
{
  const PrintOptions& po = getPrintOptions();
  int num = po.numPrintedElements;
  int num2 = po.numPrintedLastElements;
  for (int i = 0; i < std::min(num, (int)vals.size()); ++i) {
    if (i > 0)
      out << " ";
    if (withIndices)
      out << " [" << i << "]=";
    out << vals[i];
  }
  int j = std::max((int)(vals.size() - num2), num);
  if (j - num > 0)
    out << " ... ";
  for (; j < (int)vals.size(); ++j) {
    if (j > 0)
      out << " ";
    if (withIndices)
      out << " [" << j << "]=";
    out << vals[j];
  }
}

template <typename T>
void PrintUtils::printArrayWithTitle(std::ostream& out,
                                     const std::string title,
                                     const std::vector<T>& vals,
                                     bool withIndices)
{
  out << title << ":" << std::endl;
  printArray(out, vals, withIndices);
}

template <typename T>
std::string PrintUtils::toString(const std::vector<T>& vals, bool withIndices)
{
  std::ostringstream oss;
  printArray(oss, vals, withIndices);
  return oss.str();
}

} // namespace helayers

#endif /* SRC_HELAYERS_PRINTUTILS */
