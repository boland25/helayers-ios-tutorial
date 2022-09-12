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

#ifndef SRC_HELAYERS_PSEUDORANDOMGENERATOR_H
#define SRC_HELAYERS_PSEUDORANDOMGENERATOR_H

#include <vector>

#include "seal/randomgen.h"

namespace helayers {

/// A class used to generate random bytes and numbers in cryptographic secure
/// way
class PseudoRandomGenerator
{
  // Implementation is currently dependent on SEAL's UniformRandomGenerator
  std::shared_ptr<seal::UniformRandomGenerator> rng;

public:
  ///@brief Initializes PseudoRandomGenerator object seeded with random seed.
  /// The seed is created in secure way to guarantee pseudo randomness.
  PseudoRandomGenerator();

  ///@brief Initializes PseudoRandomGenerator object seeded with given seed.
  /// Notice that using the same seed twice will generate the same pseudo random
  /// bytes, and thus might be insecure for some applications.
  ///@param seed vector of at most 8 uint64_t numbers
  ///@throw invalid_argument if seed contains more than 8 elements
  PseudoRandomGenerator(const std::vector<std::uint64_t> seed);

  ///@brief Generates random bytes
  ///@param res empty vector
  ///@param byteCount number of bytes to generate
  void generate(unsigned char* res, std::size_t byteCount);

  ///@brief Generates a random uint64_t number
  std::uint64_t generate();
};
} // namespace helayers

#endif // SRC_HELAYERS_PSEUDORANDOMGENERATOR_H
