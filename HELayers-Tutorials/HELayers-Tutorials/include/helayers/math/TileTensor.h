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

#ifndef SRC_HELAYERS_TILETENSOR_H
#define SRC_HELAYERS_TILETENSOR_H

#include "helayers/hebase/utils/Saveable.h"
#include "TTShape.h"
#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>

/// enum for lazy encoding enable/disable
enum LazyMode
{
  NONE,
  LAZY_ENCODING
};

namespace helayers {

class CTileTensor;
class Tile;

///@brief A tile tensor object.
/// A tile tensor is a data structure for storing tensors
/// (multipy dimensional arrays, e.g., vectors, matrices, or higher dimensional
/// arrays)
/// in a set of fixed size tiles.
class TileTensor : public Saveable
{
protected:
  bool isPacked = false;

  TTShape shape;

  LazyMode lazyMode = NONE;

  int lazyChainIndex = 0;

  typedef boost::numeric::ublas::first_order tensorFormat;
  typedef boost::numeric::ublas::basic_strides<std::size_t, tensorFormat>
      ImplStridesType;

  /// Returns the strides of the underlying tensor object used to store tiles
  virtual ImplStridesType getImplTensorStrides() const = 0;

  size_t tileIndsToFlatInd(const std::vector<DimInt>& inds) const;

public:
  ///@brief Construct an empty object
  TileTensor() {}

  ///@brief Construct a new Tile Tensor object
  ///
  ///@param sh Tile tensor shape
  TileTensor(const TTShape& sh) : shape(sh) {}

  ///@brief Construct a new Tile Tensor object
  ///
  ///@param sh Tile tensor shape to move
  TileTensor(TTShape&& sh) : shape(std::move(sh)) {}

  virtual ~TileTensor();

  ///@brief Asserts that object is packed (filled with content).
  ///@throw runtime_error if object is not packed.
  void validatePacked() const;

  /// Returns this object tile tensor shape.
  inline const TTShape& getShape() const { return shape; }

  ///@brief Returns whether this object was packed with a tensor.
  inline bool getIsPacked() const { return isPacked; }

  /// Returns the number of tiles this tile tensor uses.
  virtual int getNumUsedTiles() const;

  ///@brief Returns an iterator for iterating over the external tensor
  TensorIterator getExternalIterator() const;

  ///@brief Returns the tile specified by the given indices.
  ///
  ///@param inds Indices of tiles. Their number should match the number of
  /// dimensions.
  virtual const Tile& getTileAt(const std::vector<DimInt>& inds) const = 0;

  ///@brief Returns the i'th tile in a flattened order according to the
  /// first-order convention
  ///
  ///@param i Index of tile
  virtual const Tile& getTileByFlatIndex(DimInt ind) const = 0;

  /// Returns the result of elementwise multiplication of this and other
  /// CTileTensor.
  /// Relies on CTile::multiply.
  /// @param[in] other other CTileTensor to multiply.
  virtual CTileTensor getMultiply(const CTileTensor& other) const = 0;

  /// Returns the result of elementwise multiplication of this and other
  /// CTileTensor.
  /// Relies on CTile::multiplyRaw.
  /// @param[in] other other CTileTensor to multiply.
  virtual CTileTensor getMultiplyRaw(const CTileTensor& other) const = 0;

  /// Returns the result of elementwise add of this and other CTileTensor.
  /// Relies on CTile::add.
  /// @param[in] other other CTileTensor to add.
  virtual CTileTensor getAdd(const CTileTensor& other) const = 0;

  /// Equivalent to other.add(*this)
  /// @param[in] other other CTileTensor to add.
  virtual void addTo(CTileTensor& other) const = 0;

  /// See addTo()
  virtual void addToRaw(CTileTensor& other) const = 0;

  /// Equivalent to other.sub(*this)
  /// @param[in] other other CTileTensor to subtract from.
  virtual void subFrom(CTileTensor& other) const = 0;

  /// See subFrom()
  virtual void subFromRaw(CTileTensor& other) const = 0;

  /// Equivalent to other.multiply(*this)
  /// @param[in] other other CTileTensor to multiply to
  virtual void multiplyTo(CTileTensor& other) const = 0;

  /// See multiplyTo()
  virtual void multiplyToRaw(CTileTensor& other) const = 0;

  /// Returns the result of elementwise add of this and other CTileTensor.
  /// Relies on CTile::addRaw.
  /// @param[in] other other CTileTensor to add.
  virtual CTileTensor getAddRaw(const CTileTensor& other) const = 0;

  ///@brief Validates the integrity of the tile tensor.
  /// Used for internal checks only.
  /// Returns the maximal diff found.
  /// If uninitialized, returns 0
  ///@param epsilon Tolerance for diffs.
  ///@throw runtime_error if test fails
  virtual double assertIntegrity(double epsilon = 1e-6) const = 0;

  ///@brief Returns the result of elementwise multiplication of this and other
  /// and summing over specified dim.
  ///
  /// See CTileTensor::multiplyAndSum for usage examples.
  ///
  ///@param other Other encrypted tile tensor
  ///@param dim Dimension to sum over
  virtual CTileTensor getMultiplyAndSum(const CTileTensor& other,
                                        int dim) const = 0;

  ///@brief Equivalent to other.multiplyAndSum(*this, dim)
  ///
  ///@param other Other encrypted tile tensor
  ///@param dim Dimension to sum over
  virtual void multiplyAndSumTo(CTileTensor& other, int dim) const = 0;

  /// Returns chain index of tiles.
  /// They are maintained to have equal chain index.
  virtual int getChainIndex() const = 0;

  ///@brief Returns an estimation of the memory usage, or -1 if not supported.
  int64_t getEstimatedMemoryUsageBytes() const;

  /// If Ptiles, encodes data if lazyMode is LAZY_ENCODING, and does nothing
  /// else
  virtual void wakeup() = 0;

  /// If Ptiles, if lazyMode is LAZY_ENCODING, clear all the tiles
  virtual void sleep() = 0;

  void setLazyMode(LazyMode lazyMode);

  virtual void setLazyChainIndex(int newChainIndex);

  bool isSleeping() const { return ((lazyMode == LAZY_ENCODING) && !isPacked); }

  ///@brief Reinterpret a given dimension by increasing or decreasing its
  /// original size. For more details and limitations, see
  /// TTDim::reinterpretOriginalSize.
  ///
  ///@param dim The dimension to reinterpret.
  ///@param newSize The new desired original size.
  void reinterpretOriginalSize(DimInt dim, DimInt newSize);
};

} // namespace helayers

#endif /* SRC_HELAYERS_CTILETENSOR_H */
