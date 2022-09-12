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

#ifndef SRC_HELAYERS_EMPTYPLAIN_H_
#define SRC_HELAYERS_EMPTYPLAIN_H_

#include "EmptyContext.h"
#include "helayers/hebase/impl/AbstractPlaintext.h"

namespace helayers {

class EmptyPlaintext : public AbstractPlaintext
{
  const EmptyContext& h;
  int chainIndex;

  EmptyPlaintext(const EmptyPlaintext& src) = default;

  inline std::shared_ptr<AbstractPlaintext> doClone() const override
  {
    return std::shared_ptr<EmptyPlaintext>(new EmptyPlaintext(*this));
  }

public:
  EmptyPlaintext(const EmptyContext& h)
      : AbstractPlaintext(h), h(h), chainIndex(h.getTopChainIndex())
  {}

  ~EmptyPlaintext() override{};

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void setChainIndex(int chainIndex) override;

  inline int getChainIndex() const override
  {
    return h.getTraits().getSupportsExplicitChainIndices() ? chainIndex : -1;
  }

  void setScale(double scale) override {}

  double getScale() const override { return 1; }

  inline int slotCount() const override { return h.slotCount(); }
};
} // namespace helayers

#endif /* SRC_HELAYERS_EMPTYPLAIN_H_ */
