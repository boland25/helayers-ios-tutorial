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

#ifndef SRC_HELAYERS_TENSOR_ITERATOR_TARGET_H
#define SRC_HELAYERS_TENSOR_ITERATOR_TARGET_H

#include <vector>
#include <iostream>
#include "helayers/hebase/Types.h"

namespace helayers {

class TensorIterator;

///@brief A parent abstract class for iterators. Used as a target for another
/// iterator
/// to update on indices changing.
///
class TensorIteratorTarget
{
public:
  ~TensorIteratorTarget() {}
  /// Update dimension to a given location
  ///
  ///  @param[in] dim dimension to set
  ///  @param[in] val new index value along specified dimension
  virtual void set(DimInt dim, DimInt val) = 0;
};

///@brief An abstract iterator target that wraps another iterator.
/// This class only provides the member and the getter
class WrappingTensorIteratorTarget : public TensorIteratorTarget
{
protected:
  TensorIterator& it;

public:
  ///@brief Construct a new object
  ///
  ///@param i Underlying iterator.
  WrappingTensorIteratorTarget(TensorIterator& i) : it(i) {}

  ~WrappingTensorIteratorTarget() {}

  ///@brief Returns underlying iterator
  inline TensorIterator& getIterator() { return it; }

  ///@brief Returns underlying iterator
  inline const TensorIterator& getIterator() const { return it; }
};

///@brief An iterator target that converts indices to zeroes when the target
/// iterator
/// has size 1 along a dimension.
class ProjectingTensorIteratorTarget : public WrappingTensorIteratorTarget
{

public:
  ///@brief Construct a new object
  ///
  ///@param i Underlying iterator.
  ProjectingTensorIteratorTarget(TensorIterator& i)
      : WrappingTensorIteratorTarget(i)
  {}
  ~ProjectingTensorIteratorTarget() {}

  ///@brief See parent method
  void set(DimInt dim, DimInt val) override;
};

///@brief An iterator target that reorders dimensions
class ReorderingTensorIteratorTarget : public WrappingTensorIteratorTarget
{
  std::vector<DimInt> dimMap;

public:
  ///@brief Construct a new object
  ///
  ///@param i Underlying iterator.
  ReorderingTensorIteratorTarget(TensorIterator& i);

  ///@brief Specify the reorder by order of dims.
  /// For example, {1,2,0} means the current second dimension will become first,
  /// the current third dimension will become second
  /// and the current first dimension will become last
  ///
  ///@param dimOrder New dimension order
  void setDimOrder(const std::vector<DimInt>& dimOrder);

  ///@brief Specify the reorder by dimension map.
  /// For example, {1,2,0} means the current first dimension will become second,
  /// the current second dimension will become third,
  /// and the current last dimension will become first
  ///
  ///@param dimOrder New dimension order
  void setDimMap(const std::vector<DimInt>& dimOrder);

  ~ReorderingTensorIteratorTarget() {}

  /// See parent method
  void set(DimInt dim, DimInt val) override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_TENSOR_ITERATOR_H */
