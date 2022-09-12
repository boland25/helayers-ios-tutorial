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

#ifndef SRC_HELAYERS_TILE_H
#define SRC_HELAYERS_TILE_H

#include "utils/Saveable.h"

namespace helayers {

class CTile;
class HeContext;

/// A class representing a tile: a vector of numbers, either encrypted or
/// encoded.
class Tile : public Saveable
{

public:
  virtual ~Tile() {}

  virtual Tile* clone() const = 0;

  /// Add content of this tile to other CTile.
  ///  @param[in] other other ciphertext to add to.
  virtual void addTo(CTile& other) const = 0;

  ///@brief Subtract content of this tile from other CTile.
  ///
  ///@param other Other CTile to subtract from.
  virtual void subFrom(CTile& other) const = 0;

  /// See subFrom()
  virtual void subFromRaw(CTile& other) const = 0;

  /// See addTo()
  virtual void addToRaw(CTile& other) const = 0;

  /// See multiplyTo()
  virtual void multiplyToRaw(CTile& other) const = 0;

  ///@brief Multiply content of this tile to other CTile.
  ///
  ///@param other Other CTile to multiply to.
  virtual void multiplyTo(CTile& other) const = 0;

  ///@brief Returns an estimation of the memory usage, or -1 if not supported.
  virtual int64_t getEstimatedMemoryUsageBytes() const = 0;

  /// Reduces the chain-index property of this object by 1.
  /// Ignored if not supported.
  /// @throw runtime_error If chain index is already at lowest value
  virtual void reduceChainIndex() = 0;

  /// Sets the chain-index property of this object to equal the specified value.
  /// Can only be used if the specified value is less than or equal to the chain
  /// index of this object.
  /// Ignored if not supported.
  /// @param[in] chainIndex The terget value to set the chain index to
  /// @throw runtime_error If the specified chain index is higher than this
  virtual void setChainIndex(int chainIndex) = 0;

  /// Returns the value of the chain-index property of this object, which is a
  /// non-negative integer.
  /// Returns a negative value if not supported.
  virtual int getChainIndex() const = 0;

  /// Sets the attached meta data scale of the plaintext / ciphertext.
  /// This may change the way its content are interpreted.
  /// @param[in] scale the new scale
  /// @throw runtime_error If not supported
  virtual void setScale(double scale) = 0;

  /// Returns the attached meta data scale of the plaintext / ciphertext.
  virtual double getScale() const = 0;

  /// A Tile represents a plaintext / ciphertext consisting of multuple slots.
  /// This method returns the number of slots in this object.
  virtual int slotCount() const = 0;
};
} // namespace helayers

#endif /* SRC_HELAYERS_TILE_H */
