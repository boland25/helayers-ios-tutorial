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

#ifndef TTCONVOLUTION_INTERLEAVED_H_
#define TTCONVOLUTION_INTERLEAVED_H_

#include <mutex>
#include "helayers/hebase/hebase.h"
#include "helayers/math/TTInterleavedTileMask.h"
#include "helayers/math/Padding2d.h"
#include "TTShape.h"
#include "PTileTensor.h"
#include "CTileTensor.h"
#include "TileTensor.h"
#include "MathUtils.h"
#include "TTIterator.h"

namespace helayers {

class CTileRotationCache;

/// A 2D convolution for iteratively evaluating CNN.
/// To be used when the XY dimensions are interleaved.
/// The parameters have one of two shapes:
///
/// option I (FXYCB):
/// image:        [*/t0,X~/t1,Y~/t2, C/t3, B/t4]
/// filter: [FX,FY,F/t0,*/t1, */t2,  C/t3, */t4]
/// bias:         [F/t0,*/t1,*/t2,*/t3,*/t4]
/// output:       [F/t0,X~/t1,Y~/t2, */t3, B/t4]
///
/// option II (CXYFB):
/// image:        [C/t0,X~/t1,Y~/t2, */t3, B/t4]
/// filter: [FX,FY,C/t0,*/t1, */t2,  F/t3, */t4]
/// bias:         [*/t0,*/t1,*/t2,F/t3,*/t4]
/// output:       [*/t0,X~/t1,Y~/t2, F/t3, B/t4]
///
/// Set the CXYFB parameter to choose between these two options.
///
class TTConvolutionInterleaved
{
private:
  const HeContext& he;
  std::shared_ptr<const CTileTensor> input;
  const TileTensor* filters;
  const TileTensor* biases;
  int strideRows;
  int strideCols;
  bool CXYFB;

  // The indexes of each dimension
  int CHANNEL_DIM;
  int ROW_DIM;
  int COL_DIM;
  int FILTER_DIM;
  int BATCH_DIM;

  const TTDim* inputChannelDim;
  const TTDim* inputRowDim;
  const TTDim* inputColDim;
  const TTDim* inputFilterDim;
  const TTDim* inputBatchDim;

  const TTDim* filterInputRowDim;
  const TTDim* filterInputColDim;
  const TTDim* filterChannelDim;
  const TTDim* filterRowDim;
  const TTDim* filterColDim;
  const TTDim* filterFilterDim;
  const TTDim* filterBatchDim;

  int rotateOneCol;
  int rotateOneRow;

  // Input external sizes
  int inputChannels;
  int inputRows;
  int inputCols;
  int inputFilters;
  int inputBatches;

  // filter external sizes
  int filterRows;
  int filterCols;
  int filterFilters;

  // output external sizes
  // Note that while outputRows and outputCols keep the external sizes of the
  // tensor, in pathological case where (orignalSize < externalSize) we decrease
  // externalSize=orignalSize. In this case outputRows (resp. outputCols) keep
  // the original size before the reduction. The value after the reduction is
  // kept by a local variable realOutputRows (resp. realOutputCols)
  int outputRows;
  int outputCols;
  int outputFilters;
  int outputBatches;
  // The output channels are going to be folded onto one tile so outputChannel=1

  // output original sizes
  int outputOriginalRows;
  int outputOriginalCols;

  // How much can the filter "glide" from each direction
  Padding2d padding;

  // Whether the input contains enough physical padding to support specified
  // padding.
  bool physicalPadding;

  // Whether masks are needed in the computation
  bool needMasks;

  // flag to determine if sum pooling
  bool sumOverChannels;

  void setBasicDimensions();
  void validateInput();
  void setSizes();
  void validateBiasShape();
  void validateFilterShape();

  void findIndexesAndRotations(int iInput,
                               int stride,
                               int inputSize,
                               int iFilterOffsetFromPivot,
                               int& iOutput,
                               int& toRotate) const;

  ///@brief Add contribution from an input tile
  ///@param output Output tile
  ///@param rotCache A cache of rotations computed on the same input tile in
  ///@param filter Filter tile
  ///@param rotateBy Rotation offset
  void addContributionToOutputTile(CTile& output,
                                   std::mutex& outputMutex,
                                   CTileRotationCache& rotCache,
                                   const Tile* filter,
                                   int rotateBy);

  ///@brief Add contribution from an input tile, including masking the filter if
  /// needed.
  ///@param output Output tile
  ///@param rotCache A cache of rotations computed on the same input tile in
  ///@param maskGenerator A generator for masks to multiply the filter with
  ///@param filter Filter tile
  ///@param rowRotateBy Number of slots to rotate on the row dimension
  ///@param colRotateBy Number of slots to rotate on the col dimension
  void maskAndAddContributionToOutputTile(
      CTile& output,
      std::mutex& outputMutex,
      CTileRotationCache& rotCache,
      const TTInterleavedTileMask& maskGenerator,
      const Tile* filter,
      int rowRotateBy,
      int colRotateBy);

  /// @brief get the filter Tile pointed by iterator f. When convolution is set
  /// to "SAME" may apply a mask
  /// @param f An iterator pointing to the filter tile
  const Tile& getFilterTile(const TensorIterator& f);

  /// @brief get the embedded col operator. For VALID mode this is simply the
  /// operator but in SAME mode an iterator of the range
  /// -filterCols/2,...,filterCols/2 is embedded 0,...,filterCols
  /// @param it The iterator in 0,...,filterCols
  /// @return The iterator in -filterCols/2,...,filterCols/2
  int getColOffsetFromPivot(int it) const;

  /// @brief get the embedded col operator. For VALID mode this is simply the
  /// operator but in SAME mode an iterator of the range
  /// -filterCols/2,...,filterCols/2 is embedded 0,...,filterCols
  /// @param it The iterator in 0,...,filterCols
  /// @return The iterator in -filterCols/2,...,filterCols/2
  int getRowOffsetFromPivot(int it) const;

  ///@brief determine whether a mask vector is all zeroes
  static bool allZeroes(const std::vector<int>& mask);

  ///@brief multiply a filter tile with a mask. Works for PTile and CTile.
  void multiplyFilterWithMask(std::shared_ptr<Tile>& filterTile,
                              const std::vector<int>& mask) const;

  ///@brief Checks if the padding mode and padding sizes are compatible, and
  /// throws an exception otherwise
  void validatePaddingAndSizes() const;

public:
  ///@brief Setup a convolution of tensor with interleaved dimensions
  ///@param input Input tensor
  ///@param filters Filters tensor
  ///@param biases Biases tensor
  ///@param strideRows Number of strides on the rows dimension
  ///@param strideCols Number of strides on the cols dimension
  ///@param CXYFB Determines the shapes of the tensors (CXYFB or FXYCB)
  ///@param mode The convolution mode VALID or SAME or SAME_WITH_PADDING
  TTConvolutionInterleaved(const std::shared_ptr<const CTileTensor> input,
                           const TileTensor& filters,
                           const TileTensor& biases,
                           int strideRows,
                           int strideCols,
                           bool CXYFB,
                           const Padding2d& padding = Padding2d())
      : he(input->getHeContext()),
        input(input),
        filters(&filters),
        biases(&biases),
        strideRows(strideRows),
        strideCols(strideCols),
        CXYFB(CXYFB),
        padding(padding),
        sumOverChannels(true)

  {
    setBasicDimensions();
    validatePaddingAndSizes();
  }

  TTConvolutionInterleaved(const std::shared_ptr<const CTileTensor> input,
                           int filterRows,
                           int filterCols,
                           int strideRows,
                           int strideCols,
                           bool CXYFB,
                           const Padding2d& padding = Padding2d())
      : he(input->getHeContext()),
        input(input),
        filters(nullptr),
        biases(nullptr),
        strideRows(strideRows),
        strideCols(strideCols),
        CXYFB(CXYFB),
        filterRows(filterRows),
        filterCols(filterCols),
        filterFilters(1),
        padding(padding),
        sumOverChannels(false)
  {
    setBasicDimensions();
    validatePaddingAndSizes();
  }

  ///@brief Check whether a convolution with certain stride can be applied on a
  /// dimention
  ///@param dim Dimension (row or col) to apply convolution on
  ///@param stride The stride in question
  static bool dimMatchesStride(const TTDim& dim, int stride);

  ///@brief Quick test if given shapes support the physical padding mode.
  /// Returns true if physical padding is supported.
  ///
  ///@param inputShape Input shape
  ///@param padding The requested amount of padding
  ///@param addDiagnosis Whether to include a human readable explanation to a
  /// false results (slows runtime)
  ///@param diagnosis Human readable explanation, when flag is on.
  static bool supportsPhysicalPadding(const TTShape& inputShape,
                                      const Padding2d& padding,
                                      bool addDiagnosis,
                                      std::string& diagnosis);

  ///@brief Quick test if given shapes support the physical padding mode.
  /// Returns true if physical padding is supported.
  ///
  ///@param inputShape Input shape
  ///@param padding The requested amount of padding
  static bool supportsPhysicalPadding(const TTShape& inputShape,
                                      const Padding2d& padding);

  ///@brief Apply the convolution and return the output tensor
  CTileTensor getConvolution();
};
} // namespace helayers

#endif /* TTCONVOLUTION_INTERLEAVED_H_ */
