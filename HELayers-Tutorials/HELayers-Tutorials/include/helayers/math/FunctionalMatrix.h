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

#ifndef SRC_MATH_FUNCTIONAL_MATRIX_H_
#define SRC_MATH_FUNCTIONAL_MATRIX_H_

#include <complex>
#include <vector>
#include "helayers/hebase/PTile.h"

namespace helayers {

class HeContext;

///@brief A FunctionalMatrix represents a matrix without storing
/// the actual matrix values, by providing an accessor to the matrix elements
/// which computes the element values on the fly when called.
class FunctionalMatrix
{
public:
  ///@brief The number of slots in the FHE configuration
  int SC;
  ///@brief Get the value in row i and column j of the matrix.
  /// This method is implemented by the inherited classes as required.
  virtual std::complex<double> getter(int, int) = 0;
  int fromRow;
  int fromCol;
  int toRow;
  int toCol;
  ///@brief True iff the matrix is indexed diagonally.
  /// Diagonalization puts the main diagonal (from [0,0]) in row 0
  /// the 2nd diagonal (starting from [0,1]) in row 1, etc.).
  /// So e.g.
  /// 00 01 02 03          00 11 22 33
  /// 10 11 12 13   ===\   01 12 23 30
  /// 20 21 22 23   ===/   02 13 20 31
  /// 30 31 32 33          03 10 21 32
  bool isDiagonalized;
  ///@brief True iff the matrix is accessed with bit-revered column indexes.
  /// For example, column 0x101110 would be reversed to 0x011101.
  bool isBitReverseColumns;

  ///@brief Construct a new FunctionalMatrix object
  ///@param rows Number of rows in the matrix
  ///@param cols Number of columns in the matrix
  FunctionalMatrix(int rows, int cols)
  {
    SC = -1;
    fromRow = fromCol = 0;
    toRow = rows - 1;
    toCol = cols - 1;
    isDiagonalized = false;
    isBitReverseColumns = false;
  }

  ~FunctionalMatrix(){};

  ///@brief Get the value in row i and column j of the matrix.
  /// This method handles the diagonalization and bit-reveral if required
  /// and will eventally call the getter(I, J) method implemented by the
  /// inherited classes as required.
  ///@param i The row index
  ///@param j The column index
  std::complex<double> get(int i, int j);
  ///@brief Set the matrix to be a cropping of the previous matrix
  /// by leaving just the area between (and including) the indicated rows/cols
  ///@param fromRow Crop from this row (inclusive)
  ///@param fromCol Cropt from this column (inclusive)
  ///@param toRow Crop up to this row (inclusive)
  ///@param toCol Crop up to this column (inclusive)
  void crop(int fromRow, int fromCol, int toRow, int toCol);
  ///@brief Declare that the matrix is "diagonalized"
  /// Diagonalization puts the main diagonal (from [0,0]) in row 0
  /// the 2nd diagonal (starting from [0,1]) in row 1, etc.).
  /// So e.g.
  /// 00 01 02 03          00 11 22 33
  /// 10 11 12 13   ===\   01 12 23 30
  /// 20 21 22 23   ===/   02 13 20 31
  /// 30 31 32 33          03 10 21 32
  ///@param _isDiagonalized True (default) if and only if digonalization is
  /// needed
  void diagonalize(bool _isDiagonalized = true)
  {
    isDiagonalized = _isDiagonalized;
  };
  ///@brief Declare that the matrix will be indexed by bit-reversed column
  /// indexes
  ///@param _isBitReverseColumns True (default) if and only if bit reversal
  /// for the column indexes is needed.
  void bitReverseColumns(bool _isBitReverseColumns = true)
  {
    isBitReverseColumns = _isBitReverseColumns;
  };
  ///@brief Returns the bit eversal of the input index value.
  /// For example, 0x101110 is reversed to 0x011101.
  ///@param index the index value to reverse
  ///@param numColumns The number of columns in the matrix.
  /// This is taken as the maximal value of the index to be reversed,
  /// and thus, the number of reversed bits is the log2 of this value.
  static int bitReverseIndex(int index, int numColumns);
  void print();
  ///@brief Print the matrix where values are first scaled by given scaling
  /// factor, and then the log is taken by the given base.
  ///@param scalingFactor Scale all matrix values by this factor before printing
  ///@param base Take the log of the scaled matrix values by this base
  void printLogScale(double scalingFactor, std::complex<double> base);
};
} // namespace helayers

#endif /* SRC_MATH_FUNCTIONAL_MATRIX_H_ */
