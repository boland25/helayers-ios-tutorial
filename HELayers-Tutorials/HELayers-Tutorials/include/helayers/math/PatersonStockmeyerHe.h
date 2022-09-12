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
#ifndef PATERSONSTOCKMEYERHE_H
#define PATERSONSTOCKMEYERHE_H
#include <vector>
#include "helayers/hebase/CTile.h"
#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/Encoder.h"
#include "FunctionEvaluator.h"

namespace helayers {
/// Evaluates a given polynomial at a given point homomorphically, using the
/// Paterson-Stockmeyer algorithm.
///
/// References:
/// Paterson, Michael S., and Larry J. Stockmeyer. "On the number of
/// nonscalar multiplications necessary to evaluate polynomials." SIAM Journal
/// on Computing 2.1 (1973): 60-66.
///
/// Chen, Hao, Ilaria Chillotti, and Yongsoo Song. "Improved bootstrapping for
/// approximate homomorphic encryption." Annual International Conference on the
/// Theory and Applications of Cryptographic Techniques. Springer, Cham, 2019.
class PatersonStockmeyerHe
{
  const HeContext& he;
  Encoder enc;
  FunctionEvaluator fe;
  int k;
  std::vector<CTile> bs;
  std::vector<CTile> gs;
  std::vector<CTile> prodGs;

  /// Returns true if coefficients are less than some constant epsilon.
  static bool isZeroPolynomial(const std::vector<double>& f);

  /// For debugging. TODO: move to unit tests.
  double psAssertEquals(const CTile& expected,
                        const CTile& val,
                        const std::string& title) const;

  // homomorpically evaluate a linear function a point.
  CTile evalLinearPoly(const CTile& u, const std::vector<double>& f) const;

  /// Non-recursive part of paterson stockmeyer algorithm.
  /// sets k,bs,gs,prod_gs in class (as opposed to returning their value as in
  /// python)
  /// Returns m, because it changes with each recursive call.
  /// @param[in] u        The (encrypted) point to evaluate the polynomial at.
  /// @param[in] n        The degree of the polynomial to be evaluated.
  int sp0(const CTile& u, int n);

  /// Recursive part of paterson stockmeyer algorithm, evaluates the given
  /// polynomial at the given (encrypted point).
  /// Returns a ciphertext with the result of the evaluation of a polynomial at
  /// the evaluation point.
  /// @param[in] u        The (encrypted) point to evaluate the polynomial at.
  /// @param[in] f        The polynomial to evaluate.
  /// @param[in] m        Parameter of paterson stockmeyer algorithm. Along with
  ///                     this->k, determines the degree of the polynomial to be
  ///                     calculated in the current recursive step.
  /// @throw              runtime_error if (k*(pow(2,(m - 1)) - 1) != s.size())
  CTile sp1(const CTile& u, const std::vector<double>& f, int m) const;

public:
  /// Constructs an empty object.
  /// @param[in] h the HE library context
  PatersonStockmeyerHe(const HeContext& h) : he(h), enc(h), fe(h) {}

  // Destructor.
  ~PatersonStockmeyerHe() {}

  /// Homomorphic evaluation of a ciphertext of a polynomial value at the
  /// ciphertext of a point.
  /// Returns a ciphertext with the result of the evaluation of the polynomial
  /// at the evaluation point.
  /// @param[in] u ciphertext of the evaluation point
  /// @param[in] f a vector of polynomial coefficients
  CTile sp(const CTile& u, const std::vector<double>& f);

  /// Horner's algorithm for evaluating homomorphically polynomial value at a
  /// point.
  /// Returns the encrypted value of a polynomial at a point.
  /// @param[in] ctx Encrypted evaluation point.
  /// @param[in] coeff The polynomial's coefficients.
  CTile horner(const CTile& ctx, const std::vector<double>& coeff);
};
} // namespace helayers

#endif