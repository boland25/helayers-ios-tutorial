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

#ifndef PALISADETFHEBITWISEBGVEVALUATOR_H
#define PALISADETFHEBITWISEBGVEVALUATOR_H

#include "helayers/hebase/impl/AbstractBitwiseEvaluator.h"
#include "PalisadeTfheContext.h"

namespace helayers {
class PalisadeTfheEvaluator : public AbstractBitwiseEvaluator
{
private:
  const PalisadeTfheContext& h;

public:
  PalisadeTfheEvaluator(const PalisadeTfheContext& h);
  ~PalisadeTfheEvaluator() {}

  std::shared_ptr<AbstractCiphertext> bitwiseOr(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> isGreater(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> isLess(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> bitwiseXor(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> bitwiseAnd(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const;

  std::shared_ptr<AbstractCiphertext> bitwiseNor(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const;

  std::shared_ptr<AbstractCiphertext> bitwiseNand(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const;

  std::shared_ptr<AbstractCiphertext> bitwiseXnor(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> getMSB(
      const AbstractCiphertext& c) const override
  {
    throw std::runtime_error("getMSB not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> getFlippedMSB(
      const AbstractCiphertext& c) const override
  {
    throw std::runtime_error("getFlippedMSB not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> hamming(const AbstractCiphertext& c,
                                              int from = 0,
                                              int to = -1) const override
  {
    throw std::runtime_error("hamming not yet implemented");
  }

  void setIsSigned(AbstractCiphertext& c, bool val) const override
  {
    throw std::runtime_error("setIsSigned not yet implemented");
  }

  bool getIsSigned(const AbstractCiphertext& c) const override
  {
    throw std::runtime_error("getIsSigned not yet implemented");
  }

  std::vector<std::shared_ptr<AbstractCiphertext>> split(
      const AbstractCiphertext& c) const override
  {
    throw std::runtime_error("split not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> combine(
      const std::vector<std::shared_ptr<AbstractCiphertext>>& cs,
      int from = 0,
      int to = -1,
      int bitsPerElement = 1) const override
  {
    throw std::runtime_error("combine not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> isEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override
  {
    throw std::runtime_error("isEqual not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> multiply(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2,
      int targetBits = -1) const override
  {
    throw std::runtime_error("multiply not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> add(const AbstractCiphertext& c1,
                                          const AbstractCiphertext& c2,
                                          int targetBits = -1) const override
  {
    throw std::runtime_error("add not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> sub(const AbstractCiphertext& c1,
                                          const AbstractCiphertext& c2,
                                          int targetBits = -1) const override
  {
    throw std::runtime_error("sub not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> multiplyBit(
      const AbstractCiphertext& c,
      const AbstractCiphertext& bit) const override
  {
    throw std::runtime_error("multiplyBit not yet implemented");
  }

  int getNumBits(const AbstractCiphertext& c) const override
  {
    throw std::runtime_error("getNumBits not yet implemented");
  }

  void setNumBits(AbstractCiphertext& c, int bits) const override
  {
    throw std::runtime_error("setNumBits not yet implemented");
  }

  int getDefaultNumBits() const override
  {
    throw std::runtime_error("getDefaultNumBits not yet implemented");
  }

  void debugPrintWithBinary(const AbstractCiphertext& c,
                            const std::string& title,
                            int maxElements = 4,
                            int verbose = 0,
                            std::ostream& out = std::cout) const override
  {
    throw std::runtime_error("debugPrintWithBinary not yet implemented");
  }

  double getScale(const AbstractCiphertext& c) const override
  {
    throw std::runtime_error("getScale not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> max(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override
  {
    throw std::runtime_error("max not yet implemented");
  }
  std::shared_ptr<AbstractCiphertext> min(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override
  {
    throw std::runtime_error("min not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> isGreaterEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override
  {
    throw std::runtime_error("isGreaterEqual not yet implemented");
  }

  std::shared_ptr<AbstractCiphertext> isLessEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override
  {
    throw std::runtime_error("isLessEqual not yet implemented");
  }
};
} // namespace helayers
#endif /* PALISADETFHEBITWISEBGVEVALUATOR_H */
