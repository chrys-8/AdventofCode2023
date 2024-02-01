/* # Advent of Code Day 11
 */

#include <algorithm>
#include <fstream>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/core.h>

//static constexpr const char * INPUT_FILE = "ex.txt";
static constexpr const char * INPUT_FILE = "21.txt";

struct File {
  size_t line_count{}, char_count{};
  std::vector<std::string> lines{};

  void parseStream(std::basic_istream<char>&);
};

struct GalaxyMap {
  size_t size_x{}, size_y{};
  std::vector<std::tuple<size_t, size_t>> galaxy_coords{};
  std::vector<bool> rows_bitmask{};
  std::vector<bool> cols_bitmask{};

  void parseFile(File&);
};

std::basic_istream<char>& streamGetLine(
    std::basic_istream<char>& stream,
    std::string& str,
    char delim = '\n') {
  str.erase();
  for (char c{}; stream.get(c); ) {
    if (c == delim)
      break;

    str += c;
  }

  return stream;
}

std::basic_istream<char>& streamNextLine(
    std::basic_istream<char>& stream,
    char delim = '\n') {
  for (char c{}; stream.get(c); )
    if (c == delim)
      break;

  return stream;
}

void File::parseStream(std::basic_istream<char>& stream) {
  lines.clear();
  for (std::string str{}; streamGetLine(stream, str); ) {
    char_count = std::max(char_count, str.size());
    lines.push_back(str);
  }
  line_count = lines.size();
}

void bitmaskAllocateCount(std::vector<bool>& bitmask, size_t count, bool value = false) {
  for (size_t i = 0; i < count; ++i)

    bitmask.push_back(value);
}

void GalaxyMap::parseFile(File& file) {
  size_x = file.char_count;
  size_y = file.line_count;
  galaxy_coords.clear();
  bitmaskAllocateCount(rows_bitmask, size_y);
  bitmaskAllocateCount(cols_bitmask, size_x);

  for (const auto [y, line] : std::views::enumerate(file.lines)) {
    for (const auto [x, ch] : std::views::enumerate(line)) {
      if (ch == '#') {
        galaxy_coords.emplace_back(x, y);
        rows_bitmask[y] = true;
        cols_bitmask[x] = true;
      }
    }
  }
}

void accumulateEmptySpaceFromBitmask(
    std::vector<bool>& in,
    std::vector<size_t>& out) {
  out.clear();
  size_t accumulated_value = 0;
  for (const bool hasGalaxy : in) {
    if (!hasGalaxy) ++accumulated_value;
    out.push_back(accumulated_value);
  }
}

void galaxymapExpandSpace(GalaxyMap& map) {
  std::vector<size_t> row_offsets{}, col_offsets{};
  accumulateEmptySpaceFromBitmask(map.rows_bitmask, row_offsets);
  accumulateEmptySpaceFromBitmask(map.cols_bitmask, col_offsets);
  for (auto& coord : map.galaxy_coords) {
    auto [x, y] = coord;
    x += col_offsets[x];
    y += row_offsets[y];
    coord = {x, y};
  }
}

int main()
{
  GalaxyMap map{};
  {
    std::ifstream file_input(INPUT_FILE);

    if (!file_input)
    {
      fmt::print("Unable to read input file{}\n", INPUT_FILE);
      return -1;
    }

    File file{};
    file.parseStream(file_input);
    map.parseFile(file);
  }

  galaxymapExpandSpace(map);

  int result = 0;
  for (auto [idx0, coords0] : std::views::enumerate(map.galaxy_coords)) {
    auto [x0, y0] = coords0;
    for (auto [idx1, coords1] : std::views::enumerate(map.galaxy_coords)) {
      if (idx0 == idx1) continue;
      auto [x1, y1] = coords1;
      int dx = std::abs((int)x0 - (int)x1);
      int dy = std::abs((int)y0 - (int)y1);
      result += dx + dy;

      //fmt::print("[{}]({}, {}) -> [{}]({}, {}): {}\n", idx0, x0, y0, idx1, x1, y1, dx + dy);
    }
  }

  result = result / 2;

  fmt::print("======\n");
  fmt::print("Result: {}\n", result);

  return 0;
}
