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

#ifndef SRC_HELAYERS_TYPES_H
#define SRC_HELAYERS_TYPES_H

#include <stdint.h>

namespace helayers {

///@brief Type for representing sizes related to tile and tensor shapes and
/// dimensions: the number of dimensions, the size of a dimension, the index of
/// dimension in a shape, the index of slot along a dimension, the number of
/// slots/cells in a tensor or tile, etc.
///
typedef int32_t DimInt;

///@brief enum for representing device types.
enum DeviceType
{
  DEVICE_CPU,
  DEVICE_GPU
};

} // namespace helayers

#endif /* SRC_HELAYERS_TYPES_H */