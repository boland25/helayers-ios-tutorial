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

#ifndef SRC_HELAYERS_HELIBCONTEXT_H_
#define SRC_HELAYERS_HELIBCONTEXT_H_

#include <memory>
#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/seal/SealCkksContext.h"
#include "helib/helib.h"
#include "HelibConfig.h"

namespace helayers {

///@brief An implementation of HeContext API with concrete subclasses for
/// HElib scheme.
class HelibContext : public HeContext
{
protected:
  /// Holds parameters that are used to configure this HeContext.
  HelibConfig config;

  /// The Context object of the underlying helib library.
  std::shared_ptr<helib::Context> context;

  /// The secret key, or nullptr if this context holds only a public key
  std::shared_ptr<helib::SecKey> secretKey;

  /// The public key
  std::shared_ptr<helib::PubKey> publicKey;

  /// The number of slots of ciphertexts and plaintexts created by this
  /// HelibContext.
  long nslots = 0;

  /// Whether the slot values should be mirrored before encoding them into a
  /// a plaintext. If true, the i-th slot will represent the he.slotCount() - i
  /// encoded value.
  bool mirrored = false;

  /// @brief Saves this helibContext to the given ostream.
  /// @param out The ostream to save to
  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;

  /// @brief Loads this helibContext from the given ostream.
  /// @param in The istream to load from.
  void loadImpl(std::istream& in) override;

public:
  /// @brief A constructor.
  HelibContext();

  virtual ~HelibContext();

  /// @brief Not yet implemented.
  /// @throw runtime_error When called.
  void init(const HeConfigRequirement& req) override;

  ///@brief Creates a new HelibContext for either CKKS or BGV, based on a preset
  /// configuration. See list of presets in HelibConfig.h
  ///
  ///@param preset Preset configuration
  static std::shared_ptr<HelibContext> create(HelibPreset preset);

  ///@brief Initalizes with a given preset. See list of presets in HelibConfig.h
  ///
  ///@param preset Preset configuration name
  void initPreset(HelibPreset preset);

  ///@brief Initialize context with given configuration.
  ///
  ///@param conf Configuration details
  virtual void init(const HelibConfig& conf);

  /// @brief Returns -1.
  int getTopChainIndex() const override;

  /// @brief returns the number of slots in plaintexts and ciphertexts created
  /// by this HelibContext.
  inline int slotCount() const override { return nslots; }

  /// @brief Returns the security level.
  inline int getSecurityLevel() const override
  {
    return (int)(context->securityLevel());
  }

  /// @brief Returns true if this HelibContext has a secret key and false
  /// otherwise.
  inline bool hasSecretKey() const override { return (bool)secretKey; }

  /// @brief Returns the public key.
  inline const helib::PubKey& getPublicKey() const { return *publicKey; }

  /// @brief Returns the secret key.
  /// @throw runtime_error If this HelibContext does not have a secret key.
  inline const helib::SecKey& getSecretKey() const
  {
    if (!hasSecretKey())
      throw std::runtime_error("This context does not have a secret key");
    return *secretKey;
  }

  /// @brief Returns the Context object of the underlying helib library.
  inline const helib::Context& getContext() const { return *context; }

  /// @brief Returns true if the slots of plaintexts encoded with this
  /// helibContext will be mirrored and false otherwise.
  inline bool getMirrored() const { return mirrored; }

  /// @brief Sets or unsets the option of mirroring plaintext slot values
  /// before encoding.
  inline void setMirrored(bool v) { mirrored = v; }

  /// @brief Prints the signature of this helibContext.
  /// @param out The ostream to print the signature to.
  void printSignature(std::ostream& out = std::cout) const override;

  /// @brief prints a debug message to the given ostream.
  /// @param title   The title of the message to be printed.
  /// @param verbose Verbositiy level. This parameter currently has no effect.
  /// @param out     The ostream to write to.
  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  /// @brief Save secret key to the given ostream.
  /// @param out The binary stream to save to.
  ///
  /// @throw runtime_error if this HelibCotext doesn't have a secret key. i.e.
  ///        hasSecretKey() is false.
  void saveSecretKey(std::ostream& out, bool seedOnly = false) override;

  /// @brief Load secret key from the given istream.
  /// @param in The binary stream to load from.
  /// @throw runtime_error if this HelibContext already has a secret key. i.e.
  ///        hasSecretKey() is ture.
  void loadSecretKey(std::istream& in, bool seedOnly = false) override;

  /// Returns the name of the underlying library
  std::string getLibraryName() const override { return "HELIB"; }

  /// Internal use
  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_HELIBCONTEXT_H_ */
