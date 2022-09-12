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

#ifndef SRC_HELAYERS_FILEUTILS_H
#define SRC_HELAYERS_FILEUTILS_H

#include <string>
#include <fstream>

namespace helayers {

/// @brief A class implementing methods to work with files.
class FileUtils
{

  FileUtils();

public:
  /// @brief Creates a directory with the given name, if it does not exist.
  /// If there is a directory with the given name already, does nothing.
  /// @param dirName The name of the directory to create.
  static void createDir(const std::string& dirName);

  /// @brief removes a directory with the given name.
  /// If there is no directory with the given name, does nothing.
  /// @param dirName The name of the directory to remove.
  static void removeDir(const std::string& dirName);

  /// @brief Creates a clean directory with the given name.
  /// If there is a directory with the given name already, all of its content is
  /// cleaned.
  /// @param dirName The name of the directory to create.
  /// @throw runtime_error If the length of "dirName" is less than 3.
  static void createCleanDir(const std::string& dirName);

  /// @brief Prints the size of the given directory.
  /// @param path        The relative path of the directory to report its
  /// size.
  /// @param logicalName The title used in the size report message. If
  /// empty,
  ///                        "path" is used instead.
  static long reportFileSize(const std::string& path,
                             const std::string& logicalName = "");

  /// @brief Returns true iff the file of the given path exists
  /// @param filePath        The relative path of the checked file
  static bool fileExists(const std::string& filePath);

  ///@brief Open a file input stream.
  /// Turns on flags to throw exception in case of subsequent read failure.
  ///@param filePath File to open
  ///@param mode The requested mode for the file
  ///@param exceptions The requested exception mask for the file
  ///@throw runtime_error if file can't be opened.
  static std::ifstream openIfstream(
      const std::string& filePath,
      std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary,
      std::ios_base::iostate exceptions = std::ifstream::failbit |
                                          std::ifstream::badbit);

  ///@brief Open a file output stream.
  /// Turns on flags to throw exception in case of subsequent write failure.
  ///@param filePath File to open
  ///@param mode The requested mode for the file
  ///@param exceptions The requested exception mask for the file
  ///@throw runtime_error if file can't be opened.
  static std::ofstream openOfstream(
      const std::string& filePath,
      std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary,
      std::ios_base::iostate exceptions = std::ifstream::failbit |
                                          std::ifstream::badbit);

  ///@brief Returns the extension of the file specified by the given path.
  /// The extension returned includes the "." character in the beginning.
  /// If the file has no extension, returns an empty string.
  ///
  ///@param filePath Path of file.
  static std::string extractExtension(const std::string& filePath);
};
} // namespace helayers

#endif /* SRC_HELAYERS_FILEUTILS_H */
