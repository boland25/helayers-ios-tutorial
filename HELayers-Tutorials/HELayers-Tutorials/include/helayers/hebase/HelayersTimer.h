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

#ifndef SRC_HELAYERS_SIMPLETIMER_H
#define SRC_HELAYERS_SIMPLETIMER_H

#include <cstdint>
#include <chrono>
#include <map>
#include <string>
#include <sstream>
#include <mutex>
#include <iostream>

#ifndef NO_INTERNAL_HELAYERS_PROFILING

#define HELAYERS_TIMER(title) helayers::HelayersTimer _helayers_timer(title)
#define HELAYERS_TIMER_SECTION(title)                                          \
  helayers::HelayersTimer::Guard guard(title)
#define HELAYERS_TIMER_PUSH(title) helayers::HelayersTimer::push(title)
#define HELAYERS_TIMER_POP() helayers::HelayersTimer::pop()
#define HELAYERS_TIMER_POP_COUNT(count) helayers::HelayersTimer::pop(count)

#define HELAYERS_TIMER_PRINT_STATE(title)                                      \
  helayers::HelayersTimer::printState(title)
#define HELAYERS_TIMER_RESTART(title) _helayers_timer.restart(title)
#define HELAYERS_TIMER_STOP() _helayers_timer.stop()

#define HELAYERS_TIMER_PRINT_MEASURE_SUMMARY(sectionName)                      \
  helayers::HelayersTimer::printMeasureSummary(sectionName)

#define HELAYERS_TIMER_PRINT_MEASURES_SUMMARY()                                \
  helayers::HelayersTimer::printMeasuresSummary()

#define HELAYERS_TIMER_PRINT_MEASURES_SUMMARY_FLAT()                           \
  helayers::HelayersTimer::printMeasuresSummaryFlat()

#else

#define HELAYERS_TIMER(title)
#define HELAYERS_TIMER_SECTION(title)
#define HELAYERS_TIMER_PUSH(title)
#define HELAYERS_TIMER_POP()
#define HELAYERS_TIMER_POP_COUNT(count)
#define HELAYERS_TIMER_SECTION_THREAD_MODE(section)
#define HELAYERS_TIMER_PRINT_STATE(title)
#define HELAYERS_TIMER_RESTART(title)
#define HELAYERS_TIMER_STOP()
#define HELAYERS_TIMER_PRINT_MEASURE_SUMMARY(sectionName)
#define HELAYERS_TIMER_PRINT_MEASURES_SUMMARY()
#define HELAYERS_TIMER_PRINT_MEASURES_SUMMARY_FLAT()

#endif

namespace helayers {

/// A class currently for internal use only.
/// Used for performing internal profiling research.
class HelayersTimer
{
  struct SectionInfo
  {
    std::mutex mtx;
    std::string name;
    std::int64_t sum = 0;
    std::int64_t sumSquares = 0;
    std::int64_t count = 0;
    std::int64_t sumCPU = 0;

    std::chrono::high_resolution_clock::time_point start;
    int64_t cpu_start;

    std::map<std::string, SectionInfo> subSections;
    SectionInfo* parent = nullptr;

    SectionInfo() {}
    SectionInfo(const std::string& n) : name(n) {}
    SectionInfo(const SectionInfo& other)
        : name(other.name),
          sum(other.sum),
          sumSquares(other.sumSquares),
          count(other.count),
          sumCPU(other.sumCPU),
          subSections(other.subSections)
    {}

    SectionInfo& operator=(const SectionInfo& other)
    {
      if (&other != this) {
        sum = other.sum;
        sumSquares = other.sumSquares;
        count = other.count;
        sumCPU = other.sumCPU;
        subSections = other.subSections;
        parent = other.parent;
        name = other.name;
      }
      return *this;
    }

    void printTopMeasureSummary(int depth, std::ostream& out) const;
    void printMeasureSummary(const std::string& sectionName,
                             std::ostream& out) const;
    void printMeasuresSummary(int depth, std::ostream& out);
    void addToFlat(std::map<std::string, SectionInfo>& flat);

    SectionInfo& find(const std::string& title, const std::string& prefix);

    void addMeasure(std::chrono::high_resolution_clock::time_point s,
                    int64_t cpu_s);
    SectionInfo& getSubSection(const std::string& title);
    void add(const SectionInfo& other);
  };
  static SectionInfo top;
  static SectionInfo* current;
  static bool multiThreadMode;

public:
  /// @brief Defines a HelayersTimer::Guard local variable to automatically push
  /// a timer section and pop it when the scope ends.
  class Guard
  {
  public:
    ///@brief Construct a new Guard object with given section name
    ///
    ///@param title Section name
    Guard(const std::string& title) { HelayersTimer::push(title); }
    ~Guard() { HelayersTimer::pop(); }
  };

  ///@brief Construct a new HelayersTimer object
  HelayersTimer();

  ///@brief Construct a new HelayersTimer object with given measure name.
  ///
  ///@param title Measure name.
  HelayersTimer(const std::string& title);
  ~HelayersTimer();

  ///@brief Open a new section
  ///
  ///@param section Section name
  static void push(const std::string& section);

  ///@brief  Close section
  static void pop();

  ///@brief  Close count sections.
  ///
  ///@param count Number of sections to close.
  static void pop(int count);

  ///@brief Print for debug purposes (internal use).
  ///
  ///@param title Print title
  static void printState(const std::string& title = "");

  ///@brief Returns accumulated time of all measures with given name
  ///
  ///@param title Name of measures to sum.
  static std::int64_t getSum(const std::string& title);

  ///@brief Stop current measure, and start a new one.
  ///
  ///@param title Name of new measure.
  void restart(const std::string& title);

  ///@brief Stops current measure.
  ///
  void stop();

  /// Prints an overview of run time.
  /// Prints specific timer results commong to typical runs.
  /// @param[in] out stream to print to (default=cout)
  static void printOverview(std::ostream& out = std::cout);

  ///@brief Print summary of a single measure
  ///
  ///@param sectionName Measure name
  ///@param out Output stream
  static void printMeasureSummary(const std::string& sectionName,
                                  std::ostream& out = std::cout);

  ///@brief Print summary of a all measure
  ///
  ///@param out Output stream
  static void printMeasuresSummary(std::ostream& out = std::cout);

  ///@brief Print summary of a all measure in a flat table (no section
  /// hierarchy).
  ///
  ///@param out Output stream
  static void printMeasuresSummaryFlat(std::ostream& out = std::cout);

  ///@brief Converts microsecs measure to a human readable string.
  ///
  ///@param microsecs Microsecs measure
  static std::string getDurationAsString(std::int64_t microsecs);

  ///@brief Returns whether we currently are in multi-thread mode.
  ///
  static bool isMultiThreadMode() { return multiThreadMode; }

  ///@brief Return the current process CPU time (accumulated for all threads) in
  /// nanoseconds
  static int64_t getProcessCPUTime();

private:
  std::chrono::high_resolution_clock::time_point start;
  std::chrono::high_resolution_clock::time_point last;
  int64_t cpu_start;
  int64_t cpu_last;
  std::string title;
  SectionInfo* info = nullptr;

  bool lastSet;
};
} // namespace helayers

#endif /* SRC_HELAYERS_SIMPLETIMER_H */
