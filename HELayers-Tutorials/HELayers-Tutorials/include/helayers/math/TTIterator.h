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

#ifndef SRC_HELAYERS_TT_ITERATOR_H
#define SRC_HELAYERS_TT_ITERATOR_H

#include "TensorIterator.h"

namespace helayers {

class TTShape;

/// A class for iterating over a tile tensor
class TTIterator : public TensorIteratorTarget
{
  TensorIterator internal;
  TensorIterator external;

  std::vector<int> maxValidSizes;
  std::vector<int> logicalAtTileStart;
  std::vector<int> logicalCurrent;
  std::vector<bool> logicalValid;
  int numInvalidDims;
  int numUnknownDims;

  TensorIterator original;
  const TTShape& shape;

protected:
  ///@brief Update the logical position including the valid flags, unknown
  /// counts, and the original tensor iterator
  void updateLogicalCurrent(int dim, int val);

  /// See parent class
  void set(int dim, int val) override;

  ///@brief Resets various data members following a move to a different tile
  void resetDataForNewTile();

public:
  ///@brief Constructs an iterator for a tile tensor.
  ///  @param[in] shape shape of tile tensor
  TTIterator(const TTShape& shape);

  ///@brief Copy constructor.
  ///@param src Object to copy from.
  TTIterator(const TTIterator& src);

  ///@brief Move to next element in tile.
  /// When end of tile is reached, it returns false and returns to start of tile
  /// (without moving to next tile)
  bool nextInTile();

  ///@brief Move to the next used element in tile.
  /// Following this call it is guaranteed that isUsed() will return true.
  /// When end of tile is reached, it returns false and returns to start of tile
  /// (without moving to next tile)
  bool nextUsedInTile();

  ///@brief  Move to next tile.
  /// Positions on first slot of next tile. If end of tile tensor is reached,
  /// It moves to first slot of first tile and returns false.
  bool nextTile();

  ///@brief Moves to a tile specified by a flat index of the external tenor.
  /// Note that this is a relatively inefficient method (O(number of dims)).
  ///
  ///@param flatIndex Flat index of tile in the external tensor
  void setTile(int flatIndex);

  ///@brief Set the external index of a tile.
  /// Positions on first slot of this tile.
  ///
  ///@param dim Dimension to set
  ///@param val Value to set
  void setExternalIndex(DimInt dim, DimInt val);

  ///@brief Print state for debug purposes.
  ///  @param out output stream
  void debugPrint(std::ostream& out) const;

  ///@brief Returns flat index (=slot number) of current position in tile
  inline int getInternalPos() const { return internal.getPos(); }

  ///@brief Returns flat index of current position in external tensor
  inline int getExternalPos() const { return external.getPos(); }

  ///@brief Returns true if current slot is used, i.e., contains an element from
  /// the original tensor or is a duplication of one
  inline bool isUsed() const { return numInvalidDims == 0; }

  ///@brief  Returns true if current slot is unused (isUsed()==false), and its
  /// value is unkown (i.e., not necessarily zero)
  inline bool isUnknown() const { return numUnknownDims > 0; }

  ///@brief Returns flat index of current position in original tensor.
  /// If isUsed()==false the return value is undefined.
  inline int getOriginalPos() const { return original.getPos(); }

  ///@brief Returns the shape of the tile tensor being iterated
  inline const TTShape& getShape() const { return shape; }

  ///@brief Returns the internal iterator scanning the internal tensor inside
  /// the tile
  inline const TensorIterator& getInternalIterator() const { return internal; }

  ///@brief Returns the external iterator scanning the external tensor (between
  /// tiles).
  inline const TensorIterator& getExternalIterator() const { return external; }

  ///@brief Returns the coordinates of current position in original tensor.
  ///
  inline const std::vector<int>& getLogicalIndices() const
  {
    return logicalCurrent;
  }

  ///@brief Assert a particular state, for testing purposes.
  ///
  ///@param ipos Expected internal position
  ///@param epos Expected external position
  ///@param opos Expected original position (tested only if used)
  ///@param used Expected used value
  ///@param unknown Expected unknown value
  void assertEquals(int ipos,
                    int epos,
                    int opos,
                    bool used = true,
                    bool unknown = false);
};

std::ostream& operator<<(std::ostream& out, const TTIterator& ei);
} // namespace helayers

#endif /* SRC_HELAYERS_TT_ITERATOR_H */
