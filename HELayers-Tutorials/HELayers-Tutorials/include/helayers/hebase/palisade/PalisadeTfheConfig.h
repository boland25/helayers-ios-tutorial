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

#ifndef SRC_HELAYERS_PALISADE_PALISADETFHECONFIG_H_
#define SRC_HELAYERS_PALISADE_PALISADETFHECONFIG_H_

#include <iostream>
#include "palisade.h"
#include "binfhecontext.h"
#include "ringcore.h"

namespace helayers {

///@brief
/// A class for holding Palisade configuration parameters.
/// It doesn't yet cover all of Palisade flags.
///
struct PalisadeTfheConfig
{
  ///@brief
  /// Palisade TFHE bootstrapping method
  lbcrypto::BINFHEMETHOD method = lbcrypto::AP;

  ///@brief
  /// TFHE security level
  lbcrypto::BINFHEPARAMSET securityLevel = lbcrypto::STD128;

  ///@brief
  /// Number of bits encrypted by TFHE which are contained in a CTile
  int numBits = 8;
};
} // namespace helayers

#endif /* SRC_HELAYERS_PALISADE_PALISADETFHECONFIG_H_ */
