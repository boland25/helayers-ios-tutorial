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

#ifndef SRC_HELAYERS_HEAANCIPHER_H_
#define SRC_HELAYERS_HEAANCIPHER_H_

#include "HEaaN/Ciphertext.hpp"
#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "HeaanContext.h"
#include "HeaanPlaintext.h"

namespace helayers {

class HeaanCiphertext : public AbstractCiphertext
{
  const HeaanContext& he;

  std::shared_ptr<HEaaN::Ciphertext> ciphertext = nullptr;

  std::shared_ptr<HEaaN::Iphertext> iphertext = nullptr;

  double currentScale = 0;

  std::shared_ptr<AbstractCiphertext> doClone() const override;

  // helper functions
  std::shared_ptr<const HEaaN::Ciphertext> getRelinRescaledOther(
      std::shared_ptr<HEaaN::Ciphertext> cipher,
      std::shared_ptr<HEaaN::Iphertext> ipher) const;

  std::shared_ptr<const HEaaN::Ciphertext> getRelinOther(
      std::shared_ptr<HEaaN::Ciphertext> cipher,
      std::shared_ptr<HEaaN::Iphertext> ipher) const;

  std::shared_ptr<const HEaaN::Ciphertext> verifyMatchingRescale(
      std::shared_ptr<const HEaaN::Ciphertext> other);

  void verifyMatchingDevice(const HeaanCiphertext& other) const;

  void verifyMatchingDevice(const HeaanPlaintext& other) const;

  void verifyRelinRescaled();

  void verifyRelinearized();

  friend class HeaanEncoder;

  friend class HeaanBootstrapEvaluator;

public:
  HeaanCiphertext(const HeaanContext& h) : AbstractCiphertext(h), he(h) {}

  HeaanCiphertext(const HeaanCiphertext& src);

  ~HeaanCiphertext() override;

  std::shared_ptr<HeaanCiphertext> clone() const
  {
    return std::static_pointer_cast<HeaanCiphertext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void conjugate() override;

  void conjugateRaw() override;

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

  void multiplyScalar(int scalar) override;

  void multiplyScalar(double scalar) override;

  void relinearize() override;

  void rescale() override;

  void rescaleRaw() override;

  void negate() override;

  void toDevice(DeviceType deviceType) override;

  DeviceType getCurrentDevice() const override;

  void setScale(double scale) override;

  double getScale() const override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  void remod(int chainIndex = -1) override;

  int slotCount() const override;

  bool isEmpty() const override;

  bool isTransparent() const override;

  void encryptZeroIfTransparent() override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_HEAANCIPHER_H_ */
