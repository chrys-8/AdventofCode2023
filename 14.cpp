/* # Advent of Code Day 6 Part 2
 *
 * Same as previous only J now represents Joker cards. The J cards are
 * the weakest individual cards, but they represent wildcards for
 * making the strongest possible type. For example:
 * ```
 * 32T3K 765
 * T55J5 684
 * KK677 28
 * KTJJT 220
 * QQQJA 483
 * ```
 *
 * T55J5 becomes T5555, KTJJT becomes KTTTT and QQQJA becomes QQQQA, all
 * four of a kind.
 *
 * The output of the program will be the total winnings for all hands
 * (see Day 6 part 1 for details about winnings and bids.)
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

static constexpr const char * INPUT_FILE = "13.txt";
//static constexpr const char * INPUT_FILE = "ex.txt";

enum Card {
	Joker = 1,
	Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
	Queen, King,
	Ace = 15,
	Error = 0
};

enum HandType {
	HighCard, OnePair, TwoPair, ThreeKind, FullHouse, FourKind, FiveKind
};

struct HandBid {
	Card hand[5];
	HandType handType;
	int64_t bid;
};

struct CardCount {
	Card card{};
	int count{};
};

struct HandStats {
	CardCount slots[4]{};
	int jokerCount{};
};

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

Card cardFromChar(char c)
{
	switch (c) {
	case 'J':
		return Card::Joker;
	case '2':
		return Card::Two;
	case '3':
		return Card::Three;
	case '4':
		return Card::Four;
	case '5':
		return Card::Five;
	case '6':
		return Card::Six;
	case '7':
		return Card::Seven;
	case '8':
		return Card::Eight;
	case '9':
		return Card::Nine;
	case 'T':
		return Card::Ten;
	case 'Q':
		return Card::Queen;
	case 'K':
		return Card::King;
	case 'A':
		return Card::Ace;
	default:
		return Card::Error; // should never happen
	}
}

HandBid makeBid(std::string_view hand, int64_t bid)
{
	HandStats stats{};
	Card cards[5]{};

	for (size_t idx = 0; idx < 5; ++idx) {
		char c = hand[idx];
		Card card = cardFromChar(c);
		cards[idx] = card;

		if (card == Card::Joker) {
			++stats.jokerCount;
			continue;
		}

		for (CardCount& c : stats.slots) {
			if (c.count == 0) {
				c.card = card;
				c.count = 1;
				break;
			}

			if (card == c.card) {
				++c.count;
				break;
			}
		}
	}

	HandType type{};
	std::sort(std::begin(stats.slots), std::end(stats.slots),
			[](auto lhs, auto rhs) { return lhs.count > rhs.count; });

	if (stats.slots[0].count + stats.jokerCount == 5)
		type = HandType::FiveKind;

	else if (stats.slots[0].count + stats.jokerCount == 4)
		type = HandType::FourKind;

	else if (stats.slots[0].count + stats.jokerCount == 3 && stats.slots[1].count == 2)
		type = HandType::FullHouse;

	else if (stats.slots[0].count + stats.jokerCount == 3)
		type = HandType::ThreeKind;

	else if (stats.slots[0].count == 2 && stats.slots[1].count == 2)
		type = HandType::TwoPair;

	else if (stats.slots[0].count + stats.jokerCount == 2)
		type = HandType::OnePair;

	else
		type = HandType::HighCard;

	return {cards[0], cards[1], cards[2], cards[3], cards[4], type, bid};
}

void parseBids(
		std::basic_istream<char>& stream,
		std::vector<HandBid> *bids)
{
	while (stream) {
		std::string hand{}, strBid{};
		streamGetLine(stream, &hand, ' ');
		streamGetLine(stream, &strBid);

		if (hand.empty())
			break;

		int64_t bid{};
		std::stringstream bidInput(strBid);
		bidInput >> bid;

		bids->push_back(makeBid(hand, bid));
	}
}

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput) {
		std::cout << "Unable to read input file\n";
		return -1;
	}

	std::vector<HandBid> bids{};
	parseBids(fInput, &bids);

	auto compareHands = [](auto a, auto b) {
		if (a.handType == b.handType) {
			for (size_t idx = 0; idx < 5; ++idx) {
				if (a.hand[idx] == b.hand[idx])
					continue;

				return a.hand[idx] < b.hand[idx];
			}

			return false; // if hands are identical
		}

		return a.handType < b.handType;
	};

	std::sort(bids.begin(), bids.end(), compareHands);

	int64_t rank = 1;
	int64_t winnings = 0;
	for (HandBid handBid : bids) {
		std::cout << rank << ": ["
			<< handBid.handType << "] ";
		for (Card card : handBid.hand)
			std::cout << card << ' ';

		std::cout << '\n';

		winnings += handBid.bid * rank;
		++rank;
	}

	std::cout << "Total winnings are: " << winnings << '\n';

	return 0;
}
