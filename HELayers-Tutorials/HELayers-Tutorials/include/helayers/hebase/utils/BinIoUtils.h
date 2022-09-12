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

#ifndef SRC_HELAYERS_BINIOUTILS_H
#define SRC_HELAYERS_BINIOUTILS_H

#include <iostream>
#include <random>
#include <string>
#include "helayers/hebase/Types.h"

namespace helayers {

/// Utlities for IO operations on binary streams
class BinIoUtils
{

public:
  static const int64_t KILOBYTE = 1024;
  static const int64_t MEGABYTE = 1024 * 1024;
  static const int64_t GIGABYTE = 1024 * 1024 * 1024;

  /// See docs for design notes about this class

  ///@brief Writes a string to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The string to write
  static void writeString(std::ostream& out, const std::string& val);

  ///@brief Writes a double to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The double to write
  static void writeDouble(std::ostream& out, double val);

  ///@brief Writes a vector of doubles to a binary stream
  ///
  ///@param out Stream to write to
  ///@param vals The vector of doubles to write
  static void writeDoubleVector(std::ostream& out, std::vector<double> vals);

  ///@brief Writes a vector of DimInt to a binary stream
  ///
  ///@param out Stream to write to
  ///@param vals The vector of DimInt to write
  static void writeDimIntVector(std::ostream& out, std::vector<DimInt> vals);

  ///@brief Writes a vector of strings to a binary stream
  ///
  ///@param out Stream to write to
  ///@param vals The vector of strings to write
  static void writeStringVector(std::ostream& out,
                                std::vector<std::string> vals);

  ///@brief Writes a long double to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The long double to write
  static void writeLongDouble(std::ostream& out, long double val);

  ///@brief Writes an int to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The int to write
  static void writeInt(std::ostream& out, int val);

  ///@brief Writes a size_t to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The size_t to write
  static void writeSizeT(std::ostream& out, size_t val);

  ///@brief Writes a bool to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The bool to write
  static void writeBool(std::ostream& out, bool val);

  static void writeUint32(std::ostream& out, uint32_t val);

  static void writeUint64(std::ostream& out, uint64_t val);

  static void writeInt32(std::ostream& out, int32_t val);

  static void writeInt16(std::ostream& out, int16_t val);

  ///@brief Writes a std::mt19937::result_type to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The result_type to write
  static void writeResultType(std::ostream& out, std::mt19937::result_type val);

  ///@brief Writes a DimInt to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The DimInt to write
  static void writeDimInt(std::ostream& out, DimInt val);

  ///@brief Writes an enum to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The enum to write
  template <typename T>
  static void writeEnum(std::ostream& out, T val)
  {
    writeInt(out, static_cast<int>(val));
  }

  ///@brief Returns a string read from a binary stream
  ///
  ///@param in        Stream to read from
  ///@param maxLength The maximum length of the string to read. (default: 10K)
  /// A value of -1 indicates the maximal size allowed.
  static std::string readString(std::istream& in,
                                int64_t maxLength = 10 * KILOBYTE);

  ///@brief Returns a double read from a binary stream
  ///
  ///@param in Stream to read from
  static double readDouble(std::istream& in);

  ///@brief Returns a vector of doubles read from a binary stream
  ///
  ///@param in Stream to read from
  static std::vector<double> readDoubleVector(std::istream& in);

  ///@brief Returns a vector of DimInt values read from a binary stream
  ///
  ///@param in Stream to read from
  static std::vector<DimInt> readDimIntVector(std::istream& in);

  ///@brief Returns a vector of strings read from a binary stream
  ///
  ///@param in Stream to read from
  ///@param maxNumElements The maximum number of strings allowed to be in the
  /// vector to read. (default: 100,000).
  ///@param maxElementLength The maximum length of each string in the vector to
  /// read. (default: 1K)
  static std::vector<std::string> readStringVector(
      std::istream& in,
      int64_t maxNumElements = 100000,
      int64_t maxElementLength = KILOBYTE);

  ///@brief Returns a long double read from a binary stream
  ///
  ///@param in Stream to read from
  static long double readLongDouble(std::istream& in);

  ///@brief Returns an int read from a binary stream
  ///
  ///@param in Stream to read from
  static int readInt(std::istream& in);

  ///@brief Returns a size_t read from a binary stream
  ///
  ///@param in Stream to read from
  static size_t readSizeT(std::istream& in);

  static uint32_t readUint32(std::istream& in);

  static uint64_t readUint64(std::istream& in);

  static int32_t readInt32(std::istream& in);

  static int16_t readInt16(std::istream& in);

  ///@brief Returns a bool read from a binary stream
  ///
  ///@param in Stream to read from
  static bool readBool(std::istream& in);

  ///@brief Returns a DimInt read from a binary stream
  ///
  ///@param in Stream to read from
  static DimInt readDimInt(std::istream& in);

  ///@brief Returns a std::mt19937::result_type read from a binary stream
  ///
  ///@param in Stream to read from
  static std::mt19937::result_type readResultType(std::istream& in);

  ///@brief Returns an enum read from a binary stream
  ///
  ///@param in Stream to read from
  template <typename T>
  static T readEnum(std::istream& in)
  {
    return static_cast<T>(readInt(in));
  }
};
} // namespace helayers

#endif /* SRC_HELAYERS_BINIOUTILS_H */
