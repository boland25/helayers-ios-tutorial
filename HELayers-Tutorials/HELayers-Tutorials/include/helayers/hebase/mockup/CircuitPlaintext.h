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

#ifndef SRC_HELAYERS_CIRCUITLAIN_H_
#define SRC_HELAYERS_CIRCUITLAIN_H_

#include "CircuitContext.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "helayers/circuit/Types.h"

namespace helayers {

class CircuitPlaintext : public AbstractPlaintext
{
  circuit::RawValues rawVals;

  int chainIndex = -1;
  circuit::PtxtId id;

  const CircuitContext& h;

  CircuitPlaintext(const CircuitPlaintext& src) = default;
  CircuitPlaintext(CircuitPlaintext&& src) = default;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class CircuitEncoder;

public:
  CircuitPlaintext(const CircuitContext& h);

  ~CircuitPlaintext() override {}

  const circuit::PtxtId& getId() const { return id; }
  const circuit::PtxtId& setId()
  {
    // TODO: can we remove this const_cast?
    id = const_cast<CircuitContext&>(h).getPtxtId();
    return id;
  }

  inline circuit::RawValues& getRawVals() { return rawVals; }

  inline const circuit::RawValues& getRawVals() const { return rawVals; }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  void setScale(double scale) override {}

  double getScale() const override { return 1; }

  inline int slotCount() const override { return h.slotCount(); }
};
} // namespace helayers

#endif
