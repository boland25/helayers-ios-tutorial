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

#ifndef BOOTSTRAP_PATERSONSTOCKMEYERCHHE_H_
#define BOOTSTRAP_PATERSONSTOCKMEYERCHHE_H_
const double CIEPSILON1 = 1e-15;

#include <vector>
#include "helayers/math/bootstrap/seal/SealBootstrapUtils.h"
#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/CTile.h"

namespace helayers {

/// Evaluates a given polynomial at a given point homomorphically.
/// The algorithm being used is a variation on the original Paterson-Stockmeyer
/// that accept coffecients of Chebyshev interpolant repesented with Chebyshev
/// base.
///
/// There are 2 ways to use this class:
/// 1) call precompute and later call eval(coeff) multiple times.
/// 2) call only eval(coef, ctx) without calling precompute.
/// The fitst method is useful for use case of computing the same evalution
/// point in two different polynomials with the same degree. For example,
/// CKKS bootstrapping algorithm is evaluating an approximation with the same
/// degree of cosine and sinus at the same evaluation point.
///
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
class PatersonStockmeyerChHe
{

public:
  /// Constructs an empty object.
  /// @param[in] _he the HE library context
  /// @param[in] _bs the SealBootstrapUtils to use
  PatersonStockmeyerChHe(HeContext& _he, const SealBootstrapUtils& _bs)
      : he(_he), bs(_bs)
  {}
  /// Destructor.
  ~PatersonStockmeyerChHe(){};

  /// Precompute and store values in Bs and Gs vectors.
  /// This method should be called before calling eval(coef).
  /// @param[in] n  degree of the evaluated polynomial
  /// @param[in] ctx ciphertext of the evaluation point
  void precompute(int n, const CTile& ctx);

  /// Homomorphic evaluation of a ciphertext of a polynomial value at the
  /// ciphertext of a point.
  /// Returns ciphertext with the result of the evaluation of the polynomial at
  /// @param[in] coef a vector of polynomial coefficients
  /// @param[in] ctx ciphertext of the evaluation point
  /// the evaluation point
  CTile eval(const std::vector<long double>& coef, const CTile& ctx);

  /// Homomorphic evaluation of a ciphertext of a polynomial value at the point,
  /// A ciphertext of the evaluation point is specified by calling 'precompute'
  /// method before calling this method.
  ///
  /// Returns a ciphertext with the result of the evaluation of the polynomial
  /// at the evaluation point
  /// @param[in] coef a vector of polynomial coefficients
  /// @throw runtime_error if the number of coefficients is different from the
  /// value used in precompute method before calling this method.
  CTile eval(const std::vector<long double>& coef);

protected:
  /// Calculate k, m, bs, gs of Paterson-Stockmeyer algorithm and store it in
  /// class fields.
  int sp0(int n, const CTile& c1);

  /// compute  bs = {T[0], T[1], ... T[k]}
  void calculateBs(const CTile& ctx, int k, const CTile& ones);

  /// compute gs = {T[k], T[2k], T[4k], T[8k], T[16k]... T[(2 ^ m) k]}
  void calculateGs(const int m,
                   const CTile& ctx,
                   const CTile& ones,
                   const CTile& tcheb_k);
  /// sp1 for the case of aa polynomial of degree 0
  PTile sp1_deg0(const std::vector<long double>& f,
                 int chainIndex,
                 double scale) const;

  /// step 7 of sp1 which calculates the polynomial with coefs based on
  /// the precomuted powers
  void sp1_eval(std::vector<long double>& coefs,
                int m,
                CTile& c_u_h,
                PTile& c_u_h_P,
                bool& c_u_h_P_isEmpty) const;

  /// recursive part of paterson stockmeyer algorithm.
  CTile sp1(const std::vector<long double>& f, int m) const;

  std::vector<CTile> bsHe;
  std::vector<CTile> gsHe;
  std::vector<CTile> prodGsHe;

private:
  HeContext& he;
  const SealBootstrapUtils& bs;
  int k;
  int mInstance;
  int nInstance;
  int topLevel;
  double topScale;

  /// Long division algorithm for polynomails representes by Chebyshev base.
  static std::pair<std::vector<long double>, std::vector<long double>> t_div(
      const std::vector<long double>& r,
      std::vector<long double> q);

  /// Add the monomial a*x^n to a polynomial f.
  static std::vector<long double> add_coef(const std::vector<long double>& f,
                                           int n,
                                           long double a);
};
} // namespace helayers

#endif /* BOOTSTRAP_PATERSONSTOCKMEYERCHHE_H_ */
