/* # Advent of Code Day 4 - Part 2
 *
 * The program needs to determine the points for a collection of
 * scratchcards. Each scratchcard contains a set of winning numbers
 * and another set of chosen numbers. For example:
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
 * and 89.
 *
 * For each winning number scored on a scratchcard, you win a copy of that
 * number of subsequent scratchcards. So, card 1 has 4 winning numbers,
 * so you would win a copy of cards 2, 3, 4 and 5.
 *
 * Card 2 has 2 matching numbers, and since we have two copies of card 2
 * (thanks to card 1) we win two more copies of 3 and 4.
 *
 * The output of the program is the total number of scratchcards we have
 * after all possible copies have been awarded.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

static constexpr const char * INPUT_FILE = "7.txt";

// size taken from program input
int cardFrequencies[219] = {};

struct Scratchcard
{
	// sizes taken from program input
	int id = 0;
	int winners[10] = {};
	int chosen[25] = {};
	int matches = 0;
};

void cardFrequenciesInit() {
	for (int& freq : cardFrequencies)
		freq = 1;
}

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

int main() {
	std::ifstream fInput(INPUT_FILE);

	if (!fInput) {
		std::cout << "Unable to read input file\n";
		return -1;
	}

	cardFrequenciesInit();

	int total = 0;
	std::string unused{};
	while (fInput >> unused) {
		Scratchcard card = readCard(fInput);
		std::for_each(std::begin(card.chosen), std::end(card.chosen),
			[&] (int choice) {
				if (std::find(std::begin(card.winners), std::end(card.winners), choice) != std::end(card.winners))
					++card.matches;
		});

		int freq = cardFrequencies[card.id - 1];
		for (int i = 0; i < card.matches; ++i) {
			int nextId = card.id + i + 1;
			cardFrequencies[nextId - 1] += freq;
		}

		total += freq;
	}

	std::cout << "Total is " << total << '\n';

	return 0;
}
