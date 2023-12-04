/* # Advent of Code: Day 3 - Part 2
 *
 * The program needs to determine the values of "gear ratios"
 * encoded within a schematic. A "gear part" is encoded by any string
 * of digits "adjacent" to an asterisk "*", and a valid gear is given
 * by an asterisk adjacent to two digit strings. For example:
 * ```
 * 467..114..
 * ...*......
 * ..35..633.
 * ......#...
 * 617*......
 * .....+.58.
 * ..592.....
 * ......755.
 * ...$.*....
 * .664.598..
 * ```
 *
 * This schematic contains two gears: (467, 35) and (755, 589). The
 * gear ratio is the product of these two numbers. The output of the
 * program will be the sum of all gear ratios.
 *
 * [Link](https://adventofcode.com/2023/day/3)
 */

#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <utility>

static constexpr const char * INPUT_FILE = "5.txt";

char grid[140][140] = {};

struct GridIterator
{
	int row = 0;
	int col = 0;

	GridIterator& operator++() {
		if (++col == 140) {
			row++;
			col = 0;
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

		return *this;
	}

	bool valid() { return row < 140; }
};

bool operator<(const GridIterator lhs, const GridIterator rhs) {
	if (lhs.row == rhs.row) return lhs.col < rhs.col;
	else                    return lhs.row < rhs.row;
}

struct Gear
{
	int val1 = 0;
	int val2 = 0;
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
		if (!it.valid()) return it;
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
	if (!it.valid()) return it;

	it.row += row_off;
	it.col += col_off;

	if (it.row < 0)   it.row = 0;
	if (it.row > 139) it.row = 139;
	if (it.col < 0)   it.col = 0;
	if (it.col > 139) it.col = 139;

	return it;
}

std::pair<bool, GridIterator> getGear(GridIterator it, int length) {
	if (!it.valid()) return { false, {} };

	for (int row_off = -1; row_off < 2; ++row_off) {
		for (int col_off = -1; col_off < length + 1; ++col_off) {
			GridIterator adj = clampedAdjacentCell(it, row_off, col_off);
			if (*adj == '*') return { true, adj };
		}
	}
	return { false, {} };
}

void addGearRatio(Gear& gear, int value) {
	if (gear.val2 != 0) {
		std::cout << "error: three-spoked gear\n";
	}
	else if (gear.val1 != 0) {
		gear.val2 = value;
	}
	else {
		gear.val1 = value;
	}
}

bool gearValid(Gear gear) {
	return gear.val2 != 0;
}

int gearRatio(Gear gear) {
	return gear.val1 * gear.val2;
}

int main() {
	std::ifstream fInput(INPUT_FILE);

	if (!fInput) {
		std::cout << "Unable to read input file\n";
		return -1;
	}

	gridInit(fInput);

	std::map<GridIterator, Gear> gearMap{};

	GridIterator it{};
	while (it.valid()) {
		it = seekDigit(it);

		if (!it.valid()) break;

		int value = getNumber(it);

		int length = 0;
		if (value > 99)     length = 3;
		else if (value > 9) length = 2;
		else                length = 1;

		auto [isGearPart, itGear] = getGear(it, length);

		if (isGearPart) {
			Gear& gear = gearMap[itGear];
			addGearRatio(gear, value);
		}

		it += length;
	}

	int total = 0;
	for (auto& [pos, gear] : gearMap) {
		if (gearValid(gear)) {
			total += gearRatio(gear);
		}
	}

	std::cout << "Total is " << total << '\n';

	return 0;
}
