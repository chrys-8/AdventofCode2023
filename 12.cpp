/* # Advent of Code Day 6
 *
 * The program needs to determine how many ways a record for a boat
 * race could be beaten. The boat needs to be charged for an amount
 * of time that counts towards the race time in order to gain speed.
 *
 * The input needs to be parsed to ignore spaces in between numbers
 * on both lines. For example:
 * ```
 * Time:     7 15  30
 * Distance: 9 40 200
 * ```
 *
 * This is a single race with a time of 71530 milliseconds and a record
 * distance of 940200 millimeters.
 *
 * The output of the program will be the total number of ways to beat
 * the record.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//static constexpr const char * INPUT_FILE = "ex.txt";
static constexpr const char * INPUT_FILE = "11.txt";

struct Race
{
	int64_t time{}, distance{};
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

int64_t parseNumberWithSpaces(std::basic_istream<char>& input)
{
	int64_t result = 0;
	for (char c{}; input.get(c); )
		switch (c)
		{
		case '0':
			result = 10 * result + 0;
			break;
		case '1':
			result = 10 * result + 1;
			break;
		case '2':
			result = 10 * result + 2;
			break;
		case '3':
			result = 10 * result + 3;
			break;
		case '4':
			result = 10 * result + 4;
			break;
		case '5':
			result = 10 * result + 5;
			break;
		case '6':
			result = 10 * result + 6;
			break;
		case '7':
			result = 10 * result + 7;
			break;
		case '8':
			result = 10 * result + 8;
			break;
		case '9':
			result = 10 * result + 9;
			break;
		default:
			break;
		}

	return result;
}

Race parseRace(std::basic_istream<char>& input)
{
	std::string line{};
	streamNextLine(input, ' ');
	streamGetLine(input, &line);

	std::stringstream timeInput(line);
	int64_t time = parseNumberWithSpaces(timeInput);

	streamNextLine(input, ' ');
	streamGetLine(input, &line);

	std::stringstream distanceInput(line);
	int64_t distance = parseNumberWithSpaces(distanceInput);

	return {time, distance};
}

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput)
	{
		std::cout << "Unable to read input file\n";
		return -1;
	}

	Race race = parseRace(fInput);
	int64_t i = 1;
	for ( ; i < race.time; ++i)
		if (i * (race.time - i) > race.distance)
			break;

	std::cout << "Number of ways to beat record: "
		<< race.time - 2*i + 1 << '\n';

	return 0;
}
