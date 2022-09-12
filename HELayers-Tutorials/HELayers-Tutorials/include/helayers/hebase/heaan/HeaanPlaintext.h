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

#ifndef SRC_HELAYERS_HEAANPLAIN_H_
#define SRC_HELAYERS_HEAANPLAIN_H_

#include <boost/multiprecision/cpp_int.hpp>
#include "HEaaN/Message.hpp"
#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "HeaanContext.h"

namespace helayers {

class HeaanPlaintext : public AbstractPlaintext
{
  const HeaanContext& he;

  std::shared_ptr<HEaaN::Plaintext> plaintext = nullptr;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class HeaanEncoder;

public:
  HeaanPlaintext(const HeaanContext& he) : AbstractPlaintext(he), he(he) {}

  HeaanPlaintext(const HeaanPlaintext& src);

  ~HeaanPlaintext() override{};

  std::shared_ptr<HeaanPlaintext> clone() const
  {
    return std::static_pointer_cast<HeaanPlaintext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void toDevice(DeviceType deviceType) override;

  DeviceType getCurrentDevice() const override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  void setScale(double scale) override;

  double getScale() const override;

  int slotCount() const override;

  bool isAllZeroes() const override;

  const std::shared_ptr<HEaaN::Plaintext> getPlaintext() const
  {
    return plaintext;
  }
};
} // namespace helayers

#endif /* SRC_HELAYERS_HEAANPLAIN_H_ */
