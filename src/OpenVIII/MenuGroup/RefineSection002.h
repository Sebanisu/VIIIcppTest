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

#ifndef VIIIARCHIVE_REFINESECTION002_H
#define VIIIARCHIVE_REFINESECTION002_H
#include "RefineSpellID.h"
#include "RefineEntry.h"
#include <iostream>
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] RefineSection002// Refine Magic to Magic
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_3
   * Ability	        # of Entries	mngrp.bin Location	        Offset	Description
   * Mid-Mag-RF	        4 entries	(mngrp.bin loc: 0x2200000)	0x0	Upgrade Magic from low level to mid
   * level High-Mag-RF	6 entries	(mngrp.bin loc: 0x2200020)	0x20	Upgrade Magic from mid level to high
   * level
   */

private:
  static constexpr auto midMagRFCount_ = 4U;
  static constexpr auto highMagRFCount_ = 6U;
  std::array<RefineEntry<RefineSpellID, RefineSpellID>, midMagRFCount_> midMagRF_{};
  std::array<RefineEntry<RefineSpellID, RefineSpellID>, highMagRFCount_> highMagRF_{};


public:
  [[nodiscard]] const auto &MidMagRF() const noexcept { return midMagRF_; }
  [[nodiscard]] const auto &HighMagRF() const noexcept { return highMagRF_; }
  [[nodiscard]] static constexpr auto size() { return midMagRFCount_ + highMagRFCount_; }
  friend std::ostream & operator <<(std::ostream & os,const RefineSection002 & input)
  {
    const auto outArray = [&os](const auto & arr){
           for(const auto item : arr)
           {
             os << item << '\n';
           }
    };
    os << "midMagRF_:\n";
    outArray(input.midMagRF_);
    os << "highMagRF_:\n";
    outArray(input.highMagRF_);
    return os;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_REFINESECTION002_H