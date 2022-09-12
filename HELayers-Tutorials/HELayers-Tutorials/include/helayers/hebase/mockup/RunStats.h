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

#ifndef SRC_HELYAERS_RUNSTATS_H
#define SRC_HELYAERS_RUNSTATS_H

#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <set>

namespace helayers {

/// @brief This class is used to measure run statistics, such as latency and
/// number of times a specific operation has been executed.
class RunStats
{
public:
  /// A type for the supported operations.
  enum OperationType
  {
    ENCODE,
    ENCRYPT,
    DECODE_DOUBLE,
    DECRYPT,
    C_COPY,
    ADD_RAW,
    SUB_RAW,
    ADD_PLAIN_RAW,
    SUB_PLAIN_RAW,
    NEGATE,
    RELINEARIZE,
    ROTATE,
    MULTIPLY_RAW,
    MULTIPLY_PLAIN_RAW,
    SQUARE_RAW,
    RESCALE_RAW,
    BOOTSTRAP,
    NUM_OPERATIONS
  };

  /// A type for the supported objects.
  enum ObjectType
  {
    CONTEXT,
    CONTEXT_BS,
    CONTEXT_BS_DEFAULT_ROTATIONS,
    ROTATION_KEY,
    ROTATION_KEY_BS,
    CTILE,
    PTILE,
    NUM_OBJECTS
  };

  /// The maximal supported chain index.
  static const int MAX_CHAIN_INDEX = 200;

private:
  /// The supported operations, as strings.
  static std::string operationTypeStrs[NUM_OPERATIONS];

  /// The supported objects, as strings.
  static std::string objectTypeStrs[NUM_OBJECTS];

  /// Element i,j of this matrix specifies the number of time that each op "i"
  /// has been executed with chain index "j".
  int operationCount[NUM_OPERATIONS][MAX_CHAIN_INDEX + 1];

  /// A mutex for operationCount.
  mutable std::mutex operationCountMutex;

  ///@brief Tracking of the sizes of rotations performed.
  ///
  std::set<int> rotations;

  ///@brief Tracking the minimal chain index seen.
  ///
  int minChainIndex = -1;

public:
  /// @brief A constructor.
  RunStats();

  ///@brief Resets all the statistics tracking.
  ///
  void resetAll();

  /// @brief Resets all operation counters to 0.
  void resetRunStats();

  /// @brief Increases operation counter for the given operation and chain
  /// index.
  /// @param op         The operation type.
  /// @param chainIndex The chain index.
  void increaseOpCounter(OperationType op, int chainIndex);

  ///@brief Resets the rotation tracking.
  ///
  void resetRotationsStats();

  ///@brief Adds new rotation size to the set of rotation sizes performed.
  ///
  ///@param rotation The rotation size to add.
  void addRotation(int rotation);

  ///@brief Returns the set of rotation sizes performed.
  ///
  std::vector<int> getRotations() const;

  ///@brief Updates the minimal chain index tracking with a new seen chain
  /// index.
  ///
  ///@param chainIndex The chain index seen.
  void addChainIndex(int chainIndex);

  ///@brief Returns the minimal chain index seen.
  ///
  int getMinChainIndex() const;

  /// @brief Returns the number of times the given operation has been exectued
  /// with the given chain index.
  /// @param op         The operation type.
  /// @param chainIndex The chain index.
  int getOperationCount(OperationType op, int chainIndex) const;

  /// @brief Returns the number of times the given operation has been executed.
  ///
  /// @param op The operation type.
  int getOperationCount(OperationType op) const;

  /// @brief Returns the total cpu time. Returns -1 if the tracked computation
  /// is too deep for the provided latencies.
  /// @param latencies        A map which maps each operation and chain index to
  ///                         the expected latency when this operation is
  ///                         executed with the corresponding chain index.
  /// @param slotCount        The number of slots in the HeContext.
  /// @param chainIndexOffset When computing latency, each operation that was
  ///                         originally performed with chain index i, will be
  ///                         considered as performed  with chain index
  ///                         i-chainIndexOffset.
  /// @throw runtime_error If there is an operation whose latency is not
  ///                      included in "latencies" map.
  int64_t getTotalCpuTime(const std::map<std::string, int64_t>& latencies,
                          int slotCount,
                          int chainIndexOffset = 0) const;

  ///@brief Returns the estimated memory size of a given object, in bytes.
  ///
  ///@param measures A map with estimated measures.
  ///@param object The given object.
  ///@param chainIndex The chain index of the object.
  ///@param numSlots The number of slots in a ciphertext.
  static int64_t getObjectSize(const std::map<std::string, int64_t>& measures,
                               ObjectType object,
                               int chainIndex,
                               int numSlots);

  ///@brief Returns the estimated CPU time of a given operation, in
  /// micro-seconds.
  ///
  ///@param measures A map with estimated measures.
  ///@param op The given operation.
  ///@param chainIndex The chain index in which the operation is performed.
  ///@param numSlots The number of slots in a ciphertext.
  static int64_t getOperationCpuTime(
      const std::map<std::string, int64_t>& measures,
      OperationType op,
      int chainIndex,
      int numSlots);
};
} // namespace helayers

#endif