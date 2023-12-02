/* # Advent of Code 2023, Day 1 - Part 2
 *
 * Sum of calibration values encoded within text
 *
 * For example:
 * ```
 * two1nine
 * eightwothree
 * abcone2threexyz
 * xtwone3four
 * 4nineeightseven2
 * zoneight234
 * 7pqrstsixteen
 * ```
 *
 * Yields values `29`, `83`, `13`, `24`, `42`, `14` and `76`,
 * which sum to `281`.
 *
 * [Link](https://adventofcode.com/2023/day/1)
 */

#include <cstring>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>

static constexpr const char * INPUT_FILE = "1.txt";
static constexpr const char * NUMBERS[] = {
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

bool startswith(auto begin, auto end, const char * c) {
	auto c_len = std::strlen(c);

	if ((size_t)std::distance(begin, end) < c_len)
		return false;

	auto haystack = begin;
	for (auto needle = c; needle < c + c_len; needle++) {
		if (*haystack != *needle)
			return false;
		else
			haystack++;
	}

	return true;
}

int firstDigit(auto begin, auto end) {
	auto it = begin;
	while (it != end) {
		switch (*it) {
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'o':
			if (startswith(it, end, "one"))
				return 1;
			else
				break;
		case 't':
			if (startswith(it, end, "two"))
				return 2;
			else if (startswith(it, end, "three"))
				return 3;
			else
				break;
		case 'f':
			if (startswith(it, end, "four"))
				return 4;
			else if (startswith(it, end, "five"))
				return 5;
			else
				break;
		case 's':
			if (startswith(it, end, "six"))
				return 6;
			else if (startswith(it, end, "seven"))
				return 7;
			else
				break;
		case 'e':
			if (startswith(it, end, "eight"))
				return 8;
			else
				break;
		case 'n':
			if (startswith(it, end, "nine"))
				return 9;
			else
				break;
		default:
			break;
		}

		it++;
	}

	return 0;
}

int lastDigit(auto rbegin, auto rend) {
	auto it = rbegin;
	while (it != rend) {
		switch (*it) {
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'e':
			if (startswith(it, rend, "eno"))
				return 1;
			if (startswith(it, rend, "eerht"))
				return 3;
			else if (startswith(it, rend, "evif"))
				return 5;
			else if (startswith(it, rend, "enin"))
				return 9;
			else
				break;
		case 'o':
			if (startswith(it, rend, "owt"))
				return 2;
			else
				break;
		case 'r':
			if (startswith(it, rend, "ruof"))
				return 4;
			else
				break;
		case 'x':
			if (startswith(it, rend, "xis"))
				return 6;
			else
				break;
		case 'n':
			if (startswith(it, rend, "neves"))
				return 7;
			else
				break;
		case 't':
			if (startswith(it, rend, "thgie"))
				return 8;
			else
				break;
		default:
			break;
		}

		it++;
	}

	return 0;
}

int main() {
	std::ifstream fInput(INPUT_FILE);
	std::string strLine{};

	if (!fInput) {
		std::cout << "Couldn't read input file\n";
		return -1;
	}

	int total = 0;

	while (fInput) {
		strLine.clear();
		fInput >> strLine;

		int first = firstDigit(strLine.begin(), strLine.end());
		total += 10 * first;

		int last = lastDigit(strLine.rbegin(), strLine.rend());
		total += last;
	}

	std::cout << "Total is: " << total << '\n';

	return 0;
}
