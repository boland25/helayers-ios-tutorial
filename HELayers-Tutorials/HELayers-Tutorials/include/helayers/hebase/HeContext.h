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

#ifndef SRC_HELAYERS_HECONTEXT_H
#define SRC_HELAYERS_HECONTEXT_H

#include <map>
#include "HeTraits.h"
#include "HeConfigRequirement.h"
#include "helayers/hebase/utils/Saveable.h"
#include "helayers/hebase/Types.h"

namespace helayers {

class AbstractCiphertext;
class AbstractPlaintext;
class AbstractEncoder;
class AbstractFunctionEvaluator;
class AbstractBitwiseEvaluator;
class AbstractBootstrapEvaluator;
class BootstrapEvaluator;

/// Use this macro to register a context class type for dynamic type loading.
/// Put this macro in the cpp file. The class must override the clone()
/// method for this to work.
/// It creates a static level variable, ensuring it will be initialized before
/// main.
#define REGISTER_HE_CONTEXT(heContextClass)                                    \
  static bool heContextClass##_static_entry =                                  \
      HeContext::internalRegisterContext(new heContextClass())

/// An abstract main class representing an underlying HE library & scheme,
/// configured, initialized, and ready to start working.
///
/// Upon initaliation, use on of the inheriting concrete classes such as
/// HelibCkksContext. Then continue with a generic reference to HeContext to
/// allow scheme obliviousness
///
/// This class bundles together all the data structures
/// required for performing HE operations, including the keys.
/// It either contains a public/private key pair, allowing all operations
/// including decryption, or just the public key allowing everything
/// but decryption.
///
/// It is used as input in constructor of many other classes
/// in this library. For example, to create a ciphertext using
/// the library represent by an HeContext, write:
/// CTile c(he);
///
/// It can be further used to query various properties of the underlying scheme
/// and library.
class HeContext
{
  DeviceType defaultDevice = DEVICE_CPU;

  double defaultScale = 1;

  /// A random id generated upon init().
  int32_t contextId;

  typedef std::map<std::string, std::shared_ptr<const HeContext>> ContextMap;

  /// Whether this HeContext supports bootstrapping
  bool bootstrappable = false;

  /// Whether bootstrapping is applied automatically to prevent runtime errors
  /// resulting from executing a chain index consuming operation on a ciphertext
  /// with too small chain index. The logic for deciding the bootstrapping
  /// points is scheme and library specific. But, in general, the bootstrappings
  /// are delyed as much as possible. This flag can only be set to true if this
  /// HeContext supports bootstrapping.
  bool automaticBootstrapping = false;

  /// The bootstrap config te be used to initialize the internal bootstrap
  /// evaluator
  std::shared_ptr<BootstrapConfig> bootstrapConfig = nullptr;

  // The internal bootstrap evaluator, used to perform bootstrap on CTile
  // objects
  std::shared_ptr<BootstrapEvaluator> bootstrapEvaluator = nullptr;

  /// This member specifies which functionalities are supported by the
  /// public keys of this HeContext
  PublicFunctions publicFunctions;

  /// returns registered context map.
  /// Used instead of a static member to ensure it's already initialized when
  /// needed.
  static ContextMap& getRegisteredHeContextMap();

  ///@brief Verifies that the class name in the header is "HeContext".
  ///
  /// @throw runtime_error If the class name is wrong.
  static void verifyHeader(const SaveableHeader& header);

  /// @brief Saves a header containing metadata about this HeContext. This
  /// header will be used to apply sanity checks when loading the saved
  /// HeContext.
  /// @param stream The stream to save to.
  void saveHeader(std::ostream& stream) const;

  ///@brief A helper class to save the secret key of an HeContext.
  class KeySaver : public Saveable
  {
    HeContext& he;

    std::string getClassNameImpl() const override;

    int32_t getContextId() const override;

    void saveImpl(std::ostream& stream) const override;

    void loadImpl(std::istream& stream) override;

  public:
    /// @brief A constructor.
    /// @param he The HeContext.
    KeySaver(HeContext& he);
  };

protected:
  HeTraits traits;

  /// Whether this HeContext is initialized
  bool initialized = false;

  /// Validates that the HeContext is initialized
  void validateInit() const;

  /// Validates that the HeContext is not initialized
  void validateNotInit() const;

  ///@brief Initialize bootstrap-related members. To be called by bootstrappable
  /// inheriting classes.
  ///
  ///@param req The bootstrap requirement.
  void initBootstrap(const BootstrapRequirement& req);

  virtual void saveImpl(std::ostream& out,
                        const PublicFunctions& publicFunctions) const;

  virtual void loadImpl(std::istream& in);

  ///@brief Save bootstrap-related members to the given ostream.
  ///
  ///@param out The binary stream to save to.
  void saveBootstrap(std::ostream& out) const;

  ///@brief Load bootstrap-related members from the given istream.
  ///
  ///@param in The binary stream to load from.
  void loadBootstrap(std::istream& in);

public:
  /// Constructs an empty object.
  HeContext();

  virtual ~HeContext();

  /// Copy constructor.
  /// @param[in] src Object to copy.
  HeContext(const HeContext& src) = delete;

  /// Copy from another object.
  /// @param[in] src Object to copy.
  HeContext& operator=(const HeContext& src) = delete;

  /// Internal use.
  void init(const PublicFunctions& publicFunctions = PublicFunctions());

  /// Internal use.
  virtual void init(const HeConfigRequirement& req) = 0;

  ///@brief Creates and returns an initialized HE context given a context name
  /// and config requirement.
  ///
  ///@param contextName The context name.
  ///@param req The config requirement.
  static std::shared_ptr<HeContext> create(const std::string& contextName,
                                           const HeConfigRequirement& req);

  /// Internal use.
  virtual bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const = 0;

  /// Returns a string that identifies a concrete HeContext class for the
  /// purpose of dynamic loading.
  std::string getContextFileHeaderCode() const;

  ///@brief Returns the actual config requirement that will be enabled by the HE
  /// context when provided with the given config requirement. Some parameters
  /// may differ from the original object and be "better", e.g. higher
  /// multiplication depth.
  ///
  ///@param req The given config requirement.
  virtual HeConfigRequirement getActualConfigRequirement(
      const HeConfigRequirement& req) const;

  /// @brief Returns the best feasible fractional part precision under the given
  /// configuration requirement. If there is no such a fractional part
  /// precision, returns -1. All the fields of the given requirement other than
  /// req.fractionalPartPrecision are expected to be present, while any value
  /// in req.fractionalPartPrecision will be ignored.
  /// @param req The requirement to get best fractional part precision for.
  virtual int getBestFeasibleFractionalPartPrecision(
      const HeConfigRequirement& req) const;

  /// @brief Returns the best feasible multiplication depth under the given
  /// configuration requirement. If there is no such a multiplication depth,
  /// returns -1. All the fields of the given requirement other than
  /// req.multiplicationDepth are expected to be present, while any value
  /// in req.multiplicationDepth will be ignored.
  /// @param req The requirement to get best multiplication depth for.
  virtual int getBestFeasibleMulDepth(const HeConfigRequirement& req) const;

  /// Returns the smallest number of slots that guarantees the other
  /// requirements in the given HeConfigRequirement. If there is not such a
  /// number of slots, returns -1. All the fields of the given requirement other
  /// than req.numSlots are expected to be present, while any value in numSlots
  /// will be ignored.
  ///
  ///@param req The HeConfigrequirement.
  ///@throw invalid_argument If one of the fields of \p req other than
  ///                        req.numSlots is not present.
  virtual int getMinFeasibleNumSlots(const HeConfigRequirement& req) const;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher() const = 0;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain() const = 0;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractEncoder> getEncoder() const = 0;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractFunctionEvaluator> getFunctionEvaluator();

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractBitwiseEvaluator> getBitwiseEvaluator() const
  {
    throw std::runtime_error("This HeContext cannot provide BitwiseEvaluator");
  }

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractBootstrapEvaluator>
  createAbstractBootstrapEvaluator(const BootstrapConfig& bsConfig);

  ///@brief Returns indication to whether this HE context is bootstrappable.
  ///
  bool getBootstrappable() const;

  ///@brief Returns the internal bootstrap configuration object.
  ///
  const BootstrapConfig& getBootstrapConfig() const;

  ///@brief Returns the internal bootstrap evaluator object.
  ///
  const BootstrapEvaluator& getBootstrapEvaluator() const;

  /// Returns an HeTraits object containing various properties of the
  /// underlying scheme.
  inline const HeTraits& getTraits() const { return traits; }

  /// Returns whether this context contains a secret key.
  /// If not, decryption and other operations relying on decryption
  /// will not be available (will throw an exception).
  virtual bool hasSecretKey() const = 0;

  /// The number of slots in each CTile (ciphertext) or PTile (plaintext)
  /// created over this context.
  virtual int slotCount() const = 0;

  /// Returns the highest available chain index (for schemes where it is
  /// applicable).
  virtual int getTopChainIndex() const = 0;

  /// Returns the security level supplied by this context.
  virtual int getSecurityLevel() const = 0;

  ///  Prints a summary of library details and configuration params.
  ///
  ///  @param[in] out output stream to write to
  virtual void printSignature(std::ostream& out = std::cout) const;

  /// Prints detailed information for debug purposes.
  ///  @param[in] title Title to print along with the information.
  ///  @param[in] verbose Verbosity level
  ///  @param[in] out Output stream to print to
  virtual void debugPrint(const std::string& title = "",
                          int verbose = 0,
                          std::ostream& out = std::cout) const = 0;

  /// Returns the modulus chain: the value of each prime in the chain (where
  /// applicable).
  virtual std::shared_ptr<std::vector<uint64_t>> getModulusChain() const
  {
    throw std::runtime_error("getModulusChain() not implemented");
  };

  /// Sets the modulus chain: the value of each prime in the chain (where
  /// applicable).
  virtual void setModulusChain(
      const std::shared_ptr<std::vector<uint64_t>>& modChain)
  {
    throw std::runtime_error("setModulusChain() not implemented");
  };

  /// @brief Save secret key to the given ostream.
  /// @param out      The binary stream to save to.
  /// @param seedOnly If true, only the seed used to control the secret key's
  ///                 randomness will be saved, rather than the whole secret
  ///                 key.
  ///
  /// @throw runtime_error If this HeContext doesn't have a secret key. i.e.
  ///                      hasSecretKey() is false.
  /// @throw runtime_error If seedOnly is true but the underlying HeContext does
  ///                      not support secret key I/O using seeds.
  virtual void saveSecretKey(std::ostream& out, bool seedOnly = false);

  /// @brief Load secret key from the given istream.
  /// @param in       The binary stream to load from.
  /// @param seedOnly If true, the seed of the secret key will be loaded from
  ///                 the given binary stream and the secret key will be
  ///                 generated using this seed.
  /// @throw runtime_error If this HeContext already has a secret key. i.e.
  ///                      hasSecretKey() is ture.
  /// @throw runtime_error If seedOnly is true but the underlying HeContext does
  ///                      not support secret key I/O using seeds.
  virtual void loadSecretKey(std::istream& in, bool seedOnly = false);

  /// save secret key to the given file.
  /// @param fileName The path of the file to save to.
  /// @param seedOnly If true, only the seed used to control the secret key's
  ///                 randomness will be saved, rather than the whole secret
  ///                 key.
  /// @throw runtime_error If this HeContext doesn't have a secret key. i.e.
  ///                      hasSecretKey() is false.
  /// @throw runtime_error If seedOnly is true but the underlying HeContext does
  ///                      not support secret key I/O using seeds.
  void saveSecretKeyToFile(const std::string& fileName, bool seedOnly = false);

  /// load secret key from the given file.
  /// @param fileName The path of the file to load from.
  /// @param seedOnly If true, the seed of the secret key will be loaded from
  ///                 the given file and the secret key will be generated using
  ///                 this seed.
  /// @throw runtime_error if this HeContext already has a secret key. i.e.
  ///        hasSecretKey() is ture.
  /// @throw runtime_error If seedOnly is true but the underlying HeContext does
  ///                      not support secret key I/O using seeds.
  void loadSecretKeyFromFile(const std::string& fileName,
                             bool seedOnly = false);

  /// Returns the default device of the ciphertexts and plaintexts generated
  /// using this HeContext.
  virtual inline DeviceType getDefaultDevice() const { return defaultDevice; }

  /// Sets the default device of the ciphertexts and plaintexts generated using
  /// this HeContext.
  ///  @param[in] device the new default device
  virtual void setDefaultDevice(DeviceType device) { defaultDevice = device; }

  /// Returns default scale used in encoding (where applicable).
  virtual inline double getDefaultScale() const { return defaultScale; }

  /// Sets the default scale to be used in encoding (where applicable).
  ///  @param[in] v the default scale
  virtual void setDefaultScale(double v) { defaultScale = v; }

  /// Returns the name of the underlying library
  virtual std::string getLibraryName() const = 0;

  /// Returns the name of the underlying scheme
  virtual std::string getSchemeName() const = 0;

  /// Returns a signature of the context that distinguishes it enough to be able
  /// to load previously stored contexts based on their signatures.
  virtual std::string getSignature() const { return getSchemeName(); };

  /// For internal use
  inline virtual std::map<std::string, int64_t> getEstimatedMeasures() const
  {
    throw std::runtime_error("not implemented");
  };

  ///@brief Returns the maximal absolute value allowed for each chain index.
  /// Values lower than the threshold are guaranteed to not preduce overflows.
  /// The value in position i corresponds to the limit for chain index i.
  ///
  virtual std::vector<double> getMaxAllowedValues() const;

  ///@brief Returns the maximal absolute value allowed for values that may be in
  /// any chain index within a range that's defined between a given low and high
  /// chain indexes.
  ///
  ///@param high The higher chain index in the range.
  ///@param low The lower chain index in the range.
  virtual double getMaxAllowedValueByRange(int high, int low) const;

  ///@brief Returns the maximal absolute value allowed for values in a given
  /// chain index.
  ///
  ///@param chainIndex The given chain index.
  virtual double getMaxAllowedValue(int chainIndex) const;

  ///@brief Returns the maximal absolute value allowed for values going through
  /// a bootstrap operation.
  ///
  virtual double getMaxAllowedBsValue() const;

  /// Registers a context object for the purpose of dynamic loading.
  /// Don't call this directly. Use REGISTER_CONTEXT (see above)
  static bool internalRegisterContext(const HeContext* context);

  /// Returns an uninitialized context of the same type. Used for dynamic
  /// type loading among others.
  virtual std::shared_ptr<HeContext> clone() const;

  /// @brief Returns the accurate scale to be used for a given chain index.
  /// Only supported when accurate scaling mode is enabled. Cannot be called
  /// before the context is initialized.
  ///
  /// @param chainIndex the chain index to get the scale of.
  /// @throw runtime_error If this SealCkksContext is not initialized.
  /// @throw runtime_error If accurate scaling is not enabled.
  virtual double getAccurateScale(int chainIndex) const;

  /// @brief Returns the context id that identifies the specific key set used by
  /// this context.
  int32_t getContextId() const { return contextId; }

  ///@brief Sets the \a automaticBootstrapping flag to the given \p val. When
  /// \a automaticBootstrapping is set to true, ciphertexts created using this
  /// HeContext will be bootstrapped automatically when needed.
  ///
  ///@param val The value to set
  inline void setAutomaticBootstrapping(bool val)
  {
    if (val && !bootstrappable) {
      throw std::runtime_error(
          "This HeContext does not support bootstrapping.");
    }

    automaticBootstrapping = val;
  }

  ///@brief Returns the value of the \a automaticBootstrapping flag
  ///
  inline bool getAutomaticBootstrapping() const
  {
    return automaticBootstrapping;
  }

  ///@brief Assuming this HeContext is bootstrappable, returns the minimum level
  /// in which ciphertexts can be bootstrapped.
  virtual int getMinChainIndexForBootstrapping() const;

  ///@brief Assuming this HeContext is bootstrappable, returns the level of
  /// ciphertexts after bootstrapping.
  virtual int getChainIndexAfterBootstrapping() const;

  ///@brief Returns a bootstrap config object with two chain index values: the
  /// minimum level in which ciphertexts can be bootstrapped and the level of
  /// ciphertexts after bootstrapping.
  ///
  /// @param req The requirement, must indicate a bootstrappable object.
  virtual BootstrapConfig getBootstrapChainIndexes(
      const HeConfigRequirement& req) const;

  ///@brief Returns the minimal number of slots supported by this HE context
  ///
  virtual int getMinSupportedNumSlots() const;

  ///@brief Returns the maximal number of slots supported by this HE context
  ///
  virtual int getMaxSupportedNumSlots() const;

  ///@brief Returns a PublicFunctions object specifying the functionalities that
  /// are supported by the public keys of this HeContext
  inline const PublicFunctions& getPublicFunctions() const
  {
    return publicFunctions;
  }

  ///@brief Returns whether this HeContext is initialized
  inline bool isInitialized() const { return initialized; }

  ///@brief Returns whether the given PublicFunctions object is supported by
  /// this HeContext. The given PublicFunctions object is assumed to be valid
  /// (see also PublicFunctions::validate()).
  ///
  ///@param publicFunctions The PublicFunctions object.
  virtual bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const = 0;

  ///@brief Verifies that this HeContext supports calling save() method with
  /// the given PublicFunctions object.
  ///@param publicFunctions The PublicFunctions object.
  ///@throw invalid_argument If saving the given PublicFunctions object is not
  /// supported by this HeContext.
  virtual void assertSavePublicFunctionsSupported(
      const PublicFunctions& publicFunctions) const;

  ///  @brief Saves this HeContext object to a file in binary form.
  ///  Returns the number of bytes written to the saved file.
  ///
  ///  @param fileName name of file to write to
  ///  @throw runtime_error    If this Hecontext is not initialized.
  std::streamoff saveToFile(const std::string& fileName) const;

  ///  @brief Saves this HeContext to a file in binary form. Returns the number
  ///  of bytes written to the saved file. \p publicFunctions determines which
  ///  of the public keys should be saved. Upon loading the saved HeContext,
  ///  only the public functionalities specified in the given PublicFunctions
  ///  object will be supported. This function assumes that this HeContext
  ///  supports saving with the given PublicFunctions object (see also \a
  ///  savePublicFunctionsSupported()).
  ///
  ///  @param fileName        Name of file to write to
  ///  @param publicFunctions Determines which of the public keys should be
  ///                         saved.
  ///  @throw invalid_arguemnt If this HeContext does not support saving with
  ///                          the given publicFunctions argument.
  ///  @throw runtime_error    If this Hecontext is not initialized.
  std::streamoff saveToFile(const std::string& fileName,
                            const PublicFunctions& publicFunctions) const;

  ///  @brief Loads this Saveable object from a file saved by saveToFile().
  ///  Returns the number of bytes read from the loaded file.
  ///
  ///  @param[in] fileName name of file to read from
  std::streamoff loadFromFile(const std::string& fileName);

  ///  @brief Saves this HeContext to a stream in binary form.
  ///  Returns the number of bytes written to the output stream.
  ///
  ///  @param[in] stream output stream to write to
  std::streamoff save(std::ostream& stream) const;

  ///  @brief Saves this HeContext to a stream in binary form.
  ///  \p publicFunctions determines which of the public keys should be saved.
  ///  Upon loading the saved HeContext, only the public functionalities
  ///  specified in the given PublicFunctions object will be supported. This
  ///  function assumes that this HeContext supports saving with the given
  ///  PublicFunctions object (see also \a savePublicFunctionsSupported()).
  ///
  ///  @param stream output   stream to write to.
  ///  @param publicFunctions Determines which of the public keys should be
  ///                         saved.
  ///  @throw invalid_arguemnt If this HeContext does not support saving with
  ///                          the given publicFunctions argument.
  ///  @throw runtime_error    If this Hecontext is not initialized.
  std::streamoff save(std::ostream& stream,
                      const PublicFunctions& publicFunctions) const;

  ///  Loads this object from the given stream.
  ///  Returns the number of bytes read from the input stream.
  ///
  ///  @param[in] stream input stream to read from
  std::streamoff load(std::istream& stream);

  ///@brief Returns registered context given a context name.
  ///
  ///@param contextName The context name (library + scheme).
  static std::shared_ptr<HeContext> getRegisteredHeContext(
      const std::string& contextName);

  ///@brief Returns indication to whether a registered context exists under a
  /// given context name.
  ///
  ///@param contextName The context name (library + scheme).
  static bool isRegisteredHeContext(const std::string& contextName);
};
} // namespace helayers

#endif /* SRC_HELAYERS_HECONTEXT_H */
