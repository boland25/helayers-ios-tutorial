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

#ifndef SRC_HELAYERS_PalisadeTFHEPLAIN_H_
#define SRC_HELAYERS_PalisadeTFHEPLAIN_H_

#include "helayers/hebase/HeContext.h"
#include "binfhecontext.h"
#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "PalisadeTfheContext.h"

namespace helayers {

class PalisadeTfhePlaintext : public AbstractPlaintext
{
private:
  shared_ptr<vector<lbcrypto::LWEPlaintext>> plaintext;

  const PalisadeTfheContext& heContext;

  friend class PalisadeTfheEncoder;

  shared_ptr<AbstractPlaintext> doClone() const override
  {
    return shared_ptr<PalisadeTfhePlaintext>(new PalisadeTfhePlaintext(*this));
  }

public:
  PalisadeTfhePlaintext(const PalisadeTfheContext& h)
      : AbstractPlaintext(h), heContext(h)
  {}

  ~PalisadeTfhePlaintext() override {}

  int slotCount() const override;

  std::streamoff save(std::ostream& stream) const override
  {
    throw std::runtime_error("save not implemented");
  }

  std::streamoff load(std::istream& stream) override
  {
    throw std::runtime_error("load not implemented");
  }

  void setChainIndex(int chainIndex) override
  {
    throw std::runtime_error("setChainIndex not implemented");
  }

  int getChainIndex() const override
  {
    throw std::runtime_error("getChainIndex not implemented");
  }

  void setScale(double val) override
  {
    throw std::runtime_error("PalisadeTfhePlaintext::setScale not implemented");
  }

  double getScale() const override
  {
    throw std::runtime_error("PalisadeTfhePlaintext::getScale not implemented");
  }
};
} // namespace helayers

#endif /* SRC_HELAYERS_PalisadeTFHEPLAIN_H_ */
