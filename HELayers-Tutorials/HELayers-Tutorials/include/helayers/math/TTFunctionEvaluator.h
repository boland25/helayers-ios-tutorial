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

#ifndef SRC_HELAYERS_TTFUNCTIONEVALUATOR_H
#define SRC_HELAYERS_TTFUNCTIONEVALUATOR_H

#include "FunctionEvaluator.h"
#include "CTileTensor.h"

namespace helayers {

/// @brief This class is used to evaluate different available functions on
/// CTileTensor objects, such as polynomial evaluation and sigmoid.
class TTFunctionEvaluator
{
  HeContext& he;

  FunctionEvaluator fe;

public:
  ///@brief Construct a new TT function evaluator object
  ///
  ///@param he the context
  TTFunctionEvaluator(HeContext& he);

  ///@brief Destroy the TT function evaluator object
  ///
  ~TTFunctionEvaluator();

  ///@brief Deleted copy constructor
  ///
  ///@param src Source object
  TTFunctionEvaluator(const TTFunctionEvaluator& src) = delete;

  ///@brief Deleted operator=
  ///
  ///@param src Source object
  TTFunctionEvaluator& operator=(const TTFunctionEvaluator& src) = delete;

  ///@brief The same as FunctionEvaluator::polyEvalInPlace, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::polyEvalInPlace.
  void polyEvalInPlace(CTileTensor& src,
                       const std::vector<double>& coefs,
                       EvalType type = DEFAULT) const;

  ///@brief The same as FunctionEvaluator::polyEvalInPlace, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::polyEvalInPlace.
  void polyEvalInPlace(CTileTensor& src,
                       const std::vector<CTile>& coefs,
                       bool normalized = false) const;

  ///@brief The same as FunctionEvaluator::powInPlace, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::powInPlace.
  void powInPlace(CTileTensor& src, int degree) const;

  ///@brief The same as FunctionEvaluator::sigmoid3InPlace, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::sigmoid3InPlace.
  /// @param src The input tensor
  void sigmoid3InPlace(CTileTensor& src) const;

  ///@brief The same as FunctionEvaluator::sigmoid7InPlace, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::sigmoid7InPlace.
  void sigmoid7InPlace(CTileTensor& src) const;

  ///@brief The same as FunctionEvaluator::sigmoid9InPlace, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::sigmoid9InPlace.
  void sigmoid9InPlace(CTileTensor& src) const;

  ///@brief The same as FunctionEvaluator::signInPlace, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::signInPlace.
  void signInPlace(CTileTensor& src, int gRep, int fRep) const;

  ///@brief The same as FunctionEvaluator::inverse, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::inverse.
  void inverse(CTileTensor& src,
               double lowerBound,
               double upperBound,
               int bitResolution = 5) const;

  ///@brief The same as FunctionEvaluator::sqrt, but works on a
  /// CTileTensor object as source, CTile-wise. For more details, see
  /// documentation of FunctionEvaluator::sqrt.
  void sqrt(CTileTensor& src, int bitResolution) const;

  ///@brief Given a one dimensional (possibly interleaved) CTileTensor src of
  /// shape [n/t, 1] (or [n~/t, 1]), return a CTileTensor res of shape [1/t, n]
  /// (or [1~/t, n]) s.t. res[0,i] = src[i,0].
  /// @param src one dimensional binary CTileTensor.
  void reshapeVectorHorizontalToVertical(CTileTensor& res,
                                         const CTileTensor& src) const;

  ///@brief see partialSumsIndicatorsGetLayer
  ///
  ///@param res an empty CTileTensor
  ///@param indicators one dimensional interleaved CTileTensor of shape
  ///[1~ / slotCount, n] that contains binary values (either 0 or 1)
  void partialSumsIndicatorsGetLayerZero(CTileTensor& res,
                                         const CTileTensor& indicators) const;

  ///@brief see partialSumsIndicatorsGetLayer
  ///
  ///@param res an empty CTileTensor
  ///@param indicators one dimensional interleaved CTileTensor of shape
  ///[1~ / slotCount, n] that contains binary values (either 0 or 1)
  void partialSumsIndicatorsGetLayerOne(CTileTensor& res,
                                        const CTileTensor& indicators) const;

  /// @brief Given a one dimensional interleaved CTileTensor indicators of shape
  ///[1~ / slotCount, n] that contains binary values (either 0 or 1) and an
  /// integer len>=2, calculates
  /// a CTileTensor of shape [n~ / slotCount, n] s.t. res[i,j]=1 iff
  /// indicators[i:i+len] contains exactly len 1's, and 0 otherwise. To run this
  /// function with len=n you must run it before with len=n-1, and transfer to
  /// this function the result of the previous run (denoted by prevLayer).
  /// Furthermore, you need to transfer the result of
  /// partialSumsIndicatorsGetLayerOne which returns the result for len=1.
  ///
  /// @param res an empty CTileTensor
  /// @param layerOne the result of partialSumsIndicatorsGetLayerOne
  /// @param prevLayer the result of running this function with len-1
  /// @param len number of elements to include in the partial sum calculation.
  /// len >= 2
  void partialSumsIndicatorsGetLayer(CTileTensor& res,
                                     const CTileTensor& layerOne,
                                     const CTileTensor& prevLayer,
                                     int len) const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_TTFUNCTIONEVALUATOR_H */
