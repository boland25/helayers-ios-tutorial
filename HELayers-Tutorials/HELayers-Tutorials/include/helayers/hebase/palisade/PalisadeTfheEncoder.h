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

#ifndef SRC_HELAYERS_PalisadeTFHEENCODER_H
#define SRC_HELAYERS_PalisadeTFHEENCODER_H

#include "helayers/hebase/impl/AbstractEncoder.h"
#include "PalisadeTfheContext.h"

namespace helayers {

class PalisadeTfheEncoder : public AbstractEncoder
{
private:
  const PalisadeTfheContext& he;

public:
  PalisadeTfheEncoder(const PalisadeTfheContext& he);

  ~PalisadeTfheEncoder();

  void encode(AbstractPlaintext& res,
              const vector<double>& vals,
              int chainIndex) const override;

  void encrypt(AbstractCiphertext& res,
               const AbstractPlaintext& src) const override;

  void decrypt(AbstractPlaintext& res,
               const AbstractCiphertext& src) const override;

  vector<int> decodeInt(const AbstractPlaintext& src) const override;

  vector<double> decodeDouble(const AbstractPlaintext& src) const override;

  vector<std::complex<double>> decodeComplex(
      const AbstractPlaintext& src) const override;

  void encodeEncrypt(AbstractCiphertext& res,
                     const vector<std::complex<double>>& vals,
                     int chainIndex) const override
  {
    throw std::runtime_error("encodeEncrypt not implemented");
  }
  void encode(AbstractPlaintext& res,
              const vector<std::complex<double>>& vals,
              int chainIndex) const override
  {
    throw std::runtime_error("encode complex not implemented");
  }

  const HeContext& getHeContext() const override { return he; };
};
} // namespace helayers

#endif /* SRC_HELAYERS_PalisadeTFHEENCODER_H */
