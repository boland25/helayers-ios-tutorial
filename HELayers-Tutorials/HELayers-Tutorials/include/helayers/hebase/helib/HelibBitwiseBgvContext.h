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

#ifndef HELIBBITWISEBGVCONTEXT_H_
#define HELIBBITWISEBGVCONTEXT_H_

#include "HelibBitwiseContext.h"
#include "../AlwaysAssert.h"

namespace helayers {

/// HeContext used to initilize classes working on bitwise ciphertexts under
/// Helib's library and BGV scheme. This is a beta, not tested version.
/// Currently there is no support for bootstrapping.
class HelibBitwiseBgvContext : public HelibBitwiseContext
{
  /// The EncryptedArray used in the underlying helib BGV scheme.
  const helib::EncryptedArray* ea = nullptr;
  /// The unpackSlotEncoding element used in the underlying helib BGV scheme.
  std::vector<helib::zzX> unpackSlotEncoding;

  void loadImpl(std::istream& in) override;

protected:
  /// A helper function for init() method
  void initCommon();

public:
  /// @brief A constructor.
  HelibBitwiseBgvContext() {}

  /// @brief A constructor.
  /// @param numBits The number of bits held by ciphertexts created by this
  ///                 context.
  HelibBitwiseBgvContext(int numBits) : HelibBitwiseContext(numBits) {}
  virtual ~HelibBitwiseBgvContext() {}

  /// Initializes the underlying helib BGV scheme with the given "m", "r" and
  /// "l". 2 used as the modolus of the BGV scheme.
  /// @param m Cyclotomic polynomial, defines phi(m).
  /// @param r Hensel lifting.
  /// @param L Number of bits of the modulus chain.
  /// @param c Number of columns of Key-Switching matix.
  /// @param[in] withBootStrapping Whether the initialized BGV scheme should
  ///                              enable bootstrapping.
  void init(unsigned long m, unsigned long r, unsigned long L, unsigned long c);

  /// Initializes context with given configuration
  ///
  ///@param[in] conf user's configuration
  void init(const HelibConfig& conf) override;

  /// @brief Initializes this HelibBitwiseBgvContext such that the ciphertexts
  /// created with it will be bootstrapable.
  /// @param m    Cyclotomic polynomial, defines phi(m).
  /// @param r    Hensel lifting.
  /// @param L    Number of bits of the modulus chain.
  /// @param c    Number of columns of Key-Switching matix.
  /// @param mvec Factorisation of m.
  /// @param gens Generating set of Zm* group.
  /// @param ords Orders of the generators in gens vector.
  void initWithBootStrapping(unsigned long m,
                             unsigned long r,
                             unsigned long L,
                             unsigned long c,
                             const std::vector<long>& mvec,
                             const std::vector<long>& gens,
                             const std::vector<long>& ords);

  /// Returns a pointer to HelibBitwseBgvCiphertext, initialized with this
  /// HelibBitwseBgvCiphertext.
  std::shared_ptr<AbstractCiphertext> createAbstractCipher() const override;

  /// Returns a pointer to HelibBitwseBgvCiphertext, initialized with this
  /// HelibBitwseBgvCiphertext.
  std::shared_ptr<AbstractPlaintext> createAbstractPlain() const override;

  /// Returns a pointer to HelibBitwseBgvCiphertext, initialized with this
  /// HelibBitwseBgvCiphertext.
  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  /// Returns a pointer to HelibBitwseBgvCiphertext, initialized with this
  /// HelibBitwseBgvCiphertext.
  std::shared_ptr<AbstractBitwiseEvaluator> getBitwiseEvaluator()
      const override;

  /// Returns a pointer to HelibBitwseBgvCiphertext, initialized with this
  /// HelibBitwseBgvCiphertext and the given scale.
  /// @param[in] scale The default scale for ciphertexts and plaintexts created
  ///                  with the returned AbstractEncoder.
  std::shared_ptr<AbstractEncoder> getEncoder(int scale);

  /// Prints a message summarizing the configuration of this
  /// HelibBitwseBgvCiphertext.
  virtual void printSignature(std::ostream& out) const override;

  /// Returns the EncryptedArray used to perform operation on ciphertexts and
  /// plaintexts that are initialized with this scheme.
  virtual inline const helib::EncryptedArray& getEncryptedArray() const
  {
    return *ea;
  }

  /// Returns the name of this scheme (i.e. "BitwiseBGV").
  std::string getSchemeName() const override { return "BitwiseBGV"; }

  /// See parent method
  std::shared_ptr<HeContext> clone() const override;
};
} // namespace helayers

#endif