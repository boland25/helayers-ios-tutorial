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

#ifndef SRC_HELAYERS_EMPTYENCODER_H
#define SRC_HELAYERS_EMPTYENCODER_H

#include "helayers/hebase/impl/AbstractEncoder.h"
#include "EmptyContext.h"

namespace helayers {

class EmptyEncoder : public AbstractEncoder
{
  const EmptyContext& he;
  double defaultScale = 1;

public:
  EmptyEncoder(const EmptyContext& he) : he(he) {}

  ~EmptyEncoder() {}

  // encode
  inline void encode(AbstractPlaintext& res,
                     const std::vector<double>& vals,
                     int chainIndex) const override
  {
    he.increaseOpCounter(RunStats::ENCODE, chainIndex);
    res.setChainIndex(chainIndex);
  }

  inline void encode(AbstractPlaintext& res,
                     const std::vector<std::complex<double>>& vals,
                     int chainIndex) const override
  {
    he.increaseOpCounter(RunStats::ENCODE, chainIndex);
    res.setChainIndex(chainIndex);
  }

  // decode
  inline std::vector<double> decodeDouble(
      const AbstractPlaintext& src) const override
  {
    he.increaseOpCounter(RunStats::DECODE_DOUBLE, src.getChainIndex());
    return std::vector<double>(he.slotCount());
  }

  inline std::vector<std::complex<double>> decodeComplex(
      const AbstractPlaintext& src) const override
  {
    he.increaseOpCounter(RunStats::DECODE_DOUBLE, src.getChainIndex());
    return std::vector<std::complex<double>>(he.slotCount());
  }

  // encrypt
  inline void encrypt(AbstractCiphertext& res,
                      const AbstractPlaintext& src) const override
  {
    he.increaseOpCounter(RunStats::ENCRYPT, src.getChainIndex());
    res.setChainIndex(src.getChainIndex());
  }

  // decrypt
  inline void decrypt(AbstractPlaintext& res,
                      const AbstractCiphertext& src) const override
  {
    he.increaseOpCounter(RunStats::DECRYPT, src.getChainIndex());
    res.setChainIndex(src.getChainIndex());
  }

  // manipulating the default scale
  inline void setDefaultScale(double scale) override { defaultScale = scale; }

  inline double getDefaultScale(int chainIndex) const override
  {
    return defaultScale;
  }

  inline void restoreDefaultScale() override { defaultScale = 1; }

  const HeContext& getHeContext() const override { return he; };
}; // namespace helayers
} // namespace helayers

#endif /* SRC_HELAYERS_EMPTYENCODER_H */
