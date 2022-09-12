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

#ifndef SRC_HELAYERS_CTILETENSOR_H
#define SRC_HELAYERS_CTILETENSOR_H

#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>
#include "helayers/hebase/hebase.h"
#include "helayers/hebase/utils/Saveable.h"
#include "TTShape.h"
#include "PTileTensor.h"
#include "TileTensor.h"

/// \mainpage Main page
/// \section Main HELAYERS
///
/// Helayers provides a smooth transition from a
/// non-encrypted environment to a privacy preserving one It can optimize
/// complicated processes that would otherwise require cryptographic and
/// specific library knowledge to run efficiently. Also, it enables HE-based
/// computations without dependence on a specific homomorphic encryption
/// library.
///
/// Here are some of the main classes:
/// helayers::HeContext helayers::CTile helayers::PTile helayers::CTileTensor
/// helayers::NeuralNet helayers::LogisticRegression

namespace helayers {

namespace circuit {
class Runner;
}

class TTEncoder;

/// An encrypted tile tensor.
/// A tile tensor is a data structure for storing tensors
/// (multipy dimensional arrays, e.g., vectors, matrices, or higher dimensional
/// arrays)
/// in a set of fixed size tiles, in this case ciphertexts.
class CTileTensor : public TileTensor
{
  REGISTER_SAVEABLE_HEADER;

public:
  ///@brief Underlying type used to store the external tensor
  typedef boost::numeric::ublas::tensor<CTile, PTileTensor::tensorFormat>
      ExternalTensorType;

protected:
  ImplStridesType getImplTensorStrides() const override
  {
    return tiles.strides();
  };

  /// Returns the tile at a given set of indices.
  /// Used for performing per-tile operations.
  template <class... size_ts>
  inline CTile& at(size_t i, size_ts... is)
  {
    return tiles.at(i, is...);
  }

  /// Returns the tile at a given set of indices.
  /// Used for performing per-tile operations.
  template <class... size_ts>
  inline const CTile& at(size_t i, size_ts... is) const
  {
    return tiles.at(i, is...);
  }

private:
  const HeContext* he;

  ///@brief Used to store the tile tensor content.
  ExternalTensorType tiles;

  // Validate object is ready to perform action with other
  void validateActionValidity(const TileTensor& other) const;

  // Construct mapping between tiles for binary operators
  std::vector<std::tuple<int, int, int>> binaryOpBuildBase(
      const TTShape& otherShape,
      bool overrideUnknowns);

  // Returns the combined external sizes after binary op
  std::vector<int> binaryOpGetNewExternalSizes(
      const std::vector<int>& aExternalSizes,
      const std::vector<int>& bExternalSizes) const;

  // Creating a mapping of tiles for binary operation
  std::vector<std::tuple<int, int, int>> binaryOpMapTiles(
      const std::vector<int>& aExternalSizes,
      const std::vector<int>& bExternalSizes) const;

  ///@brief Performs either sum or duplicate operation over a dim.
  ///
  /// Note that to get the sum's result duplicated, you need to sum
  /// over the first non-trivial dim, and sum over all elements along this dim.
  ///
  /// Returns the number of elements actually summed/duplicated.
  ///
  ///@param dim Dimension to sum/duplicate over
  ///@param minNumElements Minimal number of elements to sum/duplicate. Actual
  /// number will be rounded to nearest power of 2 from above.
  /// Value of -1 means sum/duplicate all elements.
  ///@param duplicate True means duplicate, false means sum
  int sumOrDuplicateOverDim(int dim,
                            int minNumElements = -1,
                            bool duplicate = false);

  ///@brief Returns a plaintext mask that contains the given value in all the
  /// slots correspond to used slots of this, and zeros in all the slots
  /// correspond to unused slots of this. The mask will have the same chain
  /// index as this.
  ///
  ///@param val the value to put in the scalar mask.
  PTileTensor getPlainScalarMask(double val) const;

  ///@brief Returns CTileTensor obtained by duplicating and then masking the
  /// given CTile. First, a CTileTensor whose shape is the same as this and
  /// which contains "c" in all of its tiles is created. Then, all slots
  /// corresponding to unused slots of this are zeroed. Finally, the resulting
  /// duplicated and masked CTileTensor is returned.
  ///
  ///@param c The CTile to duplicate and mask.
  CTileTensor duplicateAndMaskTile(const CTile& c) const;

  void saveImpl(std::ostream& stream) const override;

  void loadImpl(std::istream& stream) override;

  ///@brief Aligns the chain indexes of all tiles to have the same value, by
  /// reducing to the minimal one.
  ///
  void alignChainIndexes();

  ///@brief Helper method for getConcatenate method. Builds and returns the
  /// result of the concatenation.
  ///
  ///@param first The first tile tensor in the concatenation.
  ///@param second The second tile tensor in the concatenation.
  ///@param dim The dimension to concatenate along.
  ///@param exactFit Whether or not the first tile tensor exactly fits, meaning
  /// it fully populates its tiles along the concatenation dimension.
  ///@param resOriginalSize The desired original size of the result.
  static CTileTensor buildConcatenateResult(const CTileTensor& first,
                                            const CTileTensor& second,
                                            DimInt dim,
                                            bool exactFit,
                                            DimInt resOriginalSize);

  ///@brief Verifies that every dimension of this shape is either incomplete,
  /// t_i/t_i or n_i?/t_i for n_i < t_i. If a dimension is incomplete, it will
  /// be replaced with t_i/t_i.
  ///
  /// @param shape The shape to verify and complete.
  /// @throw invalid_argument If the given shape does not satisfy the above
  ///                         requirements.
  static void verifyAndCompleteTileShape(TTShape& shape);

  friend class TTEncoder;
  friend class TTConvolution;
  friend class TTConvolutionInterleaved;
  friend class TTFunctionEvaluator;
  friend class TTPermutator;
  friend class circuit::Runner;

public:
  /// Construct an empty object.
  /// @param[in] he the context
  CTileTensor(const HeContext& he);

  /// Copy constructor.
  /// @param[in] src object to copy
  CTileTensor(const CTileTensor& src);

  /// Move constructor.
  /// @param[in] src object to move
  CTileTensor(CTileTensor&& src);

  /// Construct an object of a certain chape
  /// @param[in] he the context
  /// @param[in] s shape of new object
  /// @param[in] isPacked TRUE if this tensor is filled with value, FALSE
  /// otherwise
  CTileTensor(const HeContext& he, const TTShape& s);

  ~CTileTensor();

  /// Copy other tile tensor to this one.
  /// @param[in] src object to copy
  CTileTensor& operator=(const CTileTensor& src);

  /// Move other tile tensor to this one.
  /// @param[in] src object to move
  CTileTensor& operator=(CTileTensor&& src);

  ///@brief Creates a CTileTensor with one tile.
  ///
  ///@param he    The HeContext.
  ///@param shape The shape of the CTileTensor to create. Each dimension of this
  ///             shape must be either incomplete, t_i/t_i or n_i?/t_i for n_i <
  ///             t_i. If a dimension is incomplete, it will be replaced with
  ///             t_i/t_i in the resulting CTileTensor.
  ///@param tile  The only tile of the created CTileTesnor.
  ///@throw invalid_argument If the given shape does not satisfy the
  ///                        requirement above.
  static CTileTensor createSingleTile(const HeContext& he,
                                      const TTShape& shape,
                                      const CTile& tile);

  ///@brief Creates a CTileTensor from the given vector of CTiles. "tilesVec"
  /// vector is expected to have a first-order orientation. The created
  /// CTileTensor will have the given shape, which is assumed to be complete
  /// (i.e. with all original sizes set), not to contain any duplicated dims and
  /// to have all unused slots unknown.
  ///
  ///@param he       The HeContext to initialize the created CTileTensor with.
  ///@param shape    The shape of the resulting CTileTensor.
  ///@param tilesVec The vector of CTiles, in first-order orientation.
  ///@throw invalid_argument If "shape" doesn't satisfy the above assumptions.
  static CTileTensor createFromCTileVector(const HeContext& he,
                                           const TTShape& shape,
                                           const std::vector<CTile>& tilesVec);

  /// Elementwise add with other CTileTensor.
  /// Relies on CTile::add.
  /// @param[in] other other CTileTensor to add.
  void add(const CTileTensor& other);

  /// Elementwise add with other CTileTensor.
  /// Relies on CTile::addRaw.
  /// @param[in] other other CTileTensor to add.
  void addRaw(const CTileTensor& other);

  /// Elementwise subtract other CTileTensor from this.
  /// Relies on CTile::sub.
  /// @param[in] other other CTileTensor to subtract.
  void sub(const CTileTensor& other);

  /// Elementwise subtract other CTileTensor from this.
  /// Relies on CTile::subRaw.
  /// @param[in] other other CTileTensor to subtract.
  void subRaw(const CTileTensor& other);

  /// Elementwise multiply with other CTileTensor.
  /// Relies on CTile::multiply.
  /// @param[in] other other CTileTensor to multiply.
  void multiply(const CTileTensor& other);

  /// Elementwise multiply with other CTileTensor.
  /// Relies on CTile::multiplyRaw.
  /// @param[in] other other CTileTensor to multiply.
  void multiplyRaw(const CTileTensor& other);

  /// Elementwise add with other (plaintext) PTileTensor.
  /// Relies on CTile::addPlain.
  /// @param[in] plain other PTileTensor to add.
  void addPlain(const PTileTensor& plain);

  /// Elementwise add with other (plaintext) PTileTensor.
  /// Relies on CTile::addPlainRaw.
  /// @param[in] plain other PTileTensor to add.
  void addPlainRaw(const PTileTensor& plain);

  /// Elementwise subtract other (plaintext) PTileTensor from this.
  /// Relies on CTile::subPlain.
  /// @param[in] plain other PTileTensor to subtract.
  void subPlain(const PTileTensor& plain);

  /// Elementwise subtract other (plaintext) PTileTensor from this.
  /// Relies on CTile::subPlainRaw.
  /// @param[in] plain other PTileTensor to subtract.
  void subPlainRaw(const PTileTensor& plain);

  /// Elementwise multiply with other (plaintext) PTileTensor.
  /// Relies on CTile::multiplyPlain.
  /// @param[in] plain other PTileTensor to multiply.
  void multiplyPlain(const PTileTensor& plain);

  /// Elementwise multiply with other (plaintext) PTileTensor.
  /// Relies on CTile::multiplyPlainRaw.
  /// @param[in] plain other PTileTensor to multiply.
  void multiplyPlainRaw(const PTileTensor& plain);

  /// Add scalar to all tensor elements.
  /// Relies on CTile::addScalar.
  /// Will add unknowns to unused slots, unless the keepUnknownsClear is set,
  /// which causes a more costly operation.
  /// @param[in] val Value to add.
  /// @param[in] keepUnknownsClear If set and current shape has no unknowns, it
  /// will keep it that way.
  void addScalar(double val, bool keepUnknownsClear = false);

  /// Multiplies all used tensor elements by a given scalar. Consumes one chain
  /// index. As a side effect, unused elements will be zeroed, thus unknown
  /// unused elements will be eliminated.
  /// @param[in] val Value to multiply by.
  void multiplyScalar(double val);

  /// Elementwise square. Relies on CTile::square.
  void square();

  /// Elementwise squareRaw. Relies on CTile::squareRaw.
  void squareRaw();

  /// Elementwise negate. Relies on CTile::negate.
  void negate();

  ///@brief Performs elementwise multiplication between this and other, then
  /// sums over
  /// specified dim.
  ///
  /// Example usages:
  /// If both are vectors of same length, sum over dim 0 to get inner product
  /// If this is a matrix [a/t1,b/t2] and other is a vector [a/t1,*/t2], sum
  /// over dim 0
  /// will produce the matrix-vector multiplication.
  /// Similarly for matrix [a/t1,b/t2] and vector [*/t1,b/t2], sum over dim 1.
  ///
  /// For matrix-matrix multiplication use for example:
  /// Matrix1 [ a/t1,b/t2,* /t3] and Matrix2 [a/t1,* / t2, c/t3], sum over dim
  /// 0.
  /// or:
  /// Matrix1 [ a/t1,b/t2,* /t3] and Matrix2 [* / t1,b / t2, c/t3], sum over dim
  /// 1.
  ///
  ///@param other Other encrypted tile tensor
  ///@param sumDim Dimension to sum over
  void multiplyAndSum(const CTileTensor& other, int sumDim);

  ///@brief Performs elementwise multiplication between this and other, then
  /// sums over
  /// specified dim.
  ///
  /// See CTileTensor::multiplyAndSum for usage examples.
  ///
  ///@param other Other encoded tile tensor
  ///@param sumDim Dimension to sum over
  void multiplyPlainAndSum(const PTileTensor& other, int sumDim);

  ///@brief Applies bitwise operation on each two corresponding tiles.
  ///@param other Other encrypted tile tensor
  ///@param be BitwiseEvaluator of the HeContext corresponding to this
  /// CTileTensor
  ///@param method A BitwiseEvaluator method to apply. The method must receive
  /// two CTiles as input and return a CTile
  using BitwiseEvaluatorMethod =
      CTile (BitwiseEvaluator::*)(const CTile&, const CTile&) const;
  void applyBitwiseMethod(const CTileTensor& other,
                          const BitwiseEvaluator& be,
                          BitwiseEvaluatorMethod method);

  /// Calls corresponding function for every CTile
  void relinearize();

  /// Calls corresponding function for every CTile
  void rescale();

  /// Calls corresponding functions for every CTile
  void relinearizeAndRescale();

  /// Sums over a dimension.
  /// @param[in] dim Dimension to sum over.
  void sumOverDim(int dim);

  /// Sums tile over a dimension
  void sumTilesOverDim(int dim);

  ///@brief Sum inside tiles.
  ///
  /// This method assumes the external size along this dimension is 1.
  /// It reduces the original size to 1 by summing inside the remaining tile.
  ///
  /// The result will be duplicated if this is the first non-trivial dimension.
  /// Note if the originalSize is 1, this call will still incur rotations if
  /// this is the first non-trivial dim, to satisfy the duplicated result
  /// requirement.
  ///
  ///@param dim Dimension to sum over.
  void sumInTilesOverDim(int dim);

  /// Multiplies over a dimension.
  /// @param[in] dim Dimension to multiply over.
  void multiplyOverDim(int dim);

  /// Multiplies tile over a dimension
  void multiplyTilesOverDim(int dim);

  /// Cause a dimension to become duplicated.
  /// @param[in] dim Dimension to duplicate.
  void duplicateOverDim(int dim);

  /// Clears all unknowns from this tile tensor.
  void clearUnknowns();

  /// Flattens consecutive fully-duplicated dimensions into a single
  /// fully-duplicated dimension. The Flattened dimension will have tile size
  /// equals to the product of tile sizes of all the flattened dimensions, and
  /// will be considered interleaved if and only if all the flattened dimensions
  /// are.
  /// @param[in] startDim The first dimension to be flattened.
  /// @param[in] endDim The dimension in which to end the flattening
  /// (exclusive). Flatten(a, b) will flatten dimensions [a,b).
  /// @throw runtime_error If the specified dimensions do not exist, or are not
  /// fully duplicated, or if startDim >= endDim.
  void flatten(int startDim, int endDim);

  ///@brief Reorders the dimensions of this tensor. The relative order of
  /// dimensions with tile size > 1 must be kept.
  /// For example, given {1,2,0}, the current second dimension will become
  /// first,
  /// the current third dimension will become second, and the current first
  /// dimension will become last.
  ///@param dimOrder New order of dimensions
  /// @throw runtime_error If dimOrder size doesn't match number of dims.
  /// @throw runtime_error If the relative order of dimensions with tile size >
  /// 1 is not the same as in dimOrder.
  void reorderDims(const std::vector<DimInt>& dimOrder);

  ///@brief Adds a dim with original size and tile size 1 to this tensor at
  /// given place.
  ///@param place Place of the new dimension, default is last.
  void addDim(int place = -1);

  ///@brief Removes a dim from this tensor. Only for dim with original size 1
  /// and tile size 1. Number of dims before calling the function must be at
  /// least 3.
  ///@param dim Dim to remove
  /// @throw runtime_error If dim doesn't exists
  /// @throw runtime_error If dim is not of size 1
  /// @throw runtime_error If number of dims is less than 3
  void removeDim(DimInt dim);

  /// Calls reduceChainIndex function for every CTile
  void reduceChainIndex();

  /// Calls corresponding function for every CTile
  void setChainIndex(const CTileTensor& other);

  /// Calls corresponding function for every CTile
  void setChainIndex(int chainIndex);

  /// Calls corresponding function for every CTile
  void adjustChainIndex(int chainIndex);

  /// Returns chain index of tiles.
  /// They are maintained to have equal chain index.
  int getChainIndex() const override;

  /// Returns the scale of the tiles.
  /// They are maintained to have equal scales.
  double getScale() const;

  /// Decrypt the CTileTensor and encrypts it again to bring it to top chain
  /// index.
  /// @throw runtime_error if HeContext doesn't have the secret key.
  void reEncrypt();

  /// A testing method
  void fakeBootstrap();

  ///@brief Performs bootstrap on all the tiles of this tile tensor
  ///
  void bootstrap();

  const CTile& getTileAt(const std::vector<DimInt>& inds) const override
  {
    return tiles.at(tileIndsToFlatInd(inds));
  }

  const CTile& getTileByFlatIndex(DimInt i) const override;

  CTileTensor getAdd(const CTileTensor& other) const override;

  CTileTensor getAddRaw(const CTileTensor& other) const override;

  void addTo(CTileTensor& other) const override;

  void addToRaw(CTileTensor& other) const override;

  void subFrom(CTileTensor& other) const override;

  void subFromRaw(CTileTensor& other) const override;

  void multiplyTo(CTileTensor& other) const override;

  void multiplyToRaw(CTileTensor& other) const override;

  /// Returns the result of elementwise sub of this and other CTileTensor.
  /// Relies on CTile::sub.
  /// @param[in] other other CTileTensor to sub.
  CTileTensor getSub(const CTileTensor& other) const;

  /// Returns the result of elementwise sub of this and other CTileTensor.
  /// Relies on CTile::subRaw.
  /// @param[in] other other CTileTensor to sub.
  CTileTensor getSubRaw(const CTileTensor& other) const;

  /// Returns the result of elementwise multiplication of this and other
  /// CTileTensor.
  /// Relies on CTile::multiply.
  /// @param[in] other other CTileTensor to multiply.
  CTileTensor getMultiply(const CTileTensor& other) const override;

  /// Returns the result of elementwise multiplication of this and other
  /// CTileTensor.
  /// Relies on CTile::multiplyRaw.
  /// @param[in] other other CTileTensor to multiply.
  CTileTensor getMultiplyRaw(const CTileTensor& other) const override;

  /// @brief Returns the result of elementwise add with other (plaintext)
  /// PTileTensor. Relies on CTile::addPlain.
  /// @param[in] plain other PTileTensor to add.
  CTileTensor getAddPlain(const PTileTensor& plain) const;

  /// @brief Returns the result of elementwise add with other (plaintext)
  /// PTileTensor. Relies on CTile::addPlainRaw.
  /// @param[in] plain other PTileTensor to add.
  CTileTensor getAddPlainRaw(const PTileTensor& plain) const;

  /// @brief Returns the result of elementwise subtract with other (plaintext)
  /// PTileTensor. Relies on CTile::subPlain.
  /// @param[in] plain other PTileTensor to subtract.
  CTileTensor getSubPlain(const PTileTensor& plain) const;

  /// @brief Returns the result of elementwise subtract with other (plaintext)
  /// PTileTensor. Relies on CTile::subPlainRaw.
  /// @param[in] plain other PTileTensor to subtract.
  CTileTensor getSubPlainRaw(const PTileTensor& plain) const;

  /// @brief Returns the result of elementwise multiplication with other
  /// (plaintext) PTileTensor. Relies on CTile::multiplyPlain.
  /// @param[in] plain other PTileTensor to multiply.
  CTileTensor getMultiplyPlain(const PTileTensor& plain) const;

  /// @brief Returns the result of elementwise multiplication with other
  /// (plaintext) PTileTensor. Relies on CTile::multiplyPlainRaw.
  /// @param[in] plain other PTileTensor to multiply.
  CTileTensor getMultiplyPlainRaw(const PTileTensor& plain) const;

  /// @brief Returns the result of addition with a scalar.
  /// @param[in] val Scalar to add.
  CTileTensor getAddScalar(double val) const;

  /// @brief Returns the result of multiplication with a scalar.
  /// @param[in] val Scalar to multiply.
  CTileTensor getMultiplyScalar(double val) const;

  /// @brief Returns the result of elementwise squaring.
  /// Relies on CTile::square.
  CTileTensor getSquare() const;

  /// @brief Returns the result of elementwise squaring.
  /// Relies on CTile::squareRaw.
  CTileTensor getSquareRaw() const;

  ///@brief Returns the result of elementwise multiplication of this and other
  /// and summing over specified dim.
  ///
  /// See CTileTensor::multiplyAndSum for usage examples.
  ///
  ///@param other Other encrypted tile tensor
  ///@param dim Dimension to sum over
  CTileTensor getMultiplyAndSum(const CTileTensor& other,
                                int dim) const override;

  ///@brief Returns the result of elementwise multiplication of this and other
  /// and summing over specified dim.
  ///
  /// See CTileTensor::multiplyAndSum for usage examples.
  ///
  ///@param other Other encoded tile tensor
  ///@param dim Dimension to sum over
  virtual CTileTensor getMultiplyPlainAndSum(const PTileTensor& other,
                                             int dim) const;

  ///@brief Equivalent to other.multiplyAndSum(*this, dim)
  ///
  ///@param other Other encrypted tile tensor
  ///@param dim Dimension to sum over
  void multiplyAndSumTo(CTileTensor& other, int dim) const override;

  ///@brief Returns result with applying CTile::relinearize to all tiles.
  CTileTensor getRelinearize() const;

  ///@brief Returns result with applying CTile::rescale to all tiles.
  CTileTensor getRescale() const;

  ///@brief Returns result of summing over a dimension
  ///
  ///@param dim Dimension to sum over
  CTileTensor getSumOverDim(int dim) const;

  ///@brief Returns results of summing tiles over a dimension.
  /// Note this is function is used as an intermediate step when summing over
  /// dim. Most users should call getSumOverDim() directly.
  ///@param dim Dimension to sum over
  CTileTensor getSumTilesOverDim(int dim) const;

  ///@brief Returns results of summing inside tiles over a dimension.
  /// Note this is function is used as an intermediate step when summing over
  /// dim. Most users should call getSumOverDim() directly.
  ///@param dim Dimension to sum over
  CTileTensor getSumInTilesOverDim(int dim) const;

  ///@brief Returns results of duplicating a dimension.
  ///@param dim Dimension duplicate.
  CTileTensor getDuplicateOverDim(int dim) const;

  ///@brief Returns results after multiplying with a mask to clear unkonwns.
  CTileTensor getClearUnknowns() const;

  ///@brief Returns the results after flatting some dimensions.
  /// See flatten()
  /// @param startDim The first dimension to be flattened.
  /// @param endDim The dimension in which to end the flattening
  /// (exclusive). Flatten(a, b) will flatten dimensions [a,b).
  CTileTensor getFlatten(int startDim, int endDim) const;

  ///@brief  Returns results after applying CTile::reduceChainIndex to all
  /// tiles.
  CTileTensor getReduceChainIndex() const;

  ///@brief Returns the results after setting chain index to equal other object.
  ///
  ///@param other Other object to take chain index from.
  CTileTensor getSetChainIndex(const CTileTensor& other) const;

  ///@brief Returns the results after setting chain index.
  ///
  ///@param chainIndex Chain index to set.
  CTileTensor getSetChainIndex(int chainIndex) const;

  ///@brief Returns the result of convolution between this and filters, with
  /// specified strides.
  /// Assumes all tensors are packed in SIMD fashion.
  ///@param filters Convolution filters
  ///@param biases Biases to add
  ///@param strideRows Filter strides on rows
  ///@param strideCols Filter strides on cols
  CTileTensor getConvolution(const CTileTensor& filters,
                             const CTileTensor& biases,
                             int strideRows,
                             int strideCols) const;

  ///@brief Returns the concatenation of this and other along the provided
  /// dimension. This operation may consume one chain index, and may clear
  /// unknown values as a side effect. The shapes of this and other must:
  /// (1) have the same number of dimensions.
  /// (2) for every dimention other than the concatenation dimention, dimention
  /// must be equal on original slots (same tile size, original size,
  /// interleaved mode, interleaved external size).
  /// (3) the concatenation dimention must have the same tile sizes, with no
  /// duplicaited values and not in interleaved mode.
  ///
  ///@param other The other tile tensor to concatenate to this.
  ///@param dim The dimension to concatenate along.
  CTileTensor getConcatenate(const CTileTensor& other, DimInt dim) const;

  ///@brief Returns the tile tensor resulted by re-ordering the dimensions of
  /// this tile tensor. This operation may consume one chain index, and may
  /// clear unknown values as a side effect.
  ///
  ///@param dimOrder The new order of dimensions, must include all the
  /// dimensions of the tile tensor.
  CTileTensor getReorderDims(const std::vector<DimInt>& dimOrder) const;

  ///@brief Returns a slice of the tensor in specific dimension. Result has the
  /// same number of dimensions as the original tensor. For dim with tile size
  /// of 1, getSlice is supported for every valid startIndex and
  /// sliceDepth. For a non-interleaved dim with tile size > 1, getSlice
  /// is supported only for startIndex which is a multiple of the tile size. For
  /// interleaved dim with tile size > 1, getSlice is supported only for
  /// startIndex 0.
  ///
  ///@param dim        The dimension to slice along. Specifying negative value
  ///                  will count from the last dim backwards (-1 for the last
  ///                  dim, -2 for the previoud one etc.).
  ///@param startIndex The index to start slicing at along the dimension
  ///@param sliceDepth The depth/size of the slice along the dimension
  ///@throw invalid_argument If the given dimension is greater or equal to the
  ///                        total number of dimensions in this TileTensor
  ///@throw invalid_argument If startIndex + sliceDepth is greater than the
  ///                        original size of the given dimension.
  ///@throw invalid_argument If the tile size of the given dimension is
  ///                        different than 1.
  CTileTensor getSlice(DimInt dim,
                       DimInt startIndex,
                       DimInt sliceDepth = 1) const;

  ///@brief Turns interleave flag on or off for specific dim without changing
  /// the underlying tiling. Can be used only if dim original size is 1.
  ///@param dim Which dimension to set
  ///@param isInterleaved Whether this dimension is interleaved
  ///@throw invalid_argument If dim doesn't exists
  ///@throw invalid_argument If dim original size > 1
  void setDimInterleaved(DimInt dim, bool isInterleaved);

  /// Combines the given CTileTensors into one CTileTensor. All of the given
  /// CTileTensors must have the same shape and chain index. Let cts be of
  /// dimensions [d0,d1,d2]. The shape of the combined CTileTensor is given by
  /// adding the dimensions [d0/1,d1/2,d2/1] to the shape of the given
  /// CTileTensors. cts[i][j][k][...] will be copied into indexes [i,j,k,...] of
  /// the combined CTileTensor.
  ///
  ///@param cts A 3D tensor of CTileTensors to combine.
  ///@throw invalid_Argument If "cts" does not represent a 3D tensor (e.g.
  ///                        if cts[i].size() != cts[j].size() for some i,j).
  ///@throw invalid_argument If one of cts, cts[0] or cts[0][0] is an empty
  ///       vector.
  ///@throw invalid_arugment If the given CTileTensors don't have different
  ///                        shapes or different chain indexes.
  static CTileTensor combine(
      const std::vector<std::vector<std::vector<CTileTensor>>>& cts);

  ///@brief Rotates this CTileTensor along the specified dimension (index
  /// (i+rot) % slotCount moves to i). The rotated dimension must have an
  /// original size equal to its tile size, and all other dimensions must have
  /// a tile size of 1.
  void rotateAlongDim(int dim, int rot);

  ///@brief Validates the integrity of the tile tensor.
  /// Used for internal checks only.
  /// Returns the maximal diff found.
  /// If uninitialized, returns 0
  ///@param epsilon Tolerance for diffs.
  ///@throw runtime_error if test fails
  double assertIntegrity(double epsilon = 1e-6) const override;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  int32_t getContextId() const override { return he->getContextId(); }

  inline const HeContext& getHeContext() const { return *he; }

  void sleep() override;

  void wakeup() override;
};

typedef std::shared_ptr<const CTileTensor> CTileTensorCPtr;

} // namespace helayers

#endif /* SRC_HELAYERS_CTILETENSOR_H */
