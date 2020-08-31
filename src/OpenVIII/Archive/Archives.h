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
#ifndef VIIIARCHIVE_ARCHIVES_H
#define VIIIARCHIVE_ARCHIVES_H
#include "FIFLFS.h"
#include "ZZZ.h"
#include <type_traits>
#include <string_view>
#include "OpenVIII/Strings/LangT.h"
#include <variant>
namespace OpenVIII::Archive {
// There are 6 main FIFLFS archives and 2 main zzz archives for ff8 and ff8 remaster.
enum class ArchiveTypeT : std::uint8_t {
  Battle,
  Field,
  Magic,
  Main,
  Menu,
  World,
  ZZZMain,
  ZZZOther,
  Count,
  First = Battle,
  Last = Count - 1,
};


template<std::signed_integral T> static constexpr bool TestValidArchiveTypeT(T archiveTypeT)
{
  return (static_cast<std::intmax_t>(archiveTypeT) >= static_cast<std::intmax_t>(ArchiveTypeT::First)
          || static_cast<std::intmax_t>(archiveTypeT) <= static_cast<std::intmax_t>(ArchiveTypeT::Last));
}
template<typename T>
requires(std::unsigned_integral<T> || std::is_same_v<T, ArchiveTypeT>) static constexpr bool TestValidArchiveTypeT(
  T archiveTypeT)
{
  return (static_cast<std::size_t>(archiveTypeT) >= static_cast<std::size_t>(ArchiveTypeT::First)
          || static_cast<std::size_t>(archiveTypeT) <= static_cast<std::size_t>(ArchiveTypeT::Last));
}
//
// template<ArchiveTypeT archiveTypeT>
// concept ValidArchiveTypeT = (static_cast<std::intmax_t>(archiveTypeT) >=
// static_cast<std::intmax_t>(ArchiveTypeT::First)
//                             || static_cast<std::intmax_t>(archiveTypeT)
//                                  <= static_cast<std::intmax_t>(ArchiveTypeT::Last));
template<LangT langVal> struct Archives
{
private:
  std::filesystem::path path_{};
  std::string lang_{ LangCommon::ToString<langVal>() };
  FIFLFS<false> battle_{};
  FIFLFS<true> field_{};
  FIFLFS<false> magic_{};
  FIFLFS<false> main_{};
  FIFLFS<false> menu_{};
  FIFLFS<false> world_{};
  std::optional<ZZZ> zzzMain_{};
  std::optional<ZZZ> zzzOther_{};
  /**
   * Runs lambda on archive First to Last
   * @tparam First default: ArchiveTypeT::First
   * @tparam Last default: ArchiveTypeT::Last
   * @tparam Lambda void (*)(ArchiveTypeT, std::string_view)
   * @param f of type Lambda.
   */
  template<std::intmax_t First = static_cast<std::intmax_t>(ArchiveTypeT::First),
    std::intmax_t Last = static_cast<std::intmax_t>(ArchiveTypeT::Last),
    typename lambdaT>
  requires(((TestValidArchiveTypeT(First) && TestValidArchiveTypeT(Last)) || First == Last + 1)
           && std::invocable<lambdaT,
             const ArchiveTypeT &,
             const std::string &>) void static_for([[maybe_unused]] lambdaT const &lambda)
  {// https://stackoverflow.com/questions/13816850/is-it-possible-to-develop-static-for-loop-in-c
    if constexpr (First <= Last) {
      constexpr auto archiveType_ = std::integral_constant<ArchiveTypeT, static_cast<ArchiveTypeT>(First)>{};
      lambda(archiveType_, GetString<archiveType_>());
      static_for<First + 1, Last>(lambda);
    }
  }
  /**
   * Set default language.
   */
  void SetLang() { lang_ = LangCommon::ToString<langVal>(); }
  /**
   * Search for lang.dat from steam 2013 release. TODO find cross platform way to get to remaster config.txt
   * @param path location of ff8
   */
  void SetLang(const std::filesystem::path &path)
  {
    using namespace std::string_literals;
    if (std::empty(lang_)) {
      lang_ = [this, &path]() {
        {
          // try to read lang.dat from ff8 steam folder
          // lang.dat overrides the explicitly set one.
          const std::filesystem::path &langDatPath = path / "lang.dat";
          if (std::filesystem::exists(langDatPath)) {
            std::ifstream fp = std::ifstream(langDatPath, std::ios::in);
            if (fp.is_open()) {
              fp.seekg(0, std::ios::end);
              const auto &length = fp.tellg();
              if (length <= 0) {
                std::string returnValue{};
                returnValue.resize(static_cast<unsigned>(length));// sets to length.
                fp.seekg(0);
                fp.read(returnValue.data(), length);
                fp.close();
                std::cout << "lang.dat detected: ";
                return returnValue;
              }
            }
            fp.close();
          }
        }
        // remaster stores the language value in my documents. I don't see a cross platform way to find this in cpp.
        // will probably need a c# launcher to pass the lang code to this. As .Net has a standard cross platform
        // way to get the documents folder.
        // Documents\My Games\FINAL FANTASY VIII Remastered\Steam\(\d+)\config.txt
        return ""s;// defaulting to english
      }();
    }
    if (std::empty(lang_)) {
      SetLang();
    }
    std::cout << "lang = " << lang_ << '\n';
  }
  /**
   * Set Path to FF8. Will look for Data and lang- folders.
   * @param path location of FF8.
   */
  void SetPath(const std::filesystem::path &path)
  {
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    // assert(!std::empty(lang_));
    path_ = path;
    const std::filesystem::path &dataPath = path_ / "Data"sv;
    if (std::filesystem::exists(dataPath)) {
      path_ = dataPath;
      {
        static constexpr auto langStart = "lang-"sv;
        std::filesystem::path langFolderPath = path_ / langStart;
        langFolderPath = langFolderPath.string() + lang_;
        if (!std::empty(lang_) && std::filesystem::exists(langFolderPath)) {
          path_ = langFolderPath;
        }
      }
    }
  }

  /**
   * TryToAdd Archive of type to archive member variable.
   * @param archiveType_ Type of valid archive.
   * @param path physical path on the hard drive
   * @param nestedPath path located inside the archive. default {}
   * @param offset bytes from front of file where archive is located. 0U = unlimited
   * @param size max size of archive? 0U = unlimited
   * @return
   */
  bool TryAdd(const ArchiveTypeT &archiveType_,
    const std::filesystem::path &path,
    const std::filesystem::path &nestedPath = {},
    const size_t &offset = {},
    const size_t &size = {})
  {// this string can be compared to the stem of the filename to determine which archive is try added to.
    // returns nullptr on failure.
    assert(TestValidArchiveTypeT(archiveType_));
    const auto tryAddToFIFLFS = [&path, &nestedPath, &offset, &size](auto &archive) {
      return archive.TryAdd(path, nestedPath, offset, size) != TryAddT::NotPartOfArchive;
    };
    const auto tryAddToZZZ = [&path](std::optional<ZZZ> &archive) {
      if (path.has_extension() && Tools::iEquals(path.extension().string(), ZZZ::Ext)) {
        archive.emplace(path);
        return true;
      }
      return false;
    };
    switch (archiveType_) {
    case ArchiveTypeT::Battle:
      return tryAddToFIFLFS(battle_);
    case ArchiveTypeT::Field:
      return tryAddToFIFLFS(field_);
    case ArchiveTypeT::Magic:
      return tryAddToFIFLFS(magic_);
    case ArchiveTypeT::Main:
      return tryAddToFIFLFS(main_);
    case ArchiveTypeT::Menu:
      return tryAddToFIFLFS(menu_);
    case ArchiveTypeT::World:
      return tryAddToFIFLFS(world_);
    case ArchiveTypeT::ZZZMain: {
      if (tryAddToZZZ(zzzMain_)) {
        using namespace std::string_literals;
        using namespace std::string_view_literals;
        for (const auto &dataItem : zzzMain_->Data()) {
          {
            const auto pathString = dataItem.GetPathString();
            if (FIFLFS<true>::CheckExtension(pathString) == 0U) {
              continue;
            }
            static constexpr auto data = "data"sv;
            static constexpr auto langStart = "lang-"sv;
            auto langStartingFilter = std::filesystem::path(data) / langStart;
            auto langStarting = std::filesystem::path(langStartingFilter.string() + lang_);

            if (Tools::iStartsWith(pathString, langStartingFilter.string())
                && !Tools::iStartsWith(pathString, langStarting.string())) {
              continue;
            }
            auto localPath = std::filesystem::path(pathString);
            static_for<static_cast<intmax_t>(ArchiveTypeT::First), static_cast<intmax_t>(ArchiveTypeT::ZZZMain) - 1>(
              [&localPath, &dataItem, &path, this](const ArchiveTypeT &test, const auto &stem) {
                if (!(OpenVIII::Tools::iEquals(stem, localPath.stem().string()))) {
                  return;
                }
                TryAdd(test, path, localPath, dataItem.Offset(), dataItem.UncompressedSize());
              });
          }
        }
        return true;
      }
      return false;
    }
    case ArchiveTypeT::ZZZOther:
      return tryAddToZZZ(zzzOther_);
    case ArchiveTypeT::Count:
      break;
    }
    return false;
  }

public:
  [[nodiscard]] constexpr std::variant<std::monostate, FIFLFS<true>, FIFLFS<false>, std::optional<ZZZ>>
    Get(const std::string_view &name, bool zzz = false) const
  {
    if (!zzz) {
      if (GetString<ArchiveTypeT::Battle>() == name) {
        return Get<ArchiveTypeT::Battle>();
      }

      if (GetString<ArchiveTypeT::Field>() == name) {
        return Get<ArchiveTypeT::Field>();
      }

      if (GetString<ArchiveTypeT::Magic>() == name) {
        return Get<ArchiveTypeT::Magic>();
      }

      if (GetString<ArchiveTypeT::Main>() == name) {
        return Get<ArchiveTypeT::Main>();
      }
      if (GetString<ArchiveTypeT::Menu>() == name) {
        return Get<ArchiveTypeT::Menu>();
      }
      if (GetString<ArchiveTypeT::World>() == name) {
        return Get<ArchiveTypeT::World>();
      }
      if (GetString<ArchiveTypeT::Field>().starts_with(name)) {
        std::cout << " -- nested -- " << name << '\n';
        return std::monostate{};
      }
    } else {
      if (GetString<ArchiveTypeT::ZZZMain>() == name) {
        return Get<ArchiveTypeT::ZZZMain>();
      }
      if (GetString<ArchiveTypeT::ZZZOther>() == name) {
        return Get<ArchiveTypeT::ZZZOther>();
      }
    }
    return std::monostate{};
  }
  [[maybe_unused]] [[nodiscard]] constexpr std::variant<std::monostate, FIFLFS<true>, FIFLFS<false>, std::optional<ZZZ>>
    Get(const ArchiveTypeT &type) const
  {
    if (ArchiveTypeT::Battle == type) {
      return Get<ArchiveTypeT::Battle>();
    }

    if (ArchiveTypeT::Field == type) {
      return Get<ArchiveTypeT::Field>();
    }

    if (ArchiveTypeT::Magic == type) {
      return Get<ArchiveTypeT::Magic>();
    }

    if (ArchiveTypeT::Main == type) {
      return Get<ArchiveTypeT::Main>();
    }
    if (ArchiveTypeT::Menu == type) {
      return Get<ArchiveTypeT::Menu>();
    }
    if (ArchiveTypeT::World == type) {
      return Get<ArchiveTypeT::World>();
    }
    if (ArchiveTypeT::ZZZMain == type) {
      return Get<ArchiveTypeT::ZZZMain>();
    }
    if (ArchiveTypeT::ZZZOther == type) {
      return Get<ArchiveTypeT::ZZZOther>();
    }

    return std::monostate{};
  }

  template<ArchiveTypeT archiveType_>
  requires(TestValidArchiveTypeT(archiveType_)) constexpr auto GetString() const noexcept
  {// this string can be compared to the stem of the filename to determine which archive is try added to.
    // returns nullptr on failure.
    using namespace std::literals;
    if constexpr (archiveType_ == ArchiveTypeT::Battle) {
      constexpr auto battle = "BATTLE"sv;
      return battle;
    } else if constexpr (archiveType_ == ArchiveTypeT::Field) {
      constexpr auto field = "FIELD"sv;
      return field;
    } else if constexpr (archiveType_ == ArchiveTypeT::Magic) {
      constexpr auto magic = "MAGIC"sv;
      return magic;
    } else if constexpr (archiveType_ == ArchiveTypeT::Main) {
      constexpr auto main = "MAIN"sv;
      return main;
    } else if constexpr (archiveType_ == ArchiveTypeT::Menu) {
      constexpr auto menu = "MENU"sv;
      return menu;
    } else if constexpr (archiveType_ == ArchiveTypeT::World) {
      constexpr auto world = "WORLD"sv;
      return world;
    } else if constexpr (archiveType_ == ArchiveTypeT::ZZZOther) {
      constexpr auto other = "OTHER"sv;
      return other;
    } else if constexpr (archiveType_ == ArchiveTypeT::ZZZMain) {
      constexpr auto main = "MAIN"sv;
      return main;
    } else {
      constexpr auto error = ""sv;
      return error;
    }
  }
  /**
   *
   * @tparam archiveType_
   * @return
   */
  template<ArchiveTypeT archiveType_> requires(TestValidArchiveTypeT(archiveType_)) const auto &Get() const noexcept
  {
    if constexpr (archiveType_ == ArchiveTypeT::Battle) {
      return battle_;
    } else if constexpr (archiveType_ == ArchiveTypeT::Field) {
      return field_;
    } else if constexpr (archiveType_ == ArchiveTypeT::Magic) {
      return magic_;
    } else if constexpr (archiveType_ == ArchiveTypeT::Main) {
      return main_;
    } else if constexpr (archiveType_ == ArchiveTypeT::Menu) {
      return menu_;
    } else if constexpr (archiveType_ == ArchiveTypeT::World) {
      return world_;
    } else if constexpr (archiveType_ == ArchiveTypeT::ZZZMain) {
      return zzzMain_;
    } else if constexpr (archiveType_ == ArchiveTypeT::ZZZOther) {
      return zzzOther_;
    } else {
      return nullptr;
    }
  }
  /**
   * Gets the nested FIFLFS files from Field archive.
   * @param nestedArchive string to filter results. {} will get all nested archives.
   * @return all results.
   */
  auto GetNested(const std::string_view &nestedArchive) const { return field_.GetFIFLFSEntries(nestedArchive); }
  Archives() = default;
  /**
   * Preloads all archives in the path.
   * @param path that contains FIFLFS files or ZZZ files.
   */
  explicit Archives(const std::filesystem::path &path)
  {
    SetLang(path);
    SetPath(path);

    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    for (const auto &fileEntry : std::filesystem::directory_iterator(path_, options))// todo may need sorted.
    {
      const auto &localPath = fileEntry.path();
      if (localPath.has_stem()) {
        static_for([&localPath, this](const ArchiveTypeT &test, const auto &stem) {
          if (!(OpenVIII::Tools::iEquals(stem, localPath.stem().string()))) {
            return;
          }
          TryAdd(test, localPath);
        });
      }
    }
  }

  /**
   * Search all the archives. For any of the listed strings.
   * @tparam nested enable or disable searching nested archives from field, default: true.
   * @tparam maxT Max ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::Last.
   * @tparam minT Min ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::First.
   * @param filename is an initializer-list of strings to search the archives for. use {} to get all files.
   * @return Returns a vector of pairs of archiveName and results with pairs of index number and embedded filepath.
   */
  template<bool nested = true,
    std::intmax_t maxT = static_cast<std::intmax_t>(ArchiveTypeT::Last),
    std::intmax_t minT = static_cast<std::intmax_t>(ArchiveTypeT::First)>
  requires((TestValidArchiveTypeT(maxT) || maxT >= minT - 1) && TestValidArchiveTypeT(minT))
    [[nodiscard]] std::vector<std::pair<std::string, std::vector<std::pair<unsigned int, std::string>>>> Search(
      const std::initializer_list<std::string_view> &filename) const
  {
    if constexpr (maxT >= minT) {
      std::vector<std::pair<std::string, std::vector<std::pair<unsigned int, std::string>>>> vector =
        Search<nested, maxT - 1, minT>(filename);
      constexpr auto archiveType_ = std::integral_constant<ArchiveTypeT, static_cast<ArchiveTypeT>(maxT)>{};
      auto archive = Get<archiveType_>();
      if constexpr (!std::is_null_pointer_v<decltype(archive)>) {
        if constexpr (std::is_same_v<decltype(archive), std::optional<ZZZ>>) {
          if (archive.has_value()) {
            [[maybe_unused]] const auto result = archive->GetAllEntriesData(filename);
            if (!std::ranges::empty(result)) {
              vector.emplace_back(std::make_pair(GetString<archiveType_>(), std::move(result)));
            }
          }
        } else if constexpr (std::is_same_v<decltype(archive),
                               FIFLFS<false>> || std::is_same_v<decltype(archive), FIFLFS<true>>) {
          [[maybe_unused]] auto result = archive.GetAllEntriesData(filename);
          if (!std::ranges::empty(result)) {
            vector.emplace_back(std::make_pair(GetString<archiveType_>(), std::move(result)));
          }
          if constexpr (nested) {
            auto nestedResult = archive.GetAllNestedEntriesData(filename);
            if (!std::ranges::empty(nestedResult)) {
              vector.reserve(std::ranges::size(nestedResult) + std::ranges::size(vector));
              for (auto &item : nestedResult) { vector.emplace_back(std::move(item)); }
            }
          }
        }
      }
      return vector;
    } else {
      return {};
    }
  }


  /**
   * Execute Lambda on all the archives. For any of the listed strings.
   * @tparam nested enable or disable searching nested archives from field, default: true.
   * @tparam maxT Max ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::Last.
   * @tparam minT Min ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::First.
   * @param filename is an initializer-list of strings to search the archives for. use {} to get all files.
   */
  template<bool nested = true,
    std::intmax_t maxT = static_cast<std::intmax_t>(ArchiveTypeT::Last),
    std::intmax_t minT = static_cast<std::intmax_t>(ArchiveTypeT::First),
    typename lambdaT>
  requires(
    (TestValidArchiveTypeT(maxT) || maxT >= minT - 1) && TestValidArchiveTypeT(minT)
    && std::invocable<lambdaT,
      std::vector<char>,
      std::string>) void ExecuteOn(const std::initializer_list<std::string_view> &filename, const lambdaT &lambda) const
  {
    if constexpr (maxT >= minT) {
      ExecuteOn<nested, maxT - 1, minT>(filename, lambda);
      constexpr auto archiveType_ = std::integral_constant<ArchiveTypeT, static_cast<ArchiveTypeT>(maxT)>{};
      std::cout << GetString<archiveType_>() << '\n';
      auto archive = Get<archiveType_>();
      if constexpr (!std::is_null_pointer_v<decltype(archive)>) {
        if constexpr (std::is_same_v<decltype(archive), std::optional<ZZZ>>) {
          if (archive.has_value()) {
            archive->ExecuteOn(filename, lambda);
          }
        } else if constexpr (std::is_same_v<decltype(archive),
                               FIFLFS<false>> || std::is_same_v<decltype(archive), FIFLFS<true>>) {
          archive.ExecuteOn(filename, lambda);
          if constexpr (nested) {
            archive.ExecuteOnNested(filename, lambda);
          }
        }
      }
    }
  }
};// namespace OpenVIII::Archive
}// namespace OpenVIII::Archive
#endif// VIIIARCHIVE_ARCHIVES_H
