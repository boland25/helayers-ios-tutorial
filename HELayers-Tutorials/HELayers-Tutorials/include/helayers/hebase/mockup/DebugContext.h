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

#ifndef SRC_HELAYERS_DEBUGCONTEXT_H_
#define SRC_HELAYERS_DEBUGCONTEXT_H_

#include "helayers/hebase/HeContext.h"

namespace helayers {

/// @brief A class to represent an HeContext meant for debug purposes. The
/// CTiles it creates wrap two ciphertexts, encrypted with two possibly
/// different schemes. The difference between the values of the two ciphertexts
/// is checked after each operation, and in case this difference surpasses
/// a certain certain threshold, an error will be thrown. This class can be
/// used, for example, to wrap a MockupCiphertext along with another ciphertext
/// and verify that the values of the later ciphertext are close to the mockup
/// one after each operation.
class DebugContext : public HeContext
{

  /// The first HeContext.
  HeContext& h1;
  /// The second HeContext.
  HeContext& h2;

  /// The maximal allowed difference between the values of two ciphertexts
  /// of a DebugCiphertext created with this DebugContext.
  double diffThreshold;

  /// Whether to print detailed log after each operation
  bool printLog = false;

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;

  void loadImpl(std::istream& out) override;

  friend class DebugCiphertext;
  friend class DebugPlaintext;
  friend class DebugEncoder;

public:
  /// @brief A constructor.
  /// @param h1 The first HeContext.
  /// @param h2 The second HeContext.
  /// @param diffThreshold The maximal allowed difference between the values of
  /// two ciphertexts of a DebugCiphertext created with this DebugContext.
  /// @param printLog Whether to print a log of operations.
  DebugContext(HeContext& h1,
               HeContext& h2,
               double diffThreshold = 1e-5,
               bool printLog = false);

  virtual ~DebugContext();

  /// @brief initializes the two HeContexts of this DebugContext, using the
  /// given requirements.
  void init(const HeConfigRequirement& req) override;

  /// @brief Returns a pointer to a DebugCiphertext, initialized with this
  /// DebugContext.
  std::shared_ptr<AbstractCiphertext> createAbstractCipher() const override;

  /// @brief Returns a pointer to a DebugPlaintext, initialized with this
  /// DebugContext.
  std::shared_ptr<AbstractPlaintext> createAbstractPlain() const override;

  /// @brief Returns a pointer to a DebugEncoder, initialized with this
  /// DebugContext.
  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  /// @brief The number of slots in each CTile (ciphertext) or PTile
  /// (plaintext) created over this context.
  int slotCount() const override;

  /// @brief Returns the highest available chain index (for schemes where it is
  /// applicable).
  int getTopChainIndex() const override;

  /// @brief Returns whether this context contains a secret key.
  /// If not, decryption and other operations relying on decryption
  /// will not be available (will throw an exception).
  bool hasSecretKey() const override;

  /// @brief Returns the security level supplied by this context.
  int getSecurityLevel() const override;

  /// @brief Returns a signature that identifies the first HeContext wrapped
  /// with this DebugContext.
  std::string getSignature() const override;

  /// @brief Prints a summary of library details and configuration params of
  /// the two wrapped HeContexts.
  ///
  /// @param out output stream to write to
  void printSignature(std::ostream& out) const override;

  /// @brief Prints detailed information for debug purposes.
  /// @param title   Title to print along with the information.
  /// @param verbose Verbosity level
  /// @param out     Output stream to print to
  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  /// @brief Returns the modulus chain of the first HeContext wrapped by this
  /// DebugContext.
  std::shared_ptr<std::vector<uint64_t>> getModulusChain() const override;

  /// @brief Saves the secret keys of the two wrapped HeContexts to a stream
  /// in binary form.
  /// @param out Output stream to write to.
  void saveSecretKey(std::ostream& out, bool seedOnly = false) override;

  /// @brief Loads the secret keys of the two wrapped HeContexts from the
  /// given istream,
  /// @param in Input stream to load frin.
  void loadSecretKey(std::istream& in, bool seedOnly = false) override;

  /// @brief Returns the maximal allowed difference between the values of two
  /// ciphertexts of a DebugCiphertext created with this DebugContext.
  inline double getDiffThreshold() const { return diffThreshold; }

  /// @brief Returns whether log is printed
  inline bool getPrintLog() const { return printLog; }

  /// @brief Sets the maximal allowed difference threshold to the given value.
  /// This threshold specifies the maximal allowed difference between the
  /// values of two ciphertexts o a DebugCiphertext created with this
  /// DebugContext.
  /// @param v The difference threshold value to be set.
  inline void setDiffThreshold(double v) { diffThreshold = v; }

  /// @brief Sets log-printed flag value
  inline void setPrintLog(bool val) { printLog = val; }

  /// @brief Sets the default scale of the two wrapped HeContexts to the given
  /// value.
  /// @param v The default scale value to be set.
  void setDefaultScale(double v) override;

  /// @brief Returns the default scale.
  inline double getDefaultScale() const override
  {
    return h1.getDefaultScale();
  }

  /// @brief Returns a string specifying the names of the underlying libraries.
  std::string getLibraryName() const override
  {
    return "DEBUG:" + h1.getLibraryName() + ":" + h2.getLibraryName();
  }

  /// @brief Returns a string specifying the names of the underlying schemes.
  std::string getSchemeName() const override
  {
    return "DEBUG:" + h1.getSchemeName() + ":" + h2.getSchemeName();
  }

  /// @brief Returns true if the received configuration requirement is feasible
  /// under the first HeContext wrapped with this DebugContext, and false
  /// otherwise.
  /// @param req The requirement to check its feasibility.
  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  /// @brief Returns the best feasible fraction part precision under the given
  /// configuration requirement and according to the first HeContext wrapped
  /// with this DebugContext.
  /// @brief req The requirement to get best fractional part precision for.
  int getBestFeasibleFractionalPartPrecision(
      const HeConfigRequirement& req) const override;

  /// @brief Returns the best feasible multiplication depth under the given
  /// configuration requirement and according to the first HeContext wrapped
  /// with this DebugContext.
  /// @brief req The requirement to get best multiplication depth for.
  int getBestFeasibleMulDepth(const HeConfigRequirement& req) const override;

  /// @brief Returns the first HeContext.
  inline const HeContext& getFirstContext() const { return h1; }

  /// @brief Returns the second HeContext.
  inline const HeContext& getSecondContext() const { return h2; }

  /// See parent method
  double getAccurateScale(int chainIndex) const override;

  ///@brief Returns true if the given PublicFunctions object is supported by
  /// the first HeContexts and false otherwise.
  ///@param publicFunctions The publicFunctions object.
  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_DEBUGCONTEXT_H_ */
