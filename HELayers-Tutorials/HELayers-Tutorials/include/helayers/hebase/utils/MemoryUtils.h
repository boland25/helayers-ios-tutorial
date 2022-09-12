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

#ifndef SRC_HELAYERS_MEMORYUTILS_H
#define SRC_HELAYERS_MEMORYUTILS_H

#include <fstream>

namespace helayers {

///@brief Utilities for memory related operations
class MemoryUtils
{
  ///@brief Extracts the value of the given \p field from the given memory info
  /// \p file.
  ///@param file  The memory info file.
  ///@param field The filed to extract.
  static int extractMemInfoField(std::ifstream& file, const std::string& field);

public:
  ///@brief Returns how much RAM has been used by the calling process, in MB.
  /// Returns -1 if reading the amount of available memory fails. Such failure
  /// may happen, for example, when running on an unsupported operating system.
  static int getUsedRam();

  ///@brief Returns an estimate for how much memory is available for usage by
  /// the calling process, in MB. Returns -1 if reading the amount of available
  /// memory fails. Such failure may happen, for example, when running on an
  /// unsupported operating system.
  static int getAvailableMemory();
};
} // namespace helayers

#endif
