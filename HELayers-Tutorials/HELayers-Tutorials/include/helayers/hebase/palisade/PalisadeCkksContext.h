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

#ifndef SRC_HELAYERS_PALISADE_PALISADECKKSCONTEXT_H
#define SRC_HELAYERS_PALISADE_PALISADECKKSCONTEXT_H

#include "PalisadeCkksConfig.h"
#include "palisade.h"
#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/RotateDependencyMapper.h"
namespace helayers {
class PalisadeCkksContext : public HeContext
{
  lbcrypto::LPKeyPair<lbcrypto::DCRTPoly> keys;

  lbcrypto::CryptoContext<lbcrypto::DCRTPoly> context;

  lbcrypto::SecurityLevel securityLevel = lbcrypto::HEStd_128_classic;

  PalisadeCkksConfig config;

  /// This member is used to compose a required rotation from a sequence of
  /// supported rotation steps
  RotateDependencyMapper rotateDependencyMapper;

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;
  void loadImpl(std::istream& in) override;

public:
  PalisadeCkksContext();

  ~PalisadeCkksContext();

  void init(const HeConfigRequirement& req) override;

  void init(const PalisadeCkksConfig& conf);

  void init(const PalisadeCkksConfig& conf,
            const PublicFunctions& publicFunctions);

  ///@brief Helper function for init() method
  void initCommon();

  /// @brief Converts the given security level to Palisad's SecurityLevel type.
  /// @param sec The securitly level
  static lbcrypto::SecurityLevel intSecurityToPalisadeSeucrity(int sec);

  /// @brief Returns the modulus length corresponding to the given parameters.
  /// @param multiplicationDepth     The multiplication depth
  /// @param integerPartPrecision    The integer part precision
  /// @param fractionalPartPrecision The fractional part precision
  static int getPolyModulusLength(int multiplicationDepth,
                                  int integerPartPrecision,
                                  int fractionalPartPrecision);

  ///@brief Translates the given HeConfigRequirement to a
  /// PalisadeCkksConfig.
  ///@param req The HeConfigRequirement.
  static PalisadeCkksConfig heConfReqToPalisadeConf(
      const HeConfigRequirement& req);

  void printSignature(std::ostream& out) const override;

  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  int getBestFeasibleFractionalPartPrecision(
      const HeConfigRequirement& req) const override;

  int getBestFeasibleMulDepth(const HeConfigRequirement& req) const override;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  std::shared_ptr<AbstractCiphertext> createAbstractCipher() const override;

  std::shared_ptr<AbstractPlaintext> createAbstractPlain() const override;

  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  int getTopChainIndex() const override;

  inline int slotCount() const override
  {
    return context->GetRingDimension() / 2;
  }

  int getSecurityLevel() const override;

  bool hasSecretKey() const override { return keys.secretKey != nullptr; }

  void saveSecretKey(std::ostream& out, bool seedOnly = false) override;
  void loadSecretKey(std::istream& in, bool seedOnly = false) override;

  std::string getLibraryName() const override { return "PALISADE"; }

  std::string getSchemeName() const override { return "CKKS"; }

  inline lbcrypto::CryptoContext<lbcrypto::DCRTPoly> getContext() const
  {
    return context;
  }

  inline lbcrypto::RescalingTechnique getRescaleTechnique() const
  {
    return config.rsTech;
  }

  /// Maps the received chain index (in terms of SealCKKS) to the corresponding
  /// level (in terms of PalisadeCKKS).
  /// Note that while the "chainIndex" term in SealCKKS corresponds to the
  /// "level" term in
  /// PalisadeCKKS, chain indexes in Seal start from a positive integer and
  /// decrease on each multiplication, levels in Palisade start from zero and
  /// increase on multiplications. This function does the mapping
  /// @param[in] Seal's chain index to translate to Palisade's level
  int sealChainIndexToPalisadeLevel(int chainIndex) const;

  /// Maps the received level (in terms of PalisadeCKKS) to the corresponding
  /// chain index (in terms of SealCKKS).
  /// Note that while the "chainIndex" term in SealCKKS corresponds to the
  /// "level" term in PalisadeCKKS, chain indexes in Seal start from a positive
  /// integer and decrease on each multiplication, levels in Palisade start from
  /// @param[in] Seal's chain index to translate to Palisade's level
  int palisadeLevelToSealChainIndex(uint32_t level) const;

  inline lbcrypto::LPKeyPair<lbcrypto::DCRTPoly> getKeys() const
  {
    return keys;
  }

  static std::shared_ptr<HeContext> loadHeContextFromFile(
      const std::string& fileName)
  {
    throw std::runtime_error("not yet implemented");
  }

  static std::shared_ptr<HeContext> loadHeContext(std::istream& in)
  {
    throw std::runtime_error("not yet implemented");
  }

  static bool internalRegisterContext(const HeContext* context)
  {
    throw std::runtime_error("not yet implemented");
  }

  std::shared_ptr<HeContext> clone() const override;

  /// Computes a minimal list of supported rotations that sum up to the
  /// targetRotate, if such a list exists. Returns true if it is possible
  /// to construct such a list using the supported rotations, and false
  /// otherwise.
  /// @param targetRotate The required rotate, to be compoesd from several
  ///                     supported rotations.
  /// @param rotations      The resulting set of rotations will be stored here.
  ///                     If no such a list exist, this vector will be empty.
  bool composeRotate(int targetRotate, std::vector<int>& rotations) const;

  void assertSavePublicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;

  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;

  /// @brief Returns a map estimating the run time and memory consumed by
  /// different operations made with this PalisadeCkksContext. This function
  /// assumes the existence of a palisade.json file storing the required
  /// measures.
  /// @throw runtime_error If the palisade.json file which is expected to
  /// contain the required measures, does not exist.
  std::map<std::string, int64_t> getEstimatedMeasures() const override;

  ///@brief Clears the multiplication and rotation keys, which are stored
  /// internally in PALISADE
  void clearKeys();

  ///@brief Clears all contexts from the PALISADE internal contexts vector
  static void clearAllContexts();
};
} // namespace helayers

#endif