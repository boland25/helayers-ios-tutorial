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

#ifndef SRC_HELAYERS_TENSORUTILS_H
#define SRC_HELAYERS_TENSORUTILS_H

#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>
#include "helayers/hebase/Types.h"

namespace helayers {

///@brief Class for various tensor utilities
class TensorUtils
{

public:
  ///@brief Returns basic extents object of tensor given a shape.
  ///
  ///@param shape Shape of tensor
  static boost::numeric::ublas::basic_extents<size_t> getExtents(
      const std::vector<DimInt>& shape);

  static boost::numeric::ublas::tensor<int>::extents_type toExtents(
      const std::vector<DimInt>& vector)
  {
    boost::numeric::ublas::tensor<int>::extents_type extents(
        std::vector<size_t>(vector.begin(), vector.end()));
    return extents;
  }
};

} // namespace helayers

#endif /* SRC_HELAYERS_TENSORUTILS_H */
