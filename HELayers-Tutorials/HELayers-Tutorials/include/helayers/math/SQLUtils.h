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

#ifndef SRC_MATH_SQL_UTILS_H_
#define SRC_MATH_SQL_UTILS_H_

#include <memory>
#include <vector>
#include <map>

namespace helayers {

class CTile;
class PTile;
class HeContext;

enum ComparisonType
{
  IS_EQUAL,
  IS_GREATER,
  IS_SMALLER,
  IS_GREATER_EQUAL,
  IS_SMALLER_EQUAL,
};

class SQLUtils
{
  HeContext& he;

public:
  SQLUtils(HeContext& _he);
  virtual ~SQLUtils();

  /// TODO: isEqBits should be private and move all compare functions to another
  /// file
  CTile isEqBits(const std::vector<CTile>& c1,
                 const std::vector<CTile>& c2,
                 int min,
                 int max);

  CTile isEqBitsPairs(const CTile& c1, const CTile& c2);

  CTile isEqualNoOptimization(const std::vector<CTile>& c1,
                              const std::vector<CTile>& c2);

  /// Compares two number in hybrid-bitwise representation.\n
  /// @param[in] c1 first number to compare.
  /// @param[in] c2Neg second number to compare, given in inverse representation
  /// (110101 becomes 001010).
  /// @param[in] ct is the comparison type (e.g. IS_GREATER, IS_EQUAL).
  /// @param[out] The result of the comparison, 1 if c1 ? c2 and 0 otherwise.
  CTile bitwiseCompareOptimized(const std::vector<CTile>& c1,
                                const std::vector<CTile>& c2Neg,
                                ComparisonType ct,
                                bool isSigned = true);

  // TODO: move to be private
  std::vector<CTile> createSuffixEquals(const std::vector<CTile>& c1,
                                        const std::vector<CTile>& c2Neg);
  std::vector<CTile> createSuffixEqualsRec(const std::vector<CTile>& c1,
                                           const std::vector<CTile>& c2Neg,
                                           int start,
                                           int end);
  CTile createSuffixEqualsRec(
      const std::vector<CTile>& c1,
      const std::vector<CTile>& c2Neg,
      int start,
      int end,
      std::map<std::pair<int, int>, std::shared_ptr<CTile>>& cache);

  // Expects c1 between 0.5 and 1.5
  CTile inverse(const CTile& c1, int numOfIter);
  // CTile compare(const CTile &a, const CTile &b, int dTag, int d, int t, int
  // m);

  /// TODO: move to be private
  std::shared_ptr<CTile> isEqBitsInverse(const std::vector<CTile>& c1,
                                         const std::vector<CTile>& c2,
                                         int min,
                                         int max);
  std::shared_ptr<CTile> isEqBitsPairsInverse(const CTile& c1, const CTile& c2);
  CTile isEqualInverse(const std::vector<CTile>& c1,
                       const std::vector<CTile>& c2);

  CTile isEqual(const std::vector<CTile>& c1,
                const std::vector<CTile>& c2,
                bool useOptimization);

private:
};
} // namespace helayers

#endif /* SRC_MATH_SQL_UTILS_H_ */
