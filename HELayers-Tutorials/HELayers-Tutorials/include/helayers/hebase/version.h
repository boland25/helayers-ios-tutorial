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

#ifndef SRC_HELAYERS_VERSION_H
#define SRC_HELAYERS_VERSION_H

#include "helayers/hebase/utils/Printable.h"
#include <cstdint>

namespace helayers {
/// @brief Current version of helayers.
struct HelayersVersion : public Printable
{
  /// Major version number
  uint8_t major;
  /// Minor version number
  uint8_t minor;
  /// Version's patch number
  uint8_t patch;
  /// Version's tweak number
  uint8_t tweak;

  /// @brief returns a 32-bit value indicating the current helayers version.
  /// This values is composed as (major|minor|patch|tweak).
  uint32_t getVersion() const
  {
    uint32_t version = (major << 24) + (minor << 16) + (patch << 8) + tweak;
    return version;
  }

  std::string getVersionStr() const;

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

  HelayersVersion(int major, int minor, int patch, int tweak);

  HelayersVersion(uint32_t version);

  bool operator<(const HelayersVersion& other) const;
  bool operator>(const HelayersVersion& other) const;
  bool operator==(const HelayersVersion& other) const;
  bool operator<=(const HelayersVersion& other) const;
  bool operator>=(const HelayersVersion& other) const;
};

const HelayersVersion helayersVersion(1, 5, 0, 0);
const uint32_t HELAYERS_MAGIC_NUMBER = 0x484c5952;
} // namespace helayers

#endif
