//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_TRIANGLE_HPP
#define VIIIARCHIVE_TRIANGLE_HPP
#include "GpuFlags.hpp"
#include "open_viii/graphics/Bit4Values.hpp"
#include "open_viii/graphics/Color.hpp"
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Rectangle.hpp"
#include <bit>
#include <cstdint>
#include <type_traits>
namespace open_viii::battle::stage {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Triangle
 */
struct Triangle
{
private:
  using point = graphics::Point<std::uint8_t>;
  std::uint16_t                                  m_face_indice_a{};
  std::uint16_t                                  m_face_indice_b{};
  std::uint16_t                                  m_face_indice_c{};
  point                                          m_uv1{};
  point                                          m_uv2{};
  std::uint16_t                                  m_raw_clut{};
  point                                          m_uv3{};
  graphics::Bit4Values                           m_raw_texture_page{};
  std::uint8_t                                   m_raw_hide{};
  graphics::Color24<graphics::ColorLayoutT::RGB> m_color{};
  GpuFlags                                       m_raw_gpu{};
  static constexpr std::uint8_t                  MASK_4_BIT  = 0xFU;
  static constexpr int                           SHIFT_2_BIT = 2;

public:
  static constexpr std::size_t COUNT = 3U;
  [[nodiscard]] static constexpr std::uint8_t
    clut(const std::uint16_t &in_clut_raw) noexcept
  {
    return (std::rotl(in_clut_raw, SHIFT_2_BIT) & MASK_4_BIT);
  }
  [[nodiscard]] constexpr std::uint8_t
    clut() const noexcept
  {
    return clut(m_raw_clut);
  }
  [[nodiscard]] constexpr std::uint8_t
    texture_page() const noexcept
  {
    return m_raw_texture_page.second();
  }
  [[nodiscard]] constexpr auto
    color() const noexcept
  {
    return m_color;
  }
  [[nodiscard]] constexpr bool
    hide() const noexcept
  {
    return m_raw_hide != 0U;
  }
  template<std::size_t I>
  requires(I < COUNT) [[nodiscard]] constexpr auto uv() const noexcept
  {
    if constexpr (I == 0U) {
      return m_uv1;
    }
    else if constexpr (I == 1U) {
      return m_uv2;
    }
    else if constexpr (I == 2U) {
      return m_uv3;
    }
  }
  template<std::size_t I>
  requires(I < COUNT) [[nodiscard]] constexpr auto face_indice() const noexcept
  {
    if constexpr (I == 0U) {
      return m_face_indice_a;
    }
    else if constexpr (I == 1U) {
      return m_face_indice_b;
    }
    else if constexpr (I == 2U) {
      return m_face_indice_c;
    }
  }
  static constexpr std::size_t EXPECTED_SIZE = 20U;
};
static_assert(sizeof(Triangle) == Triangle::EXPECTED_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_TRIANGLE_HPP