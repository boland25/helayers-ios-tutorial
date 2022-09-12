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

#ifndef SRC_MATH_DETERMINANT_CALCULATOR_H_
#define SRC_MATH_DETERMINANT_CALCULATOR_H_

#include <cstring>
#include <map>
#include <vector>
#include "helayers/math/CTileTensor.h"

namespace helayers {

class HeContext;

///@brief A DeterminantCalculator calculates the determinant of
/// an encrypted square matrix
class DeterminantCalculator : public Printable
{
public:
  ///@brief Construct a new DeterminantCalculator object
  ///@param[in] he The context
  ///@param[in] mat The encrypted square matrix for which the determinant will
  /// be computed
  DeterminantCalculator(const HeContext& he, const CTileTensor& mat);
  ~DeterminantCalculator(){};

  int getMatrixSize() { return matrixSize; }

  ///@brief Calculate the determinant
  ///@param res The resulting determinant of the matrix
  void calcDeterminant(CTile& res);

  ///@brief Prints the content of this object.
  ///
  ///@param title Text to add to the print
  ///@param verbosity Verbosity level
  ///@param out Output stream
  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

private:
  ///@brief A class for an identifier of a sub-matrix
  /// defined as the set of valid rows and columns in the sub-matrix.
  struct SubMatrixId : Printable
  {
  public:
    ///@brief Construct a new SubMatrixId object
    ///@param n The size of the sub-matrix is nxn
    ///@param subRows The rows of the sub-matrix
    ///@param subCols The columns of the sub-matrix
    ///@throws runtime_error when n is greater than the number of vector
    /// elements
    SubMatrixId(int n,
                const std::vector<bool>& subRows,
                const std::vector<bool>& subCols);

    int n;
    std::vector<bool> subRows;
    std::vector<bool> subCols;

    void debugPrint(const std::string& title = "",
                    Verbosity verbosity = VERBOSITY_REGULAR,
                    std::ostream& out = std::cout) const override;
  };

  struct SubMatrixIdCompare
  {
    bool operator()(const SubMatrixId& a, const SubMatrixId& b) const;
  };
  std::map<SubMatrixId, CTile, SubMatrixIdCompare> subDeterminantCache;

  const HeContext& he;

  ///@brief The number of rows and columns in the square matrix
  int matrixSize;

  ///@brief The matrix for which the determinant will be computed
  const CTileTensor& mat;

  ///@brief Calculate the determinant of the specified sub-matrix recursively
  ///@param res The resulting determinant of the sub-matrix
  ///@param n The size of sub-matrix
  ///@param id The id of the sub-matrix to compute the determnant for
  void calcSubMatrixDeterminant(CTile& res, int n, SubMatrixId& id);

  ///@brief Prints the sub-matrix content
  ///@param id The id of the sub-matrix
  void printSubMatrix(const SubMatrixId& id);

  ///@brief Return the index of the next true vector entry after "from"
  ///@param v The vector in which to search for the next active element
  ///@param from Start the search from this index
  int nextActive(const std::vector<bool>& v, int from);

  ///@brief Translate from row-col index pair to flat index
  int flatIndex(int row, int col);
};
} // namespace helayers

#endif /* SRC_MATH_DETERMINANT_CALCULATOR_H_ */
