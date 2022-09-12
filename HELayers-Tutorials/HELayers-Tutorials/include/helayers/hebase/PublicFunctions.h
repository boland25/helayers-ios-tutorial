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

#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H

#include <iostream>
#include <vector>
#include <set>
#include "helayers/hebase/utils/SaveableBasic.h"

namespace helayers {

/// @brief A struct specifying the functionalities that should be supported by
/// the public keys of an HeContext

///@brief A type describing the set of rotations supported by an HeContext
enum RotationSetType
{
  /// Rotations are not supported
  NO_ROTATIONS,
  /// The default rotation set of the used HeContext is supported
  DEFAULT_ROTATIONS,
  /// A user-specified rotation set is supported
  CUSTOM_ROTATIONS
};

///@brief A type specifying whether conjugation is supported
enum ConjugationSupport
{
  /// Conjugation is not supported
  CONJUGATION_SUPPORT_FALSE,
  /// Conjugation support is set to the default option for the used library
  CONJUGATION_SUPPORT_DEFAULT,
  /// Conjugation is supported
  CONJUGATION_SUPPORT_TRUE
};

struct PublicFunctions : public SaveableBasic
{
  /// Whether the public keys should support encryption
  bool encrypt = true;

  /// Whether the public keys should support relinearization
  bool relinearize = true;

  /// Specifies the type of the supported rotation set. See also
  /// RotationSetType's documentation.
  RotationSetType rotate = DEFAULT_ROTATIONS;

  /// Whether the public keys should support conjugation. See also
  /// ConjugationSupport's documentation.
  ConjugationSupport conjugate = CONJUGATION_SUPPORT_DEFAULT;

  /// The set of rotation offsets that can be done with one step. If this vector
  /// is left empty, then positive and negative power of two rotation steps will
  /// be supported.
  std::vector<int> rotationSteps;

  ///@brief Checks whether this and other PublicFunctions objects support the
  /// same set of rotations.
  ///
  ///@param other The other PublicFunctions object.
  bool sameRotationSteps(const PublicFunctions& other) const;

  ///@brief Checks whether this PublicFunctions object is a subset of the given
  /// \p other PublicFunctions object. This is considered to be true if all of
  /// the functionalities that are supported according to this object are
  /// supported by the other object as well.
  bool subsetOf(const PublicFunctions& other) const;

  std::streamoff load(std::istream& in) override;

  std::streamoff save(std::ostream& out) const override;

  ///@brief Validates that the flags specifying the supported public functions
  /// are consistent. This validation fails, for example, if \a rotate flag is
  /// set to false but \a rotationSteps vector is not empty.
  ///@throw runtime_error If the validation fails.
  void validate() const;

  ///@brief Returns whether this PublicFunction object supports all
  /// functionalities and uses the default set of rotation steps.
  bool allFunctionsAndDefaultRotations() const;

  ///@brief Returns whether this PublicFunction object supports the default set
  /// of functionalities
  bool isDefault() const;

  ///@brief Turns off support for all functionalities
  void clear();
};

bool operator==(const PublicFunctions& pf1, const PublicFunctions& pf2);
bool operator!=(const PublicFunctions& pf1, const PublicFunctions& pf2);

} // namespace helayers

#endif
