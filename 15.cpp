/* # Advent of Code Day 8
 *
 * MEGA BIRTHDAY EDITION
 *
 * You have been given a map with left/right instructions.Starting at
 * the node labelled `AAA` how many steps will it take following the
 * left/right instructions to get to the node labelled `ZZZ`.
 */

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

static constexpr const char * INPUT_FILE = "15.txt";
//static constexpr const char * INPUT_FILE = "ex.txt";

enum Instruction {
	Left, Right
};

struct MapNode {
	std::string label{}, left{}, right{};
};

std::basic_istream<char>& streamGetLine(
		std::basic_istream<char>& stream,
		std::string *str,
		char delim = '\n')
{
	str->erase();
	for (char c{}; stream.get(c); ) {
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

int main()
{
	std::ifstream fInput(INPUT_FILE);

	if (!fInput) {
		std::cout << "Unable to read input file\n";
		return -1;
	}

	std::vector<Instruction> instructions{};
	std::map<std::string, MapNode> map{};

	std::string strInstructions{};
	streamGetLine(fInput, &strInstructions);
	for (char c : strInstructions)
		switch (c) {
		case 'L':
			instructions.push_back(Instruction::Left);
			break;
		case 'R':
			instructions.push_back(Instruction::Right);
			break;
		default:
			break;
		}

	streamNextLine(fInput);

	while (fInput) {
		MapNode node{};
		streamGetLine(fInput, &node.label, ' ');
		streamNextLine(fInput, '(');
		streamGetLine(fInput, &node.left, ',');
		streamNextLine(fInput, ' ');
		streamGetLine(fInput, &node.right, ')');
		streamNextLine(fInput);

		if (!node.label.empty())
			map[node.label] = std::move(node);
	}

	int steps = 0;
	size_t idxInstruction = 0;
	std::string nodeLabel = "AAA";
	while (nodeLabel != "ZZZ") {
		const MapNode& node = map[nodeLabel];
		switch (instructions[idxInstruction]) {
		case Instruction::Left:
			nodeLabel = node.left;
			break;
		case Instruction::Right:
			nodeLabel = node.right;
		}

		++steps;
		if (++idxInstruction == instructions.size())
			idxInstruction = 0;
	}

	std::cout << "Total number of steps: " << steps << '\n';

	return 0;
}
