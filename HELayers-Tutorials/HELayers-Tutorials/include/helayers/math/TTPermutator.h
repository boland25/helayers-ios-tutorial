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

#ifndef SRC_HELAYERS_TTPERMUTATOR_H
#define SRC_HELAYERS_TTPERMUTATOR_H

#include "CTileTensor.h"
#include <set>

namespace helayers {

///@brief Structure to describe the details of a required rotation.
///
struct RotationDetails
{
  DimInt tileIndex;
  DimInt rotationSize;

  RotationDetails(DimInt tileIndex, DimInt rotationSize);
  bool operator<(const RotationDetails& other) const;
};

///@brief Structure to describe the details of a flat index in a tile tensor.
///
struct FlatIndexDetails
{
  DimInt tileIndex;
  DimInt indexInTile;

  FlatIndexDetails() {}
  FlatIndexDetails(DimInt tileIndex, DimInt indexInTile);
};

///@brief A class for performing permutation-like operations on tile tensors.
///
class TTPermutator
{
  const HeContext& he;
  const CTileTensor& src;
  TTShape dstShape;

  ///@brief A CTile with zero values all over it, to be used as a base.
  ///
  CTile zeroes;

  ///@brief A mapping from each required rotation to its targets, where the
  /// targets are a mapping from each destination tile index to the indexes
  /// of slots that are included in it.
  ///
  std::map<RotationDetails, std::map<DimInt, std::set<DimInt>>> mapping;

  ///@brief Returns a copy of the given shape in which all slots are marked as
  /// original slots.
  ///
  ///@param shape The given shape.
  static TTShape getFullyOriginalShape(const TTShape& shape);

  ///@brief Initializes the data structure of mapping each required rotation to
  /// its targets, given a permutation.
  ///
  ///@param permutation The given permutation to apply.
  void initMapping(
      const std::map<std::vector<DimInt>, std::vector<DimInt>>& permutation);

  ///@brief Returns a mapping of all the coordinates of a tensor with a given
  /// shape to details of their flat representation.
  ///
  ///@param shape The given shape.
  static std::map<std::vector<DimInt>, FlatIndexDetails> mapCordsToFlat(
      const TTShape& shape);

  ///@brief Applies masking on a given CTile, where only slots in the given
  /// indexes will remain.
  ///
  ///@param res The given CTile to mask.
  ///@param indexes The given indexes, only slots in these indexes will remain.
  void applyMask(CTile& res, std::set<DimInt> indexes) const;

public:
  ///@brief Construct a new TTPermutator object
  ///
  ///@param src The source tile tensor to perform permutations based on it.
  ///@param permutation The permutation to apply. A mapping between coordinates
  /// of the source and destination tile tensors.
  ///@param dstShape The shape of the desired destination tile tensor. Only tile
  /// sizes and external sizes will be considered.
  TTPermutator(
      const CTileTensor& src,
      const std::map<std::vector<DimInt>, std::vector<DimInt>>& permutation,
      const TTShape& dstShape);

  ///@brief Destroy the TT permutator object
  ///
  ~TTPermutator();

  ///@brief Deleted copy constructor
  ///
  ///@param src Source object
  TTPermutator(const TTPermutator& src) = delete;

  ///@brief Deleted operator=
  ///
  ///@param src Source object
  TTPermutator& operator=(const TTPermutator& src) = delete;

  ///@brief Returns the result of the permutation.
  ///
  ///@param res The tile tensor to store the result in. Any existing content or
  /// shape of it will be ignored and overwritten.
  void getPermutation(CTileTensor& res) const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_TTPERMUTATOR_H */
