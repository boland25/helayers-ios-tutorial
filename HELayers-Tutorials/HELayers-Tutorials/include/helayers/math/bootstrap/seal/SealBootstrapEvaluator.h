/*******************************************************************************
 *
 *   OCO Source Materials
 *   5737-A56
 *   © Copyright IBM Corp. 2017
 *
 *   The source code for this program is not published or other-wise divested
 *   of its trade secrets, irrespective of what has been deposited with the
 *   U.S. Copyright Office.
 ******************************************************************************/

#ifndef BOOTSTRAP_SEALEVALUATOR_H_
#define BOOTSTRAP_SEALEVALUATOR_H_

#include "helayers/math/bootstrap/impl/AbstractBootstrapEvaluator.h"
#include "SealBootstrapUtils.h"

namespace helayers {

class SealBootstrapUtils;

class SealBootstrapEvaluator : public AbstractBootstrapEvaluator
{
  SealBootstrapUtils* bsUtils = nullptr;

  Encoder encoder;

  int slotCount;

  uint64_t firstMod = 0;

public:
  SealBootstrapEvaluator(HeContext& he, const BootstrapConfig& bsConfig);

  ~SealBootstrapEvaluator();

  void bootstrap(CTile& c) const override;

  void bootstrapMockup(CTile& c) const override;

  SealBootstrapUtils* getBootstrapUtils() const { return bsUtils; }

private:
  ///@brief Drop the input ciphertext to lower level in the prime chain
  ///@param c The ciphertext to drop
  void dropChainIndexTo0(CTile& c) const;

  ///@brief Raise the input ciphertext to highest level in the prime chain
  ///@param c The ciphertext to raise
  void remod(CTile& c) const;

  ///@brief Manipulate input ciphertext so that plaintext coefs become slots
  /// Returns the result in two ciphertext for the two halves of the vector
  ///@param c The ciphertext t manipulate
  std::pair<CTile, CTile> coef2slot(CTile& c) const;

  ///@brief Perform the bootstrap mod step
  ///@param c the ciphertext to compute the mod on
  void mod(CTile& c) const;

  ///@brief Perform the bootstrap mod step where slots may be complex
  ///@param c the ciphertext to compute the mod on
  void modComplex(CTile& c1, CTile& c2) const;

  ///@brief Perform the bootstrap mod step using math sin
  ///       instead of sin approximation
  ///@param c the ciphertext to compute the mod on
  void modByMockSin(CTile& c) const;

  ///@brief Perform the bootstrap mod step using math mod
  ///       instead of mod approximation
  ///@param c the ciphertext to compute the mod on
  void modByMockMod(CTile& c) const;

  ///@brief Manipulate input ciphertext so that slots become plaintext coefs
  ///@param c The ciphertext t manipulate
  void slot2coef(CTile& c1) const;

  ///@brief Manipulate input ciphertext so that slots become plaintext coefs
  ///       in the case where the original raw-data was complex
  ///@param c The ciphertext t manipulate
  void slot2coefComplex(CTile& c1, CTile& c2) const;

  ///@brief Bootstrap the input ciphertext, assumed to encrypt only reals
  ///@param c the ciphertext to bootstrap
  void bootstrapReal(CTile& c) const;

  ///@brief Bootstrap the input ciphertext, which may encrypt complex numbers
  ///@param c the ciphertext to bootstrap
  void bootstrapComplex(CTile& c) const;

  ///@brief Bootstrap the input ciphertext,
  ///       assumed to encrypt the same real in all the slots
  ///@param c the ciphertext to bootstrap
  void bootstrapDuplicateReals(CTile& c) const;
};
} // namespace helayers

#endif /* BOOTSTRAP_SEALEVALUATOR_H_ */
