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

#ifndef PADDING_2D_H_
#define PADDING_2D_H_

#include "helayers/hebase/utils/SaveableBasic.h"
#include "helayers/math/MathUtils.h"

namespace helayers {

///@brief A structure that holds padding for a 2d array.
/// Used to specify convolution and mean pooling padding.
struct Padding2d : public SaveableBasic
{
  int left;
  int right;
  int up;
  int down;

  ///@brief Construct an all zero Padding 2d object
  ///
  Padding2d();

  ///@brief Construct a new Padding 2d object
  ///
  ///@param l left padding
  ///@param r right padding
  ///@param u up padding
  ///@param d down padding
  Padding2d(int l, int r, int u, int d);

  ///@brief Constructs a padding object matching the SAME convolution mode for
  /// the given parameters.
  ///
  ///@param inputRows The number of rows in the input.
  ///@param inputCols The number of columns in the input.
  ///@param kernelRows The number of rows in the kernel.
  ///@param kernelCols The number of columns in the kernel.
  ///@param strideRows The size of strides along the rows dimension.
  ///@param strideCols The size of strides along the columns dimension.
  static Padding2d same(int inputRows,
                        int inputCols,
                        int kernelRows,
                        int kernelCols,
                        int strideRows,
                        int strideCols);

  ///@brief Returns the start and end padding along a single dimension matching
  /// the SAME convolution mode for the given parameters.
  ///
  ///@param input The size of the input.
  ///@param kernel The size of the kernel.
  ///@param strides The size of the strides.
  static std::pair<int, int> sameForDim(int input, int kernel, int strides);

  ///@brief Returns if all padding values are zero
  bool allZero() const;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  ///@brief Set padding values
  ///
  ///@param l left padding
  ///@param r right padding
  ///@param u up padding
  ///@param d down padding
  void set(int l, int r, int u, int d);

  ///@brief Returns the maximum of left and right padding
  inline int getMaxLeftRight() const { return std::max(left, right); }

  ///@brief Returns the maximum of up and down padding
  inline int getMaxUpDown() const { return std::max(up, down); }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;
};

bool operator==(const Padding2d& p1, const Padding2d& p2);

bool operator!=(const Padding2d& p1, const Padding2d& p2);

} // namespace helayers

#endif /* PADDING_2D_H_ */
