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

#ifndef BOOTSTRAP_PATERSONSTOCKMEYER_H_
#define BOOTSTRAP_PATERSONSTOCKMEYER_H_
const double CIEPSILON = 1e-15;

#include <vector>
#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/CTile.h"

namespace helayers {

/// This class implements:
/// 1) original Paterson-Stockmeyer algorithm.
/// 2) variation on the original Paterson-Stockmeyer that accept coffecients of
///    Chebyshev interpolant repesented with Chebyshev base.
///
/// References:
/// 1) Chen, Hao, Ilaria Chillotti, and Yongsoo
/// Song. "Improved bootstrapping for approximate homomorphic encryption."
/// Annual International Conference on the Theory and Applications of
/// Cryptographic Techniques. Springer, Cham, 2019.
///
/// 2) Paterson, Michael S., and Larry J. Stockmeyer. "On the number of
/// nonscalar multiplications necessary to evaluate polynomials." SIAM Journal
/// on Computing 2.1 (1973): 60-66.
class PatersonStockmeyer
{

public:
  ///@brief Pair of ctiles
  ///
  typedef std::pair<CTile, CTile> PairOfCiphers;

  ///@brief Vector of pairs of ctiles
  ///
  typedef std::vector<PairOfCiphers> VectorOfPairOfCiphers;

  ///@brief Pair of pairs of ctiles
  ///
  typedef std::pair<PairOfCiphers, PairOfCiphers> PairOfPairOfCiphers;

  /// Implementation of the original Paterson-Stockmeyer algorithm.
  /// Returns the polynomial value at the evaluation point.
  /// @param[in] f The polynomial's coefficients.
  /// @param[in] u The evaluation point.
  long double sp(const std::vector<long double>& f, long double u);

  /// Implementation for Chebyshev varient of Paterson-Stockmeyer algorithm.
  /// Returns the polynomial value at the evaluation point.
  /// @param[in] f The polynomial's coefficients.
  /// @param[in] u The evaluation point.
  long double spt(const std::vector<long double>& f, long double u);

  /// Horner's algorithm for evaluating a polynomial value at a point.
  /// @param[in] f The polynomial's coefficients.
  /// @param[in] u The evaluation point.
  static long double horner(const std::vector<long double>& f, long double u);

  /// Naive evaluation of a polynomial represented by Chebyshev base at a point.
  /// @param[in] f The coefficients of a polynomial represented by Chebyshev
  /// base.
  /// @param[in] u The evaluation point.
  static long double chebyshev(const std::vector<long double>& f,
                               long double u);

protected:
  // Non-recursive part of Paterson-Stockmeyer algorithm.
  int sp0(int n, long double u);

  // Recursive part of Paterson-Stockmeyer algorithm.
  long double sp1(const std::vector<long double>& f, long double u, int m);

  // Non-recursive part of Chebishev variant of Paterson-Stockmeyer algorithm.
  int spt0(int n, long double u);

  // Recursive part of Chebishev variant of Paterson-Stockmeyer algorithm.
  long double spt1(const std::vector<long double>& f, int m);

  std::vector<long double> bs;
  std::vector<long double> gs;
  std::vector<long double> prodGs;

private:
  // HeContext& he;
  int k;
  int mul_count;
  int mul_depth;

  /// clear fields needed for computing
  void clear_all();

  /// Takes two pairs, multiplies their first elements and takes the max of
  /// their
  /// second, which represent depth.
  /// The depth is for debugging the paterson stockmeyer functions.
  std::pair<long double, int> mul_pair(std::pair<long double, int> a,
                                       std::pair<long double, int> b);

  /// Evaluate the k'th Chebishev polynomial at point x.
  static long double T(int k, long double x);

  /// long division algorithm for polynomails representes by Chebyshev base.
  static std::pair<std::vector<long double>, std::vector<long double>> t_div(
      const std::vector<long double>& r,
      std::vector<long double> q);

  /// Add the monomial a*x^n to a polynomial f.
  static std::vector<long double> add_coef(const std::vector<long double>& f,
                                           int n,
                                           long double a);
};
} // namespace helayers

#endif /* BOOTSTRAP_PATERSONSTOCKMEYER_H_ */
