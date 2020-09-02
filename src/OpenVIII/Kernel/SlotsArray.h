// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIIIARCHIVE_SLOTSARRAY_H
#define VIIIARCHIVE_SLOTSARRAY_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include <cstdint>
namespace open_viii::kernel {
struct SlotsArray
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Slot-array
   * Offset	Length	Description
   * 0x0000	1 byte	Slot Set ID
   */
private:
  std::uint8_t slotSetID_{};

public:
  [[nodiscard]] auto SlotSetID() const noexcept { return slotSetID_; }
  std::ostream &out(std::ostream &os, [[maybe_unused]] const std::string_view &buffer) const
  {
    return os << static_cast<uint16_t>(SlotSetID());
  }
  [[nodiscard]] explicit operator std::uint8_t() const { return slotSetID_; }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SLOTSARRAY_H
