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

#ifndef BOOTSTRAP_MOCKUPEVALUATOR_H_
#define BOOTSTRAP_MOCKUPEVALUATOR_H_

#include "helayers/math/bootstrap/impl/AbstractBootstrapEvaluator.h"
#include "helayers/hebase/mockup/MockupContext.h"

namespace helayers {

class MockupBootstrapEvaluator : public AbstractBootstrapEvaluator
{
public:
  MockupBootstrapEvaluator(MockupContext& he, const BootstrapConfig& bsConfig);

  ~MockupBootstrapEvaluator();

  void bootstrap(CTile& c) const override;
};

} // namespace helayers

#endif /* BOOTSTRAP_MOCKUPEVALUATOR_H_ */
