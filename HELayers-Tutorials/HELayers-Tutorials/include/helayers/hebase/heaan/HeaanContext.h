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

#ifndef SRC_HELAYERS_HEAANCONTEXT_H
#define SRC_HELAYERS_HEAANCONTEXT_H

#include "HEaaN/heaan.hpp"
#include "helayers/hebase/HeContext.h"
#include "helayers/math/bootstrap/BootstrapEvaluator.h"
#include "helayers/hebase/RotateDependencyMapper.h"
#include <map>

namespace helayers {

/// A struct containing details on a HEAAN-provided preset configuration
struct HeaanParameterPreset
{
  /// The preset enum
  HEaaN::ParameterPreset preset;

  /// The preset's name
  std::string parameterPresetStr;

  /// The number of slots of plaintexts and ciphertexts created with this preset
  int numSlots;

  /// The maximal possible chain index for ciphertexts encrypted with this
  /// preset
  int topChainIndex;

  /// The fractional part precision of plaintexts and ciphertexts created with
  /// this preset
  int fractionalPartPrecision;

  /// The integer part precision of plaintexts and ciphertexts created with this
  /// preset
  int integerPartPrecision;

  /// Whether this preset is bootstrappable
  bool bootstrappable = false;

  /// Whether this preset is bootstrappable in extended range [-2^20,2^20]
  bool extendedBootstrappable = false;

  /// If this preset is bootstrappable, this variable indicates the chain index
  /// ciphertexts get after they are bootstrapped
  int cIAfterBootstrapping = -1;

  /// The security level
  int securityLevel = 128;

  ///@brief A constructor.
  ///@param parameterPresetStr      The preset's name
  ///@param numSlots                The number of slots of ciphertexts and
  ///                               plaintexts created with this preset
  ///@param topChainIndex           The maximal possible chain index for
  ///                               ciphertexts encrypted with this preset
  ///@param fractionalPartPrecision The fractional part precision of
  ///                               plaintexts and ciphertexts created with
  ///                               this preset

  ///@param integerPartPrecision    The integer part precision of
  ///                               plaintexts and ciphertexts created with
  ///                               this preset
  ///@param bootstrappable          Whether this preset is bootstrappable
  ///@param minCIForBootstrapping   If this preset is bootstrappable, this
  ///                               argument specifies the minimum chain index
  ///                               in which ciphertexts can be bootstrapped
  ///@param cIAfterBootstrapping    If this preset is bootstrappable, this
  ///                               argument specifies the chain index
  ///                               ciphertexts get after they are bootstrapped
  ///@param securityLevel           The security level
  HeaanParameterPreset(HEaaN::ParameterPreset preset,
                       const std::string& parameterPresetStr,
                       int numSlots,
                       int topChainIndex,
                       int fractionalPartPrecision,
                       int integerPartPrecision,
                       bool bootstrappable = false,
                       bool extendedBootstrappable = false,
                       int cIAfterBootstrapping = -1,
                       int securityLevel = 128)
      : preset(preset),
        parameterPresetStr(parameterPresetStr),
        numSlots(numSlots),
        topChainIndex(topChainIndex),
        fractionalPartPrecision(fractionalPartPrecision),
        integerPartPrecision(integerPartPrecision),
        bootstrappable(bootstrappable),
        extendedBootstrappable(extendedBootstrappable),
        cIAfterBootstrapping(cIAfterBootstrapping),
        securityLevel(securityLevel)
  {}

  ///@brief Returns true if this preset satisfies the given HeConfigRequirement
  /// and false otherwise
  bool satisfiesRequirement(const HeConfigRequirement& req) const;
};

/// @brief An implementation of HeContext for HEaaN.
///
/// It can be either initialized via parameters, via an HeConfigRequirement,
/// or loaded from a file.
///
/// It is recommended not to use directly after initialization, but
/// use an HeContext reference instead.
class HeaanContext : public HeContext
{
  /// Max prime size in the underlying HEaaN library.
  const int HEaaNMaxPrimeSize = 61;

  // internal use
  const bool alwaysUsePresets = false;
  const std::string heaanTmpDirName = "/heaan_context";
  const std::string contextFileName = "context.tmp";
  const std::string conjKeyName = "ConjKey";
  const std::string encKeyName = "EncKey";
  const std::string multKeyName = "MultKey";
  const std::string rotKeyName = "RotKey";

  /// Maps the names of the HEAAN-provided parameter presets to their
  /// corresponding HeaaNParameterPreset struct, which contains details on these
  /// presets (e.g. numSlots and topChainIndex)
  static std::map<HEaaN::ParameterPreset, HeaanParameterPreset>
      parameterPresetsMap;

  /// The Context object of the underlying HEaaN library.
  std::shared_ptr<HEaaN::Context> context;

  /// The KeyPack object of the underlying HEaaN library.
  std::shared_ptr<HEaaN::KeyPack> keyPack;

  /// The SecretKey object of the underlying HEaaN library.
  std::shared_ptr<HEaaN::SecretKey> secretKey;

  /// The EnDecoder object of the underlying HEaaN library.
  std::shared_ptr<HEaaN::EnDecoder> endecoder;

  /// The Encryptor object of the underlying HEaaN library.
  std::shared_ptr<HEaaN::Encryptor> encryptor;

  /// The Evaluator object of the underlying HEaaN library.
  std::shared_ptr<HEaaN::HomEvaluator> evaluator;

  /// The Decryptor object of the underlying HEaaN library.
  std::shared_ptr<HEaaN::Decryptor> decryptor;

  /// The ParameterPreset object of the underlying HEaaN library.
  HEaaN::ParameterPreset parameterPreset;

  // Preset parameters of the underlying HEaaN library. relevant only if
  // HeaanContext::getUsingCustomPreset() == true.
  uint64_t bpsize;
  uint64_t qpsize;

  /// The security level
  int securityLevel = 128;

  /// The top chain index of ciphertexts and plaintexts created by this
  /// HeaanContext. This may be different than the top chain index of the
  /// underlying HEaaN library (a.k.a., HeaanContext::chainLength) when using a
  /// non-custom preset.
  int topChainIndex = -1;

  /// The default scale of ciphertexts and plaintexts created by this
  /// HeaanContext.
  double defaultScale = -1;

  /// The modulus chain of ciphertexts created by this HeaanContext.
  std::vector<std::uint64_t> modulusChain;

  /// This member is used to compose required rotations from the sequence of
  /// supported rotation steps
  RotateDependencyMapper rotateDependencyMapper;

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;

  void loadImpl(std::istream& in) override;

  // Helper functions for init() method. Initializes HeaanContext::defaultScale,
  // HeaanContext::modulusChain and HeaanContext::rotateDependencyMapper
  void initCommon();

  void saveFileToStream(const std::string& path, std::ostream& out) const;
  void loadFileFromStream(const std::string& path, std::istream& in) const;
  std::string getFilePath(const std::string& dir,
                          const std::string& file,
                          bool key = true) const;

  void initUsingPresets(const HeConfigRequirement& req);

  void initCustom(const HeConfigRequirement& req);

  /// @brief Initializes this HeaanContext with the given parameters.
  /// @param req The bootstrap requirement.
  /// @param parameterPreset The ParameterPreset object of the underlying HEaaN
  ///              library defining the scheme's configuration parameters.
  /// @param logDimension logDimension = log2(N), should satisfy 10 <=
  /// logDimension <= 20. Taken into account only if parameterPreset ==
  /// ParameterPreset::CUSTOM
  /// @param chainLength The number of multiplications available. should satisfy
  /// 1 <= chainLength <= 50. Taken into account only if parameterPreset ==
  /// ParameterPreset::CUSTOM
  /// @param bpsize bpsize = Base prime size. should satisfy 40 <=
  /// bpsize <= 62. Taken into account only if parameterPreset ==
  /// ParameterPreset::CUSTOM
  /// @param qpsize qpsize = Quantize prime size. should satisfy 40 <=
  /// qpsize <= 62. Taken into account only if parameterPreset ==
  /// ParameterPreset::CUSTOM
  /// @param tpsize The size of the temporary primes in bits. The value must
  /// be greater than qpsize + (bpsize - qpsize) / numTP, less than or equal to
  /// 61. Note that numTP = chainLength / gadgetRank.
  /// @param gadgetRank This is the number of decomposed polynomials when one
  /// does the "modup" process in the middle of keyswitching process. See more
  /// details in HEaaN documentation.
  /// @throw runtime_error If this HeaanContext is already initialized.
  void doInit(const HeConfigRequirement& req,
              const HEaaN::ParameterPreset& parameterPreset,
              uint64_t logDimension = 0,
              uint64_t chainLength = 0,
              uint64_t bpsize = 0,
              uint64_t qpsize = 0,
              uint64_t tpsize = 0,
              uint64_t gadgetRank = 0);

  ///@brief Returns the parameter preset that best suits the given config
  /// requirement. If no preset satisfies the given requirement then null is
  /// returned. If more than one preset satisfies the given requirement, the one
  /// with lowest multiplication depth is returned.
  ///
  ///@param req The given config requirement.
  std::optional<HEaaN::ParameterPreset> getMostSuitablePreset(
      const HeConfigRequirement& req) const;

  ///@brief Returns a string with the name of the HEaaN Parameter Preset of this
  /// HeaanContext object
  std::string getParameterPresetStr() const;

  ///@brief Returns whether this HeaanContext object use a custom preset of
  /// the underlying HEaaN library
  bool getUsingCustomPreset() const
  {
    return parameterPreset == HEaaN::ParameterPreset::CUSTOM;
  }

public:
  /// @brief A constructor.
  HeaanContext();

  ~HeaanContext() override;

  void init(const HeConfigRequirement& req) override;

  void printSignature(std::ostream& out) const override;

  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  HeConfigRequirement getActualConfigRequirement(
      const HeConfigRequirement& req) const override;

  int getBestFeasibleFractionalPartPrecision(
      const HeConfigRequirement& req) const override;

  int getBestFeasibleMulDepth(const HeConfigRequirement& req) const override;

  int getMaxPolyModulusLength(int securityLevel, int polyModulusDegree) const;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  std::shared_ptr<AbstractCiphertext> createAbstractCipher() const override;

  std::shared_ptr<AbstractPlaintext> createAbstractPlain() const override;

  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  std::shared_ptr<AbstractBootstrapEvaluator> createAbstractBootstrapEvaluator(
      const BootstrapConfig& bsConfig) override;

  int getTopChainIndex() const override { return topChainIndex; };

  std::shared_ptr<std::vector<uint64_t>> getModulusChain() const override;

  /// @brief Returns the log number of slots of ciphertexts and plaintexts
  /// created by this HeaanContext.
  inline int getLogSlots() const
  {
    return HEaaN::getLogFullSlots(getContext());
  }

  inline int slotCount() const override { return std::pow(2, getLogSlots()); }

  int getSecurityLevel() const override;

  /// @brief Returns the Context object of the underlying HEaaN library.
  inline const HEaaN::Context& getContext() const { return *context; }

  /// @brief Returns the SecretKey object of the underlying HEaaN library.
  /// @throw runtime_error if this HeaanContext does not have a secret key.
  inline const HEaaN::SecretKey& getSecretKey() const
  {
    if (!hasSecretKey())
      throw std::runtime_error("This context has no secret key");
    return *secretKey;
  }

  /// @brief Returns the KeyPack object of the underlying HEaaN library.
  inline const HEaaN::KeyPack& getKeyPack() const { return *keyPack; }

  /// @brief Returns the EnDecoder object of the underlying HEaaN library.
  inline const HEaaN::EnDecoder& getEndecoder() const { return *endecoder; }

  /// @brief Returns the Encryptor object of the underlying HEaaN library.
  inline const HEaaN::Encryptor& getEncryptor() const { return *encryptor; }

  /// @brief Returns the Evaluator object of the underlying HEaaN library.
  inline const HEaaN::HomEvaluator& getEvaluator() const { return *evaluator; }

  /// @brief Returns the Decryptor object of the underlying HEaaN library.
  /// @throw runtime_error if this HeaanContext does not have a secret key.
  inline const HEaaN::Decryptor& getDecryptor() const
  {
    if (!hasSecretKey())
      throw std::runtime_error("This context has no secret key");
    return *decryptor;
  }

  /// @brief Returns the default scale of ciphertexts and plaintexts created
  /// by this HeaanContext.
  inline double getDefaultScale() const override { return defaultScale; }

  bool hasSecretKey() const override;

  void saveSecretKey(std::ostream& out, bool seedOnly = false) override;

  void loadSecretKey(std::istream& in, bool seedOnly = false) override;

  std::string getLibraryName() const override { return "HEaaN"; }

  std::string getSchemeName() const override { return "CKKS"; }

  std::string getSignature() const override;

  std::map<std::string, int64_t> getEstimatedMeasures() const override;

  std::shared_ptr<HeContext> clone() const override;

  double getAccurateScale(int chainIndex) const override;

  /// Returns a mapping of HEAAN's presets to their corresponding
  /// HeaanParameterPreset objects, which contian details on these presets (e.g.
  /// numSlots and topChainIndex)
  static std::map<HEaaN::ParameterPreset, HeaanParameterPreset>
  getParameterPresetsMap()
  {
    return parameterPresetsMap;
  }

  ///@brief Returns the HeaanParameterPreset object corresponding to the
  /// parameterPreset object of this HeaanContext
  HeaanParameterPreset getHeaanParameterPreset() const;

  static int getHeaanMinChainIndexForBootstrapping(
      BootstrapRange range = DEFAULT_RANGE);

  int getMinChainIndexForBootstrapping() const override;

  int getChainIndexAfterBootstrapping() const override;

  BootstrapConfig getBootstrapChainIndexes(
      const HeConfigRequirement& req) const override;

  int getMinSupportedNumSlots() const override;

  int getMaxSupportedNumSlots() const override;

  std::vector<double> getMaxAllowedValues() const override;

  double getMaxAllowedBsValue() const override;

  /// Computes a minimal list of supported rotations that sum up to the
  /// targetRotate, if such a list exists. Returns true if it is possible
  /// to construct such a list using the supported rotations, and false
  /// otherwise.
  /// @param targetRotate The required rotate, to be compoesd from several
  ///                     supported rotations.
  /// @param rotations      The resulting set of rotations will be stored here.
  ///                     If no such a list exist, this vector will be empty.
  bool composeRotate(int targetRotate, std::vector<int>& rotations) const;

  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;

  inline DeviceType deviceTypeFromHeaanDeviceType(
      HEaaN::DeviceType deviceType) const
  {
    return deviceType == HEaaN::DeviceType::GPU ? DEVICE_GPU : DEVICE_CPU;
  }

  inline HEaaN::DeviceType heaanDeviceTypeFromDeviceType(
      DeviceType deviceType) const
  {
    return deviceType == DEVICE_GPU ? HEaaN::DeviceType::GPU
                                    : HEaaN::DeviceType::CPU;
  }
};
} // namespace helayers

#endif /* SRC_HELAYERS_HEAANCONTEXT_H */
