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

#ifndef SRC_HELAYERS_DEBUGPLAINTEXT_H_
#define SRC_HELAYERS_DEBUGPLAINTEXT_H_

#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "DebugContext.h"

namespace helayers {

class DebugPlaintext : public AbstractPlaintext
{
  const DebugContext& he;

  std::shared_ptr<AbstractPlaintext> p1;
  std::shared_ptr<AbstractPlaintext> p2;

  DebugPlaintext(const DebugPlaintext& src);

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class DebugEncoder;
  friend class DebugCiphertext;

public:
  DebugPlaintext(const DebugContext& h)
      : AbstractPlaintext(h),
        he(h),
        p1(h.h1.createAbstractPlain()),
        p2(h.h2.createAbstractPlain())
  {}

  ~DebugPlaintext() override;

  std::shared_ptr<DebugPlaintext> clone() const
  {
    return std::static_pointer_cast<DebugPlaintext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  void setScale(double scale) override;

  double getScale() const override;

  int slotCount() const override;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_DEBUGPLAINTEXT_H_ */
