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

#ifndef BOOTSTRAP_HEAANEVALUATOR_H_
#define BOOTSTRAP_HEAANEVALUATOR_H_

#include "helayers/hebase/heaan/HeaanCiphertext.h"
#include "helayers/math/bootstrap/impl/AbstractBootstrapEvaluator.h"

namespace helayers {

class HeaanBootstrapEvaluator : public AbstractBootstrapEvaluator
{
public:
  HeaanBootstrapEvaluator(HeaanContext& he, const BootstrapConfig& bsConfig);
  ~HeaanBootstrapEvaluator() {}

  void bootstrap(CTile& c) const override;
};
} // namespace helayers

#endif /* BOOTSTRAP_HEAANEVALUATOR_H_ */
