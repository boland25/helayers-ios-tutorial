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

#ifndef SRC_HELAYERS_MOCKUPCIPHER_H_
#define SRC_HELAYERS_MOCKUPCIPHER_H_

#include "MockupContext.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "MockupPlaintext.h"

namespace helayers {

class MockupCiphertext : public AbstractCiphertext
{
  std::vector<std::complex<long double>> rawVals;

  int chainIndex = -1;

  double scale;

  const MockupContext& h;

  void validateCompatibility(const MockupCiphertext& other) const;
  void validateCompatibility(const MockupPlaintext& other) const;

  MockupCiphertext(const MockupCiphertext& src) = default;

  std::shared_ptr<AbstractCiphertext> doClone() const override;

  friend class MockupEncoder;
  friend class MockupBootstrapEvaluator;

public:
  MockupCiphertext(const MockupContext& h);
  ~MockupCiphertext() {}

  inline std::vector<std::complex<long double>>& getRawVals()
  {
    return rawVals;
  }
  inline const std::vector<std::complex<long double>>& getRawVals() const
  {
    return rawVals;
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  std::shared_ptr<MockupCiphertext> clone() const
  {
    return std::static_pointer_cast<MockupCiphertext>(doClone());
  }

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

  void setScale(double s) override { scale = s; }

  double getScale() const override { return scale; }

  void negate() override;

  void conjugate() override;

  void conjugateRaw() override;

  void multiplyByChangingScale(double factor) override;

  void multiplyScalar(int scalar) override;

  // rotate left
  void rotate(int n) override;

  void innerSum(int rot1, int rot2, bool reverse = false) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  inline int slotCount() const override { return h.slotCount(); }

  bool isEmpty() const override { return (rawVals.size() == 0); }
};
} // namespace helayers

#endif
