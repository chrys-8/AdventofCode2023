/* # Advent of Code 2023, Day 1
 *
 * Sum of calibration values encoded within text
 *
 * For example:
 * ```
 * 1abc2
 * pqr3stu8vwx
 * a1b2c3d4e5f
 * treb7uchet
 * ```
 *
 * Yields values, `12`, `38`, `15` and `77`, which sum to `142`.
 *
 * [Link](https://adventofcode.com/2023/day/1)
 */

#include <algorithm>
#include <cstdlib>
#include <locale>
#include <iostream>
#include <fstream>
#include <string>

static constexpr const char * INPUT_FILE = "1.txt";

int main() {
	std::ifstream fInput(INPUT_FILE);
	std::string strLine{};
	char cstrValue[3] = {'\0', '\0', '\0'};

	if (!fInput) {
		std::cout << "Couldn't read input file\n";
		return -1;
	}

	int total = 0;

	while (fInput) {
		strLine.clear();
		cstrValue[0] = '\0';
		cstrValue[0] = '\0';
		fInput >> strLine;

		auto itFirstDigit = std::find_if(
				strLine.cbegin(), strLine.cend(), [](auto c) {
					return std::isdigit(c, std::locale(""));
				});

		auto itLastDigit = std::find_if(
				strLine.crbegin(), strLine.crend(), [](auto c) {
					return std::isdigit(c, std::locale(""));
				});

		cstrValue[0] = *itFirstDigit;
		cstrValue[1] = *itLastDigit;
		total +=  std::atoi(cstrValue);
	}

	std::cout << "Total is: " << total << '\n';

	return 0;
}
