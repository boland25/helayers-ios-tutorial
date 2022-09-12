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

#ifndef HELIBBITWISEBGVCIPHER_H_
#define HELIBBITWISEBGVCIPHER_H_

#include "HelibBitwiseBgvContext.h"
#include "HelibBitwiseCiphertext.h"

namespace helayers {

class HelibBitwiseBgvCiphertext : public HelibBitwiseCiphertext
{
  std::shared_ptr<AbstractCiphertext> doClone() const override;

  void addBits(int n);

  void addWithTargetBits(const AbstractCiphertext& other, int targetBits = -1);

  void subWithTargetBits(const AbstractCiphertext& other, int targetBits = -1);

  void makeSignsEqual(HelibBitwiseBgvCiphertext& other);

  void multiplyRawNoRescale(const AbstractCiphertext& other);

  // Returns floor(log_2(scale))
  int getLogScale() const;

  const HelibBitwiseBgvContext& h;

  bool isSigned = true;

  friend class HelibBitwiseBgvEncoder;
  friend class HelibBitwiseBgvEvaluator;

public:
  HelibBitwiseBgvCiphertext(const HelibBitwiseBgvContext& h, double scale)
      : HelibBitwiseCiphertext(h, scale), h(h)
  {}

  HelibBitwiseBgvCiphertext(const HelibBitwiseBgvContext& h,
                            double scale,
                            int numBits)
      : HelibBitwiseCiphertext(h, scale, numBits), h(h)
  {}

  HelibBitwiseBgvCiphertext(const HelibBitwiseBgvContext& h)
      : HelibBitwiseCiphertext(h), h(h)
  {}

  HelibBitwiseBgvCiphertext(const HelibBitwiseBgvContext& h, int numBits)
      : HelibBitwiseCiphertext(h, numBits), h(h)
  {}

  HelibBitwiseBgvCiphertext(const HelibBitwiseBgvCiphertext& src);

  virtual ~HelibBitwiseBgvCiphertext() {}

  std::shared_ptr<HelibBitwiseBgvCiphertext> clone() const
  {
    return std::static_pointer_cast<HelibBitwiseBgvCiphertext>(doClone());
  }

  std::vector<long> decryptBit(int i) const;

  void negate() override;

  void add(const AbstractCiphertext& other) override;

  void addRaw(const AbstractCiphertext& other) override;

  void sub(const AbstractCiphertext& other) override;

  void subRaw(const AbstractCiphertext& other) override;

  void multiply(const AbstractCiphertext& other) override;

  void multiplyRaw(const AbstractCiphertext& other) override;

  void addPlain(const AbstractPlaintext& plain) override;

  void addPlainRaw(const AbstractPlaintext& plain) override;

  void subPlain(const AbstractPlaintext& plain) override;

  void subPlainRaw(const AbstractPlaintext& plain) override;

  void multiplyPlain(const AbstractPlaintext& plain) override;

  void multiplyPlainRaw(const AbstractPlaintext& plain) override;

  void conjugate() override;

  void conjugateRaw() override;

  // rotate right
  void rotate(int n) override;

  int slotCount() const override;

  void debugPrintWithBinary(const std::string& title = "",
                            Verbosity verbosity = VERBOSITY_REGULAR,
                            std::ostream& out = std::cout) const;

  inline bool getIsSigned() const { return isSigned; }
  inline void setIsSigned(bool val) { isSigned = val; }
};
} // namespace helayers

#endif