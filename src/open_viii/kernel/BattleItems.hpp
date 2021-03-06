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
#ifndef VIIIARCHIVE_BATTLEITEMS_HPP
#define VIIIARCHIVE_BATTLEITEMS_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
struct BattleItems
{
  /**
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to item name
   * 0x0002	2 bytes	Offset to item description
   * 0x0004	2 bytes	Magic ID
   * 0x0006	1 byte	Attack type
   * 0x0007	1 byte	Attack power
   * 0x0008	1 byte	Unknown
   * 0x0009	1 byte	Target info
   * 0x000A	1 byte	Unknown
   * 0x000B	1 byte	Attack flags
   * 0x000C	1 bytes	Unknown
   * 0x000D	1 byte	Status Attack Enabler
   * 0x000E	2 bytes	status_0; //statuses 0-7
   * 0x0010	4 bytes	status_1; //statuses 8-39
   * 0x0014	1 byte	Attack Param
   * 0x0015	1 byte	Unknown
   * 0x0016	1 bytes	Hit Count
   * 0x0017	1 bytes	Element
   * @see https://github.com/DarkShinryu/doomtrain/wiki/Battle-items
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint16_t       m_magic_id{};
  AttackTypeT         m_attack_type{};
  std::uint8_t        m_attack_power{};
  std::uint8_t        m_unknown0{};
  TargetT             m_target{};
  std::uint8_t        m_unknown1{};
  AttackFlagsT        m_attack_flags{};
  std::uint8_t        m_unknown2{};
  std::uint8_t        m_status_attack_enabler{};
  PersistentStatusesT m_persistent_statuses{}; // statuses 0-7
  BattleOnlyStatusesT m_battle_only_statuses{};// statuses 8-39
  std::uint8_t        m_attack_param{};
  std::uint8_t        m_unknown3{};
  std::uint8_t        m_hit_count{};
  ElementT            m_element{};

public:
  static constexpr std::size_t EXPECTED_SIZE = 24U;
  constexpr auto
    operator<=>(const BattleItems &right) const noexcept = default;
  [[maybe_unused]] [[nodiscard]] constexpr auto
    name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    description_offset() const noexcept
  {
    return m_description_offset;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    attack_power() const noexcept
  {
    return m_attack_power;
  }
  [[nodiscard]] constexpr auto
    unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[nodiscard]] constexpr auto
    target() const noexcept
  {
    return m_target;
  }
  [[nodiscard]] constexpr auto
    unknown1() const noexcept
  {
    return m_unknown1;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    attack_flags() const noexcept
  {
    return m_attack_flags;
  }
  [[nodiscard]] constexpr auto
    unknown2() const noexcept
  {
    return m_unknown2;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    status_attack_enabler() const noexcept
  {
    return m_status_attack_enabler;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    persistent_statuses() const noexcept
  {
    return m_persistent_statuses;
  }// statuses 0-7
  [[nodiscard]] constexpr auto
    battle_only_statuses() const noexcept
  {
    return m_battle_only_statuses;
  }// statuses 8-39
  [[maybe_unused]] [[nodiscard]] constexpr auto
    attack_param() const noexcept
  {
    return m_attack_param;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    unknown3() const noexcept
  {
    return m_unknown3;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    hit_count() const noexcept
  {
    return m_hit_count;
  }
  [[nodiscard]] constexpr auto
    element() const noexcept
  {
    return m_element;
  }
  std::ostream &
    out(std::ostream &                                os,
        [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os
        << ", " << static_cast<std::uint32_t>(m_magic_id) << ", "
        << static_cast<std::uint32_t>(m_attack_type) << ", "
        << static_cast<std::uint32_t>(m_attack_power) << ", "
        << static_cast<std::uint32_t>(m_unknown0) << ", "
        << static_cast<std::uint32_t>(m_target) << ", "
        << static_cast<std::uint32_t>(m_unknown1) << ", "
        << static_cast<std::uint32_t>(m_attack_flags) << ", "
        << static_cast<std::uint32_t>(m_unknown2) << ", "
        << static_cast<std::uint32_t>(m_status_attack_enabler) << ", "
        << static_cast<std::uint32_t>(m_persistent_statuses)// statuses 0-7
        << ", "
        << static_cast<std::uint32_t>(m_battle_only_statuses)// statuses 8-39
        << ", " << static_cast<std::uint32_t>(m_attack_param) << ", "
        << static_cast<std::uint32_t>(m_unknown3) << ", "
        << static_cast<std::uint32_t>(m_hit_count) << ", "
        << static_cast<std::uint32_t>(m_element);
  }
};
static_assert(sizeof(BattleItems) == BattleItems::EXPECTED_SIZE);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_BATTLEITEMS_HPP
