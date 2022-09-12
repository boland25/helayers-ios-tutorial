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

#ifndef HELIBBITWISECONTEXT_H
#define HELIBBITWISECONTEXT_H

#include "HelibContext.h"

namespace helayers {

/// An abstract base class for helib bitwise schemes.
/// Beta, not tested, version.
class HelibBitwiseContext : public HelibContext
{
  /// The number of bits held by ciphertexts created with this
  /// HelibBitwiseContext.
  int numBits;

  /// @brief Configures the HeTraits object corresponding to this
  /// HelibBitwiseContext.
  void configureTraits();

protected:
  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;

  void loadImpl(std::istream& in) override;

public:
  /// @brief A constructor.
  HelibBitwiseContext() : HelibContext() { configureTraits(); }

  /// @brief A constructor.
  /// @param numBits The number of bits held by ciphertexts created with this
  /// HelibBitwiseContext.
  HelibBitwiseContext(int numBits) : HelibContext(), numBits(numBits)
  {
    configureTraits();
  }

  ~HelibBitwiseContext() {}

  /// Returns the default number of bits for a CTile created with this contexts.
  inline int getNumBits() const { return numBits; }

  /// Sets the default number of bits to n.
  /// @param[in] n The default number of bits.
  inline void setNumBits(int n) { numBits = n; }
};
} // namespace helayers

#endif