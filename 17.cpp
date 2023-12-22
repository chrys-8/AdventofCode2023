/* # Advent of Code Day 9
 *
 * Given a sequence of values, determine the sequence of the differences
 * between terms recursively until all 0s. For example:
 * ```
 * 0   3   6   9  12  15
 *   3   3   3   3   3
 *     0   0   0   0
 * ```
 *
 * The next value can be extrapolated from this to be 18 (15 + 3). The
 * output of the program will be the sum of extrapolated values for
 * all sequences.
 */

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static constexpr const char * INPUT_FILE = "17.txt";
//static constexpr const char * INPUT_FILE = "ex.txt";

namespace ranges = std::ranges;

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

std::basic_istream<char>& streamParseSequence(
		std::basic_istream<char>& stream,
		std::vector<int64_t> *vec)
{
	std::string line{};
	streamGetLine(stream, &line);

	vec->clear();

	std::stringstream strSequence(line);
	for (int64_t term{}; strSequence >> term; )
		vec->push_back(term);

	return stream;
}

void sequenceDifference(
		const std::vector<int64_t>& input,
		std::vector<int64_t> *output)
{
	output->clear();
	// pairwise transform would be nice
	// after this, I'm updating my compiler
	for (size_t idx = 0; idx < input.size() - 1; ++idx)
		output->push_back(input[idx + 1] - input[idx]);
}

void printVec(const std::vector<int64_t>& v)
{
	for (auto element : v)
		std::cout << element << ' ';
	std::cout << '\n';
}

int64_t sequenceExtrapolateNextTerm(const std::vector<int64_t>& seq)
{
	std::vector<int64_t> diff{}, tmp{};
	sequenceDifference(seq, &diff);
	printVec(seq);
	printVec(diff);

	// extrapolate next value by adding end terms for all diff sequences
	int64_t result = seq.back();
	while (!ranges::all_of(diff, std::bind_front(std::equal_to(), 0))) {
		result += diff.back();
		sequenceDifference(diff, &tmp);
		std::swap(diff, tmp);
		printVec(diff);
	}

	return result;
}

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput)
	{
		std::cout << "Unable to read input file\n";
		return 1;
	}

	std::vector<int64_t> seq{};
	int64_t sum_of_extrapolated_values = 0;
	while (streamParseSequence(fInput, &seq)) {
		sum_of_extrapolated_values += sequenceExtrapolateNextTerm(seq);
		std::cout << "---\n";
	}

	std::cout << "Sum is: " << sum_of_extrapolated_values << '\n';

	return 0;
}
