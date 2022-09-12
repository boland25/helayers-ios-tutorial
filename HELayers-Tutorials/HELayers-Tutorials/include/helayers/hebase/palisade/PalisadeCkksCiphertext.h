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

#ifndef SRC_HELAYERS_PalisadeCKKSCIPHER_H_
#define SRC_HELAYERS_PalisadeCKKSCIPHER_H_

#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "PalisadeCkksContext.h"

namespace helayers {

class PalisadeCkksCiphertext : public AbstractCiphertext
{
private:
  const PalisadeCkksContext& he;

  lbcrypto::Ciphertext<lbcrypto::DCRTPoly> encrypted;

  PalisadeCkksCiphertext(const PalisadeCkksCiphertext& src) = default;

  std::shared_ptr<AbstractCiphertext> doClone() const override;

  void multiplyMakeCtxtPtxtCompatible(
      lbcrypto::Ciphertext<lbcrypto::DCRTPoly>& ciphertext,
      lbcrypto::Plaintext& plaintext) const;

  friend class PalisadeCkksEncoder;

public:
  PalisadeCkksCiphertext(const PalisadeCkksContext& h)
      : AbstractCiphertext(h), he(h)
  {}

  ~PalisadeCkksCiphertext() override;

  std::shared_ptr<PalisadeCkksCiphertext> clone() const
  {
    return std::static_pointer_cast<PalisadeCkksCiphertext>(doClone());
  }

  void add(const AbstractCiphertext& other) override;

  void sub(const AbstractCiphertext& other) override;

  void multiply(const AbstractCiphertext& other) override;

  void addRaw(const AbstractCiphertext& other) override;

  void subRaw(const AbstractCiphertext& other) override;

  void multiplyRaw(const AbstractCiphertext& other) override;

  void addPlain(const AbstractPlaintext& plain) override;

  void addPlainRaw(const AbstractPlaintext& plain) override;

  void subPlain(const AbstractPlaintext& plain) override;

  void subPlainRaw(const AbstractPlaintext& plain) override;

  void multiplyPlain(const AbstractPlaintext& plain) override;

  void multiplyPlainRaw(const AbstractPlaintext& plain) override;

  void addScalar(int scalar) override;

  void addScalar(double scalar) override;

  void multiplyScalar(int scalar) override;

  void multiplyScalar(double scalar) override;

  void squareRaw() override;

  void square() override;

  void relinearize() override;

  void rescale() override;

  void rescaleRaw() override;

  void setScale(double val) override;

  double getScale() const override;

  void conjugate() override;

  void conjugateRaw() override;

  // rotate right
  void rotate(int n) override;

  void negate() override;

  int slotCount() const override { return he.slotCount(); }

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  bool isEmpty() const override;

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_PalisadeCKKSCIPHER_H_ */
