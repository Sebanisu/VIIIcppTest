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
#ifndef VIIIARCHIVE_STATGROUPNOEVANOHIT_HPP
#define VIIIARCHIVE_STATGROUPNOEVANOHIT_HPP
#include <array>
#include <compare>
#include <iostream>
namespace open_viii::kernel {
template<typename T>
struct StatGroupNoEVANoHIT
{
public:
  static constexpr auto MAX_COUNT{ 7U };
  static constexpr auto HP{ 0U };
  static constexpr auto STR{ 1U };
  static constexpr auto VIT{ 2U };
  static constexpr auto MAG{ 3U };
  static constexpr auto SPR{ 4U };
  static constexpr auto SPD{ 5U };
  static constexpr auto LUCK{ 6U };
  constexpr auto
    operator<=>(const StatGroupNoEVANoHIT<T> &right) const noexcept = default;
  [[nodiscard]] constexpr T
    hp() const noexcept
  {
    return m_stats.at(HP);
  }
  [[nodiscard]] constexpr T
    str() const noexcept
  {
    return m_stats.at(STR);
  }
  [[nodiscard]] constexpr T
    vit() const noexcept
  {
    return m_stats.at(VIT);
  }
  [[nodiscard]] constexpr T
    mag() const noexcept
  {
    return m_stats.at(MAG);
  }
  [[nodiscard]] constexpr T
    spr() const noexcept
  {
    return m_stats.at(SPR);
  }
  [[nodiscard]] constexpr T
    spd() const noexcept
  {
    return m_stats.at(SPD);
  }
  [[nodiscard]] constexpr T
    luck() const noexcept
  {
    return m_stats.at(LUCK);
  }
  friend std::ostream &
    operator<<(std::ostream &os, const StatGroupNoEVANoHIT<T> &input)
  {
    bool first{ true };
    for (const auto &item : input.m_stats) {
      if (!first) {
        os << ", ";
      }
      else {
        first = false;
      }
      if constexpr (std::is_integral_v<T>) {
        os << static_cast<size_t>(item);
      }
      else {
        os << '{';
        bool subFirst{ true };
        for (const auto &subItem : item) {
          if (!subFirst) {
            os << ", ";
          }
          else {
            subFirst = false;
          }
          if (std::is_integral_v<decltype(subItem)>) {
            os << static_cast<size_t>(subItem);
          }
          else {
            os << subItem;
          }
        }
        os << '}';
      }
    }
    return os;
  }

private:
  std::array<T, MAX_COUNT> m_stats{};
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_STATGROUPNOEVANOHIT_HPP
