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

#ifndef BOOTSTRAP_ABSTRACTEVALUATOR_H_
#define BOOTSTRAP_ABSTRACTEVALUATOR_H_

#include "helayers/math/bootstrap/BootstrapConfig.h"
#include "helayers/hebase/hebase.h"
#include "AbstractBootstrapEvaluator.h"

namespace helayers {

///@brief Internal abstract implementation of a bootstrap evaluator, do not use
/// directly.
/// Used as parent class for concrete classes implementing bootstrap evaluators
/// in various schemes. Do not use directly, use a BootstrapEvaluator class
/// instead.
class AbstractBootstrapEvaluator
{
  HeContext& he;

  BootstrapConfig bsConfig;

public:
  AbstractBootstrapEvaluator(HeContext& he, const BootstrapConfig& bsConfig)
      : he(he), bsConfig(bsConfig)
  {}
  ~AbstractBootstrapEvaluator() {}
  AbstractBootstrapEvaluator(const AbstractBootstrapEvaluator& src) = delete;
  AbstractBootstrapEvaluator& operator=(const AbstractBootstrapEvaluator& src) =
      delete;

  HeContext& getHeContext() const { return he; };

  BootstrapConfig& getBootstrapConfig() { return bsConfig; }

  const BootstrapConfig& getBootstrapConfig() const { return bsConfig; }

  virtual void bootstrap(CTile& src) const = 0;

  inline virtual void bootstrapMockup(CTile& src) const
  {
    throw std::runtime_error("bootstrapMockup not implemented");
  }
};
} // namespace helayers

#endif /* BOOTSTRAP_ABSTRACTEVALUATOR_H_ */
