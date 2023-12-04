/* # Advent of Code: Day 3
 *
 * Determine whether a numeric encoded within text is "adjacent"
 * to a symbol, not including '.'. For example:
 * ```
 * .....
 * .456.
 * .....
 * ```
 * These would be considered cells adjacent to the number 456 in text.
 *
 * A valid number is "adjacent" to one or more valid symbols, which is
 * any non-numerical character other than '.' and the result of the
 * program is the sum of these valid numbers.
 *
 * [Link](https://adventofcode.com/2023/day/3)
 */

#include <fstream>
#include <iostream>
#include <locale>
#include <string>

static constexpr const char * INPUT_FILE = "5.txt";

char grid[140][140] = {};

struct GridIterator
{
	int row = 0;
	int col = 0;
	bool valid = true;

	GridIterator& operator++() {
		if (++col == 140) {
			row++;
			col = 0;
		}

		if (row == 140) {
			valid = false;
		}

		return *this;
	}

	GridIterator operator++(int) {
		GridIterator old = *this;
		operator++();
		return old;
	}

	char operator*() {
		return grid[row][col];
	}

	GridIterator& operator+=(size_t offset) {
		col += offset;
		if (col >= 140) {
			row++;
			col -= 140;
		}

		if (row == 140) {
			valid = false;
		}

		return *this;
	}
};

void gridInit(std::ifstream &stream) {
	for (size_t row = 0; row < 140; row++) {
		for (size_t col = 0; col < 140; col++) {
			char c;
			stream >> c;
			grid[row][col] = c;
		}
	}
}

GridIterator seekDigit(GridIterator it) {
	while (!std::isdigit(*it, std::locale(""))) {
		++it;
		if (!it.valid) return it;
	}

	return it;
}

int getNumber(GridIterator it) {
	int value = 0;

	while (std::isdigit(*it, std::locale(""))) {
		value = value * 10;

		switch (*it) {
		case '1':
			value += 1;
			break;
		case '2':
			value += 2;
			break;
		case '3':
			value += 3;
			break;
		case '4':
			value += 4;
			break;
		case '5':
			value += 5;
			break;
		case '6':
			value += 6;
			break;
		case '7':
			value += 7;
			break;
		case '8':
			value += 8;
			break;
		case '9':
			value += 9;
			break;
		default:
			break;
		}

		++it;
	}

	return value;
}

GridIterator clampedAdjacentCell(GridIterator it, int row_off, int col_off) {
	if (!it.valid) return it;

	it.row += row_off;
	it.col += col_off;

	if (it.row < 0)   it.row = 0;
	if (it.row > 139) it.row = 139;
	if (it.col < 0)   it.col = 0;
	if (it.col > 139) it.col = 139;

	return it;
}

bool isPartNumberValid(GridIterator it, int length) {
	for (int row_off = -1; row_off < 2; ++row_off) {
		for (int col_off = -1; col_off < length + 1; ++col_off) {
			GridIterator adj = clampedAdjacentCell(it, row_off, col_off);
			if (!std::isdigit(*adj, std::locale("")))
				if (*adj != '.') return true;
		}
	}
	return false;
}

int main() {
	std::ifstream fInput(INPUT_FILE);

	if (!fInput) {
		std::cout << "Unable to read input file\n";
		return -1;
	}

	gridInit(fInput);

	int total = 0;

	GridIterator it{};
	while (it.valid) {
		it = seekDigit(it);

		if (!it.valid) break;

		int value = getNumber(it);

		int length = 0;
		if (value > 99)     length = 3;
		else if (value > 9) length = 2;
		else                length = 1;

		if (isPartNumberValid(it, length))
			total += value;

		it += length;
	}

	std::cout << "Total is " << total << '\n';

	return 0;
}
