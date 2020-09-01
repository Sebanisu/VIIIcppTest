//
// Created by pcvii on 7/20/2020.
//

#ifndef VIIIARCHIVE_MENUGROUPHEADERSECTION_H
#define VIIIARCHIVE_MENUGROUPHEADERSECTION_H
#include <cstring>
#include <cstdint>
#include <string_view>
namespace open_viii::menu_group {
struct MenuGroupHeaderSection
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrphd_bin
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_bin
private:
  static constexpr std::uint32_t invalidSize_ = 0U;
  static constexpr std::uint32_t invalidFileOffset_ = 0xFFFFFFFF;
  static constexpr std::uint8_t permOffset_{ 1 };
  std::uint32_t fileOffset_{};
  std::uint32_t size_{};

public:
  [[nodiscard]] auto FileOffset() const noexcept { return fileOffset_ - permOffset_; }
  [[nodiscard]] auto Size() const noexcept { return size_; }
  template<typename charContainer = std::string_view>
  [[nodiscard]] std::string_view GetSectionBuffer(const charContainer &fileBuffer) const
  {
    if (Size() == invalidSize_ || fileOffset_ == invalidFileOffset_
        || static_cast<std::size_t>(FileOffset()) + static_cast<std::size_t>(Size()) > std::size(fileBuffer)) {
      return {};// returns empty buffer if invalid or error.
    }
    std::cout << '<' << FileOffset() << ',' << Size() << ">\n";
    return { std::data(fileBuffer) + FileOffset(), Size() };
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_MENUGROUPHEADERSECTION_H
