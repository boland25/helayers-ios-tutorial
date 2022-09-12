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

#ifndef SRC_HELAYERS_FUNCTIONEVALUATOR_H
#define SRC_HELAYERS_FUNCTIONEVALUATOR_H

#include "CTileTensor.h"
#include "helayers/hebase/hebase.h"

namespace helayers {

/// @brief This type is used to specify a polynomial evaluation algorithm
enum EvalType
{
  /// Polynomial evaluation using the paterson stockmayer algorithm
  PATERSONSTOCKMAYER,

  /// A polynomial evaluation algorithm which reuses computed powers and aims to
  /// minimize the number of products
  EFFICIENT_POWERS,

  /// A polynomial evaluation algorithm which aims to minimize the
  /// multiplication depth
  MIN_DEPTH,

  DEFAULT = MIN_DEPTH
};

/// @brief This class is used to evaluate different available functions on
/// CTiles, such as polynomial evaluation and sigmoid.
class FunctionEvaluator
{
  /// The HeContext.
  const HeContext& he;

  /// The smallest positive number which the polynomial evaluation functions
  /// consider as non-zero
  static const double POLY_EVAL_EPS;

  /// @brief If "coef" is not 1, multiplies c by coef. Otherwise, does nothing.
  /// @param c    The CTile to multiply
  /// @param coef To multiply "c" with.
  static inline void multiplyIfNot1(CTile& c, double coef)
  {
    if (coef != 1.0) {
      c.multiplyScalar(coef);
    }
  }

  /// @brief Returns true if the "coef" is not considered zero and false
  /// otherwise.
  /// @param coef To check whether it is considered zero.
  static inline bool coefIsNotZero(double coef)
  {
    return fabs(coef) > POLY_EVAL_EPS;
  }

  /// @brief Returns the maximum index of a nonzero coefficient in the given
  /// vector of coefficients.
  /// @param coefs The vector of coefficients.
  /// @throws runtime_error If all coefficients are zero.
  static int polyEvalGetMaxExponent(const std::vector<double>& coefs);

  /// @brief Computes coef * powersStore[1]^exp.
  /// @param exp         The required exponent.
  /// @param powersStore A vector used to store powers computed by this function
  ///                    and reuse them in future calls. powersStore[1] mustn't
  ///                    be empty.
  /// @param coef        The coefficient to multiply with.
  CTile computeMonomial(int32_t exp,
                        std::vector<CTile>& powersStore,
                        double coef) const;

  /// @brief Computes coef * powersStore[1]^exp.
  /// @param exp         The required exponent.
  /// @param powersStore A vector used to store powers computed by this function
  ///                    and reuse them in future calls. powersStore[1] mustn't
  ///                    be empty.
  /// @param coef        The (encrypted) coefficient to multiply with.
  CTile computeMonomial(int32_t exp,
                        std::vector<CTile>& powersStore,
                        const CTile& coef) const;

  /// @brief An auxiliary function to polyEval(). Given an exponent, pow,
  /// This function computes all exponents which should be pre-computed
  /// to be able to compute the given exponent of a CTile while minimizing
  /// the multiplication depth of the computation.
  /// @param pow       The exponent to compute its dependencies.
  /// @param powerDeps The found dependencies will be stored here.
  static void polyEvalPowerDeps(int pow, std::vector<int>& powerDeps);

  /// @brief Evaluates the given polynomial using the Paterson-Stockmeyer
  /// algorithm, and returns the polynomial evaluation result.
  /// @param src   The point to evaluate the polynomial at.
  /// @param coefs The polynomial to evaluate. coefs[0] is the free
  ///              coefficient.
  CTile patersonStockmeyerPolyEval(const CTile& src,
                                   const std::vector<double>& coefs) const;

  ///@brief Multiplies the CTileTensors in the indexes [begin, end) (inclusive)
  /// in the given "cts" vector. The multiplication depth consumed by this
  /// function is ceil(log(end - begin)).
  ///
  ///@param cts   The vector of CTileTensors to be multiplied
  ///@param begin The index to start multiplying from
  ///@param end   The index of the last CTileTensor to be multiplied
  static void multiplyMany(CTileTensor& output,
                           const std::vector<CTileTensor>& cts,
                           DimInt begin,
                           DimInt end);

  /// @brief Updates the given coefficients of a polynomial, p(x), to the
  /// coefficients of p(x*scale).
  /// @param coeffs The coefficients of the polynomial to scale its input
  /// @param scale  The value to scale by.
  void scalePolyInput(std::vector<double>& coeffs, double scale) const;

  /// @brief Updates the given coefficients of a polynomial, p(x), to the
  /// coefficients of scale * p(x).
  /// @param coeffs The coefficients of the polynomial to be scaled.
  /// @param scale  The value to scale by.
  void scalePolyRes(std::vector<double>& coeffs, double scale) const;

  /// @brief Checks whether a bootstrapping will be required to complete the
  /// specified polynomial evlauation. If a bootstrapping is required to
  /// evaluate the given polynomial on the given \p src ciphretext and using the
  /// given evaluation method, returns true. Otherwise, returns false.
  ///
  /// @param src    The cipertext to evaluate the polynomial on.
  /// @param coeffs The polynomial's coefficients.
  /// @param type   The polynomial evaluation method.
  bool polyEvalNeedBs(const CTile& src,
                      const std::vector<double>& coeffs,
                      EvalType type) const;

  /// @brief Checks whether bootstrapping will be required to complete the
  /// specified polynomial evlauation. If bootstrapping is required to
  /// evaluate the given polynomial on the given \p src ciphretext and using the
  /// given evaluation method, returns true. Otherwise, returns false.
  ///
  /// @param src        The cipertext to evaluate the polynomial on.
  /// @param coeffs     The (encrypted) polynomial's coefficients.
  /// @param normalized If false, the polynomial to evaluate will be composed
  ///                   from the coefficients in "coeffs" vector only.
  ///                   Otherwise, an extra term, whose coefficient is 1 and
  ///                   whose power is coefs.size(), will be added.
  bool polyEvalNeedBs(const CTile& src,
                      const std::vector<CTile>& coeffs,
                      bool normalized) const;

public:
  /// @brief A constructor
  /// @param he The HeContext.
  FunctionEvaluator(const HeContext& he);
  ~FunctionEvaluator();
  FunctionEvaluator(const NativeFunctionEvaluator& src) = delete;
  FunctionEvaluator& operator=(const NativeFunctionEvaluator& src) = delete;

  /// @brief Polynomial evaluation. Evaluates the given polynomial on input
  /// "src", and stores the result in "res".
  ///
  /// @param res   Where to store the result.
  /// @param src   The input of the polynomial.
  /// @param coefs The coefficients of the polynomial. coefs[0] is the
  ///              free coefficient.
  /// @param type  The type of the evaluation algorithm. See also the
  ///              documnetation of "EvalType".
  /// @throw runtime_error If for each i,
  ///                      abs(coefs[i]) < (1 / he.getDefaultScale()).
  void polyEval(CTile& res,
                const CTile& src,
                const std::vector<double>& coefs,
                EvalType type = DEFAULT) const;

  /// @brief Polynomial evaluation, in place. Evaluates the given polynomial on
  /// input "src", and stores the result in "src".
  ///
  /// @param src   The input of the polynomial. This will contain the result of
  ///              the evaluation at the end of the function execution.
  /// @param coefs The coefficients of the polynomial. coefs[0] is the free
  ///              coefficient.
  /// @param type  The type of the evaluation algorithm. See also the
  ///              documnetation of "EvalType".
  /// @throw runtime_error If for each i,
  ///                      abs(coefs[i]) < (1 / he.getDefaultScale()).
  void polyEvalInPlace(CTile& src,
                       const std::vector<double>& coefs,
                       EvalType type = DEFAULT) const;

  /// @brief Polynomial evaluation, in place. Evaluates the given polynomial on
  /// input "src", and stores the result in "src".
  ///
  /// @param src           The input of the polynomial. This will contain the
  ///                      result of the evaluation at the end of the function
  ///                      execution.
  /// @param coefs         The (encrypted) coefficients of the polynomial.
  ///                      coefs[0] is the free coefficient.
  /// @param normalized    If false, the polynomial to evaluate will be composed
  ///                      from the coefficients in "coeffs" vector only.
  ///                      Otherwise, an extra term, whose coefficient is 1 and
  ///                      whose power is coefs.size(), will be added.
  /// @param ignoreFreeCoef If true, the first coefficient in "coefs" array is
  ///                       not added to the final result.
  void polyEvalInPlace(CTile& src,
                       const std::vector<CTile>& coefs,
                       bool normalized = false,
                       bool ignoreFreeCoef = false) const;

  /// @brief Returns the required multiplication depth to evaluate the given
  /// polynomial, using the given evaluation method.
  /// @param coefs The polynomial's coeffiecients.
  /// @param type  The polynomial evaluation method.
  static int getPolyEvalMulDepth(const std::vector<double>& coefs,
                                 EvalType type);

  /// @brief Returns the required multiplication depth to evaluate the given
  /// polynomial.
  /// @param coefs      The polynomial's coeffiecients.
  /// @param normalized If false, the polynomial to evaluate will be composed
  ///                   from the coefficients in "coeffs" vector only.
  ///                   Otherwise, an extra term, whose coefficient is 1 and
  ///                   whose power is coefs.size(), will be added.
  static int getPolyEvalMulDepth(const std::vector<CTile>& coefs,
                                 bool normalized);

  /// @brief Composite polynomial evaluation. Evaluates the composition of the
  /// given polynomials on the given "src", and stores the result in "res".
  ///
  /// @param res   Where to store the result.
  /// @param src   The input of the composite polynomial.
  /// @param polys The coefficients of the composition polynomials.
  ///              polys[0] is evaluated first, and polys[i][0] is the free
  ///              coefficient of the i-th polynomial.
  /// @param type  The type of the evaluation algorithm. See also the
  ///              documnetation of "EvalType".
  /// @throw runtime_error If for there exists an i s.t. for each j,
  ///                      abs(polys[i][j]) < (1 / he.getDefaultScale()).
  void polyCompEval(CTile& res,
                    const CTile& src,
                    const std::vector<std::vector<double>>& polys,
                    EvalType type = DEFAULT) const;

  /// @brief Composite polynomial evaluation, in place. Evaluates the
  /// composition of the given polynomials on the given "src", and stores the
  /// result in "src".
  ///
  /// @param src   The input of the composite polynomial. This will contain the
  ///              result of the evaluation at the end of the function
  ///              execution.
  /// @param polys The coefficients of the composition polynomials.
  ///              polys[0] is evaluated first, and polys[i][0] is the free
  ///              coefficient of the i-th polynomial.
  /// @param type  The type of the evaluation algorithm. See also the
  ///              documnetation of "EvalType".
  /// @throw runtime_error If for there exists an i s.t. for each j,
  ///                      abs(polys[i][j]) < (1 / he.getDefaultScale()).
  void polyCompEvalInPlace(CTile& src,
                           const std::vector<std::vector<double>>& polys,
                           EvalType type = DEFAULT) const;

  /// @brief Computes src^degree, and stores the result into res.
  /// @param res    Where the result should be stored.
  /// @param src    To compute its power.
  /// @param degree The required exponent.
  void pow(CTile& res, const CTile& src, int degree) const;

  /// @brief Computes src^degree, in place.
  /// @param src    To compute its power. The result will be stored here.
  /// @param degree The required exponent.
  void powInPlace(CTile& c, int degree) const;

  /// @brief Coefficients of the polynomials used in the computation of
  /// sigmoid3(), sigmoid7() and sigmoid9()
  static const double sig3Factor, sig7Factor, sig9Factor;
  static const std::vector<double> sig3Coeffs, sig7Coeffs, sig9Coeffs;

  /// An approximation of sigmoid by an order 3 polynomial.
  /// @param[out] res     Result of the function.
  /// @param[in] src      Cipher to calculate its sigmoid.
  void sigmoid3(CTile& res, const CTile& src) const;

  // void sigmoid3B(CTile& res, const CTile& src) const;

  /// An approximation of sigmoid by an order 3 polynomial, in place.
  /// @param[in, out] src Cipher to calculate its sigmoid. This will contain the
  ///                     sigmoid result at the end of the function execution.
  void sigmoid3InPlace(CTile& src) const;

  /// An approximation of sigmoid by an order 7 polynomial.
  /// @param[out] res     Result of the function.
  /// @param[in] src      Cipher to calculate its sigmoid.
  void sigmoid7(CTile& res, const CTile& src) const;

  // void sigmoid7B(CTile& res, const CTile& src) const;

  /// An approximation of sigmoid by an order 7 polynomial, in place.
  /// @param src Cipher to calculate its sigmoid. This will contain the
  ///            sigmoid result at the end of the function execution.
  void sigmoid7InPlace(CTile& src) const;

  /// @brief An approximation of sigmoid by an order 9 polynomial.
  /// @param res Cipher to calculate its sigmoid. This will contain
  ///        the sigmoid result at the end of the function execution.
  void sigmoid9(CTile& res, const CTile& src) const;

  /// @brief An approximation of sigmoid by an order 9 polynomial, in place.
  /// @param src Cipher to calculate its sigmoid. This will contain the sigmoid
  ///        result at the end of the function execution.
  void sigmoid9InPlace(CTile& src) const;

  /// Calculates absolute value. \n
  /// This method return the absolute value of a cipher text. maxPossibleValue
  /// is the maximum of the absolute value of the possible value of a.
  ///
  /// @param[out] res                       result of the function.
  /// @param[in] a                          Cipher to calculate absolute for.
  /// @param[in] gRep                       How many repetitions of g(x)
  /// @param[in] fRep                       How many repetitions of f(x)
  /// @param[in] maxPossibleAbsValue        The maximum absolute value possible
  /// for a.
  CTile abs(const CTile& a, int gRep, int fRep, double maxPossibleAbsValue);

  /// Comparison of two numbers. \n
  /// This method compares two numbers a and b and return 1 if a>b, 0 if b<a and
  /// 0.5 if a==b by calculating approximation of sign(a-b).
  ///
  /// @param[in] a                          First ciphertext to compare
  /// @param[in] b                          Second ciphertext to compare
  /// @param[in] gRep                       How many repetitions of g(x).
  ///                                       See further explanation in sign()
  /// @param[in] fRep                       How many repetitions of f(x).
  ///                                       See further explanation in sign()
  /// @param[in] maxPossibleAbsOfDiff       An upper upper bound on |a-b|. The
  ///                                       tighter this bound is, the more
  ///                                       accurate the result will be.
  ///
  CTile compare(const CTile& a,
                const CTile& b,
                int gRep,
                int fRep,
                double maxPossibleAbsOfDiff) const;

  ///@brief Comparison of two numbers.
  /// This method compares two numbers a and b and return 1 if a>b, 0 if b<a and
  /// 0.5 if a==b.
  ///
  ///@param a                          First ciphertext to compare
  ///@param b                          Second ciphertext to compare
  ///@param gRep                       How many repetitions of g(x). See further
  ///                                  explanation in sign().
  ///@param fRep                       How many repetitions of f(x). See further
  ///                                  explanation in sign().
  ///@param maxPossibleAbsOfDiff       An upper upper bound on |a-b|. The
  ///                                  tighter this bound is, the more accurate
  ///                                  the result will be.
  ///
  CTile compare(const CTile& a,
                const Tile& b,
                int gRep,
                int fRep,
                double maxPossibleAbsOfDiff) const;

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

  ///@brief Stores the minimum of "cs" CTiles in "minRes" and stores in
  /// minIndicators[i] a CTile contianing 1 if cs[i] is the minimum and 0
  /// otherwise.
  ///@param gRep,fRep Determine the accuracy of the result. Greater gRep and
  ///                 fRep will result with a greater accuracy on the account
  ///                 of slower runtime and more multiplication depth.
  ///@param maxDiff   The maximum possible absolute difference between two
  ///                 values in cs[i].
  void min(CTile& minRes,
           std::vector<CTile>& minIndicators,
           const std::vector<CTile>& cs,
           int gRep,
           int fRep,
           double maxDiff = 1) const;

  /// @brief Returns the (approximated) sign of \p a. The sign is computed as
  /// the g(g(...g(f(f(...f(x)))))), where g(x) and f(x) are two degree 7
  /// polynomials. The number of times g(x) and f(x) appear in the composition
  /// can be controlled by \p gRep and \p fRep arguments, respectively. \p a
  /// must be in the range [-maxAbsVal, maxAbsVal].
  ///
  /// @param a         Ciphertext we want to find its sign. a must be in
  ///                  the range [-maxAbsVal, maxAbsVal].
  /// @param gRep      How many repetitions of g(x).
  /// @param fRep      How many repetitions of f(x).
  /// @param maxAbsVal An upper bound on the absolute value of a. Defaults to 1.
  /// @param binaryRes If true the result will be close to 0 when a < 0 and
  ///                  close to 1 when a > 0. If false, the result will be close
  ///                  to -1 when a < 0 and close to 1 when a > 0. Defaults to
  ///                  false.
  CTile sign(const CTile& a,
             int gRep,
             int fRep,
             double maxAbsVal = 1,
             bool binaryRes = false) const;

  /// @brief Computes the (approximated) sign of \p src, in place. The
  /// sign is computed as the g(g(...g(f(f(...f(x)))))), where g(x) and f(x) are
  /// two degree 7 polynomials. The number of times g(x) and f(x) appear in the
  /// composition can be controlled by \p gRep and \p fRep arguments,
  /// respectively. \p src must be in the range [-maxAbsVal, maxAbsVal].
  ///
  /// @param src       Cipher text we want to find its sign. a must be in
  ///                  the range [-maxAbsVal, maxAbsVal].
  /// @param gRep      How many repetitions of g(x).
  /// @param fRep      How many repetitions of f(x).
  /// @param maxAbsVal An upper bound on the absolute value of src. Defaults
  ///                  to 1.
  /// @param binaryRes If true the result will be close to 0 when a < 0 and
  ///                  close to 1 when a > 0. If false, the result will be close
  ///                  to -1 when a < 0 and close to 1 when a > 0. Defaults to
  ///                  false.
  void signInPlace(CTile& src,
                   int gRep,
                   int fRep,
                   double maxAbsVal = 1,
                   bool binaryRes = false) const;

  /// @brief Returns the maximal absolute input value which is supported by the
  /// sign polynomial approximation without the need of scaling the input to a
  /// smaller value
  inline static double getSignAbsThreshold() { return 10; }

  /// Calculates a polynomial at a point using Exponentiation by squaring
  /// @param[out] src      polynomial value at the point (encrypted)
  /// @param[in]  coefs    polynomial coeefficents
  void efficientPowersPolyEvalInPlace(CTile& src,
                                      const std::vector<double>& coefs) const;

  /// @brief Calculates a polynomial at a point using Exponentiation by
  /// squaring. Changes the multiplications order in some cases to reduce
  /// multiplication depth. However, this comes at the expense of increasing
  /// the total number of multiplications
  /// @param src      polynomial value at the point (encrypted)
  /// @param  coefs   polynomial coefficents
  void minDepthPolyEvalInPlace(CTile& src,
                               const std::vector<double>& coefs) const;

  /// @brief Calculates (an approximation of) 1/src. src must be between
  /// lowerBound and upperBound, and lowerBound must be non-negative. The
  /// tighter the given bounds are, the more accurate the result will be.
  /// @param  src            ciphertext to calculate its inverse.
  /// @param  lowerBound     a lower bound on src. The tighter this bound
  ///                        is, the more accurate the result will be. This
  ///                        lowerBound must be non-negative.
  /// @param  upperBound     an upper upperBound on src. The tighter this bound
  ///                        is, the more accurate the result will be.
  /// @param bitResolution   Controls the accuracy of the result. A higher
  ///                        bitResolution value will increase the accuracy of
  ///                        the result at the account of consumig more
  ///                        multiplication depth.
  void inverse(CTile& src,
               double lowerBound,
               double upperBound,
               int bitResolution = 5) const;

  ///@brief Calculates (an approximation of) sqrt(src). src must be in the range
  /// [0,1].
  ///
  ///@param src           To compute its square root.
  ///@param bitResolution Controls the accuracy of the result. A higher value
  ///                     results with a better accuracy at the account of
  ///                     higher multiplication depth and higher latency.
  void sqrt(CTile& src, int bitResolution) const;

  ///@brief Returns the product of the given CTileTensors. The multiplication
  /// depth consumed by this function is ceil(log(cst.size()))
  ///
  ///@param cts The vector of CTileTensors to multiply
  static CTileTensor multiplyMany(const std::vector<CTileTensor>& cts);

  /// @brief Given ciphertext src and a vector possibleValues of the possible
  /// values inside src, calculates |possibleValues| indicator ciphertexts that
  /// indicates which slots in src contain the corresponding value. Formally,
  /// the i'th indicator ciphertext contains 1 in slot j only if
  /// src[j]=possibleValues[i], and 0 otherwise.
  ///
  /// @param  res            An empty vector of CTiles
  /// @param  src            The source ciphertext
  /// @param  possibleValues A vector of doubles contain the possible values
  ///                        inside src
  void oneHot(std::vector<CTile>& res,
              const CTile& src,
              const std::vector<double>& possibleValues) const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_FUNCTIONEVALUATOR_H */
