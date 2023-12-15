/* # Advent of Code Day 6
 *
 * The program needs to determine how to beat the distance record for
 * several boat races. For each race, the boat needs to be charged
 * for an amount of time which counts towards the total time permitted
 * to increase the speed of the boat. For every millisecond charged,
 * the speed increases by 1 millimeter per millisecond.
 *
 * For example:
 * ```
 * Time:     7 15  30
 * Distance: 9 40 200
 * ```
 *
 * The table above lists three races: their total times and their
 * records to beat. The boat starts with 0 mm/ms. So you can:
 * - Charge for 0 ms to a speed of 0 mm/ms and travel 0 mm in the
 *   remaining 7 ms
 * - Charge for 1 ms to a speed of 1 mm/ms and travel 6 mm in the
 *   remaining 6 ms
 * - Charge for 2 ms to a speed of 2 mm/ms and travel 10 mm in the
 *   remaining 5 ms
 * - ...and so on.
 *
 * The program needs to determine how maby ways the time for each race
 * can be discretely divided to yield a distance greater than the
 * record.
 *
 * The output of the program will be the product of the number of ways
 * to win each race.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//static constexpr const char * INPUT_FILE = "ex.txt";
static constexpr const char * INPUT_FILE = "11.txt";

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

struct Race
{
	int time{}, distance{};
};

void parseRaces(
		std::basic_istream<char>& input,
		std::vector<Race> *output)
{
	std::vector<int> times{}, distances{};
	std::string line{};

	streamGetLine(input, &line);
	std::stringstream timesInput(line);
	streamNextLine(timesInput, ' ');
	for (int time{}; timesInput >> time; )
		times.push_back(time);

	streamGetLine(input, &line);
	std::stringstream distancesInput(line);
	streamNextLine(distancesInput, ' ');
	for (int distance{}; distancesInput >> distance; )
		distances.push_back(distance);

	// would use views::zip if I had C++23 -_-
	output->clear();
	for (size_t idx = 0; idx < times.size(); ++idx)
		output->emplace_back(times[idx], distances[idx]);
}

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput)
	{
		std::cout << "Unable to read input file\n";
		return -1;
	}

	std::vector<Race> races{};
	parseRaces(fInput, &races);

	int result = 1;
	for (Race race : races)
	{
		int nWaysToWin = 0;

		for (int i = 1; i < race.time; ++i)
			if (i * (race.time - i) > race.distance)
				++nWaysToWin;

		result *= nWaysToWin;
	}

	std::cout << "Product: " << result << '\n';

	return 0;
}
