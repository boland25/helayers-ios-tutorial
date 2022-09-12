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

#ifndef SRC_HELAYERS_SEALCKKSCIPHER_H_
#define SRC_HELAYERS_SEALCKKSCIPHER_H_

#include "seal/seal.h"
#include "helayers/hebase/impl/AbstractCiphertext.h"
#include "SealCkksContext.h"
#ifdef USE_SEAL_HELAYERS
#include "seal/util/seal_IBM_extensions.h"
#endif

namespace helayers {

class SealCkksCiphertext : public AbstractCiphertext
{

  const SealCkksContext& he;

  seal::Ciphertext encrypted;

  SealCkksCiphertext(const SealCkksCiphertext& src) = default;

  std::shared_ptr<AbstractCiphertext> doClone() const override;

  void setChainIndex(const seal::parms_id_type& parms_id);

  ///@brief Performs raw multiplication, without rescale, between this and a
  /// plaintext that contains "1" values on all its slots. The plaintext will be
  /// encoded with the same chain index as this and with the given scale.
  ///
  ///@param scale the scale to encode the ones plaintext in.
  void multiplyByOneWithScale(double scale);

  ///@brief Returns a boolean indicating whether this follows the accurate
  /// scaling table. Can only be called when using the accurate scaling mode.
  ///
  bool followsAccurateScaling() const;

  ///@brief Make the scale of this align with the accurate scale it should have
  /// according to the accurate scales table. Used to prevent any future
  /// mismatches that might grow big.
  ///
  void alignWithAccurateScaling();

  friend class SealCkksEncoder;

public:
  SealCkksCiphertext(const SealCkksContext& h) : AbstractCiphertext(h), he(h) {}

  ~SealCkksCiphertext() override;

  std::shared_ptr<SealCkksCiphertext> clone() const
  {
    return std::static_pointer_cast<SealCkksCiphertext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void conjugate() override;

  void conjugateRaw() override;

  // rotate right
  void rotate(int n) override;

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

  void square() override;

  void squareRaw() override;

  void multiplyScalar(int scalar) override;

  void multiplyScalar(double scalar) override;

  void relinearize() override;

  void rescale() override;

  void rescaleRaw() override;

  void negate() override;

  void setScale(double scale) override;

  double getScale() const override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  ///@brief Set the chain index higher than the current level
  ///@param chainIndex the chainIndex to "jump" up to.
  ///       -1 implies - jump as high as possible
  ///       This function is only useful as part of the CKKS bootstrap
  ///       process, since it changes the underlying plaintext -
  ///       it adds to every plaintext coefficient a multiple of the 1st prime
  ///       in the prime chain.
  ///       The Bootstrap then fixes this by removing the added multiples.
  void remod(int chainIndex = -1) override;

  int slotCount() const override;

  bool isEmpty() const override;

  bool isTransparent() const override;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  ///@brief Returns an estimation of the memory usage.
  int64_t getEstimatedMemoryUsageBytes() const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_SEALCKKSCIPHER_H_ */
