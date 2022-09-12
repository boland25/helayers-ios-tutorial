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

#ifndef STRAINER_TTSHAPE_H_
#define STRAINER_TTSHAPE_H_

#include <vector>
#include "TTDim.h"
#include "helayers/hebase/utils/Printable.h"
#include "helayers/hebase/utils/SaveableBasic.h"

namespace helayers {

class TensorIterator;

///@brief Tile tensor shape.
/// Contains meta data defining packing details of a tile tensor.
/// Printable to a string in tile tensor shape notation
class TTShape : public SaveableBasic
{

  std::vector<TTDim> dims;

  inline void validateDimExists(DimInt dim) const
  {
    if (dim >= dims.size())
      throw std::invalid_argument("Dimension " + std::to_string(dim) +
                                  " does not exist for this shape");
  }

  ///@brief Prints error to stderr with relevant shape details
  ///
  ///@param error Error message to print
  ///@param atDim Optional: specify a specific dimension
  void printError(const std::string& error, DimInt atDim = -1) const;

public:
  ///@brief Constructs an empty object
  TTShape() {}

  ///@brief Constructs a TTShape with given list of TTDim specification.
  ///
  ///@param v Dimension specifications.
  TTShape(const std::vector<TTDim>& v) : dims(v) {}

  /// Initialize a ttshape
  /// Example usage: TTShape myShape({16,32,8});

  ///@brief Construct a new TTShape object with just tile dimension sizes
  /// defined. In this state it is incomplete (since it has no original sizes)
  /// but it can be used for packing tensors.
  ///
  ///@param tshape Tile shape
  TTShape(std::initializer_list<DimInt> tshape);

  ///@brief Construct a new TTShape object with just tile dimension sizes
  /// defined. In this state it is incomplete (since it has no original sizes)
  /// but it can be used for packing tensors.
  ///
  ///@param tshape Tile shape
  TTShape(const std::vector<DimInt>& tshape);

  ~TTShape() {}

  ///@brief Returns a copy of current ttshape with each of the specified
  /// dimensions
  /// fully duplicated along tile size.
  ///
  ///@param dimsToDuplicate List of dimension indices to duplicate
  TTShape getWithDuplicatedDims(
      std::initializer_list<DimInt> dimsToDuplicate) const;

  ///@brief Returns a copy of current ttshape with each of the specified
  /// dimensions
  /// fully duplicated along tile size.
  ///
  ///@param dimsToDuplicate List of dimension indices to duplicate
  TTShape getWithDuplicatedDims(
      const std::vector<DimInt>& dimsToDuplicate) const;

  /// Get a copy of current ttshape with the specified dimension
  /// fully duplicated along tile size.
  /// @param[in] dim dimension to duplicate
  /// to the entire tile size

  ///@brief Returns a copy of current ttshape with the specified
  /// dimension
  /// fully duplicated along tile size.
  ///
  ///@param dim Dimension index to duplicate
  TTShape getWithDuplicatedDim(DimInt dim) const;

  ///@brief Returns a ttshape containing just the tile shape of this object.
  /// The returned ttshape is in incomplete state, but can be used for packing
  /// tensors.
  TTShape getIncompleteTileSizes() const;

  ///@brief Returns a dimension specification.
  ///
  ///@param dim Dimension index
  inline const TTDim& getDim(DimInt dim) const
  {
    validateDimExists(dim);
    return dims.at(dim);
  }

  ///@brief Returns a dimension specification.
  ///
  ///@param dim Dimension index
  inline TTDim& getDim(DimInt dim)
  {
    validateDimExists(dim);
    return dims.at(dim);
  }

  //

  ///@brief Returns a sub-ttshape of dimensions [first,last)
  ///
  ///@param first First dimension index (inclusive)
  ///@param last  Last dimension index (exclusive)
  inline TTShape getSubShape(DimInt first, DimInt last) const
  {
    return TTShape(
        std::vector<TTDim>(dims.begin() + first, dims.begin() + last));
  }

  ///@brief Set the original sizes of all dimensions.
  /// Usually used for incomplete shapes.
  /// If some dimensions are complete, their original size is overriden.
  ///@param orgSizes Original sizes vector
  void setOriginalSizes(std::initializer_list<DimInt> orgSizes);

  ///@brief Set the original sizes of all dimensions.
  /// Usually used for incomplete shapes.
  /// If some dimensions are complete, their original size is overriden.
  ///@param orgSizes Original sizes vector
  void setOriginalSizes(const std::vector<DimInt>& orgSizes);

  ///@brief Validates the original sizes of the shape are as expected.
  ///@param orgSizes Expected original sizes. If a zero value is passed for a
  /// certain dimension, it means this dimension should be skipped when
  /// verifying its size.
  /// @throw invalid_argument If assertion fails.
  void assertOriginalSizes(const std::vector<DimInt>& orgSizes) const;

  ///@brief Returns whether object is in incomplete state.
  /// Incomplete ttshape are missing some details but have the tile shape, and
  /// can be used for packing tensors.
  bool isIncomplete() const;

  ///@brief Returns true if all dimensions are incomplete.
  /// See isIncomplete()
  bool areAllDimsIncomplete() const;

  ///@brief Returns true if this ttshape is compatiable with other.
  /// Compatibility means elementwise binary operators can be applied.
  ///@param other Other ttshape to test with
  bool isCompatible(const TTShape& other) const;

  ///@brief Asserts that other ttshape is compatible with this.
  /// See isCompatible().
  ///@param other Other shape to test with
  ///@param text Text to include in error
  ///@throw invalid_argument if not compatible
  void assertCompatible(const TTShape& other,
                        const std::string& text = "") const;

  ///@brief Update current shape to the result of applying a binary elementwise
  /// operator on this and other shape.
  /// The two shapes are assumed to be compatible.
  ///@param other Other input shape.
  ///@param overrideUnknowns Whether in the operator being applied unknowns
  /// values are overridden by zeroes, as in the multiply operator.
  void applyCompatibilityAdjustments(const TTShape& other,
                                     bool overrideUnknowns = false);

  ///@brief Add a dimension to this ttshape.
  ///
  ///@param dim Dimension to add.
  ///@param place Place of the new dimension, default is last.
  void addDim(const TTDim& dim, int place = -1);

  ///@brief Removes a dim from this ttshape. Only for dim with original size 1
  /// and tile size 1. Number of dims before calling the function must be at
  /// least 3.
  ///@param dim Dim to remove
  /// @throw runtime_error If dim doesn't exists
  /// @throw runtime_error If dim is not of size 1
  /// @throw runtime_error If number of dims is less than 3
  void removeDim(DimInt dim);

  ///@brief Reorders the dimensions of this ttshape.
  ///@param dimOrder New order of dimensions
  void reorderDims(const std::vector<DimInt>& dimOrder);

  ///@brief Returns dimension number
  inline DimInt getNumDims() const { return dims.size(); }

  ///@brief Returns the original shape.
  ///
  ///@param includeDuplications If true, the returned list contains number of
  /// duplicates for each duplicated dimension, otherwise 1 is returned for
  /// these dimensions.
  std::vector<DimInt> getOriginalSizes(bool includeDuplications = false) const;

  ///@brief Returns the tile shape
  std::vector<DimInt> getTileSizes() const;

  ///@brief Returns true if all tile sizes are one except the last.
  /// Optionally, it conditions on number of dimensions.
  ///
  ///@param numDims Required number of dimensions or -1 if can be any.
  bool isBatchPacking(DimInt numDims = -1) const;

  ///@brief Returns the external shape
  std::vector<DimInt> getExternalSizes() const;

  ///@brief Returns the number of tiles required by this ttshape
  DimInt getNumUsedTiles() const;

  ///@brief Returns the number of slots in each tile.
  DimInt getNumSlotsInTile() const;

  ///@brief Returns the number of slots in the original tensor represented by
  /// this shape
  DimInt getNumOriginalSlots() const;

  ///@brief Returns the rotation offset of a flattened tile to achieve rotation
  /// over the given dimension.
  ///
  ///@param i Dimension to rotate
  DimInt getRotateOffsetOfDim(DimInt i) const;

  ///@brief Returns true if any dimension has unknown values in unused slots.
  bool containsUnknownUnusedSlots() const;

  ///@brief Remove all unknown values in unused slots.
  void clearUnknowns();

  ///@brief Turns on the unknown flag for all dimensions which have unused
  /// slots.
  void setAllUnusedSlotsUnknown();

  ///@brief Inserts a dimension at given position.
  /// If it is inserted at pos i, current dimension at i becomes i+1.
  ///@param dim Dimension to add
  ///@param pos Position to insert to.
  void insertDimAt(const TTDim& dim, DimInt pos);

  ///@brief Returns true if given dimension is the first dimension whose tile
  /// size is larger than one.
  /// This dimension allows true rotation, and summation ends with duplication.
  ///
  ///@param dim Input dimension.
  bool isFirstNonDegenerateTileDim(DimInt dim) const;

  ///@brief Returns true if this TTShape includes duplicated slots and false
  /// otherwise. This TTShape is considered to contain duplicated slots if at
  /// least one of its dimensions includes duplicated slots.
  bool includesDuplicatedSlots() const;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std ::cout) const override;

  ///@brief Returns tile shape in string representation: ( t1 x t2 x ... x tk )
  std::string tileLayoutToString() const;

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  ///@brief Report an error has occurred related to this shape.
  /// This call throws an exception giving relevant details and supplied error
  /// message.
  ///
  ///@param error Error to report.
  ///@param dimIndex Optional specification of dimension index
  ///@throw invalid_argument
  void reportError(const std::string& error, DimInt atDim = -1) const;

  ///@brief Report an error has occurred: this shape is different from an
  /// expected one.
  /// This call throws an exception giving relevant details and supplied error
  /// message.
  ///
  ///@param error Error to report.
  ///@param expected Expected shape.
  ///@throw invalid_argument
  void reportError(const std::string& error, const TTShape& expected) const;

  ///@brief Report an error has occurred: a dimension in this shape is different
  /// from an
  /// expected one.
  /// This call throws an exception giving relevant details and supplied error
  /// message.
  ///
  ///@param error Error to report.
  ///@param dimIndex dimension of erroneous dimension
  ///@param expected Expected dimension.
  ///@throw invalid_argument
  void reportError(const std::string& error,
                   DimInt dimIndex,
                   const TTDim& expected) const;
};

bool operator==(const TTShape& a, const TTShape& b);

bool operator!=(const TTShape& a, const TTShape& b);

} // namespace helayers

#endif /* STRAINER_TTSHAPE_H_ */
