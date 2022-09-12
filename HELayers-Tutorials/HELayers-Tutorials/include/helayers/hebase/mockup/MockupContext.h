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

#ifndef SRC_HELAYERS_MOCKUPCONTEXT_H
#define SRC_HELAYERS_MOCKUPCONTEXT_H

#include "helayers/hebase/HeContext.h"
#include <complex>
#include <iostream>
#include <fstream>
#include <vector>

namespace helayers {

/// A class to represent mockup HE context. It creates mockup ciphertext and
/// plaintext objects that contain non-encrypted values and meta-data, and keep
/// track of the values and meta-data when performing various "mockup" HE
/// operations
class MockupContext : public HeContext
{
  bool hasSecKey = false;

  int slots;

  bool supportsChainIndices;

  int topChainIndex;

  std::vector<uint64_t> modChain;

  ///@brief Members used for handling the mechanism of tracking seen values and
  /// comparing them to the limitations of maximal abosulute values allowed. Are
  /// not being saved or loaded.
  ///
  bool trackingMaxValues = false;
  std::vector<double> maxAllowedValues;
  std::vector<double> maxValuesSeen;
  bool trackingMaxBsValues = false;
  double maxAllowedBsValue = -1.0;
  double maxBsValueSeen = -1.0;

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;
  void loadImpl(std::istream& in) override;

public:
  MockupContext();

  virtual ~MockupContext(){};

  void init(const HeConfigRequirement& req) override;

  ///@brief Set an indication to whether chain indexes track should be
  /// supported.
  ///
  ///@param supportsChainIndices
  void setSupportsChainIndices(bool supportsChainIndices);

  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher()
      const override;

  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain()
      const override;

  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  std::shared_ptr<AbstractBootstrapEvaluator> createAbstractBootstrapEvaluator(
      const BootstrapConfig& bsConfig) override;

  virtual inline int slotCount() const override { return slots; }

  virtual inline int getTopChainIndex() const override { return topChainIndex; }

  virtual std::shared_ptr<std::vector<uint64_t>> getModulusChain()
      const override;

  virtual void setModulusChain(
      const std::shared_ptr<std::vector<uint64_t>>& modChain) override;

  bool hasSecretKey() const override { return hasSecKey; }

  inline int getSecurityLevel() const override { return 0; }

  virtual void printSignature(std::ostream& out = std::cout) const override;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  void saveSecretKey(std::ostream& out, bool seedOnly = false) override;
  void loadSecretKey(std::istream& in, bool seedOnly = false) override;

  std::string getLibraryName() const override { return "Mockup"; }

  std::string getSchemeName() const override { return "Mockup"; }

  std::string getSignature() const override;

  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  std::shared_ptr<HeContext> clone() const override;

  double getAccurateScale(int chainIndex) const override;

  std::vector<double> getMaxAllowedValues() const override;

  double getMaxAllowedBsValue() const override;

  ///@brief Sets the maximal absolute value allowed for each chain index.
  ///
  ///@param maxValues The limitation values to set. Should be in the format of
  /// the result of calling "HeContext::getMaxAllowedValues".
  void setMaxAllowedValues(const std::vector<double>& maxValues);

  ///@brief Sets the maximal absolute value allowed for values going through a
  /// bootstrap operation.
  ///
  ///@param maxBsValue
  void setMaxAllowedBsValue(double maxBsValue);

  ///@brief Update the tracking mechanism with an array of newly seen values in
  /// a given chain index.
  ///
  ///@param vals The values seen.
  ///@param chainIndex The chain index of the values.
  void updateSeenValues(const std::vector<std::complex<long double>>& vals,
                        int chainIndex);

  ///@brief Update the bootstrap tracking mechanism with an array of newly seen
  /// values.
  ///
  ///@param vals The values seen.
  void updateBsSeenValues(const std::vector<std::complex<long double>>& vals);

  ///@brief Returns the maximal absolute values seen so far, for each chain
  /// index. The value in position i corresponds to chain index i.
  ///
  const std::vector<double>& getMaxValuesSeen() const;

  ///@brief Returns the highest ratio between the absolute value of any value
  /// seen so far and the limitation that applies on it. Result higher than one
  /// means an overflow.
  ///
  double getHighestSeenToMaxRatio() const;

  ///@brief Returns the highest ratio between the absolute value of any value
  /// seen so far with respect to bootstrap operation, and the limitation that
  /// applies on it. Result higher than one means an overflow.
  ///
  double getHighestBsSeenToMaxRatio() const;

  ///@brief Clears the tracking of values that were seen so far.
  ///
  void clearMaxValuesSeen();

  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;

  int getMinChainIndexForBootstrapping() const override;

  int getChainIndexAfterBootstrapping() const override;
};
} // namespace helayers

#endif
