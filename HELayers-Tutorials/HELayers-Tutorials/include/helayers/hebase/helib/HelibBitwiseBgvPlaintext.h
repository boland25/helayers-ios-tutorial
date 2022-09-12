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

#ifndef HELIBBITWISEBGVPLAIN_H_
#define HELIBBITWISEBGVPLAIN_H_

#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "HelibBitwiseBgvContext.h"

namespace helayers {

class HelibBitwiseBgvPlaintext : public AbstractPlaintext
{
  const HelibBitwiseBgvContext& h;

  // ptxtBits[i][j] = the ith bit of the jth slot. 0th bit is LSB.
  std::vector<helib::Ptxt<helib::BGV>> ptxtBits;

  double scale;

  bool isSigned = true;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class HelibBitwiseBgvEncoder;

public:
  HelibBitwiseBgvPlaintext(const HelibBitwiseBgvContext& h);

  HelibBitwiseBgvPlaintext(const HelibBitwiseBgvPlaintext& src);

  virtual ~HelibBitwiseBgvPlaintext(){};

  virtual std::shared_ptr<HelibBitwiseBgvPlaintext> clone() const
  {
    return std::static_pointer_cast<HelibBitwiseBgvPlaintext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  void debugPrintWithBinary(const std::string& title = "",
                            Verbosity verbosity = VERBOSITY_REGULAR,
                            std::ostream& out = std::cout) const;

  inline void setScale(double s) override { scale = s; }
  inline double getScale() const override { return scale; }

  inline int getNumBits() const { return ptxtBits.size(); }

  int slotCount() const override { return h.slotCount(); }

  inline const HelibBitwiseBgvContext& getHeContext() { return h; }

  inline const HelibBitwiseBgvContext& getHeContext() const { return h; }

  inline const std::vector<helib::Ptxt<helib::BGV>>& getPlaintext() const
  {
    return ptxtBits;
  }

  inline bool getIsSigned() const { return isSigned; }
  inline void setIsSigned(bool val) { isSigned = val; }
};
} // namespace helayers

#endif