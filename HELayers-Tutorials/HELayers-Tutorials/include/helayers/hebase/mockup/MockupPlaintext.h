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

#ifndef SRC_HELAYERS_MOCKUPLAIN_H_
#define SRC_HELAYERS_MOCKUPLAIN_H_

#include "MockupContext.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "helayers/hebase/impl/AbstractPlaintext.h"

namespace helayers {

class MockupPlaintext : public AbstractPlaintext
{
  std::vector<std::complex<long double>> rawVals;

  int chainIndex = -1;

  double scale;

  const MockupContext& h;

  MockupPlaintext(const MockupPlaintext& src) = default;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class MockupEncoder;

public:
  MockupPlaintext(const MockupContext& h);

  ~MockupPlaintext() override{};

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

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  void setScale(double s) override { scale = s; }

  double getScale() const override { return scale; }

  inline int slotCount() const override { return h.slotCount(); }
};
} // namespace helayers

#endif
