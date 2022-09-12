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

#ifndef BOOTSTRAP_EVALUATOR_H_
#define BOOTSTRAP_EVALUATOR_H_

#include "helayers/hebase/hebase.h"
#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/CTile.h"

#include "BootstrapConfig.h"
#include "impl/AbstractBootstrapEvaluator.h"

namespace helayers {

class BootstrapConfig;
class BootstrapUtils;
class CTile;
class HeContext;

///@brief A helper class for performing bootstrap over ciphertexts
class BootstrapEvaluator
{
  std::shared_ptr<AbstractBootstrapEvaluator> impl;

public:
  ///@brief Construct a new Bootstrap Evaluator object
  ///
  ///@param he the context in which the bootstrap will be computed
  ///@param bsConfig the configuration of the bootstrap process
  BootstrapEvaluator(HeContext& he, const BootstrapConfig& bsConfig);

  ///@brief For internal use only.
  BootstrapEvaluator(HeContext& he,
                     const BootstrapConfig& bsConfig,
                     const std::shared_ptr<AbstractBootstrapEvaluator> impl);

  ~BootstrapEvaluator();

  ///@brief Returns the He Context object
  HeContext& getHeContext() { return impl->getHeContext(); }

  ///@brief Returns the Bootstrap Config object
  BootstrapConfig& getBootstrapConfig() { return impl->getBootstrapConfig(); }

  ///@brief Returns the Bootstrap Config object
  const BootstrapConfig& getBootstrapConfig() const
  {
    return impl->getBootstrapConfig();
  }

  ///@brief Bootstrap the input ciphertext.
  ///@param src the ciphertext to bootstrap
  void bootstrap(CTile& c) const;

  ///@brief Perform the bootstrap process directly on the original plain values
  /// This is faster than the FHE version and doesn't incur FHE related noise.
  ///@param src the ciphertext to bootstrap
  void bootstrapMockup(CTile& c) const;

  /// Reserved for debugging and internal use.
  AbstractBootstrapEvaluator& getImpl() { return *impl; };
};
} // namespace helayers

#endif /* BOOTSTRAP_EVALUATOR_H_ */
