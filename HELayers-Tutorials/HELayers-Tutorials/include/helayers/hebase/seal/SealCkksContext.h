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

#ifndef SRC_HELAYERS_SEALCKKSCONTEXT_H
#define SRC_HELAYERS_SEALCKKSCONTEXT_H

#include "seal/seal.h"
#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/PublicFunctions.h"
#include "helayers/hebase/RotateDependencyMapper.h"

// TODO: allow referencing it using const HeContext&

namespace helayers {

/// @brief An implementation of HeContext for CKKS scheme in Seal.
///
/// It can be either initialized via parameters, via an HeConfigRequirement,
/// or loaded from a file.
///
/// It is recommended not to use directly after initialization, but
/// use an HeContext reference instead.
class SealCkksContext : public HeContext
{

  /// Whether this SealCkksContext has a secret key
  bool hasSecKey;

  /// The seed used to generate randomness when creating the secret key. A new
  /// seed is randomly generated upon calling init().
  std::optional<seal::prng_seed_type> seedArray;

  /// The SealContext object of the underlying Seal library.
  std::shared_ptr<seal::SEALContext> context;

  /// The KeyGenerator object of the underlying Seal library.
  std::shared_ptr<seal::KeyGenerator> keygen;

  /// The PublicKey object of the underlying Seal library.
  std::shared_ptr<seal::PublicKey> publicKey;

  /// The SecretKey object of the underlying Seal library.
  std::shared_ptr<const seal::SecretKey> secretKey;

  /// The RelinKeys object of the underlying Seal library.
  std::shared_ptr<seal::RelinKeys> relinKeys;

  /// The GaloisKeys object of the underlying Seal library.
  std::shared_ptr<seal::GaloisKeys> galKeys;

  /// The Encryptor object of the underlying Seal library.
  std::shared_ptr<seal::Encryptor> encryptor;

  /// The Evaluator object of the underlying Seal library.
  std::shared_ptr<seal::Evaluator> evaluator;

  /// The Decryptor object of the underlying Seal library.
  std::shared_ptr<seal::Decryptor> decryptor;

  /// The CKKSEncoder object of the underlying Seal library.
  std::shared_ptr<seal::CKKSEncoder> encoder;

  /// The encParms object of the underlying Seal library.
  std::shared_ptr<seal::EncryptionParameters> encParms;

  /// The security level of this SealCkksContext.
  seal::sec_level_type secLevel = seal::sec_level_type::tc128;

  /// The signature of this SealCkksContext
  std::string signature;

  /// Whether accurate scaling is supported. The automatic rescaling
  /// supported by Seal CKKS scheme implies small errors to the rescaled
  /// ciphertext. If this flag is set to true, some extra modifications are
  /// done upon rescaling to sustain precision.
  bool useAccurateScaling = true;

  /// This table maps each chain index to its precise rescaling factor that
  /// should be used in case useAccurateScaling is set to true.
  std::vector<double> accurateScalesTable;

  ///@brief Initializes the table of accurate scales, in case of using the
  /// accurate scaling mode.
  void initAccurateScalesTable();

  /// This member is used to compose required rotations from the sequence of
  /// supported rotation steps
  RotateDependencyMapper rotateDependencyMapper;

  ///@brief Helper function for init() method
  void initCommon();

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;

  void loadImpl(std::istream& in) override;

  /// @brief Initializes this SealCkkContext with the given parameters.
  /// @param publicFunctions An object specifying the public functionalities
  ///                        that will be supported by the public keys.
  /// @param parms           The EncryptionParameters object of the underlying
  ///                        Seal library defining the scheme's configuration
  ///                        parameters. See also SealCkksContext::genParams().
  /// @throw runtime_error If this SealCkksContext is already initialized.
  void initHelper(const seal::EncryptionParameters& parms,
                  const PublicFunctions& publicFunctions = PublicFunctions());

  /// @brief Generates a seal::EncryptionParameters object using the given
  /// configuration parameters.
  /// @param polyDegree The degree of the modulus polynomial.
  /// @param midBits    The number of bits for the fractional part precision.
  /// @param depth      The maximal support multiplication depth.
  /// @param extraBits  The number of bits for the integer part precision.
  static seal::EncryptionParameters genParams(int polyDegree,
                                              int midBits,
                                              int depth,
                                              int extraBits);

  /// @brief Generates a random seed to be used to control future randomness
  /// generation
  static seal::prng_seed_type getRandomSeed();

  /// @brief Creates a secret key using the given seed. Multiple calls to this
  /// function with the same parameters will result with the same secret key.
  ///
  /// @param encParms      The encryption parameters to create the secret key
  ///                      from.
  /// @param securityLevel The security level.
  /// @param seedArray     A seed used to control the randomness of the secret
  ///                      key creation.
  static std::shared_ptr<seal::SecretKey> createSecretKey(
      const seal::EncryptionParameters& encParms,
      seal::sec_level_type securityLevel,
      const seal::prng_seed_type& seedArray);

public:
  /// @brief A constructor.
  SealCkksContext();

  ~SealCkksContext() override;

  /// @brief Initializes this SealCkksContext using the given configuration
  /// requirement.
  /// @param req The configuration requirement.
  /// @throw runtime_error If this SealCkksContext is already initialized.
  /// @throw runtime_error If the given configuration requirement is not
  ///                      feasible.
  /// @throw runtime_error If the required securtiy level is above 256.
  /// @throw runtime_error If the signature of this SealCkksContext is not
  ///                      empty and it is not consistent with the new
  ///                      signature corresponding to the given requirement.
  void init(const HeConfigRequirement& req) override;

  /// @brief Initializes this SealCkksContext using the given configuration
  /// requirement.
  /// @param req The configuration requirement.
  void initAndSaveOrLoad(const HeConfigRequirement& req);

  ///  Prints a summary of library details and configuration params.
  ///
  ///  @param out Output stream to write to.
  void printSignature(std::ostream& out) const override;

  /// @brief Returns true if the received configuration requirement is feasible
  /// under this SealCkksContext, and false otherwise.
  /// @param req The requirement to check its feasibility.
  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  int getBestFeasibleFractionalPartPrecision(
      const HeConfigRequirement& req) const override;

  int getBestFeasibleMulDepth(const HeConfigRequirement& req) const override;

  /// @brief returns the maximum polynomial modulus length that is supported
  /// with the given securityLevel and polyModulusDegree.
  /// @param securityLevel     The securtiy level.
  /// @param polyModulusDegree The polynomial modulus degree.
  /// @throw runtime_error If securityLevel is greater than 256.
  int getMaxPolyModulusLength(int securityLevel, int polyModulusDegree) const;

  /// Prints detailed information for debug purposes.
  ///  @param title   Title to print along with the information.
  ///  @param verbose Verbosity level.
  ///  @param out     Output stream to print to.
  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  /// @brief Returns a pointer to a SealCkksCiphertext, initialized with this
  /// SealCkksContext.
  std::shared_ptr<AbstractCiphertext> createAbstractCipher() const override;

  /// @brief Returns a pointer to a SealCkksPlaintext, initialized with this
  /// SealCkksContext.
  std::shared_ptr<AbstractPlaintext> createAbstractPlain() const override;

  /// @brief Returns a pointer to a SealCkksEncoder, initialized with this
  /// SealCkksContext.
  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  std::shared_ptr<AbstractBootstrapEvaluator> createAbstractBootstrapEvaluator(
      const BootstrapConfig& bsConfig) override;

  /// @brief Returns the top chain index.
  int getTopChainIndex() const override;

  /// @brief Prints a summary of library details and configuration params to
  /// the given ostream.
  /// @param out The ostream to write to.
  /// @throw runtime_error If failed verifying that the underlying Seal scheme
  ///                      is the CKKS scheme.
  void print(std::ostream& out);

  /// @brief Returns the modulus chain: the value of each prime in the chain.
  /// @throw runtime_error If the context is not yet initialized.
  std::shared_ptr<std::vector<uint64_t>> getModulusChain() const override;

  /// @brief The number of slots in each CTile (ciphertext) or PTile
  /// (plaintext) created over this context.
  inline int slotCount() const override { return encoder->slot_count(); }

  /// @brief Returns the security level supplied by this context.
  int getSecurityLevel() const override;

  /// @brief Returns the CKKSEncoder object of the underlying Seal library.
  inline const seal::CKKSEncoder& getSealEncoder() const { return *encoder; }

  /// @brief Returns the Evaluator object of the underlying Seal library.
  inline const seal::Evaluator& getEvaluator() const { return *evaluator; }

  /// @brief Returns the Encryptor object of the underlying Seal library.
  const seal::Encryptor& getEncryptor() const;

  /// @brief Returns the Decryptor object of the underlying Seal library.
  /// @throw runtime_error if this SealCkksContext does not have a secret key.
  inline const seal::Decryptor& getDecryptor() const
  {
    if (!hasSecretKey())
      throw std::runtime_error("This context has no secret key");
    return *decryptor;
  }

  /// @brief Returns the SEALContext object of the underlying Seal library.
  inline std::shared_ptr<seal::SEALContext> getContext() const
  {
    return context;
  }

  /// @brief Returns the ParmsId object of the underlying Seal library
  /// corresponding to the given chain index.
  /// @param chainIndex The chain index to get its ParmsId.
  /// @throw runtime_error If the given chain index is illegal.
  const seal::parms_id_type& getParmsId(int chainIndex) const;

  /// @brief Returns the RelinKeys object of the underlying Seal library.
  const seal::RelinKeys& getRelinKeys() const;

  /// @brief Returns the GaloisKeys object of the underlying Seal library.
  const seal::GaloisKeys& getGalKeys() const;

  /// @brief Returns whether this context contains a secret key.
  /// If not, decryption and other operations relying on decryption
  /// will not be available (will throw an exception).
  bool hasSecretKey() const override;

  /// @brief Save secret key to the given ostream.
  /// @param out The binary stream to save to.
  ///
  /// @throw runtime_error if this HeContext doesn't have a secret key. i.e.
  ///        hasSecretKey() is false.
  void saveSecretKey(std::ostream& out, bool seedOnly = false) override;

  /// @brief Load secret key from the given istream.
  /// @param in the binary stream to load from.
  /// @throw runtime_error If this HeContext already has a secret key. i.e.
  ///        hasSecretKey() is ture.
  void loadSecretKey(std::istream& in, bool seedOnly = false) override;

  /// @brief Returns the bit count corresponding to the given chain index.
  /// @param chainIndex To get its bit count.
  int getBitCountAtChainIndex(int chainIndex) const;

  /// @brief Returns The sum of the bit counts of all chain indices starting
  /// from 0 and up to the given chainIndex, inclusive.
  /// @param chainIndex The chain index up to which the sum of bit counts will
  ///                   be calculated.
  int getTotalBitCountAtChainIndex(int chainIndex) const;

  /// @brief Returns the name of the underlying library
  std::string getLibraryName() const override { return "SEAL"; }

  /// @brief Returns the name of the underlying scheme
  std::string getSchemeName() const override { return "CKKS"; }

  /// @brief Returns a signature of the context that distinguishes it enough to
  /// be able to load previously stored contexts based on their signatures.
  std::string getSignature() const override { return signature; };

  /// @brief Returns a map estimating the run time and memory consumed by
  /// different operations made with this SealCkksContext. This function
  /// assumes the existence of a seal.json file storing the required measures.
  /// @throw runtime_error If the seal.json file which is expected to contain
  ///                      the required measures, does not exist.
  std::map<std::string, int64_t> getEstimatedMeasures() const override;

  /// See parent method
  std::shared_ptr<HeContext> clone() const override;

  /// @brief Sets whether to enable the accurate scaling mode or not
  /// (default:enabled). Cannot be called after the context is initialized.
  ///
  ///@param useAccurateScaling whether to enable the accurate scaling mode or
  /// not.
  /// @throw runtime_error If this SealCkksContext is already intialized.
  void setUseAccurateScaling(bool useAccurateScaling);

  /// @brief Returns whether accurate scaling mode is enabled or not.
  bool getUseAccurateScaling() const;

  /// See parent method
  double getAccurateScale(int chainIndex) const override;

  int getMinChainIndexForBootstrapping() const override;

  int getChainIndexAfterBootstrapping() const override;

  BootstrapConfig getBootstrapChainIndexes(
      const HeConfigRequirement& req) const override;

  int getMinSupportedNumSlots() const override;

  int getMaxSupportedNumSlots() const override;

  /// See parent method
  std::vector<double> getMaxAllowedValues() const override;

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
};

inline std::ostream& operator<<(std::ostream& out, SealCkksContext& he)
{
  he.print(out);
  return out;
}
} // namespace helayers

#endif /* SRC_HELAYERS_SEALCKKSCONTEXT_H */
