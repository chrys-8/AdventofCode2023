/* # Advent of Code Day 9 Part 2
 *
 * Given a sequence of values, determine the sequence of the differences
 * between terms recursively until all 0s. For example:
 * ```
 * 0   3   6   9  12  15
 *   3   3   3   3   3
 *     0   0   0   0
 * ```
 *
 * The previous value can be extrapolated, -3. The program will return
 * the sum of the extrapolated values.
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

int64_t sequenceExtrapolatePrevTerm(const std::vector<int64_t>& seq)
{
	std::vector<int64_t> diff{}, tmp{};
	sequenceDifference(seq, &diff);
	printVec(seq);
	printVec(diff);

	// extrapolate prev value by taking an alternating sum of the first
	// terms of all diff sequences and subtracting this from the first
	// term of the input sequence
	int64_t result = seq.front();
	int64_t parity = -1;
	while (!ranges::all_of(diff, std::bind_front(std::equal_to(), 0))) {
		result += parity * diff.front();
		parity *= -1;
		sequenceDifference(diff, &tmp);
		std::swap(diff, tmp);
		printVec(diff);
	}

	std::cout << result << '\n';

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
		sum_of_extrapolated_values += sequenceExtrapolatePrevTerm(seq);
		std::cout << "---\n";
	}

	std::cout << "Sum is: " << sum_of_extrapolated_values << '\n';

	return 0;
}
