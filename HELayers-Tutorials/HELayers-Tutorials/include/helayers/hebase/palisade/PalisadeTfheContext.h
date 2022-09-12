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

#ifndef SRC_HELAYERS_PALISADE_PALISADETFHECONTEXT_H
#define SRC_HELAYERS_PALISADE_PALISADETFHECONTEXT_H

#include "helayers/hebase/HeContext.h"
#include "palisade.h"
#include "PalisadeTfheConfig.h"
#include "binfhecontext.h"

namespace helayers {
///@brief
/// A context class for Palisade's TFHE scheme
class PalisadeTfheContext : public HeContext
{

private:
  int numBits = 8;
  lbcrypto::BinFHEContext context;
  lbcrypto::LWEPrivateKey sk;
  PalisadeTfheConfig config;

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override
  {
    throw std::runtime_error("save not yet implemented");
  }

  void loadImpl(std::istream& in) override
  {
    throw std::runtime_error("load not yet implemented");
  }

public:
  PalisadeTfheContext();
  ~PalisadeTfheContext() {}

  ///@brief Initialize context with config object
  ///
  ///@param config
  void init(const PalisadeTfheConfig& config);

  ///@brief Returns number of bits that are contained in a single CTile
  ///
  ///@return int
  int getNumBits() const;

  std::shared_ptr<AbstractCiphertext> createAbstractCipher() const override;

  std::shared_ptr<AbstractPlaintext> createAbstractPlain() const override;

  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  void printSignature(std::ostream& out) const override;

  std::shared_ptr<AbstractBitwiseEvaluator> getBitwiseEvaluator()
      const override;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  int getTopChainIndex() const override;

  inline int slotCount() const override;

  int getSecurityLevel() const override;

  bool hasSecretKey() const override;

  std::string getLibraryName() const override;

  std::string getSchemeName() const override;

  ///@brief Returns the Palisade's context
  ///
  ///@return lbcrypto::BinFHEContext
  lbcrypto::BinFHEContext getContext() const;

  ///@brief Returns Palisade's TFHE secret ket
  ///
  ///@return lbcrypto::LWEPrivateKey
  lbcrypto::LWEPrivateKey getKey() const;

  void saveSecretKey(std::ostream& out, bool seedOnly) override
  {
    throw std::runtime_error("saveSecretKey not yet implemented");
  }

  void loadSecretKey(std::istream& in, bool seedOnly) override
  {
    throw std::runtime_error("loadSecretKey not yet implemented");
  }

  static std::shared_ptr<HeContext> loadHeContextFromFile(
      const std::string& fileName)
  {
    throw std::runtime_error("loadHeContextFromFile not yet implemented");
  }

  std::shared_ptr<HeContext> clone() const override
  {
    throw std::runtime_error("clone not yet implemented");
  }
  void init(const HeConfigRequirement& req) override
  {
    throw std::runtime_error("init not implemented");
  }

  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override
  {
    throw std::runtime_error("isConfigRequirementFeasible not implemented");
  }

  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;
};
} // namespace helayers
#endif