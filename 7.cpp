/* # Advent of Code Day 4
 *
 * The program needs to determine the points for a collection of
 * scratchcards. Each scratchcard contains a set of winning numbers
 * and another set of chosen numbers. The points for each scratchcard
 * is determined by how many of the chosen numbers are also within
 * the winning numbers. For example:
 * ```
 * Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
 * Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
 * Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
 * Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
 * Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
 * Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
 * ```
 *
 * For Card 1, the chosen numbers are also winning numbers: 48, 83, 17
 * and 89. When 1 number wins, the card is worth 1 point. For each extra
 * winning number, the point value of the card doubles. So Card 1 is worth
 * 8 points.
 *
 * The output of the program will be the total sum of points for all cards.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

static constexpr const char * INPUT_FILE = "7.txt";

struct Scratchcard
{
	// sizes taken from program input
	int id = 0;
	int winners[10] = {};
	int chosen[25] = {};
	int points = 0;
};

Scratchcard readCard(std::ifstream& stream) {
	Scratchcard card{};
	std::string unused{}, cardNumber{};

	stream >> cardNumber;

	card.id = std::stoi(cardNumber);

	for (int& winner : card.winners)
		stream >> winner;

	stream >> unused;

	for (int& choice : card.chosen)
		stream >> choice;

	return card;
}

void incrementPoints(Scratchcard& card) {
	if (card.points == 0) card.points = 1;
	else                  card.points = card.points * 2;
}

int main() {
	std::ifstream fInput(INPUT_FILE);

	if (!fInput) {
		std::cout << "Unable to read input file\n";
		return -1;
	}

	int total = 0;
	std::string unused{};
	while (fInput >> unused) {
		Scratchcard card = readCard(fInput);
		std::for_each(std::begin(card.chosen), std::end(card.chosen),
			[&] (int choice) {
				if (std::find(std::begin(card.winners), std::end(card.winners), choice) != std::end(card.winners))
					incrementPoints(card);
		});

		total += card.points;
	}

	std::cout << "Total is " << total << '\n';

	return 0;
}
