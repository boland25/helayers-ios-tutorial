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

#ifndef CTILE_ROTATION_CACHE_H_
#define CTILE_ROTATION_CACHE_H_

#include <map>
#include "helayers/hebase/CTile.h"

namespace helayers {

/// A rotation cache for a single CTILE
class CTileRotationCache
{
private:
  // cache[0] holds the subject of the RotationCache
  std::map<int, CTile> cache;

public:
  /// @brief Create a rotation cache for c
  /// @param c The CTile the cache holds rotations for
  CTileRotationCache(const CTile& c);

  /// @brief Put in c a CTile with
  /// @param out The CTile that will be overwritten with the rotated version of
  /// the rotation subject
  /// @param rot Number of rotattions to apply on the rotation subject
  void rotate(CTile& out, int rot);
};

} // namespace helayers

#endif
