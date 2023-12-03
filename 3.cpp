/* # Advent of Code 2023 - Day 2
 *
 * Filter a collection of "games" by whether they would be possible
 * given a maximum number of coloured cubes in a bag.
 *
 * A "game" consists of a bag containing a constant number of cubes,
 * which may each be one of three colours: `red`, `green` or `blue`,
 * and a handful of these cubes is revealed in any number of "rounds".
 *
 * The data for each "game" is structured like the following:
 * ```
 * Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
 * Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
 * Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
 * Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
 * Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
 * ```
 *
 * The filter needs to determine whether a "game" is possible for a bag
 * containing `12 red, 13 green, 14 blue`. In the case of the examples
 * above, only "games" `1`, `2` and `5`, would be possible, since all
 * others exceed the maximum for any/all of the types of coloured cube.
 *
 * The result will be the sum of the IDs for valid "games".
 *
 * [Link](https://adventofcode.com/2023/day/2)
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

static constexpr const char * INPUT_FILE = "3.txt";

struct Game
{
	int id, red, green, blue = 0;
};

bool isEOL(std::ifstream &stream) {
	return static_cast<char>(stream.peek()) == '\n';
}

bool isGameValid(const Game& game) {
	if (game.red > 12)   return false;
	if (game.green > 13) return false;
	if (game.blue > 14)  return false;
	return true;
}

int main() {
	std::ifstream fInput(INPUT_FILE);

	if (!fInput) {
		std::cout << "Unable to read input file\n";
		return -1;
	}

	int total = 0;
	Game game{};

	// data is always encoded in pairs
	std::string inFirst{};
	std::string inSecond{};
	while (fInput >> inFirst >> inSecond) {
		if (inFirst == "Game") {
			game.id = std::stoi(inSecond);
		}
		else if (inSecond.starts_with("red")) {
			game.red = std::max(game.red, std::stoi(inFirst));
		}
		else if (inSecond.starts_with("green")) {
			game.green = std::max(game.green, std::stoi(inFirst));
		}
		else if (inSecond.starts_with("blue")) {
			game.blue = std::max(game.blue, std::stoi(inFirst));
		}
		else {
			std::cout << "Parsing error\n";
			return -1;
		}

		if (isEOL(fInput)) {
			if (isGameValid(game)) {
				total += game.id;
				std::cout << "Game " << game.id << " is valid\n";
			}
			else {
				std::cout << "Game " << game.id << " is invalid\n";
			}

			game = Game{}; // reset game
		}
	}

	std::cout << "Total is " << total << "\n";

	return 0;
}
