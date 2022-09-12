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

#ifndef HELIBBITWISECIPHER_H
#define HELIBBITWISECIPHER_H

#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "HelibBitwiseContext.h"

#include <vector>

namespace helayers {

class HelibBitwiseCiphertext : public AbstractCiphertext
{
  const HelibBitwiseContext& h;

protected:
  // bitCs[0] is LSB
  std::vector<helib::Ctxt> bitCs;

  double scale;

public:
  HelibBitwiseCiphertext(const HelibBitwiseContext& h, double scale)
      : AbstractCiphertext(h),
        h(h),
        bitCs(std::vector<helib::Ctxt>(h.getNumBits(),
                                       helib::Ctxt(h.getPublicKey()))),
        scale(scale)
  {}

  HelibBitwiseCiphertext(const HelibBitwiseContext& h,
                         double scale,
                         int numBits)
      : AbstractCiphertext(h),
        h(h),
        bitCs(std::vector<helib::Ctxt>(numBits, helib::Ctxt(h.getPublicKey()))),
        scale(scale)
  {}

  HelibBitwiseCiphertext(const HelibBitwiseContext& h);

  HelibBitwiseCiphertext(const HelibBitwiseContext& h, int numBits);

  HelibBitwiseCiphertext(const HelibBitwiseCiphertext& src)
      : AbstractCiphertext(src), h(src.h), bitCs(src.bitCs), scale(src.scale)
  {}

  virtual ~HelibBitwiseCiphertext() {}

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void square() override;

  void squareRaw() override;

  // Square in-place without relin/rescale

  void relinearize() override;

  void rescale() override;
  void rescaleRaw() override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  bool isEmpty() const override
  {
    return bitCs.size() == 0 || bitCs[0].isEmpty();
  }

  inline int getNumBits() const { return bitCs.size(); }

  inline double getScale() const override { return scale; }
  inline void setScale(double s) override { scale = s; }
};
} // namespace helayers

#endif