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

#ifndef SRC_HELAYERS_HE_CONFIG_REQUIREMENT_H
#define SRC_HELAYERS_HE_CONFIG_REQUIREMENT_H

#include <optional>
#include "PublicFunctions.h"
#include "mockup/RunStats.h"
#include "utils/JsonWrapper.h"
#include "helayers/math/bootstrap/BootstrapConfig.h"

namespace helayers {

///@brief For internal use.
struct HeConfigRequirement : public Printable
{
  /// The required number of slots.
  int numSlots = -1;

  /// The required multiplication depth.
  int multiplicationDepth = -1;

  /// The required fractional part precision.
  int fractionalPartPrecision = -1;

  /// The required integer part precision.
  int integerPartPrecision = -1;

  /// The required security level.
  int securityLevel = 128;

  /// Whether CTiles created by the HeContext will be bootstrappable. Setting
  /// this flag to true will lead to the creation of a BootsrapEvaluator object
  /// upon the initialization of the HeContext. Note that this BootstrapEvalutor
  /// creation increases the runtime of the initialization.
  bool bootstrappable = false;

  /// Whether bootstrapping is applied automatically to prevent runtime errors
  /// resulting from executing a chain index consuming operation on a ciphertext
  /// with too small chain index. The logic for deciding the bootstrapping
  /// points is scheme and library specific. But, in general, the bootstrappings
  /// are delyed as much as possible. This flag can only be set to true if this
  /// HeContext supports bootstrapping.
  bool automaticBootstrapping = false;

  /// If \a bootstrappable flag is set to true, this optional member can be used
  /// to set the bootstrap configuration. If this member is empty and
  /// \p bootstrappable flag is true, a default bootstrap configuraition will be
  /// used.
  std::optional<BootstrapConfig> bootstrapConfig;

  /// This member specifies which functionalities will be supported by the
  /// public keys of the HeContext
  PublicFunctions publicFunctions;

  /// @brief A constructor.
  HeConfigRequirement();

  /// @brief A constructor.
  /// @param numSlots                The required number of slots.
  /// @param multiplicationDepth     The required multiplication depth.
  /// @param fractionalPartPrecision The required fractional part precision.
  /// @param integerPartPrecision    The required integer part precision.
  /// @param securityLevel           The required security level.
  HeConfigRequirement(int numSlots,
                      int multiplicationDepth,
                      int fractionalPartPrecision,
                      int integerPartPrecision,
                      int securityLevel = 128);

  ///@brief Returns an object with an insecure configuration.
  ///
  /// @param numSlots                The required number of slots.
  /// @param multiplicationDepth     The required multiplication depth.
  /// @param fractionalPartPrecision The required fractional part precision.
  /// @param integerPartPrecision    The required integer part precision.
  static HeConfigRequirement insecure(
      int numSlots,
      int multiplicationDepth = RunStats::MAX_CHAIN_INDEX,
      int fractionalPartPrecision = -1,
      int integerPartPrecision = -1);

  ///@brief Asserts the five main attributes of an HE config requirement are
  /// populated: securityLevel, numSlots, multiplicationDepth,
  /// fractionalPartPrecision and integerPartPrecision.
  ///
  void assertComplete() const;

  ///@brief Returns the number of rotation keys required by this HE config
  /// requirement.
  ///
  int getNumRotationKeys() const;

  void toJson(JsonWrapper& jw, const std::string& prefix) const;

  void fromJson(const JsonWrapper& jw, const std::string& prefix);

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;
};

///@brief For internal use.
struct BootstrapRequirement
{
  /// See in HeConfigRequirement above
  bool bootstrappable = false;

  /// See in HeConfigRequirement above
  bool automaticBootstrapping = false;

  /// See in HeConfigRequirement above
  std::optional<BootstrapConfig> bootstrapConfig;

  /// @brief A constructor.
  /// @param bootstrappable See in HeConfigRequirement above.
  /// @param automaticBootstrapping See in HeConfigRequirement above.
  /// @param bootstrapConfig See in HeConfigRequirement above.
  BootstrapRequirement(bool bootstrappable,
                       bool automaticBootstrapping,
                       std::optional<BootstrapConfig> bootstrapConfig);

  /// @brief A constructor.
  /// @param req An HeConfigRequirement object.
  BootstrapRequirement(const HeConfigRequirement& req);
};

} // namespace helayers

#endif /* SRC_HELAYERS_HE_CONFIG_REQUIREMENT_H */
