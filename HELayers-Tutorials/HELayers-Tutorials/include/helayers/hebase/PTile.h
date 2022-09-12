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

#ifndef SRC_HELAYERS_PTILE_H
#define SRC_HELAYERS_PTILE_H

#include "HeContext.h"
#include "impl/AbstractPlaintext.h"
#include "Tile.h"

namespace helayers {

/// A class representing an encoded (unencrypted) plaintext.
///
/// It's called a PTile because from a high-level point of view
/// we'll usually use several of these combined for holding a more complicated
/// object such as a matrix.
class PTile : public Tile
{
  REGISTER_SAVEABLE_HEADER;

  std::shared_ptr<AbstractPlaintext> impl;

  void saveImpl(std::ostream& stream) const override;

  void loadImpl(std::istream& stream) override;

  friend class CTile;
  friend class Encoder;

public:
  /// Constructs an empty object.
  /// @param[in] he the underlying context.
  PTile(const HeContext& he);

  /// Copy constructor.
  /// @param[in] src Object to copy.
  PTile(const PTile& src);

  virtual ~PTile();

  PTile* clone() const override { return new PTile(*this); }

  /// Copy from another object.
  /// @param[in] src Object to copy.
  PTile& operator=(const PTile& src);

  void addTo(CTile& other) const override;

  void subFrom(CTile& other) const override;

  void subFromRaw(CTile& other) const override;

  void addToRaw(CTile& other) const override;

  void multiplyToRaw(CTile& other) const override;

  void multiplyTo(CTile& other) const override;

  /// Move this PTile to another device.
  ///  @param[in] device The device type to move to.
  void toDevice(DeviceType device);

  /// Returns the current device of this PTile.
  DeviceType getCurrentDevice() const;

  /// Reduces the chain-index property of this object by 1.
  /// Ignored if not supported.
  /// @throw runtime_error If chain index is already at lowest value
  void reduceChainIndex() override;

  /// Sets the chain-index property of this object to equal the specified value.
  /// Can only be used if the specified value is less than or equal to the chain
  /// index of this object.
  /// Ignored if not supported.
  /// @param[in] chainIndex The terget value to set the chain index to
  /// @throw runtime_error If the specified chain index is higher than this
  void setChainIndex(int chainIndex) override;

  /// Returns the value of the chain-index property of this object, which is a
  /// non-negative integer.
  /// Returns a negative value if not supported.
  int getChainIndex() const override;

  /// Sets the attached meta data scale of the plaintext.
  /// This may change the way its content are interpreted.
  /// @param[in] scale the new scale
  /// @throw runtime_error If not supported
  void setScale(double scale) override;

  /// Returns the attached meta data scale of the plaintext.
  double getScale() const override;

  /// A PTile represents a plaintext consisting of multuple slots.
  /// This method returns the number of slots in this object.
  int slotCount() const override;

  // Returns true if this PTile encodes all zeroes plaintext, and false
  // otherwise.
  bool isAllZeroes() const;

  /// Reserved for debugging and internal use.
  const AbstractPlaintext& getImpl() const { return *impl; };

  ///@brief Returns an estimation of the memory usage, or -1 if not supported.
  int64_t getEstimatedMemoryUsageBytes() const override;

  int32_t getContextId() const override
  {
    return impl->getHeContext().getContextId();
  }

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  ///@brief Decodes this PTile and encodes the result into a new PTile whose
  /// chain index is equal to the given chain index. Returns the new encoded
  /// PTile in the output parameter.
  ///
  ///@param res The output parameter where the result PTile will be stored at.
  ///@param newChainIndex The required chain index of the new PTile.
  void reencode(PTile& res, int newChainIndex) const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_PTILE_H */
