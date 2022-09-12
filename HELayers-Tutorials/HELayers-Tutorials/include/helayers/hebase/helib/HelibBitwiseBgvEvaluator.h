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

#ifndef HELIBBITWISEBGVEVALUATOR_H
#define HELIBBITWISEBGVEVALUATOR_H

#include "HelibBitwiseBgvCiphertext.h"
#include "HelibBitwiseBgvEncoder.h"
#include "HelibBitwiseBgvContext.h"
#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "helayers/hebase/impl/AbstractBitwiseEvaluator.h"
#include "helayers/hebase/HeContext.h"
#include "helib/helib.h"
#include "helib/Ctxt.h"
#include <iostream>
#include <vector>

namespace helayers {

class HelibBitwiseBgvEvaluator : public AbstractBitwiseEvaluator
{
  const HelibBitwiseBgvContext& h;
  std::shared_ptr<HelibBitwiseBgvEncoder> enc;

  void addBit(HelibBitwiseBgvCiphertext& c, const helib::Ctxt& bit) const;

  helib::Ctxt bitOr(const helib::Ctxt& x, const helib::Ctxt& y) const;
  helib::Ctxt bitIsEqual(const helib::Ctxt& x, const helib::Ctxt& y) const;
  helib::Ctxt bitIsLess(const helib::Ctxt& x, const helib::Ctxt& y) const;
  helib::Ctxt bitNot(const helib::Ctxt& x) const;

  std::vector<helib::Ctxt> hamming(const std::vector<helib::Ctxt>& c,
                                   int from = 0,
                                   int to = -1) const;

public:
  HelibBitwiseBgvEvaluator(const HelibBitwiseBgvContext& h);
  ~HelibBitwiseBgvEvaluator() {}

  std::shared_ptr<AbstractCiphertext> getMSB(
      const AbstractCiphertext& c) const override;

  std::shared_ptr<AbstractCiphertext> getFlippedMSB(
      const AbstractCiphertext& c) const override;

  void setIsSigned(AbstractCiphertext& c, bool val) const override;

  bool getIsSigned(const AbstractCiphertext& c) const override;

  // AbstractCiphertext abs(const AbstractCiphertext &c) const;

  std::shared_ptr<AbstractCiphertext> hamming(const AbstractCiphertext& c,
                                              int from = 0,
                                              int to = -1) const override;

  std::vector<std::shared_ptr<AbstractCiphertext>> split(
      const AbstractCiphertext& c) const override;

  std::shared_ptr<AbstractCiphertext> combine(
      const std::vector<std::shared_ptr<AbstractCiphertext>>& cs,
      int from = 0,
      int to = -1,
      int bitsPerElement = 1) const override;

  std::shared_ptr<AbstractCiphertext> isEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> multiply(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2,
      int targetBits = -1) const override;

  std::shared_ptr<AbstractCiphertext> add(const AbstractCiphertext& c1,
                                          const AbstractCiphertext& c2,
                                          int targetBits = -1) const override;

  std::shared_ptr<AbstractCiphertext> sub(const AbstractCiphertext& c1,
                                          const AbstractCiphertext& c2,
                                          int targetBits = -1) const override;

  std::shared_ptr<AbstractCiphertext> multiplyBit(
      const AbstractCiphertext& c,
      const AbstractCiphertext& bit) const override;

  std::shared_ptr<AbstractCiphertext> bitwiseXor(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> bitwiseXnor(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  std::shared_ptr<AbstractCiphertext> bitwiseOr(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  int getNumBits(const AbstractCiphertext& c) const override;
  void setNumBits(AbstractCiphertext& c, int bits) const override;

  int getDefaultNumBits() const override;

  void debugPrintWithBinary(const AbstractCiphertext& c,
                            const std::string& title,
                            int maxElements = 4,
                            int verbose = 0,
                            std::ostream& out = std::cout) const override;

  double getScale(const AbstractCiphertext& c) const override;

  std::shared_ptr<AbstractCiphertext> max(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;
  std::shared_ptr<AbstractCiphertext> min(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;

  // TODO: check bette implementations
  std::shared_ptr<AbstractCiphertext> isGreater(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;
  std::shared_ptr<AbstractCiphertext> isLess(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;
  std::shared_ptr<AbstractCiphertext> isGreaterEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;
  std::shared_ptr<AbstractCiphertext> isLessEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const override;
};
} // namespace helayers

#endif
