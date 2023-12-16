/* # Advent of Code Day 6
 *
 *
 * The program needs to determine the total winnings from a list of bids
 * on hands in a card game. The hands are ranked weakest to strongest,
 * with rank 1 being the weakest and so on.
 *
 * A hand consists of 6 cards. A hand can have one of the following types,
 * ordered strongest to weakest:
 * - Five of a kind
 * - Four of a kind
 * - Full house (three of a kind and a pair)
 * - Three of a kind
 * - Two pairs
 * - One pair
 * - High card
 *
 * Hands with the same type are ranked by comprehensively searching for
 * whichever has a higher card value first. Cards values from strongest
 * to weakest are: A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, 2.
 *
 * Each bid is multiplied by the rank of the corresponding hand to
 * yield the winnings for that bid. The program will ouput the total
 * winnings.
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
	Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
	Jack, Queen, King,
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

struct HandStats {
	Card card{};
	int count{};
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
	case 'J':
		return Card::Jack;
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
	HandStats stats[4]{};
	Card cards[5]{};

	for (size_t idx = 0; idx < 5; ++idx) {
		char c = hand[idx];
		Card card = cardFromChar(c);
		cards[idx] = card;

		for (HandStats& st : stats) {
			if (st.count == 0) {
				st.card = card;
				st.count = 1;
				break;
			}

			if (card == st.card) {
				++st.count;
				break;
			}
		}
	}

	HandType type{};
	std::sort(std::begin(stats), std::end(stats),
			[](auto lhs, auto rhs) { return lhs.count > rhs.count; });

	if (stats[0].count == 5)
		type = HandType::FiveKind;

	else if (stats[0].count == 4)
		type = HandType::FourKind;

	else if (stats[0].count == 3 && stats[1].count == 2)
		type = HandType::FullHouse;

	else if (stats[0].count == 3)
		type = HandType::ThreeKind;

	else if (stats[0].count == 2 && stats[1].count == 2)
		type = HandType::TwoPair;

	else if (stats[0].count == 2)
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
