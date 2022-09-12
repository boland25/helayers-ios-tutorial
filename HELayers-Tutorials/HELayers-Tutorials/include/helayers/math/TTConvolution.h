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

#ifndef TTCONVOLUTION_H_
#define TTCONVOLUTION_H_

#include "helayers/hebase/hebase.h"
#include "TTShape.h"
#include "PTileTensor.h"
#include "CTileTensor.h"

namespace helayers {

class TTConvolution
{
public:
  /// Compute a convolution
  /// @param input the input to the convolution
  /// @param filters the filters of the convolution
  /// @param biases the biases of the convolution
  /// @param strideRows the stires along the rows
  /// @param strideCols the stires along the columns
  static CTileTensor getConvolution(const CTileTensor& input,
                                    const TileTensor& filters,
                                    const TileTensor& biases,
                                    int strideRows,
                                    int strideCols);

  /// Validate batch-packing style convolution
  /// @param input the input to the convolution
  /// @param filters the filters of the convolution
  /// @param biases the biases of the convolution
  /// @param strideRows the stires along the rows
  /// @param strideCols the stires along the columns
  static void validateConvolution(const CTileTensor& input,
                                  const TileTensor& filters,
                                  const TileTensor& biases,
                                  int strideRows,
                                  int strideCols);

  /// Compute tiles of batch packing convolution
  /// @param input the input to the convolution
  /// @param filters the filters of the convolution
  /// @param biases the biases of the convolution
  static CTileTensor buildConvolutionOutput(const CTileTensor& input,
                                            const std::vector<CTile>& newTiles,
                                            const TileTensor& filters,
                                            const TileTensor& biases);
};
} // namespace helayers

#endif /* TTCONVOLUTION_H_ */
