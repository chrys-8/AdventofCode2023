/* # Advent of Code Day 12
 */

#include <fstream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <fmt/core.h>

static constexpr const char * INPUT_FILE = "ex.txt";
//static constexpr const char * INPUT_FILE = "23.txt";

enum SpringState {
  Operational, Damaged, Unknown
};

struct Spring {
  SpringState state{};
  int block_size = 0;
};

struct SpringRow {
  std::vector<Spring> springs{};
  std::vector<int> damaged_blocks{};

  void parseString(const std::string&);
};

class SpringRowStackframe {
  public:
    const SpringRow& source;
    size_t stack_offset;
    std::vector<int> remaining_blocks;
    std::vector<std::tuple<int, size_t>> pathway;

  public:
    explicit SpringRowStackframe(const SpringRow& _source) :
      source{_source},
      stack_offset{0},
      remaining_blocks(_make_blocks(source)),
      pathway{} {}

    bool empty() const { return stack_offset == source.damaged_blocks.size(); }

    bool valid() const;

    SpringRowStackframe& advance() {
      if (empty()) return *this;
      ++stack_offset;
      return *this;
    }

    int block_size() const {
      return stack_offset < source.damaged_blocks.size() ?
        source.damaged_blocks[stack_offset] :
        0;
    }

    SpringRowStackframe allotBlockAt(size_t) const;

  private:
    static std::vector<int> _make_blocks(const SpringRow&);
};

struct File {
  std::vector<std::string> lines{};

  void parseStream(std::basic_istream<char>&);
};

std::vector<int> SpringRowStackframe::_make_blocks(const SpringRow& source) {
  std::vector<int> result{};
  for (Spring spring: source.springs)
    result.push_back(spring.block_size);

  return result;
}

SpringRowStackframe SpringRowStackframe::allotBlockAt(size_t index) const {
  SpringRowStackframe result = *this;
  size_t last = std::min(index + result.block_size() + 1, result.remaining_blocks.size());
  for (size_t idx = 0; idx < last; ++idx)
    result.remaining_blocks[idx] = 0;

  result.pathway.emplace_back(index, result.block_size());
  result.advance();
  return result;
}

bool SpringRowStackframe::valid() const {
  if (!this->empty()) return false;

  std::vector<SpringState> final_states(this->source.springs.size());
  for (auto [block_size, index]: this->pathway)
    for (size_t idx = index; idx < index + block_size - 1; ++idx)
      final_states[idx] = Damaged;

  for (const auto& [spring, state]: std::views::zip(this->source.springs, final_states))
    if (spring.state == Damaged && state != Damaged)
      return false;

  return true;
}

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

void File::parseStream(std::basic_istream<char>& stream) {
  for (std::string line{}; streamGetLine(stream, line); )
    this->lines.push_back(line);
}

SpringState convertSymbolToSpringState(char c) {
  switch (c) {
    case '#':
      return Damaged;
    case '?':
      return Unknown;
    default:
      return Operational;
  }
}

void SpringRow::parseString(const std::string& str) {
  std::stringstream stream{str};
  std::string symbols{}, blocks{};

  streamGetLine(stream, symbols, ' ');
  for (char symbol : symbols)
    this->springs.emplace_back(convertSymbolToSpringState(symbol));

  auto springs_sentinel = this->springs.end();
  auto assignKnownBlock = [=] (auto begin_iter, auto sentinel) {
    if (begin_iter == springs_sentinel)
      return springs_sentinel;

    auto block_size = std::distance(begin_iter, sentinel);
    for (Spring& spring: std::ranges::subrange(begin_iter, sentinel))
      spring.block_size = block_size--;

    return springs_sentinel;
  };

  auto block_begin_iter = springs_sentinel;
  auto iter = this->springs.begin();
  for ( ; iter != springs_sentinel; ++iter)
    switch (iter->state) {
      case Damaged:
      case Unknown:
        if (block_begin_iter == springs_sentinel) block_begin_iter = iter;
        break;
      default:
        block_begin_iter = assignKnownBlock(block_begin_iter, iter);
        break;
    }

  assignKnownBlock(block_begin_iter, iter);

  char delim{};
  int block_size{};
  do {
    stream >> block_size;
    this->damaged_blocks.push_back(block_size);
    stream.get(delim);
  } while (stream);
}

char convertToSymbol(SpringState state) {
  switch (state) {
    case Operational:
      return '.';
    case Damaged:
      return '#';
    case Unknown:
      return '?';
    default:
      return '.';
  }
}

void printSpringSymbols(const SpringRow& row) {
  for (Spring spring: row.springs)
    fmt::print("{}", convertToSymbol(spring.state));

  fmt::print("\n");
}

void printSpringSymbols(const std::vector<SpringState>& states) {
  for (SpringState state: states)
    fmt::print("{}", convertToSymbol(state));

  fmt::print("\n");
}

void printBlockSizes(const SpringRow& row) {
  std::stringstream block_size_stream{};
  for (const auto& spring : row.springs)
    block_size_stream << spring.block_size;

  fmt::print("{}\n", block_size_stream.view());
}

void printBlockSizes(const std::vector<int>& blocks) {
  std::stringstream block_size_stream{};
  for (auto block_size: blocks)
    block_size_stream << block_size;

  fmt::print("{}\n", block_size_stream.view());
}

void printStackframe(const SpringRowStackframe& frame) {
  fmt::print("[{}]: block size {}\n  ", frame.stack_offset, frame.block_size());
  printBlockSizes(frame.remaining_blocks);
  for (const auto& [block_size, index]: frame.pathway)
    fmt::print("{{{}, {}}}", block_size, index);
  fmt::print("\n");
}

int main() {
  std::vector<SpringRow> spring_rows{};
  {
    std::ifstream file_input(INPUT_FILE);

    if (!file_input) {
      fmt::print("Unable to read input file{}\n", INPUT_FILE);
      return -1;
    }

    File file{};
    file.parseStream(file_input);
    for (auto line : file.lines) {
      SpringRow next_spring{};
      next_spring.parseString(line);
      spring_rows.push_back(std::move(next_spring));
    }
  }

  int possible_arrangements = 0;

  // will probably want to move rows to stack
  SpringRow row = spring_rows[2];
  std::vector<SpringRowStackframe> row_stack{};
  row_stack.emplace_back(row);

  printSpringSymbols(row);

  do {
    SpringRowStackframe frame = std::move(row_stack.back());
    row_stack.pop_back();

    printStackframe(frame);
    fmt::print("  ");

    for (auto [idx, block_size] : std::views::enumerate(frame.remaining_blocks)) {
      if (block_size >= frame.block_size()) {
        SpringRowStackframe next_frame = frame.allotBlockAt(idx);
        if (next_frame.valid()) {
          ++possible_arrangements;
          fmt::print("===\nValid stackframe:\n");
          printStackframe(next_frame);
          continue;
        }

        row_stack.push_back(std::move(next_frame));
        fmt::print("{} ", idx);
      }
    }
    fmt::print("\n");
  } while (!row_stack.empty());

  fmt::print("===\nPossible combinations: {}\n", possible_arrangements);
}
