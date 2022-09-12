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

#ifndef SRC_HELAYERS_SEALCKKSPLAIN_H_
#define SRC_HELAYERS_SEALCKKSPLAIN_H_

#include <boost/multiprecision/cpp_int.hpp>
#include "seal/seal.h"
#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "SealCkksContext.h"

namespace helayers {

class SealCkksPlaintext : public AbstractPlaintext
{

  const SealCkksContext& heContext;

  seal::Plaintext pt;

  SealCkksPlaintext(const SealCkksPlaintext& src) = default;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  void gcd_extended(const boost::multiprecision::cpp_int& a,
                    const boost::multiprecision::cpp_int& b,
                    boost::multiprecision::cpp_int& gcd,
                    boost::multiprecision::cpp_int& x,
                    boost::multiprecision::cpp_int& y) const;
  boost::multiprecision::cpp_int crt(std::vector<uint64_t>& rems,
                                     std::vector<uint64_t>& mods) const;

  friend class SealCkksEncoder;

public:
  SealCkksPlaintext(const SealCkksContext& h)
      : AbstractPlaintext(h), heContext(h)
  {}

  ~SealCkksPlaintext() override{};

  std::shared_ptr<SealCkksPlaintext> clone() const
  {
    return std::static_pointer_cast<SealCkksPlaintext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  void setScale(double scale) override;

  double getScale() const override;

  int slotCount() const override;

  bool isAllZeroes() const override;

  const seal::Plaintext& getPlaintext() const;

  void printLongCoefs(int maxVals = 4, std::ostream& out = std::cout) const;

  ///@brief Returns an estimation of the memory usage, or -1 if not supported.
  int64_t getEstimatedMemoryUsageBytes() const override;

  ///@brief Returns a boolean indicating whether this follows the accurate
  /// scaling table. Can only be called when using the accurate scaling mode.
  ///
  bool followsAccurateScaling() const;

  // debugPrint utils
private:
  std::shared_ptr<std::uint64_t[]> getLongArray(
      const seal::DynArray<std::uint64_t>& data,
      int size) const;
  std::shared_ptr<std::uint64_t[]> getLongArray(
      const seal::util::Pointer<uint64_t, void>& data,
      int size) const;
  bool equalCoefs(const std::shared_ptr<std::uint64_t[]> data,
                  std::size_t coeff_count,
                  std::size_t coeff_modulus_size) const;
  bool constCoefs(const std::shared_ptr<std::uint64_t[]> data,
                  std::size_t coeff_count,
                  std::size_t coeff_modulus_size) const;
  std::vector<std::vector<uint64_t>> getCoefsCRT(
      std::shared_ptr<seal::SEALContext> context,
      seal::Plaintext pt) const;

  void debugPrintArray(const std::shared_ptr<std::uint64_t[]> data,
                       std::size_t coeff_count,
                       std::size_t coeff_modulus_size,
                       std::ostream& out = std::cout) const;
  void debugPrintPlaintext(const seal::CKKSEncoder& encoder,
                           const SealCkksContext& context,
                           seal::Plaintext pt,
                           const std::string& title,
                           Verbosity verbosity,
                           std::ostream& out) const;

public:
  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_SEALCKKSPLAIN_H_ */
