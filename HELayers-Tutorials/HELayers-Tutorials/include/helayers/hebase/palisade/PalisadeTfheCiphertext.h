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

#ifndef SRC_HELAYERS_PalisadeTFHECIPHER_H_
#define SRC_HELAYERS_PalisadeTFHECIPHER_H_

#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "PalisadeTfheContext.h"
#include "binfhecontext.h"

namespace helayers {

class PalisadeTfheCiphertext : public AbstractCiphertext
{
private:
  const PalisadeTfheContext& he;

  shared_ptr<std::vector<lbcrypto::LWECiphertext>> encrypted;

  shared_ptr<AbstractCiphertext> doClone() const override;

  friend class PalisadeTfheEncoder;

  static lbcrypto::LWECiphertext mux(const lbcrypto::LWECiphertext& a,
                                     const lbcrypto::LWECiphertext& x,
                                     const lbcrypto::LWECiphertext& y,
                                     const PalisadeTfheContext& he);

  static lbcrypto::LWECiphertext compareBit(
      const lbcrypto::LWECiphertext& a,
      const lbcrypto::LWECiphertext& b,
      const lbcrypto::LWECiphertext& lsbCarry,
      bool isFirstGreaterThanSecond,
      const PalisadeTfheContext& he);

  shared_ptr<PalisadeTfheCiphertext> isGreaterOrSmallerThan(
      const shared_ptr<PalisadeTfheCiphertext> other,
      bool isFirstGreaterThanSecond) const;

  shared_ptr<PalisadeTfheCiphertext> getBitwiseOperation(
      const lbcrypto::BINGATE gate,
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  void bitwiseOperation(const lbcrypto::BINGATE gate,
                        const PalisadeTfheCiphertext other);

public:
  PalisadeTfheCiphertext(const PalisadeTfheContext& h);

  PalisadeTfheCiphertext(const PalisadeTfheContext& h,
                         std::vector<lbcrypto::LWECiphertext>& bits);

  PalisadeTfheCiphertext(const PalisadeTfheCiphertext& src);

  ~PalisadeTfheCiphertext() override;
  void negate() override;

  shared_ptr<PalisadeTfheCiphertext> isGreaterThan(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  shared_ptr<PalisadeTfheCiphertext> isLessThan(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  shared_ptr<PalisadeTfheCiphertext> getBitwiseOr(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  shared_ptr<PalisadeTfheCiphertext> getBitwiseAnd(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  shared_ptr<PalisadeTfheCiphertext> getBitwiseXor(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  shared_ptr<PalisadeTfheCiphertext> getBitwiseNor(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  shared_ptr<PalisadeTfheCiphertext> getBitwiseNand(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  shared_ptr<PalisadeTfheCiphertext> getBitwiseXnor(
      const shared_ptr<PalisadeTfheCiphertext> other) const;

  int getChainIndex() const override;

  void add(const AbstractCiphertext& other) override { addRaw(other); }

  void sub(const AbstractCiphertext& other) override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::sub not implemented");
  }

  void multiply(const AbstractCiphertext& other) override
  {
    multiplyRaw(other);
  }

  void addRaw(const AbstractCiphertext& other) override;

  void subRaw(const AbstractCiphertext& other) override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::subRaw not implemented");
  }

  void multiplyRaw(const AbstractCiphertext& other) override;

  void addPlain(const AbstractPlaintext& plain) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::addPlain not implemented");
  }

  void addPlainRaw(const AbstractPlaintext& plain) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::addPlainRaw not implemented");
  }

  void subPlain(const AbstractPlaintext& plain) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::subPlain not implemented");
  }

  void subPlainRaw(const AbstractPlaintext& plain) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::subPlainRaw not implemented");
  }

  void multiplyPlain(const AbstractPlaintext& plain) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::multiplyPlain not implemented");
  }

  void multiplyPlainRaw(const AbstractPlaintext& plain) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::multiplyPlainRaw not implemented");
  }

  void addScalar(int scalar) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::addScalar not implemented");
  }

  void addScalar(double scalar) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::addScalar not implemented");
  }

  void multiplyScalar(int scalar) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::multiplyScalar not implemented");
  }

  void multiplyScalar(double scalar) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::multiplyScalar not implemented");
  }

  void squareRaw() override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::squareRaw not implemented");
  }

  void square() override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::square not implemented");
  }

  void relinearize() override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::relinearize not implemented");
  }

  void rescale() override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::rescale not implemented");
  }

  void rescaleRaw() override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::rescaleRaw not implemented");
  }

  void setScale(double val) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::setScale not implemented");
  }

  double getScale() const override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::getScale not implemented");
  }

  void conjugate() override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::conjugate not implemented");
  }

  void conjugateRaw() override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::conjugateRaw not implemented");
  }

  void rotate(int n) override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::rotate not implemented");
  }

  int slotCount() const override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::slotCount not implemented");
  }

  void setChainIndex(int chainIndex) override
  {
    throw std::runtime_error(
        "PalisadeTfheCiphertext::setChainIndex not implemented");
  }

  bool isEmpty() const override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::isEmpty not implemented");
  }

  std::streamoff save(std::ostream& stream) const override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::save not implemented");
  }

  std::streamoff load(std::istream& stream) override
  {
    throw std::runtime_error("PalisadeTfheCiphertext::load not implemented");
  }
};

} // namespace helayers

#endif /* SRC_HELAYERS_PalisadeTFHECIPHER_H_ */
