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

#ifndef SRC_HELAYERS_CTILE_H
#define SRC_HELAYERS_CTILE_H

#include "HeContext.h"
#include "impl/AbstractCiphertext.h"
#include "Tile.h"
#include "PTile.h"
#include "version.h"

namespace helayers {

/// A class representing a ciphertext.
///
/// It's called a CTile because from a high-level point of view we'll usually
/// use several of these combined for holding a more complicated object such as
/// a matrix. For a lower level documentation of different functionalities (such
/// as the effect on chainIndex), see documentation of the underlying encryption
/// scheme.
///
/// Notice that some of the methods below implement a binary operation between
/// two CTile objects or between a CTile and a PTile object. In these binary
/// operations, and in the case where chain index is a relevant concept for the
/// underlying scheme, the implementation will automatically handle the case
/// where the chain indexes of the two operands are diffetrent.
///
/// Notice that some of the methods below have two versions: a "raw" and a
/// "non-raw" version. The "raw" version differs from the "non-raw" version by
/// not performing relinearize and rescale as part of the operation.
class CTile : public Tile
{
  REGISTER_SAVEABLE_HEADER;

  std::shared_ptr<AbstractCiphertext> impl;

  void saveImpl(std::ostream& stream) const override;

  void loadImpl(std::istream& stream) override;

  ///@brief Automatically performs BS on this object if needed
  ///
  void handleAutomaticBs();

  ///@brief Automatically performs BS on this object and/or on the other object
  /// if needed and if configured to. If BS is performed on the other object,
  /// the new one will be returned, otherwise null will be returned.
  ///
  ///@param other The other object.
  std::shared_ptr<CTile> handleAutomaticBs(const CTile& other);

  ///@brief Makes the chain index of this object and the other object equal, if
  /// needed and if configured to. If operation is performed on the other
  /// object, the new one will be returned, otherwise null will be returned.
  ///
  ///@param other The other object.
  std::shared_ptr<CTile> makeChainIndexesEqual(const CTile& other);

  ///@brief Makes the chain index of this object and the other object equal, if
  /// needed and if configured to. If operation is performed on the other
  /// object, the new one will be returned, otherwise null will be returned.
  ///
  ///@param other The other object.
  std::shared_ptr<PTile> makeChainIndexesEqual(const PTile& other);

  friend class Encoder;

  friend class BitwiseEvaluator;

  friend class NativeFunctionEvaluator;

public:
  CTile() = default;

  /// Constructs an empty object.
  /// @param[in] he the underlying context.
  CTile(const HeContext& he);

  /// Copy constructor.
  /// @param[in] src Object to copy.
  CTile(const CTile& src);

  /// Move constructor.
  /// @param[in] src Object to move.
  CTile(CTile&& src);

  virtual ~CTile();

  CTile* clone() const override { return new CTile(*this); }

  void addTo(CTile& other) const override;

  void subFrom(CTile& other) const override;

  void subFromRaw(CTile& other) const override;

  void addToRaw(CTile& other) const override;

  void multiplyToRaw(CTile& other) const override;

  void multiplyTo(CTile& other) const override;

  /// Copy from another object.
  /// @param[in] src Object to copy.
  CTile& operator=(const CTile& src);

  /// Move from another object.
  /// @param[in] src Object to move.
  CTile& operator=(CTile&& src);

  ///  Conjugates contents of this CTile in place, elementwise.
  ///  For non-complex numbers this has no effect.
  ///  Depending on scheme, this may perform some additional
  ///  light-weight tasks allowing for a smooth sequence of operations. Use
  ///  conjugateRaw() to get the scheme's raw conjugate operation.
  void conjugate();

  /// See conjugate().
  void conjugateRaw();

  ///  Rotate left.
  ///  For an offset of n, the element at slots 10 will move to slot 10-n.
  ///  The first n elements are rotated back to be last.
  ///  @param[in] n rotate offset
  void rotate(int n);

  /// Add content of another ciphertext to this one, elementwise.
  /// Result is stored in place.
  /// Depending on scheme, this may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// addRaw() to get
  /// the scheme's raw addition operation.
  ///  @param[in] other other ciphertext to add.
  void add(const CTile& other);

  /// See add()
  void addRaw(const CTile& other);

  /// Subtract content of another ciphertext from this one, elementwise.
  /// Result is stored in place.
  /// Depending on scheme, this may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// subRaw() to get
  /// the scheme's raw subtraction operation.
  ///  @param[in] other other ciphertext to subtract.
  void sub(const CTile& other);

  /// See sub()
  void subRaw(const CTile& other);

  /// Multiply another ciphertext with this one, elementwise.
  /// Result is stored in place.
  /// Depending on scheme, this may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// multiplyRaw() to get
  /// the scheme's raw addition operation.
  ///  @param[in] other other ciphertext to multiply.
  void multiply(const CTile& other);

  /// see multiply()
  void multiplyRaw(const CTile& other);

  /// Add content of another PTile to this one, elementwise.
  /// Result is stored in place.
  /// Depending on scheme, this may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// addPlainRaw() to get
  /// the scheme's raw addition operation.
  ///  @param[in] plain PTile to add.
  void addPlain(const PTile& plain);

  /// See addPlain()
  void addPlainRaw(const PTile& plain);

  /// Subtract content of another PTile from this one, elementwise.
  /// Result is stored in place.
  /// Depending on scheme, this may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// subPlainRaw() to get
  /// the scheme's raw subtract operation.
  ///  @param[in] plain PTile to subtract.
  void subPlain(const PTile& plain);

  /// See subPlain()
  void subPlainRaw(const PTile& plain);

  /// Multiply a PTile with this CTile, elementwise.
  /// Result is stored in place.
  /// Depending on scheme, this may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// multiplyPlainRaw() to get
  /// the scheme's raw multiplication operation.
  ///  @param[in] plain PTile to multiply with.
  void multiplyPlain(const PTile& plain);

  /// See multiplyPlain()
  void multiplyPlainRaw(const PTile& plain);

  /// Square content of this ciphertext, elementwise.
  /// Result is stored in place.
  /// Depending on scheme, this may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// squareRaw() to get
  /// the scheme's raw square operation.
  void square();

  /// See square()
  void squareRaw();

  /// Multiply this ciphertext by a scalar double value, elementwise, by a
  /// light-weight scale changing.
  /// This multiplies every slot by the given factor.
  /// The operation is done by changing the scale meta-data combined with this
  /// ciphertext, and is therefore extermely fast.
  /// It is not available for all underlying schemes, and also may cause
  /// difficulties
  /// down the line due to mismatching scales.
  ///  @param[in] factor factor to multiply with.
  /// @throw runtime_error If not supported
  void multiplyByChangingScale(double factor);

  /// Adds an int scalar value to all slots in this ciphertext.
  /// @param[in] scalar value to add.
  void addScalar(int scalar);

  /// Adds a double scalar value to all slots in this ciphertext.
  /// @param[in] scalar value to add.
  void addScalar(double scalar);

  /// Multiplies all slots in this ciphertext with an int scalar value.
  /// @param[in] scalar value to multiply.
  void multiplyScalar(int scalar);

  /// Multiplies all slots in this ciphertext with a double scalar value.
  /// @param[in] scalar value to multiply.
  void multiplyScalar(double scalar);

  /// Performs a relinearize operation (reducing ciphertext size after
  /// multiplication)
  /// The non-raw version of multiplication takes care of this automatically
  /// when needed.
  /// Ignored if not needed or not supported.
  void relinearize();

  /// Performs a rescale operation (reducing ciphertext modulus bits and
  /// attached meta data scale).
  /// The non-raw version of multiplication takes care of this automatically.
  /// Ignored if not needed or not supported.
  /// Note that depending on scheme, this method may perform some additional
  /// light-weight tasks allowing for a smooth sequence of operations. Use
  /// rescaleRaw() to get
  /// the scheme's raw rescale operation.
  void rescale();

  /// See rescale()
  void rescaleRaw();

  /// Negates content of this ciphertext.
  void negate();

  /// Sets the attached meta data scale of the ciphertext.
  /// This may change the way its content are interpreted.
  /// @param[in] scale the new scale
  /// @throw runtime_error If not supported
  void setScale(double scale) override;

  /// Returns the attached meta data scale of the ciphertext.
  double getScale() const override;

  /// Reduces the chain-index property of the ciphertext by 1.
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

  /// A CTile represents a ciphertext consisting of multuple slots.
  /// This method returns the number of slots in this object.
  int slotCount() const override;

  ///@brief Computes the inner sum of some of the ciphertext's slots.
  /// Performs the computation by repeated rotations and additions, with
  /// rotations rot1, 2*rot1, 4*rot1, up to rot2 (exclusive).
  /// Using this method properly is complicated, and is used by some higher
  /// level
  /// algorithms.
  ///@param rot1 First rotation in the rotate-and-add loop
  ///@param rot2 Last rotation (exclusive) in the rotate-and-add loop
  ///@param reverse If true, flips all rotations (left to right and vice versa)
  void innerSum(int rot1, int rot2, bool reverse = false);

  /// Summarizes n slots of this ciphertext.
  /// After summing, slot i will contain the sum of slots i, i+1, i+2, . . .,
  /// i+n-1,
  /// where indices are cyclic, i.e., roll back to the start if run out over the
  /// edge.
  /// Summation is done similar to left-to-right repeated squaring algorithm.
  ///  @param[in] n number of elements to summarize
  void sumExpBySquaringLeftToRight(int n);

  /// Summarizes n slots of this ciphertext.
  /// After summing, slot i will contain the sum of slots i, i+1, i+2, . . .,
  /// i+n-1,
  /// where indices are cyclic, i.e., roll back to the start if run out over the
  /// edge.
  /// Summation is done similar to right-to-left repeated squaring algorithm.
  ///  @param[in] n number of elements to summarize
  void sumExpBySquaringRightToLeft(int n);

  /// Bootstrap this CTile, if this is supported by the underlying HeContext.
  /// Bootstrapping will raise the chain index of this CTile, allowing more
  /// multiplications to be done. However, this operation is likely to be
  /// time-consuming, and may add noise to the ciphetext in some schemes
  /// (such as CKKS). Bootstrapping is only supported if the HeContext used
  /// to create this CTile was initialized to be bootstrappable (see also
  /// HeConfigRequirement::bootstrappable).
  ///@throw runtime_error If the HeContext used to create this CTile is not
  ///                     bootstrappable.
  void bootstrap();

  /// Move this CTile to another device.
  ///  @param[in] device The device type to move to.
  void toDevice(DeviceType device);

  /// Returns the current device of this CTile.
  DeviceType getCurrentDevice() const;

  /// Returns true if this CTile does not include an encryption of any value,
  /// and false otherwise.
  bool isEmpty() const;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  /// Reserved for debugging and internal use.
  const AbstractCiphertext& getImpl() const { return *impl; };

  /// Reserved for debugging and internal use.
  AbstractCiphertext& getImpl() { return *impl; };

  ///@brief Returns an estimation of the memory usage, or -1 if not supported.
  int64_t getEstimatedMemoryUsageBytes() const override;

  ///@brief Return the HE context
  const HeContext& getHeContext();

  int32_t getContextId() const override
  {
    return impl->getHeContext().getContextId();
  }

  ///@brief Verify this CTile has a correct scale, and optionally correct it.
  ///
  ///@param message Message to include in case of warning or error.
  ///@param warnEpsilon Warn epsilon (used with MathUtils::isEqual)
  ///@param errorEpsilon Error epsilon (used with MathUtils::isEqual)
  ///@param fix If true (default) the scale is fixed if to target scale.
  ///@param targetScale The scale to test with and to fix. If set to -1
  ///(default) the usual accuracte scale for this level is used.
  void assertCorrectScale(const std::string& message = "",
                          double warnEpsilon = 1e-20,
                          double errorEpsilon = 1e-10,
                          bool fix = true,
                          double targetScale = -1);
};

} // namespace helayers

#endif /* SRC_HELAYERS_CTILE_H */
