#include "open_viii/graphics/background/Tile1.hpp"
#include "open_viii/graphics/background/Tile2.hpp"
#include "open_viii/graphics/background/Tile3.hpp"
#include <boost/ut.hpp>
#include <ciso646>
#include <tuple>
#include <variant>
int
  main()
{
  using v_tile = std::variant<open_viii::graphics::background::Tile1,
                              open_viii::graphics::background::Tile2,
                              open_viii::graphics::background::Tile3>;
  using t_tile = std::tuple<open_viii::graphics::background::Tile1,
                            open_viii::graphics::background::Tile2,
                            open_viii::graphics::background::Tile3>;
  static constexpr auto tile_test = [](v_tile tile_var) {
    std::visit(
      [](auto tile) {
        using namespace boost::ut::literals;
        using namespace boost::ut::operators::terse;
        using namespace boost::ut::operators;
        using namespace boost::ut;
        using namespace open_viii;
        using namespace open_viii::graphics::literals;
        "Test tile values"_test = [&tile] {
          expect(eq(tile.source_x(), 0U));
          expect(eq(tile.source_y(), 0U));
          expect(eq(tile.x(), 0));
          expect(eq(tile.y(), 0));
          expect(eq(tile.z(), 0U));
          expect(eq(tile.blend(), 0U));
          expect(eq(static_cast<std::uint16_t>(tile.blend_mode()),
                    static_cast<std::uint16_t>(
                      open_viii::graphics::background::BlendModeT::none)));
          expect(eq(tile.draw(), false));
          expect(eq(static_cast<std::uint16_t>(tile.animation_id()),
                    static_cast<std::uint16_t>(0xFFU)));
          expect(eq(tile.animation_state(), 0U));
          expect(eq(tile.depth(), 4_bpp));
          expect(eq(tile.texture_id(), 0U));
          expect(eq(tile.layer_id(), 0U));
          expect(eq(tile.palette_id(), 0U));
        };
        "Test Rectangles of tile"_test = [&tile] {
          const auto src_rec = tile.source_rectangle();
          expect(eq(src_rec.x(), 0U));
          expect(eq(src_rec.y(), 0U));
          expect(eq(src_rec.width(), 16U));
          expect(eq(src_rec.height(), 16U));
          const auto out_rec = tile.output_rectangle();
          expect(eq(out_rec.x(), 0));
          expect(eq(out_rec.y(), 0));
          expect(eq(out_rec.width(), 16));
          expect(eq(out_rec.height(), 16));
        };
        "Test modification of tile"_test = [&tile] {
          tile = tile.with_source_x(1U)
                   .with_source_y(2U)
                   .with_x(3)
                   .with_y(4)
                   .with_z(5U)
                   .with_texture_id(6U)
                   .with_blend(1U)
                   .with_depth(8_bpp)
                   .with_draw(true)
                   .with_palette_id(1U);
          {
            using T = decltype(tile);
            using namespace open_viii::graphics::background;
            if constexpr (has_with_layer_id<T>)
              tile = tile.with_layer_id(1U);
            if constexpr (has_with_blend_mode<T>)
              tile = tile.with_blend_mode(
                open_viii::graphics::background::BlendModeT::quarter_add);
            if constexpr (has_with_animation_id<T>)
              tile = tile.with_animation_id(1U);
            if constexpr (has_with_animation_state<T>)
              tile = tile.with_animation_state(1U);
          }
          expect(eq(tile.source_x(), 1U));
          expect(eq(tile.source_y(), 2U));
          expect(eq(tile.x(), 3));
          expect(eq(tile.y(), 4));
          expect(eq(tile.z(), 5U));
          expect(eq(tile.texture_id(), 6U));
          expect(eq(static_cast<std::uint16_t>(tile.blend()), 1U));
          expect(eq(tile.depth(), 8_bpp));
          expect(eq(tile.draw(), true));
          tile = tile.with_xy(5, 6);
          expect(eq(tile.x(), 5));
          expect(eq(tile.y(), 6));
          tile = tile.with_source_xy(7U, 8U);
          expect(eq(tile.source_x(), 7U));
          expect(eq(tile.source_y(), 8U));
          tile = tile.with_xy(decltype(tile.xy()){ 10, 11 });
          expect(eq(tile.x(), 10));
          expect(eq(tile.y(), 11));
          tile = tile.with_source_xy(decltype(tile.source_xy()){ 15U, 16U });
          expect(eq(tile.source_x(), 15U));
          expect(eq(tile.source_y(), 16U));
          // below may not change if it's const.
          using namespace boost::ut::operators;
          expect(
            bool{ eq(static_cast<std::uint16_t>(tile.layer_id()), 1U) }
            or bool{ eq(static_cast<std::uint16_t>(tile.layer_id()), 0U) });
          expect(
            bool{
              eq(static_cast<std::uint16_t>(tile.blend_mode()),
                 static_cast<std::uint16_t>(
                   open_viii::graphics::background::BlendModeT::quarter_add)) }
            or bool{
              eq(static_cast<std::uint16_t>(tile.blend_mode()),
                 static_cast<std::uint16_t>(
                   open_viii::graphics::background::BlendModeT::none)) });
          expect(bool{ eq(static_cast<std::uint16_t>(tile.animation_id()), 1U) }
                 or bool{
                   eq(static_cast<std::uint16_t>(tile.animation_id()), 255U) });
          expect(
            bool{ eq(static_cast<std::uint16_t>(tile.animation_state()), 1U) }
            or bool{
              eq(static_cast<std::uint16_t>(tile.animation_state()), 0U) });
        };
      },
      tile_var);
    {
      using namespace boost::ut;
      auto tile1 = open_viii::graphics::background::Tile1{};
      memcpy(&tile1,
             "\x08\x00\x68\x00\x04\x00\x34\x00\x40\x3E\xD0\xC0\x00\x01\xFF\x00",
             sizeof(tile1));
      using namespace open_viii::graphics::literals;
      auto tile1_gamma
        = tile1.with_source_xy({ 208, 192 })
            .with_xy({ 8, 104 })
            .with_z(4)
            .with_depth(4_bpp)
            .with_palette_id(9)
            .with_texture_id(4)
            .with_layer_id(0)
            .with_blend_mode(open_viii::graphics::background::BlendModeT::add)
            .with_blend(1)
            .with_animation_id(255)
            .with_animation_state(0)
            .with_draw(1);
      expect(eq(tile1, tile1_gamma));
    }
  };
  std::apply(
    [](auto... values) {
      ((void)tile_test(values), ...);
    },
    t_tile{});
}