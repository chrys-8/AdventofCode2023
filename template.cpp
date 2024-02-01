/* # Advent of Code
 */

#include <fstream>
#include <string>

#include <fmt/core.h>

static constexpr const char * INPUT_FILE = "ex.txt";

std::basic_istream<char>& streamGetLine(
    std::basic_istream<char>& stream,
    std::string& str,
    char delim = '\n') {
  str->erase();
  for (char c{}; stream.get(c); )
  {
    if (c == delim)
      break;

    *(str) += c;
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

int main() {
  std::ifstream file_input(INPUT_FILE);

  if (!file_input)
  {
    fmt::print("Unable to read input file{}\n", INPUT_FILE);
    return -1;
  }

  return 0;
}
