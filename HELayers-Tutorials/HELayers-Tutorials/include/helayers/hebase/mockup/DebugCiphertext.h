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

#ifndef SRC_HELAYERS_DEBUGCIPHERTEXT_H_
#define SRC_HELAYERS_DEBUGCIPHERTEXT_H_

#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "DebugContext.h"

namespace helayers {

class DebugCiphertext : public AbstractCiphertext
{

  const DebugContext& he;

  std::shared_ptr<AbstractCiphertext> c1;
  std::shared_ptr<AbstractCiphertext> c2;

  std::string log;

  DebugCiphertext(const DebugCiphertext& src);

  std::shared_ptr<AbstractCiphertext> doClone() const override;

  ///@brief Records that the given action took place, and does some verification
  /// checks. The exact actions taken depend on the context's configurations.
  ///@param operation Operation name
  void verifyAndLog(const std::string& operation);

  ///@brief Asserts that the two internal copies are similar.
  ///@param title Title in case prints are issued
  void assertSimilar(const std::string& title) const;

  ///@brief Adds operation name to the internal log
  ///
  ///@param operation Operaton name
  void addToLog(const std::string& operation);

  friend class DebugEncoder;

public:
  DebugCiphertext(const DebugContext& h)
      : AbstractCiphertext(h),
        he(h),
        c1(h.h1.createAbstractCipher()),
        c2(h.h2.createAbstractCipher())
  {}

  ~DebugCiphertext() override;

  std::shared_ptr<DebugCiphertext> clone() const
  {
    return std::static_pointer_cast<DebugCiphertext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void conjugate() override;

  void conjugateRaw() override;
  // rotate right
  void rotate(int n) override;

  void add(const AbstractCiphertext& other) override;

  void addRaw(const AbstractCiphertext& other) override;

  void sub(const AbstractCiphertext& other) override;

  void subRaw(const AbstractCiphertext& other) override;

  void multiply(const AbstractCiphertext& other) override;

  void multiplyRaw(const AbstractCiphertext& other) override;

  void addPlain(const AbstractPlaintext& plain) override;

  void addPlainRaw(const AbstractPlaintext& plain) override;

  void subPlain(const AbstractPlaintext& plain) override;

  void subPlainRaw(const AbstractPlaintext& plain) override;

  void multiplyPlain(const AbstractPlaintext& plain) override;

  void multiplyPlainRaw(const AbstractPlaintext& plain) override;

  void square() override;

  void squareRaw() override;

  void relinearize() override;

  void rescale() override;

  void rescaleRaw() override;

  void negate() override;

  void setScale(double scale) override;

  double getScale() const override;

  void multiplyByChangingScale(double factor) override;

  void addScalar(int scalar) override;

  void addScalar(double scalar) override;

  void multiplyScalar(int scalar) override;

  void multiplyScalar(double scalar) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  ///@brief Set the chain index higher than the current level
  ///@param chainIndex the chainIndex to "jump" up to.
  ///       -1 implies - jump as high as possible
  ///       This function is only useful as part of the CKKS bootstrap
  ///       process, since it changes the underlying plaintext -
  ///       it adds to every plaintext coefficient a multiple of the 1st prime
  ///       in the prime chain.
  ///       The Bootstrap then fixes this by removing the added multiples.
  void remod(int chainIndex = -1) override;

  int slotCount() const override;

  bool isEmpty() const override;

  bool isTransparent() const override;

  void copyFirstToSecond();

  void copySecondToFirst();

  ///@brief Get the mean absolute error between the slots of the two ciphertexts
  double getMeanAbsoluteError() const;

  virtual void debugPrint(const std::string& title = "",
                          Verbosity verbosity = VERBOSITY_REGULAR,
                          std::ostream& out = std::cout) const override;

  inline AbstractCiphertext& getFirst() const { return *c1; }
  inline AbstractCiphertext& getSecond() const { return *c2; }
};
} // namespace helayers

#endif /* SRC_HELAYERS_DEBUGCIPHERTEXT_H_ */
