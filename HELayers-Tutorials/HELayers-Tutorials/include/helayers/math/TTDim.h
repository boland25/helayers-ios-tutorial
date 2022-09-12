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

#ifndef STRAINER_TTDIM_H_
#define STRAINER_TTDIM_H_

#include <math.h>
#include <ostream>
#include <iostream>
#include "helayers/hebase/utils/Printable.h"
#include "helayers/hebase/utils/SaveableBasic.h"
#include "helayers/hebase/Types.h"

namespace helayers {

///@brief Specification of a single dimension in a tile tensor shape.
/// See TTShape.
class TTDim : public SaveableBasic
{

  DimInt originalSize;

  // When a dimension is interleaved, we may drop the last slot in the tile
  // (e.g. when applying a convolution). In this case the new originalSize may
  // induce a new externalSize which would lead to bad decoding. To avoid this
  // we keep the externalSize computed at the encoding.
  DimInt interleavedExternalSize;

  DimInt tileSize;

  DimInt numDuplicated;

  bool areUnusedSlotsUnknown;

  bool interleaved;

  void validateValues() const;

  DimInt getMinimalExternalSize() const;

public:
  ///@brief Construct a new TTDim object.
  ///
  ///@param originalSize Original size, or -1 if not specified.
  ///@param tileSize Tile size.
  ///@param numDuplicated Number of duplications.
  ///@param areUnusedSlotsUnknown Indication whether unused slots are unknown.
  ///@param isInterleaved Indication whether the interleaved mode is used.
  TTDim(DimInt originalSize,
        DimInt tileSize,
        DimInt numDuplicated = 1,
        bool areUnusedSlotsUnknown = false,
        bool isInterleaved = false);

  virtual ~TTDim() {}

  ///@brief Returns true if no original size is specified.
  bool isIncomplete() const;

  ///@brief Returns whether other TTDim is compatible for tile tensor
  /// operations.
  ///
  ///@param other Other TTDim to check compatibility with.
  bool isCompatible(const TTDim& other) const;

  ///@brief Change this TTDim to be the result of applying a tile tensor
  /// operator on this object and other object.
  ///
  ///@param other Other TTDim on which an operator is applied.
  ///@param overrideUnknowns Whether in the operator being applied unknowns
  /// values are overridden by zeroes, as in the multiply operator.
  void applyCompatibilityAdjustments(const TTDim& other,
                                     bool overrideUnknowns = false);

  ///@brief Reduces original size to a new value.
  /// This method performs validity checks, and allows overriding the
  /// interleaved external size.
  /// Returns a reference to this object for further manipulation.
  ///@param newOriginalSize New value to set.
  ///@param newInterleavedExternalSize New interleaved external size (optional).
  TTDim& reduceOriginalSize(DimInt newOriginalSize,
                            DimInt newInterleavedExternalSize = -1);

  ///@brief Set the original size.
  /// In interleaved mode, the interleaved external size is set to the new
  /// minimum.
  /// Returns a reference to this object for further manipulation.
  ///@param size New value to set.
  ///@param keepInterleavedPlacing if in interleaved mode and this param set to
  /// true, the external size will be modified s.t. the first 'size' elements in
  /// this dimension will be kept.
  TTDim& setOriginalSize(DimInt size, bool keepInterleavedPlacing = false);

  ///@brief Set the tile size.
  ///
  /// Returns a reference to this object for further manipulation.
  ///@param size New value to set.
  TTDim& setTileSize(DimInt size);

  ///@brief Set the number of duplications.
  ///
  /// Returns a reference to this object for further manipulation.
  ///@param num New value to set.
  TTDim& setNumDuplicated(DimInt num);

  ///@brief Sets the value of the unknown flag for this dimension.
  ///@param areUnknown Value to set
  ///@throw invalid_argument if areUknown is true and this dimension has no
  /// unused slots.
  TTDim& setAreUnusedSlotsUnknown(bool areUnknown);

  ///@brief Indicates that unused slots if they exist are unknown.
  /// This turns on the unknown flag for this dimension if it has unused
  /// slots.
  TTDim& setUnusedSlotsUnknown();

  ///@brief Turns interleave flag on or off
  ///
  ///@param isInterleaved Whether this dimension is interleaved
  ///@param externalSizeDivisor If isInterleaved, you can force the external
  /// size to be divisible by this value
  TTDim& setInterleaved(bool isInterleaved, DimInt externalSizeDivisor = 1);

  ///@brief Set the interleaved external size of this dimension.
  /// Can be called only for dimensions marked with setInterleaved(true).
  /// Given size must be at least getMinExternalSize()
  ///@param newSize new interleaved size
  TTDim& setInterleavedExternalSize(DimInt newSize);

  ///@brief Erases original size information, sets number of duplicates to 1,
  /// and unknown values to false.
  void setIncomplete();

  /// Sets dim to be duplicated.
  /// Assumes this dim is either incomplete or originalSize==1
  /// and currently not duplicated.
  /// @param[in] dup dupplication count. The default value of -1 means duplicate
  /// to the entire tile size
  /// @throw invalid_argument If the specified dimension has originalSize>1, or
  /// is already duplicated
  void duplicate(DimInt dup = -1);

  ///@brief Returns original size.
  inline DimInt getOriginalSize() const { return originalSize; }

  ///@brief Returns tile size.
  inline DimInt getTileSize() const { return tileSize; }

  ///@brief Returns number of duplications.
  inline DimInt getNumDuplicated() const { return numDuplicated; }

  ///@brief Returns whether unused slots are unknown.
  /// If false, it means they are set to zero.
  inline bool getAreUnusedSlotsUnknown() const { return areUnusedSlotsUnknown; }

  ///@brief Returns true if data is fully duplicated along this dimension.
  /// Equivalent to getNumDuplicated()==getTileSize()
  bool isFullyDuplicated() const;

  ///@brief Returns the external size.
  DimInt getExternalSize() const;

  ///@brief Returns the number of slots that are used along this dimension.
  DimInt getNumUsedSlots() const;

  ///@brief Returns the number of slots that are unused along this dimension.
  DimInt getNumUnusedSlots() const;

  ///@brief Returns true if the tile size is 1.
  bool isDegenerateTile() const;

  ///@brief Returns whether the interleaved mode is used.
  bool isInterleaved() const;

  ///@brief Returns a bound on the number of used slots along this dimension
  // in a single tile
  DimInt getMaxUsedSlotsInTile() const;

  ///@brief Returns whether tile sizes, original sizes, interleaved modes and
  /// optionally interleaved external sizes are equal.
  ///
  ///@param other Other object to compare.
  ///@param enforceInterleavedExternalSize Whether to enforce the same
  /// interleaved external size in case of interleaved mode.
  bool isEqualOnOriginalSlots(const TTDim& other,
                              bool enforceInterleavedExternalSize = true) const;

  ///@brief Returns whether this object can be made fully duplicated. To be made
  /// fully duplicated, it must either already be fully duplicated, or have
  /// original size equal 1 and no duplicated values at all.
  ///
  ///@param clearUnknownsAllowed Whether clear unknown unused slots is allowed
  /// in order to become fully duplicated.
  bool canBecomeFullyDuplicated(bool clearUnknownsAllowed) const;

  ///@brief Reinterpret this dimension by increasing or decreasing its original
  /// size. Cannot change the external size. Does not work on interleaved
  /// dimension. Eliminates any duplicated slots if exist, turning them to
  /// original or to unused slot. Newly-created unused slots will be marked as
  /// unknown.
  ///
  ///@param newSize The new desired original size.
  void reinterpretOriginalSize(DimInt newSize);

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;
};

bool operator==(const TTDim& a, const TTDim& b);

bool operator!=(const TTDim& a, const TTDim& b);

} // namespace helayers

#endif /* STRAINER_TTDIM_H_ */
