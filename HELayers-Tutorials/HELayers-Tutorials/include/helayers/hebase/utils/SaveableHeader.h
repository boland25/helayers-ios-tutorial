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

#ifndef SRC_HELAYERS_SAVEABLE_HEADER_H
#define SRC_HELAYERS_SAVEABLE_HEADER_H

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "Printable.h"
#include "SaveableBasic.h"
#include "helayers/hebase/version.h"

namespace helayers {

/// A struct representing a header of a saved object. It contains metadata
/// corresponding to the Saveable object (such as the helayers version it was
/// saved with).
struct SaveableHeader
{
  /// A magic number of helayers
  uint32_t magicNumber;

  /// The helayers version the object was saved with
  uint32_t helayersVersion_;

  /// The class name of the saved object
  std::string className;

  /// The id of the context the saved object was initialized with
  int32_t contextId;

  /// The maximum allowed class name length
  const int MAX_CLASS_NAME_LENGTH = 100;

  ///@brief Construct a new empty SaveableHeader object
  SaveableHeader() {}

  ///@brief Construct a SaveableHeader and fills its fields
  ///
  ///@param className Class name
  ///@param contextId Context id.
  SaveableHeader(const std::string& className, int32_t contextId);

  ///@brief Construct a new SaveableHeader object loaded from stream
  ///
  ///@param in Input stream
  SaveableHeader(std::istream& in);

  ///@brief Reports an error which happened because this object was saved with
  /// an unsupported version of helayers.
  ///@throw invalid_argument
  void reportVersionError() const;

  ///@brief Reports an error which happened because a loaded object header
  /// contains a
  /// wrong magic number.
  ///@throw runtime_error
  void reportMagicNumberError() const;

  ///@brief Reports an error happened due to an unexpected className field of
  /// a loaded object header.
  ///
  ///@param expectedClassName The expected className.
  ///@throw runtime_error
  void reportClassNameError(const std::string& expectedClassName) const;

  ///@brief Reports an error happened due to an unexpected contextId field of
  /// a loaded object header.
  ///
  ///@param expectedContextId The expected contextId.
  ///@throw runtime_error
  void reportContextIdError(int32_t expectedContextId) const;

  ///@brief Loads this header from the given stream.
  ///
  ///@param stream The stream to load from.
  void load(std::istream& stream);

  ///@brief Saves this header to the given stream.
  ///
  ///@param stream The stream to save to.
  void save(std::ostream& stream) const;

  ///@brief Checks whether the helayers version field of this SaveableHeader
  /// specifies a supported version.
  bool isVersionSupported() const;

  ///@brief Returns the helayers version object stored in this header
  HelayersVersion getHelayersVersion() const;

  ///@brief Verifies that the magic data and version metadata are valid.
  ///
  /// @throw runtime_error If the tests fail.
  void verifyMagicNumberAndVersion() const;
};

} // namespace helayers

#endif
