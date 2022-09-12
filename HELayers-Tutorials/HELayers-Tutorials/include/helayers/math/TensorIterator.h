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

#ifndef SRC_HELAYERS_TENSOR_ITERATOR_H
#define SRC_HELAYERS_TENSOR_ITERATOR_H

#include <vector>
#include <iostream>
#include "TensorIteratorTarget.h"

namespace helayers {

/// A class for iterating over a tensor
class TensorIterator : public TensorIteratorTarget
{
protected:
  const std::vector<DimInt> extents;
  const bool firstOrder;

  DimInt pos;

  std::vector<DimInt> curIndexes;

  std::vector<DimInt> dimSteps;

  /// Total number of elements in tensor
  DimInt numElements;

  TensorIteratorTarget* target = nullptr;

  /// Compute the step size of each dim
  void calcDimSteps();

  /// Updates targets added to this iterator this dim has changed
  void updateTarget(DimInt dim);

  /// Moves given dim one step forward.
  /// Returns true if it didn't rewind back to 0
  bool nextDim(DimInt dim);

public:
  /// Constructs an iterator for a tensor.
  /// After construction, the iterator is at the first element (indices
  /// (0,0,0...0))
  ///  @param[in] extents shape of tensor
  ///  @param[in] firstOrder whether first dimension is moving first
  TensorIterator(const std::vector<DimInt>& extents, bool firstOrder = false);

  /// Move to next element. Returns false if no next element.
  /// In case it returns false it also rewinds.
  bool next();

  /// Advance iterator starting from specified dim.
  /// If it is a firstOrder iterator, it doesn't move dimensions 0...dim-1,
  /// Otherwise it doesn't move dimensions dim+1,...lastDim.
  /// Returns false if no next element in the movable dimension range.
  /// In case it returns false it also rewinds the movable dimensions.
  bool nextFromDim(DimInt dim);

  /// Print state for debug purposes.
  ///  @param[in] out output stream
  virtual void debugPrint(std::ostream& out) const;

  /// Rewind to first element
  void reset();

  /// Jump to location for a given dimension.
  ///  @param[in] dim dimension to move along
  ///  @param[in] val new index value along specified dimension
  void set(DimInt dim, DimInt val) override;

  ///@brief Returns the index at specified dimension.
  ///
  ///@param dim Queried dimension
  inline DimInt get(DimInt dim) const { return curIndexes[dim]; }

  ///@brief Move one step along specified dim.
  /// If we reached the end along this dimension, we wrap back
  /// to 0, but no other dimension is moved.
  /// Returns the updated index along this dimension.
  ///
  ///@param dim Dimension to move along
  DimInt advanceDim(DimInt dim);

  /// Returns index location at current position
  inline const std::vector<DimInt>& getCurIndexes() const { return curIndexes; }

  /// Returns flat index at current position.
  inline const DimInt getPos() const { return pos; }

  ///@brief Set the flat index to a specific value.
  /// Note this is a relatively inefficient call (O(number of dims))).
  ///
  ///@param val New flat index value
  void setPos(DimInt val);

  /// Returns whether we are using the first dimension moving first convention.
  inline bool isFirstOrder() const { return firstOrder; }

  /// Returns number of dimensions
  inline DimInt getNumDims() const { return extents.size(); }

  /// Returns total number of elements in tensor
  inline DimInt getNumElements() const { return numElements; }

  /// Returns extents tracked by this iterator
  inline const std::vector<DimInt> getExtents() const { return extents; }

  /// Adds a target to notify when an index changes.
  /// @param[in] target target to add
  void setTarget(TensorIteratorTarget& target);

  ///@brief Is iterator at a valid position.
  /// For this class it always return true. For some inheriting classes it might
  /// not.
  virtual bool isValid() const { return true; }
};

///@brief A tensor iterator that allows setting illegal values, putting it
/// in an invalid state
///
class PermissiveTensorIterator : public TensorIterator
{
  std::vector<bool> validDim;
  DimInt numInvalidDims = 0;

public:
  ///@brief See parent class
  PermissiveTensorIterator(const std::vector<DimInt>& extents,
                           bool firstOrder = false);

  ///@brief  Returns false if any dimension was set to an invalid position
  bool isValid() const override { return numInvalidDims == 0; }

  ///@brief See parent method
  void set(DimInt dim, DimInt val) override;

  ///@brief See parent method
  void debugPrint(std::ostream& out) const override;
};

std::ostream& operator<<(std::ostream& out, const TensorIterator& ei);
} // namespace helayers

#endif /* SRC_HELAYERS_TENSOR_ITERATOR_H */
