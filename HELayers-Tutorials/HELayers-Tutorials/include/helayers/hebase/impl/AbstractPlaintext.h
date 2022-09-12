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

#ifndef SRC_HELAYERS_ABSTRACTPLAIN_H_
#define SRC_HELAYERS_ABSTRACTPLAIN_H_

#include <vector>
#include <memory>
#include <iostream>
#include <complex>

#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/utils/Printable.h"
#include "helayers/hebase/HelayersTimer.h"
#include "helayers/hebase/Types.h"

namespace helayers {

///@brief Internal abstract implementation of a plaintext, do not use directly.
/// Used as parent class for concrete classes implementing plaintexts in
/// various schemes.
/// Do not use directly, use a PTile class instead.
class AbstractPlaintext : public Printable
{

  const HeContext& he;

  virtual std::shared_ptr<AbstractPlaintext> doClone() const = 0;

protected:
  AbstractPlaintext(const AbstractPlaintext& src) = default;

public:
  AbstractPlaintext(const HeContext& he) : he(he){};

  virtual ~AbstractPlaintext(){};

  AbstractPlaintext& operator=(const AbstractPlaintext& src) = delete;

  std::shared_ptr<AbstractPlaintext> clone() const
  {
    return std::static_pointer_cast<AbstractPlaintext>(doClone());
  }

  virtual std::streamoff save(std::ostream& stream) const = 0;

  virtual std::streamoff load(std::istream& stream) = 0;

  virtual void toDevice(DeviceType device)
  { // Default behaviour is silent ignore
  }

  virtual DeviceType getCurrentDevice() const { return DEVICE_CPU; }

  inline const HeContext& getHeContext() const { return he; }

  void reduceChainIndex();

  void setChainIndex(const AbstractPlaintext& other);

  virtual void setChainIndex(int chainIndex) = 0;

  virtual int getChainIndex() const = 0;

  virtual void setScale(double scale) = 0;

  virtual double getScale() const = 0;

  virtual int slotCount() const = 0;

  virtual bool isAllZeroes() const;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  ///@brief Returns an estimation of the memory usage, or -1 if not supported.
  virtual int64_t getEstimatedMemoryUsageBytes() const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_ABSTRACTPLAIN_H_ */
