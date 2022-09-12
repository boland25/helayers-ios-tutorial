/*
 * MIT License
 *
 * Copyright (c) 2020 International Business Machines
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SRC_HELAYERS_ROTATEDEPENDENCYMAPPER_H
#define SRC_HELAYERS_ROTATEDEPENDENCYMAPPER_H

#include <vector>
#include <map>

namespace helayers {

class RotateDependencyMapper
{
  /// Number of slots in the ciphertexts to calculate rotate dependencies for
  int slotCount;

  /// For each possible rotate r, rotateDependencyMap[r] gives the first
  /// rotation offset in the set of rotations that together compose a rotation
  /// by r slots
  std::map<int, int> rotateDependencyMap;

  /// For each possible rotate r, rotateDepths[r] gives the minimal number of
  /// rotations required to compose a rotation by r slots
  std::map<int, int> rotateDepths;

public:
  /// A constructor
  RotateDependencyMapper() {}

  ///@brief Initializes the rotate dependency mapper.
  ///
  ///@param slotCount        The number of slots in the ciphertexts to calculate
  ///                        rotate dependencies for.
  ///@param supportedRotates The set of rotations supported by one rotate
  ///                        operation.
  void init(int slotCount, const std::vector<int>& supportedRotates);

  /// Computes a minimal list of supported rotations that sum up to the
  /// targetRotate, if such a list exists. Returns true if it is possible
  /// to construct such a list using the supported rotations, and false
  /// otherwise.
  /// @param targetRotate The required rotate, to be compoesd from several
  ///                     supported rotations.
  /// @param rotations      The resulting set of rotations will be stored here.
  ///                     If no such a list exist, this vector will be empty.
  bool composeRotate(int targetRotate, std::vector<int>& rotations) const;
};
} // namespace helayers

#endif