//
// Created by pcvii on 1/18/2021.
//

#ifndef VIIIARCHIVE_JSM_OLD_ENTITY_HPP
#define VIIIARCHIVE_JSM_OLD_ENTITY_HPP
#include <cstdint>
#include <type_traits>
#include <utility>

namespace open_viii::field::scripts {
/**
 * @see
 * https://github.com/myst6re/deling/blob/develop/src/files/JsmFile.cpp#L117
 */
struct jsm_old_entity
{
private:
  std::uint16_t m_raw{};

public:
  constexpr jsm_entity() = default;
  [[nodiscard]] constexpr std::uint16_t raw() const noexcept
  {
    return m_raw;
  }

  [[nodiscard]] constexpr std::uint8_t count() const noexcept
  {
    constexpr std::uint16_t mask = 0xFFU;
    return m_raw & mask;
  }
  [[nodiscard]] constexpr std::uint16_t label() const noexcept
  {
    constexpr std::uint16_t mask = 0xFF;
    constexpr std::uint16_t shift = 8U;
    return static_cast<std::uint16_t>(m_raw >> shift) & mask;
  }

  [[nodiscard]] constexpr std::size_t calc_total() const
  {
    return lable()+count()+1U;
  }

  /**
   * Get Value
   *@note required to structured binding support

   */
  template<std::size_t I>
  requires(I < 2U) [[nodiscard]] constexpr auto get() const noexcept
  {
    if constexpr (I == 0U) {
      return count();
    } else if constexpr (I == 1U) {
      return label();
    }
  }
};
}// namespace open_viii::field::scripts

namespace std {
/**
 * number of arguments
 * @note required to structured binding support
 */
template<>
struct tuple_size<open_viii::field::scripts::jsm_entity>
  : std::integral_constant<size_t, 2U>
{
};

/**
 * type of argument 0
 * @note required to structured binding support
 */
template<> struct tuple_element<0U, open_viii::field::scripts::jsm_entity>
{
  using type = std::uint16_t;
};
/**
 * type of argument 1
 * @note required to structured binding support
 */
template<> struct tuple_element<1U, open_viii::field::scripts::jsm_entity>
{
  using type = std::uint16_t;
};
}// namespace std
#endif// VIIIARCHIVE_JSM_OLD_ENTITY_HPP