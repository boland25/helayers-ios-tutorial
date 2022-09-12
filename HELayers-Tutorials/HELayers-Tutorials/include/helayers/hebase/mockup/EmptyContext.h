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

#ifndef SRC_HELAYERS_EMPTYCONTEXT_H
#define SRC_HELAYERS_EMPTYCONTEXT_H

#include "helayers/hebase/HeContext.h"
#include "RunStats.h"
#include <atomic>

namespace helayers {

/// A class to represent empty mockup HE context. It creates empty mockup
/// ciphertext and plaintext objects that contain meta-data and no values at
/// all, and keep track of the meta-data when performing various "mockup" HE
/// operations
class EmptyContext : public HeContext
{
  int slots;
  bool supportsChainIndices;
  int topChainIndex;

  std::map<std::string, int64_t> estimatedMeasures;
  bool trackingOperationCount = false;
  int chainIndexOffset = 0;
  mutable std::shared_ptr<RunStats> runStats;

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;

  void loadImpl(std::istream& in) override;

public:
  EmptyContext();

  virtual ~EmptyContext(){};

  void init(const HeConfigRequirement& req) override;

  ///@brief Set an indication to whether chain indexes track should be
  /// supported.
  ///
  ///@param supportsChainIndices
  void setSupportsChainIndices(bool supportsChainIndices);

  std::shared_ptr<AbstractCiphertext> createAbstractCipher() const override;

  std::shared_ptr<AbstractPlaintext> createAbstractPlain() const override;

  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  std::shared_ptr<AbstractBootstrapEvaluator> createAbstractBootstrapEvaluator(
      const BootstrapConfig& bsConfig) override;

  inline int slotCount() const override { return slots; }

  inline int getTopChainIndex() const override { return topChainIndex; }

  inline bool hasSecretKey() const override { return true; }

  inline int getSecurityLevel() const override { return 0; }

  void printSignature(std::ostream& out) const override;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  void saveSecretKey(std::ostream& out, bool seedOnly = false) override;

  void loadSecretKey(std::istream& in, bool seedOnly = false) override;

  inline std::string getLibraryName() const override { return "Empty"; }

  inline std::string getSchemeName() const override { return "Empty"; }

  std::string getSignature() const override;

  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  ///@brief Store estimated measures with respect to the times of various HE
  /// operations
  ///
  ///@param estimatedMeasures a map with the estimated measures
  void setEstimatedMeasures(
      const std::map<std::string, int64_t>& estimatedMeasures);

  std::map<std::string, int64_t> getEstimatedMeasures() const override;

  ///@brief Set a chain index offset.
  ///
  ///@param chainIndexOffset The desired chain index offset.
  void setChainIndexOffset(int chainIndexOffset);

  ///@brief Returns the chain index offset used.
  ///
  int getChainIndexOffset() const;

  ///@brief Start the CPU time tracking from 0
  ///
  void startOperationCountTrack();

  ///@brief Resume the operation count tracking
  void resumeOperationCountTrack();

  ///@brief Stop the operation count tracking
  void stopOperationCountTrack();

  ///@brief Update the current opeation count tracking
  ///
  ///@param operation the type of HE operation currently done
  ///@param chainIndex the chain index of the elements involved in the HE
  /// operation
  void increaseOpCounter(RunStats::OperationType op, int chainIndex) const;

  ///@brief Update the current performed rotations tracking.
  ///
  ///@param rotation the rotation size to add.
  void addRotation(int rotation) const;

  ///@brief Update the current chain indexes seen tracking.
  ///
  ///@param chainIndex The chain index to update.
  void addChainIndex(int chainIndex) const;

  /// @brief Returns runStats member, which can be used to get different
  /// run statistics, such as the number of times each operation has been
  /// performed.
  inline std::shared_ptr<const RunStats> getRunStats() const
  {
    return runStats;
  }

  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;

  int getMinChainIndexForBootstrapping() const override;

  int getChainIndexAfterBootstrapping() const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_EMPTYCONTEXT_H */
