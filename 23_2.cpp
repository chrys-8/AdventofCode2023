/* # Advent of Code Day 12
 */

#include <algorithm>
#include <fstream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

namespace ranges = std::ranges;
namespace views = std::views;

static constexpr const char * INPUT_FILE = "ex.txt";
//static constexpr const char * INPUT_FILE = "23.txt";

enum SpringState {
  Normal, Damaged, Unknown
};

// required for DamageReport
std::string symbol_string(const std::vector<SpringState>&);

struct DamageReport {
  std::vector<SpringState> states{};
  std::vector<uint32_t> block_sizes{};
  std::vector<bool> unknowns_bitmask{};

  std::string symbol_string() const {
    return ::symbol_string(states);
  }
};

struct File {
  std::vector<SpringState> states{};
  std::vector<DamageReport> reports{};

  File& parseStream(std::basic_istream<char>& stream);
};

std::basic_istream<char>& streamGetLine(
    std::basic_istream<char>& stream,
    std::string& str,
    char delim = '\n') {
  str.erase();
  for (char c{}; stream.get(c); )
  {
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

SpringState convertToSpringState(char c) {
  switch (c) {
    case '#':
      return Damaged;
    case '?':
      return Unknown;
    default:
      return Normal;
  }
}

char convertToChar(SpringState state) {
  switch (state) {
    case Normal:
      return '.';
    case Damaged:
      return '#';
    case Unknown:
    default:
      return '?';
  }
}

File& File::parseStream(std::basic_istream<char>& stream) {
  std::string line{};
  while (streamGetLine(stream, line, ' ')) {
    auto& report = this->reports.emplace_back();

    for (char c: line) {
      SpringState state = convertToSpringState(c);
      report.states.push_back(state);
      report.unknowns_bitmask.push_back(state == Unknown);
    }

    char delim{};
    int block_size{};
    do {
      stream >> block_size;
      stream.get(delim);
      report.block_sizes.push_back(block_size);
    } while (delim != '\n');
  }

  return *this;
}

std::string symbol_string(const std::vector<SpringState>& states) {
  std::string result{};
  ranges::transform(states, std::back_inserter(result), convertToChar);
  return result;
}

std::string bitmask_string(const std::vector<bool>& bitmask) {
  std::string result{};
  ranges::transform(bitmask, std::back_inserter(result), [](bool b) { return b ? '1' : '0'; });
  return result;
}

struct DamageReportStrings {
  std::string symbol_string{}, block_sizes_string{}, unknowns_bitmask_string{};

  explicit DamageReportStrings(const DamageReport& report) :
    symbol_string(report.symbol_string()),
    block_sizes_string(fmt::format("{}", fmt::join(report.block_sizes, ","))),
    unknowns_bitmask_string(bitmask_string(report.unknowns_bitmask)) {}
};

int main() {
  File file{};
  {
    std::ifstream file_input(INPUT_FILE);

    if (!file_input) {
      fmt::print("Unable to read input file{}\n", INPUT_FILE);
      return -1;
    }

    file.parseStream(file_input);
  }

  DamageReport& report = file.reports[0];
  DamageReportStrings rs(report);
  fmt::print("{} {}\n{}\n", rs.symbol_string, rs.block_sizes_string, rs.unknowns_bitmask_string);
}
