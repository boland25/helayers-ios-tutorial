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

#ifndef SRC_HELAYERS_PALISADE_PALISADECONFIG_H_
#define SRC_HELAYERS_PALISADE_PALISADECONFIG_H_

#include <iostream>
#include "palisade.h"
#include "utils/inttypes.h"

namespace helayers {

/// A class for holding PALISDAE configuration parameters.
/// For more details on the configuration parameters, refer to PALISDE Uesr
/// Manual, Section 9.2, "Real-number arithmetic".
struct PalisadeCkksConfig
{
  /// The maximal supported multiplication depth.
  usint multDepth;

  /// The number of bits in the scaling factor.
  usint scaleFactorBits;

  /// The minimal required batch size.
  usint batchSize;

  /// The security level.
  lbcrypto::SecurityLevel securityLevel;

  /// The ring dimension (which is twice the number of slots). If 0, PALISDAE
  /// will choose the ring dimension based on security level.
  usint ringDimension = 0;

  /// The rescaling technique. This determines whether rescaling is managed
  /// automatically by PALISDAE and whether the rescaling is done precisely or
  /// approximately.
  /// If EXACTRESCALE - rescaling is done precisely and automatically.
  /// If APPROXRAUTO - rescaling is done approximately and automatically.
  /// If APPROXRESCALE - rescaling is done approximately and manually.
  /// EXACTRESCALE is the simplest to use, but it comes at the account of
  /// 5-30% performance cost, according to PALISADE User Manual.
  lbcrypto::RescalingTechnique rsTech = lbcrypto::EXACTRESCALE;

  /// The key switching technique. PALISDAE supports three key switching
  /// techniques: BV, GHS and HYBRID. HYBRID is the key switching technique
  /// recommended for use by PALISADE in most cases.
  lbcrypto::KeySwitchTechnique ksTech = lbcrypto::HYBRID;

  /// The number of large digits in HYBRID decomposition.
  uint32_t dnum = 0;

  /// Specifies how many multiplications are possible without rescaling. This
  /// parameter is only useful when using the APPROXRESCALE rescaling
  /// technique.
  int maxDepth = 1;

  /// The size of the first modulus.
  usint firstModSize = 60;

  /// Controls the digit composition granularity during the key switching. This
  /// parameter does not have an effect unless the used key switching technique
  /// is BV.
  usint relinWindow = 10;

  /// RLWE (gaussian distribution), OPTIMIZED (ternary distribution) or SPARSE
  /// (sparse distribution)
  MODE mode = OPTIMIZED;

  /// The set of rotate indexes that are required to be supported by the
  /// underlying Palisade CKKS configuration.
  vector<int32_t> rotateIndexes;

  ///@brief Loads this PalisadeCkksConfig from a binary stream.
  ///@param in The stream to load from.
  void load(std::istream& in);

  ///@brief Saves this PalisadeCkksConfig to a binary stream.
  ///@param out The stream to save to.
  void save(std::ostream& out) const;

  ///@brief For each i < nslots s.t. i is a power of two, this function adds
  /// i and -i to the set of rotations supported by this configuration.
  ///
  ///@param nslots The number of slots. Used as an upper bound for the largest
  ///              supported rotation.
  void supportPowerOf2Rotations(int nslots);
};
} // namespace helayers

#endif /* SRC_HELAYERS_PALISADE_PALISADECONFIG_H_ */
