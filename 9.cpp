/* # Advent of Code Day 5
 *
 * The program needs to map a seed number to a location number through
 * a series of mappings. For example
 * ```
 * seed-to-soil map:
 * 50 98 2
 * 52 50 48
 * ```
 *
 * This mapping describes two ranges of mappings from seed number to
 * soil number. The first range maps seed [98, 99] -> soil [50, 51] and
 * the second range maps [50, 97] -> [52, 99]. All other numbers not
 * specified within a range are mapped to the same number, so 10 -> 10.
 *
 * The program will output the lowest possible location number mapped to
 * by the seed numbers specified by the input.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static constexpr const char * INPUT_FILE = "9.txt";

struct MappingRange
{
	int64_t dstBegin, srcBegin, size;
};

std::vector<MappingRange> gSeedSoilMap{};
std::vector<MappingRange> gSoilFertilizerMap{};
std::vector<MappingRange> gFertilizerWaterMap{};
std::vector<MappingRange> gWaterLightMap{};
std::vector<MappingRange> gLightTemperatureMap{};
std::vector<MappingRange> gTemperatureHumidityMap{};
std::vector<MappingRange> gHumidityLocationMap{};

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

std::basic_istream<char>& buildMap(
		std::basic_istream<char>& stream,
		std::vector<MappingRange>* map)
{
	streamNextLine(stream); // skip heading
	std::string line{};
	while (streamGetLine(stream, &line))
	{
		if (line.empty())
			break;

		int64_t dstBegin{}, srcBegin{}, size{};
		std::stringstream range(line);
		range >> dstBegin >> srcBegin >> size;
		map->emplace_back(dstBegin, srcBegin, size);
	}

	return stream;
}

int64_t mapWithRanges(
		int64_t val,
		std::vector<MappingRange>& map)
{
	int64_t mappedVal = -1;
	for (MappingRange r : map)
	{
		if (r.srcBegin > val or r.srcBegin + r.size <= val)
			continue;

		mappedVal = r.dstBegin + val - r.srcBegin;
		break;
	}

	if (mappedVal == -1)
		mappedVal = val;

	return mappedVal;
}

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput)
	{
		std::cout << "Unable to read input file\n";
		return -1;
	}

	std::vector<int64_t> seeds{};
	{
		std::string line{};
		streamGetLine(fInput, &line);

		std::stringstream seedInput(line);
		streamNextLine(seedInput, ' ');

		for (int64_t seed{}; seedInput >> seed; )
			seeds.push_back(seed);

		streamNextLine(fInput);
	}

	buildMap(fInput, &gSeedSoilMap);
	buildMap(fInput, &gSoilFertilizerMap);
	buildMap(fInput, &gFertilizerWaterMap);
	buildMap(fInput, &gWaterLightMap);
	buildMap(fInput, &gLightTemperatureMap);
	buildMap(fInput, &gTemperatureHumidityMap);
	buildMap(fInput, &gHumidityLocationMap);

	int64_t minLocation = -1;
	for (int64_t seed : seeds)
	{
		int64_t soil        = mapWithRanges(seed,        gSeedSoilMap);
		int64_t fertilizer  = mapWithRanges(soil,        gSoilFertilizerMap);
		int64_t water       = mapWithRanges(fertilizer,  gFertilizerWaterMap);
		int64_t light       = mapWithRanges(water,       gWaterLightMap);
		int64_t temperature = mapWithRanges(light,       gLightTemperatureMap);
		int64_t humidity    = mapWithRanges(temperature, gTemperatureHumidityMap);
		int64_t location    = mapWithRanges(humidity,    gHumidityLocationMap);

		if (location < minLocation or minLocation == -1)
			minLocation = location;

		std::cout << "--------\n";
		std::cout << seed        << " -> " << soil        << '\n';
		std::cout << soil        << " -> " << fertilizer  << '\n';
		std::cout << fertilizer  << " -> " << water       << '\n';
		std::cout << water       << " -> " << light       << '\n';
		std::cout << light       << " -> " << temperature << '\n';
		std::cout << temperature << " -> " << humidity    << '\n';
		std::cout << humidity    << " -> " << location    << '\n';
		std::cout << location    << '\n';
	}

	std::cout << "Lowest location number is " << minLocation << '\n';

	return 0;
}
