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

#ifndef SRC_HELAYERS_HELIBBITWISEBGVENCODER_H_
#define SRC_HELAYERS_HELIBBITWISEBGVENCODER_H_

#include "helayers/hebase/impl/AbstractEncoder.h"
#include "HelibBitwiseBgvContext.h"

namespace helayers {

class HelibBitwiseBgvEncoder : public AbstractEncoder
{
  const HelibBitwiseBgvContext& h;

  double scale;

  std::shared_ptr<helib::Ptxt<helib::BGV>> zeroPtxt = nullptr;
  std::shared_ptr<helib::Ptxt<helib::BGV>> onePtxt = nullptr;
  std::shared_ptr<helib::Ctxt> zeroCtxt = nullptr;
  std::shared_ptr<helib::Ctxt> oneCtxt = nullptr;

public:
  HelibBitwiseBgvEncoder(const HelibBitwiseBgvContext& h, int scale)
      : h(h), scale(scale)
  {}
  HelibBitwiseBgvEncoder(const HelibBitwiseBgvContext& h);
  HelibBitwiseBgvEncoder(const HelibBitwiseBgvEncoder& src);
  ~HelibBitwiseBgvEncoder() {}

  // encode
  void encode(AbstractPlaintext& res,
              const std::vector<double>& vals,
              int chainIndex) const override;
  void encode(AbstractPlaintext& res,
              const std::vector<std::complex<double>>& vals,
              int chainIndex) const override;

  // decode
  std::vector<double> decodeDouble(const AbstractPlaintext& src) const override;
  std::vector<std::complex<double>> decodeComplex(
      const AbstractPlaintext& src) const override;

  // encrypt
  void encrypt(AbstractCiphertext& res,
               const AbstractPlaintext& src) const override;

  // decrypt
  void decrypt(AbstractPlaintext& res,
               const AbstractCiphertext& src) const override;

  // setScale + geScale
  inline void setScale(int s) { scale = s; }
  inline int getScale() { return scale; }

  helib::Ptxt<helib::BGV> getZeroPtxt();
  helib::Ptxt<helib::BGV> getOnePtxt();
  helib::Ctxt getZeroCtxt();
  helib::Ctxt getOneCtxt();

  const HeContext& getHeContext() const override { return h; };
};
} // namespace helayers

#endif