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

#ifndef SRC_HELAYERS_HELIBBGVCONTEXT_H_
#define SRC_HELAYERS_HELIBBGVCONTEXT_H_

#include "HelibContext.h"

namespace helayers {

/// An implementation of HeContext for BGV scheme in HElib.
///
/// It can be either initialized via parameters, via an HelibConfig,
/// or loaded from a file.
///
/// It is recommended not to use directly after initialization, but
/// use an HeContext reference instead.
///
/// It is currently in beta version, as not all BGV operators are covered.
class HelibBgvContext : public HelibContext
{

  /// The prime modulos.
  unsigned long p;

  /// The EncryptedArray used in the underlying helib BGV scheme.
  const helib::EncryptedArray* ea = nullptr;

  // double defaultScale = 100;

  /// Loads this HelibBgvcontext from the given istream.
  /// @param in The istream to load from.
  void loadImpl(std::istream& in) override;

protected:
  /// A helper function for init() method
  void initCommon();

public:
  HelibBgvContext();
  virtual ~HelibBgvContext();

  /// @brief Initializes this HelibBgvContext with the given parameters.
  /// @param p The prime modulos.
  /// @param m Cyclotomic polynomial, defines phi(m).
  /// @param r Hensel lifting.
  /// @param L Number of bits of the modulus chain.
  void init(unsigned long p, unsigned long m, unsigned long r, unsigned long L);

  /// Initializes context with given configuration
  /// @param[in] conf user configuration
  void init(const HelibConfig& conf) override;

  /// Returns a pointer to HelibBgvCiphertext, initialized with this
  /// HelibBgvContext.
  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher()
      const override;

  /// Returns a pointer to HelibBgvPlaintext, initialized with this
  /// HelibBgvContext.
  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain()
      const override;

  /// Returns a pointer to HelibBgvNativeFunctionEvaluator, initialized with
  /// this HelibBgvContext.
  std::shared_ptr<AbstractFunctionEvaluator> getFunctionEvaluator() override;

  int getMinFeasibleNumSlots(const HeConfigRequirement& req) const override
  {
    throw std::runtime_error("not implemented");
  }

  /// Returns a pointer to HelibBgvEncoder, initialized with this
  /// HelibBgvContext.
  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  /// Returns the EncryptedArray used to perform operation on ciphertexts and
  /// plaintexts that are initialized with this scheme.
  virtual inline const helib::EncryptedArray& getEncryptedArray() const
  {
    return *ea;
  }

  /// Returns the name of this scheme (i.e. "BGV").
  std::string getSchemeName() const override { return "BGV"; }

  // void setDefaultScale(double val) { defaultScale = val; }
  // inline double getDefaultScale() const { return defaultScale; }

  /// See parent method
  std::shared_ptr<HeContext> clone() const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_HELIBBGVCONTEXT_H_ */
