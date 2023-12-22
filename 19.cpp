/* # Advent of Code Day 10
 *
 * For a map of pipe, there is a loop containing the S pipe, the starting
 * pipe, which connects two neibouring pipes. Find the further pipe along
 * the loop. For example:
 * ```
 * ..F7.
 * .FJ|.
 * SJ.L7
 * |F--J
 * LJ...
 * ```
 *
 * The further pipe from the starting pipe in this map is 8, located at
 * line 3, column 5.
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

static constexpr const char * INPUT_FILE = "19.txt";

enum Tile {
	Ground = 0,
	NorthPipe = 1 << 1,
	EastPipe = 1 << 2,
	SouthPipe = 1 << 3,
	WestPipe = 1 << 4
};

int gMap[140][140]{};
int gStartX{};
int gStartY{};

std::basic_istream<char>& streamGetLine(
		std::basic_istream<char>& stream,
		std::string *str,
		char delim = '\n')
{
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
		char delim = '\n')
{
	for (char c{}; stream.get(c); )
		if (c == delim)
			break;

	return stream;
}

int tileFromCharacter(char c)
{
	switch (c) {
	case 'L':
		return Tile::NorthPipe | Tile::EastPipe;
	case '|':
		return Tile::NorthPipe | Tile::SouthPipe;
	case 'J':
		return Tile::NorthPipe | Tile::WestPipe;
	case 'F':
		return Tile::EastPipe | Tile::SouthPipe;
	case '-':
		return Tile::EastPipe | Tile::WestPipe;
	case '7':
		return Tile::SouthPipe | Tile::WestPipe;
	default:
		return Tile::Ground;
	}
}

char tileToCharacter(int tile)
{
	switch (tile) {
	case Tile::NorthPipe | Tile::EastPipe:
		return 'L';
	case Tile::NorthPipe | Tile::SouthPipe:
		return '|';
	case Tile::NorthPipe | Tile::WestPipe:
		return 'J';
	case Tile::EastPipe | Tile::SouthPipe:
		return 'F';
	case Tile::EastPipe | Tile::WestPipe:
		return '-';
	case Tile::SouthPipe | Tile::WestPipe:
		return '7';
	default:
		return '.';
	}
}

void parseMap(std::basic_istream<char>& stream)
{
	size_t y = 0;
	for (std::string line{}; streamGetLine(stream, &line); ) {
		size_t x = 0;
		for (char c : line) {
			if (c == 'S') {
				gStartX = x;
				gStartY = y;
			}
			gMap[y][x] = tileFromCharacter(c);
			++x;
		}
		++y;
	}
}

bool isStartPipeValid()
{
	// assuming that start pipe is not on the edge of the grid (I know it isn't)
	int x = gStartX, y = gStartY, connections = 0, tile = Tile::Ground;
	if (gMap[y-1][x] & Tile::SouthPipe) {
		tile |= Tile::NorthPipe;
		++connections;
	}
	if (gMap[y][x-1] & Tile::EastPipe) {
		tile |= Tile::WestPipe;
		++connections;
	}
	if (gMap[y][x+1] & Tile::WestPipe) {
		tile |= Tile::EastPipe;
		++connections;
	}
	if (gMap[y+1][x] & Tile::NorthPipe) {
		tile |= Tile::SouthPipe;
		++connections;
	}

	gMap[y][x] = tile;
	return connections <= 2;
}

// returns {newX, newY, movementToNewTile}
std::tuple<int, int, Tile> mapStep(int x, int y, Tile lastMovement = Tile::Ground)
{
	int tile = gMap[y][x];

	if (x >= 140 || y >= 140)
		return {-1, -1, Tile::Ground};

	// ensure the direction we're checking isn't the one we came from
	if (tile & Tile::NorthPipe && lastMovement != Tile::SouthPipe)
		return {x, y-1, Tile::NorthPipe};

	if (tile & Tile::EastPipe && lastMovement != Tile::WestPipe)
		return {x+1, y, Tile::EastPipe};

	if (tile & Tile::SouthPipe && lastMovement != Tile::NorthPipe)
		return {x, y+1, Tile::SouthPipe};

	if (tile & Tile::WestPipe && lastMovement != Tile::EastPipe)
		return {x-1, y, Tile::WestPipe};

	return {-1, -1, Tile::Ground};
}

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput)
	{
		std::cout << "Unable to read input file\n";
		return -1;
	}

	parseMap(fInput);
	assert(isStartPipeValid());

	int x = gStartX, y = gStartY;
	Tile movement{};
	size_t stepCount = 0;
	do {
		std::cout << "At " <<  x << ", " << y << '\n';
		std::tie(x, y, movement) = mapStep(x, y, movement);
		++stepCount;
	} while (x != gStartX || y != gStartY);

	std::cout << "\nFurthest pipe is " << stepCount / 2 << " steps away\n";

	return 0;
}
