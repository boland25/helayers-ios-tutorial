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

#ifndef SRC_HELAYERS_CTILEMINTREE_H
#define SRC_HELAYERS_CTILEMINTREE_H

#include "helayers/hebase/hebase.h"

namespace helayers {

class FunctionEvaluator;

///@brief A struct representing an indicator. It can either be set to
/// zero or one (in plaintext), or contain an encrypted value.
struct Indicator
{
  /// An HeContext
  const HeContext& he;

  /// The encrypted indicator value. If the indicator is set to plaintext zero
  /// or one, this CTile will be empty.
  CTile val;

  /// Whether this indicator is equal to plaintext one
  bool isOne = true;

  /// Whether this indicator is equal to plaintext zero
  bool isZero = false;

  ///@brief A constructor. Constructs an indicator which is initialized to
  /// plaintext one.
  ///
  ///@param he An HeContext.
  Indicator(const HeContext& he) : he(he), val(he) {}

  ///@brief Asserts that this Indicator is valid. An indicator is considered
  /// valid if exactly one of the following conditions hold: "isOne" flag is
  /// set, "isZero" flag is set or "val" is not empty.
  void validate() const;

  ///@brief Multiplies this Indicator with the given other Indicator.
  ///
  ///@param other The other Indicator
  void multiply(const Indicator& other);

  ///@brief Negates the value of this Indicator
  void negate();

  ///@brief Multiplies this indicator with the negated value of the given other
  /// Indicator
  ///
  ///@param other The other indicator
  void multiplyNeg(const Indicator& other);
};

///@brief A class used to compute minimum or maximum of ciphertexts
class MinMaxEvaluator
{
  /// The HeContext
  const HeContext& he;

  /// A FunctionEvlauator object
  const FunctionEvaluator& fe;

  ///@brief Computes min(a,b), stores it in minRes and stores the minimum
  /// indicator in "minIndicator" (1 if a < b and 0 otherwise).
  ///
  ///@param minRes           min(a,b) will be stored here.
  ///@param minIndicator     The resulting minimum indicator. This will contain
  ///                        an encryption of 1 if a < b and 0 otherwise.
  ///@param a                The first CTile for the minimum computation.
  ///@param b                The second CTile for the minimum computation.
  ///@param gRep,fRep        Determine the accuracy of the result. Greater gRep
  ///                        and fRep will result with a greater accuracy on the
  ///                        account of slower runtime and more multiplication
  ///                        depth.
  ///@param maxDiff          The maximum possible absolute difference between a
  ///                        and b.
  ///@param delayMulByHalf   If true, minRes will contain two times min(a,b) and
  ///                        the function will consume one less chain index in
  ///                        "minRes".
  void min(CTile& minRes,
           Indicator& minIndicator,
           const CTile& a,
           const CTile& b,
           int gRep,
           int fRep,
           double maxDiff = 1,
           bool delayMulByHalf = false) const;

public:
  ///@brief A constructor.
  ///
  ///@param he The HeContext.
  ///@param fe A functionEvaluator.
  MinMaxEvaluator(const HeContext& he, const FunctionEvaluator& fe);

  ///@brief Computes min(a,b), stores it in minRes and stores the minimum
  /// indicator in "minIndicator" (1 if a < b and 0 otherwise).
  ///
  ///@param minRes           min(a,b) will be stored here.
  ///@param minIndicator     The resulting minimum indicator. This will contain
  ///                        an encryption of 1 if a < b and 0 otherwise.
  ///@param a                The first CTile for the minimum computation.
  ///@param b                The second CTile for the minimum computation.
  ///@param gRep,fRep        Determine the accuracy of the result. Greater gRep
  ///                        and fRep will result with a greater accuracy on the
  ///                        account of slower runtime and more multiplication
  ///                        depth.
  ///@param maxDiff          The maximum possible absolute difference between a
  ///                        and b.
  ///@param delayMulByHalf   If true, minRes will contain two times min(a,b) and
  ///                        the function will consume one less chain index in
  ///                        "minRes".
  void min(CTile& minRes,
           CTile& minIndicator,
           const CTile& a,
           const CTile& b,
           int gRep,
           int fRep,
           double maxDiff = 1,
           bool delayMulByHalf = false) const;

  ///@brief Stores the minimum of "cs" CTiles in "minRes", and stores in
  /// minIndicators[i] a CTile contianing 1 if cs[i] is the minimum and 0
  /// otherwise.
  ///@param minRes        The minimum of "cs" vector will be stored here
  ///@param minIndicators A vector whose i-th element will contain an encryption
  ///                     of 1 if the i-th CTile in "cs" is the minimal one and
  ///                     will contain an encryption of 0 otherwise.
  ///@param cs            The CTiles to compute their minimum.
  ///@param gRep,fRep     Determine the accuracy of the result. Greater gRep and
  ///                     fRep will result with a greater accuracy on the
  ///                     account of slower runtime and more multiplication
  ///                     depth.
  ///@param maxDiff       The maximum possible absolute difference between two
  ///                     values in cs[i].
  void min(CTile& minRes,
           std::vector<CTile>& minIndicators,
           const std::vector<CTile>& cs,
           int gRep,
           int fRep,
           double maxDiff = 1) const;
};
} // namespace helayers

#endif