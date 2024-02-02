# Advent of Code 2023

This repository has been archived. See commit [a6b2b42](https://github.com/chrys-8/AdventofCode2023/commit/a6b2b42f74e32f9bf8dda51f6d9e623a2ad583f2) for more information.

I’m doing Advent of Code 2023 in C++!

Compile with fmt and C++23!

## Advent of Code 2023, Day 1

Sum of calibration values encoded within text

For example:
```
1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet
```

Yields values, `12`, `38`, `15` and `77`, which sum to `142`.

[Link](https://adventofcode.com/2023/day/1)

## Advent of Code 2023, Day 1 - Part 2

Sum of calibration values encoded within text

For example:
```
two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen
```

Yields values `29`, `83`, `13`, `24`, `42`, `14` and `76`,
which sum to `281`.

[Link](https://adventofcode.com/2023/day/1)

## Advent of Code 2023 - Day 2

Filter a collection of "games" by whether they would be possible
given a maximum number of coloured cubes in a bag.

A "game" consists of a bag containing a constant number of cubes,
which may each be one of three colours: `red`, `green` or `blue`,
and a handful of these cubes is revealed in any number of "rounds".

The data for each "game" is structured like the following:
```
Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
```

The filter needs to determine whether a "game" is possible for a bag
containing `12 red, 13 green, 14 blue`. In the case of the examples
above, only "games" `1`, `2` and `5`, would be possible, since all
others exceed the maximum for any/all of the types of coloured cube.

The result will be the sum of the IDs for valid "games".

[Link](https://adventofcode.com/2023/day/2)

## Advent of Code 2023 - Day 2 - Part 2

Filter a collection of "games" by whether they would be possible
given a maximum number of coloured cubes in a bag.

A "game" consists of a bag containing a constant number of cubes,
which may each be one of three colours: `red`, `green` or `blue`,
and a handful of these cubes is revealed in any number of "rounds".

The data for each "game" is structured like the following:
```
Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
```

The program needs to determine the power of the minimum set for each
game. So for Game 1 above, this game would be possible with as few as
4 red, 2 green and 6 blue cubes. The power is the product of these
three values.

The result of the program is the sum of the power of all games.

[Link](https://adventofcode.com/2023/day/2)

## Advent of Code: Day 3

Determine whether a numeric encoded within text is "adjacent"
to a symbol, not including '.'. For example:
```
.....
.456.
.....
```
These would be considered cells adjacent to the number 456 in text.

A valid number is "adjacent" to one or more valid symbols, which is
any non-numerical character other than '.' and the result of the
program is the sum of these valid numbers.

[Link](https://adventofcode.com/2023/day/3)

## Advent of Code: Day 3 - Part 2

The program needs to determine the values of "gear ratios"
encoded within a schematic. A "gear part" is encoded by any string
of digits "adjacent" to an asterisk "\*", and a valid gear is given
by an asterisk adjacent to two digit strings. For example:
```
467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..
```

This schematic contains two gears: (467, 35) and (755, 589). The
gear ratio is the product of these two numbers. The output of the
program will be the sum of all gear ratios.

[Link](https://adventofcode.com/2023/day/3)
