/* Abandoned for now........
 *
 * # Advent of Code Day 10 Part 2
 *
 * The program will output the number of tiles that are fully enclosed
 * within a given loop.
 *
 * FF7FSF7F7F7F7F7F---7 ┌┌┐┌S┌┐┌┐┌┐┌┐┌┐┌───┐ ┌┌┐┌╗┌┐┌┐┌┐┌┐┌┐┌───┐
 * L|LJ||||||||||||F--J └│└┘││││││││││││┌──┘ └│└┘││││││││││││┌──┘
 * FL-7LJLJ||||||LJL-77 ┌└─┐└┘└┘││││││└┘└─┐┐ ┌└─┐└┘└┘││││││└┘└─┐┐
 * F--JF--7||LJLJ.F7FJ- ┌──┘┌──┐││└┘└┘.┌┐┌┘─ ┌──┘┌──┐││└┘└┘.┌┐┌┘─
 * L---JF-JLJ....FJLJJ7 └───┘┌─┘└┘....┌┘└┘┘┐ └───┘┌─┘└┘....┌┘└┘┘┐
 * |F|F-JF---7...L7L|7| │┌│┌─┘┌───┐...└┐└│┐│ │┌│┌─┘┌───┐...└┐└│┐│
 * |FFJF7L7F-JF7..L---7 │┌┌┘┌┐└┐┌─┘┌┐..└───┐ │┌┌┘┌┐└┐┌─┘┌┐..└───┐
 * 7-L-JL7||F7|L7F-7F7| ┐─└─┘└┐││┌┐│└┐┌─┐┌┐│ ┐─└─┘└┐││┌┐│└┐┌─┐┌┐│
 * L.L7LFJ|||||FJL7||LJ └.└┐└┌┘│││││┌┘└┐││└┘ └.└┐└┌┘│││││┌┘└┐││└┘
 * L7JLJL-JLJLJL--JLJ.L └┐┘└┘└─┘└┘└┘└──┘└┘.└ └┐┘└┘└─┘└┘└┘└──┘└┘.└
 * ((1))                ((2))                ((3))
 *
 * X┌┐┌╗┌┐┌┐┌┐┌┐┌┐┌───┐ (upscale map)        X┌┐┌╗┌┐┌┐┌┐┌┐┌┐┌───┐
 * X│└┘││││││││││││┌──┘                      X│└┘││││││││││││┌──┘
 * X└─┐└┘└┘││││││└┘└─┐X                      X└─┐└┘└┘││││││└┘└─┐X
 * ┌──┘┌──┐││└┘└┘.┌┐┌┘X                      ┌──┘┌──┐││└┘└┘A┌┐┌┘X
 * └───┘┌─┘└┘....┌┘└┘XX                      └───┘┌─┘└┘AAAA┌┘└┘XX
 * XXX┌─┘┌───┐...└┐XXXX                      XXX┌─┘┌───┐AAA└┐XXXX
 * XX┌┘┌┐└┐┌─┘┌┐..└───┐                      XX┌┘┌┐└┐┌─┘┌┐AA└───┐
 * XX└─┘└┐││┌┐│└┐┌─┐┌┐│                      XX└─┘└┐││┌┐│└┐┌─┐┌┐│
 * X.XXX┌┘│││││┌┘└┐││└┘                      XbXXX┌┘│││││┌┘└┐││└┘
 * XXXXX└─┘└┘└┘└──┘└┘.X                      XXXXX└─┘└┘└┘└──┘└┘cX
 * ((4))                ((5))                ((6))
 *
 * Processing steps:
 * 1. Input from file
 * 2. Same, simplified for readability
 * 3. Determine the shape of the starting pipe S
 * 4. Follow loop from starting pipe and invalidate all pipes not in
 *    this loop
 * 5. Expand map, connecting pipes in loop
 * 6. Determine each connected region of ground tiles and invalidate
 *    regions that touch the edge
 *
 * The output of the program will be the total number of valid ground
 * tiles.
 */

#ifdef DEBUG
#define DO_PRINT 0
#else
#define DO_PRINT 1
#endif //DEBUG

#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <tuple>

#if DO_PRINT
#include <fmt/core.h>
#endif

//static constexpr const char * INPUT_FILE = "ex.txt";
static constexpr const char * INPUT_FILE = "19.txt";

enum TileType {
  Ground    = 0,
  North     = 1 << 0,
  East      = 1 << 1,
  South     = 1 << 2,
  West      = 1 << 3,
  Connected = 1 << 4,
};

struct Region {
  bool valid = true;
  size_t count = 0;
};

struct Tile {
  int type{};
  bool in_region = false;
};

struct Coord {
  int x{}, y{};

  bool valid() const;
};

struct MapStep {
  Coord from{}, to{};

  TileType direction() const;
  void next();
  void advanceBy(TileType nextDirection);

  int x() const { return to.x; }
  int y() const { return to.y; }
};

static constexpr const int HEIGHT = 140;
static constexpr const int WIDTH  = 140;

bool Coord::valid() const {
  return x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT;
}

TileType MapStep::direction() const {
  int x = to.x - from.x, y = to.y - from.y;
  if (x > 0) return West;
  if (x < 0) return East;
  if (y > 0) return South;
  if (y < 0) return North;
  return Ground;
}

Tile g_map[HEIGHT][WIDTH]{};
Coord g_start{};
Tile g_upscaled_map[HEIGHT*2][WIDTH*2]{};

template <class... U>
Tile& g_map_tile(const U&... u) {
  const Coord c{std::forward<const U&>(u)...};
  return g_map_tile[c.y][c.x];
}

template <class... U>
int& g_map_tile_type(const U&... u) {
  return g_map_tile(u...).type;
}

template <class... U>
Tile& g_upscaled_map_tile(const U&... u) {
  const Coord c{std::forward<const U&>(u)...};
  return g_upscaled_map[c.y][c.x];
}

template <class... U>
Tile& tileUpscaledCoords(const U&... u) {
  const Coord c{std::forward<const U&>(u)...};
  return g_upscaled_map[2*c.y][2*c.x];
}

std::basic_istream<char>& streamGetLine(
    std::basic_istream<char>& stream,
    std::string *str,
    char delim = '\n') {
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
    char delim = '\n') {
  for (char c{}; stream.get(c); )
    if (c == delim)
      break;

  return stream;
}

template <class Contained>
auto popAndReturnFromQueue(std::queue<Contained>& queue) {
  auto result = queue.front();
  queue.pop();
  return result;
}

int tileFromCharacter(char c) {
  switch (c) {
  case 'S':
    return TileType::Connected;
  case 'L':
    return TileType::North | TileType::East;
  case '|':
    return TileType::North | TileType::South;
  case 'J':
    return TileType::North | TileType::West;
  case 'F':
    return TileType::East | TileType::South;
  case '-':
    return TileType::East | TileType::West;
  case '7':
    return TileType::South | TileType::West;
  default:
    return TileType::Ground;
  }
}

char tileToCharacter(int tile) {
  switch (tile) {
  case TileType::North | TileType::East:
    return 'L';
  case TileType::North | TileType::South:
    return '|';
  case TileType::North | TileType::West:
    return 'J';
  case TileType::East | TileType::South:
    return 'F';
  case TileType::East | TileType::West:
    return '-';
  case TileType::South | TileType::West:
    return '7';
  default:
    return '.';
  }
}

void parseMapAndGroundQueue(
    std::basic_istream<char>& stream,
    std::queue<Coord>& ground_queue) {
  size_t y = 0;
  for (std::string line{}; streamGetLine(stream, &line); ) {
    size_t x = 0;
    for (char c : line) {
      // would need a guard here for linting
      // but we're fine without one
      if (c == 'S')
        g_start = {x, y};

      g_map_tile_type(x, y) = tileFromCharacter(c);
      if (g_map_tile_type(x, y) == TileType::Ground)
        ground_queue.emplace(x, y);
      ++x;
    }
    ++y;
  }
}

bool isStartPipeValid() {
  // assuming that start pipe is not on the edge of the grid (I know it isn't)
  Coord c{g_start.x, g_start.y};
  Tile tile{};
  int connections = 0;
  if (g_map_tile_type(x, y-1) & TileType::South) {
    tile.type |= TileType::North;
    ++connections;
  }
  if (g_map_tile_type(x-1, y) & TileType::East) {
    tile.type |= TileType::West;
    ++connections;
  }
  if (g_map_tile_type(x+1, y) & TileType::West) {
    tile.type |= TileType::East;
    ++connections;
  }
  if (g_map_tile_type(x, y+1) & TileType::North) {
    tile.type |= TileType::South;
    ++connections;
  }

  g_map_tile(x, y) = tile;
  return connections <= 2;
}

void MapStep::advanceBy(TileType nextDirection) {
  from = to;
  switch (nextDirection) {
    case North:
      to = {from.x, from.y-1};
      break;
    case East:
      to = {from.x+1, from.y};
      break;
    case South:
      to = {from.x, from.y+1};
      break;
    case West:
      to = {from.x-1, from.y};
      break;
    default:
      break;
  }
}

// also assumes valid input, which may be naïve
void MapStep::next() {
  int tile_type = g_map_tile_type(x, y);

  // ensure the direction we're checking isn't the one we came from
  if (tile_type & TileType::North && direction() != TileType::South)
    advanceBy(North);

  if (tile_type & TileType::East && direction() != TileType::West)
    advanceBy(East);

  if (tile_type & TileType::South && direction() != TileType::North)
    advanceBy(South);

  if (tile_type & TileType::West && direction() != TileType::East)
    advanceBy(West);
}

void connectLoopAndSetUpscaledLoop() {
  MapStep step{g_start, g_start};
  do {
    tileUpscaledCoords(step.to) = g_map_tile(step.to);
    step.next();
    g_map_tile_type(x, y) |= TileType::Connected;

    switch (step.direction()) {
      case North: // so go south (see mapStep)
        g_upscaled_map_tile(2*x, 2*y+1).type = TileType::North | TileType::South;
        break;
      case South:
        g_upscaled_map_tile(2*x, 2*y-1).type = TileType::North | TileType::South;
        break;
      case East:
        g_upscaled_map_tile(2*x-1, 2*y).type = TileType::East | TileType::West;
        break;
      case West:
        g_upscaled_map_tile(2*x+1, 2*y).type = TileType::East | TileType::West;
        break;
      default:
        break;
    }
  } while (false);
}

bool canVisitTile(int x, int y) {
  if (x < 0 || x >= WIDTH*2 || y < 0 || y >= HEIGHT*2)
    return false;

  if (!(g_map[y][x].type == TileType::Ground))
    return false;

  return not g_map[y][x].in_region;
}

Region bucketfillRegion(int xstart, int ystart) {
  Region region{};
  std::queue<std::tuple<int, int>> tiles_to_visit{};
  tiles_to_visit.emplace(xstart, ystart);

  // coords refer to small map coords

  [[maybe_unused]] bool db_large_queue = false;
  while (!tiles_to_visit.empty()) {
#if DO_PRINT
    fmt::print("Size of queue: {}\n", tiles_to_visit.size());
    if (!db_large_queue && tiles_to_visit.size() > 100) {
      db_large_queue = true;
      fmt::print("Large queue at {}, {}\n", xstart, ystart);
    }
#endif

    auto [x, y] = popAndReturnFromQueue(tiles_to_visit);

    g_upscaled_map[y][x].in_region = true;
    ++region.count;

    region.valid = x > 0 || x < WIDTH*2-1 || y > 0 || y < HEIGHT*2-1;

    if (canVisitTile(x, y-1)) tiles_to_visit.emplace(x, y-1);
    if (canVisitTile(x, y+1)) tiles_to_visit.emplace(x, y+1);
    if (canVisitTile(x-1, y)) tiles_to_visit.emplace(x-1, y);
    if (canVisitTile(x+1, y)) tiles_to_visit.emplace(x+1, y);
  }

  return region;
}

int main() {
  std::ifstream file_input(INPUT_FILE);

  if (!file_input)
  {
#if DO_PRINT
    fmt::print("Unable to read input file{}\n", INPUT_FILE);
#endif
    return -1;
  }

  std::queue<Coord> ground_coords{};

  parseMapAndGroundQueue(file_input, ground_coords);
  if (!isStartPipeValid()){
#if DO_PRINT
    fmt::print("Start pipe either doesn't exist or there are too many connectors\n");
#endif
    return -1;
  }
  connectLoopAndSetUpscaledLoop();

  [[maybe_unused]] int result = 0;
  [[maybe_unused]] int dbloops = 0;
  while (!ground_coords.empty()) {
    auto [x, y] = popAndReturnFromQueue(ground_coords);
    Region r  = bucketfillRegion(x, y);

#if 0
    Region r = bucketfillRegion(x, y);

#if DO_PRINT
    const char* str;
    if (r.valid) str = "Valid";
    else str = "Invalid";
    fmt::print("{}: {} region with {} tiles\n",++dbloops, str, r.count);
#endif

    if (r.valid)
      result += r.count;
#endif
  }

#if DO_PRINT
  //fmt::print("{} valid ground tiles\n", result);
#endif

  return 0;
}
