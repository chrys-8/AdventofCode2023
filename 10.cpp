/* # Advent of Code Day 5 Part 2
 *
 * The program now needs to map a **range** of seed numbers to their
 * corresponding location numbers through a series of mappings. For
 * example
 * ```
 * seed-to-soil map:
 * 50 98 2
 * 52 50 48
 * ```
 *
 * This mapping describes two ranges of mappings from seed number to
 * soil number. For a given input range, which is formatted as a pair,
 * `79 14` means seed numbers [79, 92].
 *
 * Rather than naïvely mapping every individual seed number to its
 * location number, the program will map ranges to ranges through a
 * method which I can only call *segmentation* (though there's likely
 * a fancier name for it out there). I'll explain this more below.
 *
 * Take the example above: seed range [79, 92]. This range is not
 * included within the first mapping range [98, 99], but it does fall
 * fully within the second mapping range [50, 97], since 50 ≤ 79 and
 * 92 ≤ 97. [60, 97] maps to [52, 99], so the output soil range would
 * be [81, 94].
 *
 * Now for soil to fertilizer with the soil range [81, 94]. The first
 * mapping is [15, 51], the second [52, 53], the third [0, 14]. Since
 * none of the mapping ranges contain the input, we can assume the
 * fertilizer numbers to be the same as the soil numbers.
 *
 * There may be a case in which only part of the input range is
 * contained within a mapping range, in which case the input is
 * segmented into two ranges, split at the terminus number of the
 * mapping range, depending on whether the input precedes or succeeds
 * the map.
 *
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

static constexpr const char * INPUT_FILE = "9.txt";
//static constexpr const char * INPUT_FILE = "ex.txt";

struct Range
{
	int64_t begin, size;
};

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

std::tuple<Range, Range, Range, Range> mapRangeToRange(
		Range r,
		const std::vector<MappingRange>& maps)
{
	Range mapped{-1, -1}, leading{-1, -1}, trailing{-1, -1}, intersect{-1, -1};

	for (MappingRange map : maps)
	{
		int64_t rEnd = r.begin + r.size;
		int64_t mapEnd = map.srcBegin + map.size;
		int64_t intersectEnd = -1;

		// no overlap
		if (rEnd <= map.srcBegin || r.begin >= mapEnd)
			continue;

		// leading range
		if (r.begin < map.srcBegin)
		{
			leading = {r.begin, map.srcBegin - r.begin};
			intersect.begin = map.srcBegin;
		}
		else
		{
			intersect.begin = r.begin;
		}

		// trailing range
		if (rEnd > mapEnd)
		{
			trailing = {mapEnd, rEnd - mapEnd};
			intersectEnd = mapEnd;
		}
		else
		{
			intersectEnd = rEnd;
		}

		intersect.size = intersectEnd - intersect.begin;
		mapped = {intersect.begin + map.dstBegin - map.srcBegin, intersect.size};
		break;
	}

	if (mapped.begin == -1)
		mapped = r;

	return {mapped, leading, trailing, intersect};
}

void printRange(Range r)
{
	std::cout << '['
		<< r.begin << ", "
		<< r.begin + r.size << ')';
}

std::tuple<Range, Range, Range> printMappingStage(
		Range r,
		const std::vector<MappingRange>& map)
{
	auto [mapped, leading, trailing, intersect] = mapRangeToRange(r, map);

	printRange(r);
	std::cout << " -> ";
	printRange(mapped);

	if (leading.begin != -1)
	{
		std::cout << " L: ";
		printRange(leading);
	}

	if (intersect.begin != -1)
	{
		std::cout << " I: ";
		printRange(intersect);
	}

	if (trailing.begin != -1)
	{
		std::cout << " T: ";
		printRange(trailing);
	}

	std::cout << '\n';

	return {mapped, leading, trailing};
}

void mapRangesStack(
		std::vector<Range> * const input,
		const std::vector<MappingRange>& map,
		std::vector<Range> * const output)
{
	output->clear();
	while (!input->empty())
	{
		Range range = input->back();
		input->pop_back();

		auto [soil, leading, trailing] = printMappingStage(range, map);

		output->push_back(soil);
		if (leading.begin != -1)  input->push_back(leading);
		if (trailing.begin != -1) input->push_back(trailing);
	}
}

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput)
	{
		std::cout << "Unable to read input file\n";
		return -1;
	}

	std::vector<Range> inputRanges{};
	{
		std::string line{};
		streamGetLine(fInput, &line);

		std::stringstream seedInput(line);
		streamNextLine(seedInput, ' ');

		for (Range seed{}; seedInput >> seed.begin >> seed.size; )
			inputRanges.push_back(seed);

		streamNextLine(fInput);
	}

	buildMap(fInput, &gSeedSoilMap);
	buildMap(fInput, &gSoilFertilizerMap);
	buildMap(fInput, &gFertilizerWaterMap);
	buildMap(fInput, &gWaterLightMap);
	buildMap(fInput, &gLightTemperatureMap);
	buildMap(fInput, &gTemperatureHumidityMap);
	buildMap(fInput, &gHumidityLocationMap);

	std::vector<Range> outputRanges{};

	std::cout << "Seed -> Soil\n";
	mapRangesStack(&inputRanges, gSeedSoilMap, &outputRanges);
	std::swap(inputRanges, outputRanges);

	std::cout << "Soil -> Fertilizer\n";
	mapRangesStack(&inputRanges, gSoilFertilizerMap, &outputRanges);
	std::swap(inputRanges, outputRanges);

	std::cout << "Fertilizer -> Water\n";
	mapRangesStack(&inputRanges, gFertilizerWaterMap, &outputRanges);
	std::swap(inputRanges, outputRanges);

	std::cout << "Water -> Light\n";
	mapRangesStack(&inputRanges, gWaterLightMap, &outputRanges);
	std::swap(inputRanges, outputRanges);

	std::cout << "Light -> Temperature\n";
	mapRangesStack(&inputRanges, gLightTemperatureMap, &outputRanges);
	std::swap(inputRanges, outputRanges);

	std::cout << "Temperature -> Humidty\n";
	mapRangesStack(&inputRanges, gTemperatureHumidityMap, &outputRanges);
	std::swap(inputRanges, outputRanges);

	std::cout << "Humidty -> Location\n";
	mapRangesStack(&inputRanges, gHumidityLocationMap, &outputRanges);

	int64_t minLocation = LLONG_MAX;
	for (Range location : outputRanges)
		if (location.begin < minLocation)
			minLocation = location.begin;

	std::cout << "Minimum location is: " << minLocation << '\n';
	return 0;
}
