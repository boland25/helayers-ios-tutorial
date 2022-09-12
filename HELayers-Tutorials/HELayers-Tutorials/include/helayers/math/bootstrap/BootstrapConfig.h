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

#ifndef BOOTSTRAP_CONFIG_H_
#define BOOTSTRAP_CONFIG_H_

#include <memory>
#include "helayers/hebase/utils/SaveableBasic.h"

namespace helayers {

class BootstrapEvaluator;
class SealBootstrapUtils;
class SealBootstrapEvaluator;

///@brief enum for controlling the range of supported values for bootstrapping
enum BootstrapRange
{
  ///@brief default bootstrapping range of the underlying library
  DEFAULT_RANGE,

  ///@brief supports bootstrapping in range [-1,1]
  UNIT_RANGE,

  ///@brief supports bootstrapping in extended range (differentiate across
  /// different libraries)
  EXTENDED_RANGE
};

/// A configuration of the Bootstrap process via various parameter setups
class BootstrapConfig : public SaveableBasic
{
  ///@brief Degree of Cheb. approximation polynomial
  int nT = 14;

public:
  ///@brief If true then the slots to be bootstrapped may contain complex values
  /// and the bootstrap will take more time.
  /// If you know that all the slots are real (with no imaginary component) then
  /// set this to false (the default).
  bool supportComplexData = false;

  ///@brief If true then the bootstrap will assume that all the slots contain
  /// the same value, and will run faster. The default is not to make this
  /// assumption.
  bool duplicateRealInput = false;

  ///@brief The bootstrap will at this level in the chain index
  int targetChainIndex = -1;

  ///@brief The bootstrap must be performed no lower than this level in the
  /// chain index
  int minChainIndexForBootstrapping = -1;

  ///@brief Verbosity of debug printing.
  Verbosity verbose = VERBOSITY_NONE;

  ///@brief Should the large FFT matrices be stored to file so that on the next
  /// run they will be loaded rather than re-computed?
  /// This slows down the first Bootstrap, but speeds up the subsequent runs.
  bool storeEncMatrices = true;

  ///@brief Should the large FFT matrices be loaded from a file if it exists?
  /// Loading is faster than recomputing, but requires setting the
  /// storeEncMatrices flag in the previous run.
  bool loadEncMatrices = true;

  ///@brief determines the range of supported values for bootstrapping
  BootstrapRange range = BootstrapRange::DEFAULT_RANGE;

  ///@brief Validates that the configuration is valid, and throws an error
  /// otherwise.
  ///@throw runtime_error
  void validate() const
  {
    if (!coef2slotViaFFT && slot2coefViaFFT)
      throw std::runtime_error(
          "!coef2slotViaFFT & slot2coefViaFFT not supported together");
  }

  std::streamoff save(std::ostream& stream) const override;
  std::streamoff load(std::istream& stream) override;

  friend bool operator==(const BootstrapConfig& bc1,
                         const BootstrapConfig& bc2);

  friend BootstrapEvaluator;
  friend SealBootstrapUtils;
  friend SealBootstrapEvaluator;

protected:
  bool slot2coefViaFFT = true;
  bool coef2slotViaFFT = true;
  bool modIncludesInputOutputFactors = false;
  bool matricesIncludeFactors = true;

  // the following are for test purposes
  bool usePureSin = false; // use math sin instead of estimated sin
  bool usePureMod = false; // use mod instead of sin
};

bool operator!=(const BootstrapConfig& bc1, const BootstrapConfig& bc2);

} // namespace helayers

#endif /* BOOTSTRAP_CONFIG_H_ */
