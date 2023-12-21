/* # Advent of Code Day 8 Part 2
 *
 * MEGA BIRTHDAY EDITION
 *
 * Given a map with multiple starting points, how many steps would need
 * to be taken to reach all end points simulatenously?
 *
 * From testing, I've found that that paths taken by each "ghost" are
 * cyclic, that the number of steps from A -> Z is the same as Z -> Z,
 * as in the smaller given example (see the Part 1). So, the resulting
 * number of steps should be the product (which hopefully is the LCM)
 * of the steps for each path.
 */

#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
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
	std::vector<std::string> startingNodeLabels{};
	std::map<std::string, MapNode> map{};

	// parse instructions
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

	// parse map nodes
	while (fInput) {
		MapNode node{};
		streamGetLine(fInput, &node.label, ' ');
		streamNextLine(fInput, '(');
		streamGetLine(fInput, &node.left, ',');
		streamNextLine(fInput, ' ');
		streamGetLine(fInput, &node.right, ')');
		streamNextLine(fInput);

		if (!node.label.empty()) {
			if (node.label.ends_with('A'))
				startingNodeLabels.push_back(node.label);

			map[node.label] = std::move(node);
		}
	}

	// traverse map from A->Z for each beginning node
	//  assuming that Z->Z will be the same +1.
	int64_t stepsProduct = 1;
	size_t idxLabel = 0; // for enumerating
	for (std::string& label : startingNodeLabels) {
		int steps = 0;
		size_t idxInstruction = 0;

		while (!label.ends_with('Z')) {
			const MapNode& node = map[label];
			switch(instructions[idxInstruction]) {
			case Instruction::Left:
				label = node.left;
				break;
			case Instruction::Right:
				label = node.right;
				break;
			}

			++steps;
			if(++idxInstruction == instructions.size())
				idxInstruction = 0;
		}

		++idxLabel;
		// ouput lowest commom multiple
		stepsProduct = std::lcm(stepsProduct, steps);

		std::cout << idxLabel << ' ' << steps << '\n';
	}

	std::cout << "Total number of steps: " << stepsProduct << '\n';

	return 0;
}
