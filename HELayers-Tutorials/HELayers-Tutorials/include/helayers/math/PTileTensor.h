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

#ifndef SRC_HELAYERS_PTILETENSOR_H
#define SRC_HELAYERS_PTILETENSOR_H

#include "TileTensor.h"
#include "helayers/hebase/hebase.h"
#include "helayers/hebase/utils/Saveable.h"
#include "helayers/math/DoubleTensor.h"

namespace helayers {

class TTEncoder;

///@brief An encoded tile tensor.
/// A tile tensor is a data structure for storing tensors
/// (multipy dimensional arrays, e.g., vectors, matrices, or higher dimensional
/// arrays)
/// in a set of fixed size tiles, in this case PTile-s (encoded data).
class PTileTensor : public TileTensor
{
  REGISTER_SAVEABLE_HEADER;

public:
  /// Defines flattening convention of the external tensor to be first-order
  typedef boost::numeric::ublas::first_order tensorFormat;

  /// Returns whether we use the first-order convention for the external tensor
  static bool doesExternalFirstDimRunFirst() { return true; };

  // typedef boost::numeric::ublas::last_order tensorFormat;
  // static bool doesExternalFirstDimRunFirst() { return false; };

  ///@brief Underlying type used to store the external tensor
  typedef boost::numeric::ublas::tensor<PTile, tensorFormat> ExternalTensorType;

private:
  const HeContext* he;

  ExternalTensorType tiles;

  DoubleTensor* rawData = nullptr;

  void saveImpl(std::ostream& stream) const override;

  void loadImpl(std::istream& stream) override;

  friend class TTEncoder;
  friend class CTileTensor;

protected:
  ImplStridesType getImplTensorStrides() const override
  {
    return tiles.strides();
  };

  /// Returns the tile at a given set of indices.
  /// Used for performing per-tile operations.
  template <class... size_ts>
  inline PTile& at(size_t i, size_ts... is)
  {
    return tiles.at(i, is...);
  }

  /// Returns the tile at a given set of indices.
  /// Used for performing per-tile operations.
  template <class... size_ts>
  inline const PTile& at(size_t i, size_ts... is) const
  {
    return tiles.at(i, is...);
  }

public:
  ///@brief Construct an empty PTileTensor object
  ///
  ///@param he The HE context
  PTileTensor(const HeContext& he);

  /// Copy constructor.
  /// @param[in] src object to copy
  PTileTensor(const PTileTensor& src);

  ~PTileTensor();

  /// Copy other tile tensor to this one.
  /// @param[in] src object to copy
  PTileTensor& operator=(const PTileTensor& src);

  const PTile& getTileAt(const std::vector<DimInt>& inds) const override
  {
    return tiles.at(tileIndsToFlatInd(inds));
  }

  const PTile& getTileByFlatIndex(DimInt i) const override;

  ///@brief Calls corresponding function for every CTile
  void reduceChainIndex();

  ///@brief Calls setChainIndex for every CTile
  ///
  ///@param other Other tile tensor to match chain index
  void setChainIndex(const PTileTensor& other);

  ///@brief Calls setChainIndex for every CTile
  ///
  ///@param chainIndex value of chain index
  void setChainIndex(int chainIndex);

  int getChainIndex() const override;

  ///@brief Returns result after calling reduceChainIndex()
  PTileTensor getReduceChainIndex() const;

  ///@brief Returns result after calling setChainIndex()
  ///
  ///@param other Other tile tensor to match chain index
  PTileTensor getSetChainIndex(const PTileTensor& other) const;

  ///@brief Returns result after calling setChainIndex()
  ///
  ///@param chainIndex value of chain index
  PTileTensor getSetChainIndex(int chainIndex) const;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  double assertIntegrity(double epsilon = 1e-6) const override;

  CTileTensor getMultiply(const CTileTensor& other) const override;

  CTileTensor getMultiplyRaw(const CTileTensor& other) const override;

  CTileTensor getAdd(const CTileTensor& other) const override;

  CTileTensor getAddRaw(const CTileTensor& other) const override;

  void addTo(CTileTensor& other) const override;

  void addToRaw(CTileTensor& other) const override;

  void subFrom(CTileTensor& other) const override;

  void subFromRaw(CTileTensor& other) const override;

  void multiplyTo(CTileTensor& other) const override;

  void multiplyToRaw(CTileTensor& other) const override;

  ///@brief Returns the result of elementwise multiplication of this and other
  /// and summing over specified dim.
  ///
  /// See CTileTensor::multiplyAndSum for usage examples.
  ///
  ///@param other Other encrypted tile tensor
  ///@param dim Dimension to sum over
  CTileTensor getMultiplyAndSum(const CTileTensor& other,
                                int dim) const override;

  ///@brief Returns a slice of the tensor in specific dimension. Result has the
  /// same number of dimensions as the original tensor. For dim with tile size
  /// of 1, getSlice is supported for every valid startIndex and
  /// sliceDepthThe. For a non-interleaved dim with tile size > 1, getSlice
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
  PTileTensor getSlice(DimInt dim,
                       DimInt startIndex,
                       DimInt sliceDepth = 1) const;

  ///@brief Equivalent to other.multiplyPlainAndSum(*this, dim)
  ///
  ///@param other Other encrypted tile tensor
  ///@param dim Dimension to sum over
  void multiplyAndSumTo(CTileTensor& other, int dim) const override;

  int32_t getContextId() const override { return he->getContextId(); }

  void sleep() override;

  void wakeup() override;

  void setRawData(const DoubleTensor& newRawData);

  void setLazyChainIndex(int newChainIndex) override;

  int getNumUsedTiles() const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_PTILETENSOR_H */
