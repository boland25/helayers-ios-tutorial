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

#ifndef SRC_MATH_UTILS_H_
#define SRC_MATH_UTILS_H_

#include <cstdint>
#include <random>
#include <stdexcept>
#include <vector>
#include <map>
#include <algorithm>

/// @brief A class providing utility functions to perform mathematical
/// operations on numbers.
namespace helayers {

class MathUtils
{
public:
  ///@brief Computes (a % modulus) and returns a positive number in
  ///[0,modulus-1]
  ///@param a the dividend
  ///@param modulus the modulus
  static int mod(int a, int modulus);

  ///@brief Computes (a % modulus) and returns a positive number in
  ///[0,modulus)
  ///@param a the dividend
  ///@param modulus the modulus
  static double mod(double a, double modulus);

  ///@brief Rounds down x to the biggest power of 2 that is at most x
  ///@param x number to round down
  static unsigned long roundDownToPowerOf2(unsigned long x);

  ///@brief Rounds up x to the smallest power of 2 that is at least x
  ///@param x number to round up
  static uint64_t roundUpToPowerOf2(uint64_t x);

  ///@brief Returns true if \p x is power of 2 and false otherwise
  ///@param x Number to check whether it is a power of 2
  static bool isPowerOf2(uint64_t x);

  ///@brief Returns the size of the output of the convolution operator for a
  /// specific dimension.
  ///
  ///@param inputSize the size of the input
  ///@param filterSize the size of the filter
  ///@param stride the size of the stride
  ///@param startPadding padding added at the start of the input
  ///@param endPadding padding added at the end of the input
  static int getConvolutionOutputSize(int inputSize,
                                      int filterSize,
                                      int stride,
                                      int startPadding = 0,
                                      int endPadding = 0);

  ///@brief Returns a uniform random integer in [min, max] (inclusive). This
  /// version of \a randInt can be used when the user does not care about
  /// ensuring reproducibility or controlling the randomness of the chosen
  /// integer.
  ///
  ///@param min The left boundary of the range to choose from.
  ///@param max The right boundary of the range to choose from.
  ///@throw std::exception If random devices are not available to produce random
  ///                      numbers in the used system
  static int randInt(int min, int max);

  ///@brief Returns a uniform random integer in [min, max] (inclusive), using
  /// \p randomNumberGenerator random engine. This version of \a randInt can be
  /// used to control the randomness of the generated number (by using a a
  /// random engine initialized with a fixed seed, for example).
  ///@param min                   The left boundary of the range to choose from.
  ///@param max                   The right boundary of the range to choose
  ///                             from.
  ///@param randomNumberGenerator The random engine used to generate a random
  ///                             number.
  static int randInt(int min, int max, std::mt19937& randomNumberGenerator);

  ///@brief Returns indication to whether two values should be considered equal.
  /// Taking into account the relative difference, unless at least one of the
  /// inputs equals zero, in this case the absolute difference is considered.
  ///
  ///@param a The first value.
  ///@param b The second value.
  ///@param eps The maximal allowed relative difference:
  /// (abs(a-b) / max(abs(a),abs(b)))
  /// or the maximal allowed absolute difference if at least one of the inputs
  /// equals zero.
  static bool isEqual(double a, double b, double eps = 1e-10);

  ///@brief Returns indication to whether one value should be considered smaller
  /// than another. Based on "MathUtils::isEqual" method, see its description
  /// for more details.
  ///
  ///@param a The first value.
  ///@param b The second value.
  ///@param eps See description of "MathUtils::isEqual" method.
  static bool isLess(double a, double b, double eps = 1e-10);

  ///@brief Returns indication to whether one value should be considered smaller
  /// than another or equal to it. Based on "MathUtils::isEqual" method, see its
  /// description for more details.
  ///
  ///@param a The first value.
  ///@param b The second value.
  ///@param eps See description of "MathUtils::isEqual" method.
  static bool isLessOrEqual(double a, double b, double eps = 1e-10);

  ///@brief Returns element-wise inverse vector R given a vector of values V,
  /// where R[i] = 1 / V[i] for every index i.
  ///
  ///@param src The given input vector.
  static std::vector<double> getInverse(const std::vector<double>& src);

  ///@brief Returns a string representing the given double, using the given
  /// number of precision digits.
  ///
  ///@param d         The double to convert to a string.
  ///@param precision The number of precision digits to use in the string
  ///                 representation of "d".
  static std::string toString(double d, int precision = 6);

  ///@brief Casts "v" to be of type T. Asserts the result of the casting has the
  /// same value (up to "eps") as the original value of "v".
  ///
  /// @param v The double to cast.
  /// @param eps The maximal allowed differene between the casting result and
  /// "v".
  template <typename T>
  static T safeCast(double v, double eps = 1e-6);
};

template <typename T>
T MathUtils::safeCast(double v, double eps)
{
  T res = static_cast<T>(v);
  always_assert(fabs(v - res) < eps);
  return res;
}

///@brief Print a list of numbers
///
///@param out Output stream
///@param vec List of numbers
std::ostream& operator<<(std::ostream& out, const std::vector<int>& vec);

} // namespace helayers

#endif /* SRC_MATH_UTILS_H_ */
