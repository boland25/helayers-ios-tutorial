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

#ifndef SRC_HELAYERS_TT_INTERLEAVED_TILE_MASK_H
#define SRC_HELAYERS_TT_INTERLEAVED_TILE_MASK_H

#include "TTIterator.h"
#include "helayers/hebase/utils/Printable.h"
#include <vector>

namespace helayers {

class TTInterleavedTileMask : public Printable
{
  struct SlotInfo
  {
    int regionRow, regionCol;
    bool isUsed;
    bool isUnknown;
  };

  std::vector<SlotInfo> slotInfoVec;

  DimInt rowDim;
  DimInt colDim;

  ///@brief Recompute the underlying info based on current iterator position.
  /// Can be used to recalc after moving to another tile
  ///@param it Tile tensor iterator to scan a tile
  void recalc(TTIterator& it);

public:
  ///@brief Init a TTInterleavedTileMask object.
  ///
  ///@param it Tile tensor iterator to scan a tile
  ///@param rowDim Dimension to take region row from
  ///@param colDim Dimension to take region col from
  void init(TTIterator& it, DimInt rowDim, DimInt colDim);

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  ///@brief Generate a mask with given rotation and expected offsets.
  /// Returns true if the mask contains zeros at non-trivial places and is
  /// therefore needed to be applied.
  ///
  /// Example: original tile has these region indices:
  /// (0,0) (0,1) (0,2) (?,?)
  /// (1,0) (1,1) (1,2) (?,?)
  ///
  /// After rotate 1:
  /// (0,1) (0,2) (?,?) (1,0)
  /// (1,1) (1,2) (?,?) (0,0)
  ///
  /// Expected offsets: (+0,+1)
  /// mask:
  /// 1 1 0 0
  /// 1 1 0 0
  ///
  ///@param mask Output mask
  ///@param rotOffset The offset by which the tile is rotated (Left rotation: 1
  /// means cell 1 is moved to cell 0)
  ///@param expectedRowOffset The expected offset in region row (post
  /// rotation-pre rotation)
  ///@param expectedColOffset The expected offset in region col (post
  /// rotation-col rotation)
  bool genMask(std::vector<int>& mask,
               DimInt rotOffset,
               DimInt expectedRowOffset,
               DimInt expectedColOffset) const;
};

} // namespace helayers

#endif /* SRC_HELAYERS_TT_INTERLEAVED_TILE_MASK_H */
