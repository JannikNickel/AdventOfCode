#include "day20.h"
#include "common/stringutils.h"
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <algorithm>
#include <array>

const int ORIENTATIONS = 8;
const int MONSTER_TILES = 15;
const std::array<std::string, 3> monster =
{
	"                  # ",
	"#    ##    ##    ###",
	" #  #  #  #  #  #   "
};

ResultBase* Day20::SolveFirst() const
{
	std::vector<Tile> tiles = ParseInput();
	std::vector<std::vector<Tile*>> result = SolvePuzzle(tiles);
	return new Result(
		static_cast<uint64_t>(result[0][0]->id)
		* static_cast<uint64_t>(result[result.size() - 1][0]->id)
		* static_cast<uint64_t>(result[0][result.size() - 1]->id)
		* static_cast<uint64_t>(result[result.size() - 1][result.size() - 1]->id)
	);
}

ResultBase* Day20::SolveSecond() const
{
	std::vector<Tile> tiles = ParseInput();
	std::vector<std::vector<Tile*>> puzzle = SolvePuzzle(tiles);
	Tile image = CreateImage(puzzle);

	for(int i = 0; i < ORIENTATIONS; i++)
	{
		int c = CountMonsters(image);
		if(c != 0)
		{
			return new Result(std::ranges::count_if(image.data.Data(), [](char c) { return c == '#'; }) - c * MONSTER_TILES);
		}
		image.NextOrientation();
	}
	return nullptr;
}

std::vector<std::vector<Day20::Tile*>> Day20::SolvePuzzle(std::vector<Tile>& tiles) const
{
	std::unordered_map<std::string, std::vector<Tile*>> pairs = {};
	for(Tile& tile : tiles)
	{
		for(int i = 0; i < ORIENTATIONS; i++)
		{
			pairs[tile.Top()].push_back(&tile);
			tile.NextOrientation();
		}
	}

	int size = std::sqrt(tiles.size());
	std::vector<std::vector<Tile*>> puzzle = {};
	for(int col = 0; col < size; col++)
	{
		puzzle.push_back(std::vector<Tile*>(size, nullptr));
		for(int row = 0; row < size; row++)
		{
			Tile* above = row != 0 ? puzzle[col][row - 1] : nullptr;
			Tile* left = col != 0 ? puzzle[col - 1][row] : nullptr;
			puzzle[col][row] = PlaceTile(tiles, pairs, above, left);
		}
	}
	return puzzle;
}

Day20::Tile* Day20::PlaceTile(std::vector<Tile>& tiles, const std::unordered_map<std::string, std::vector<Tile*>>& pairs, const Tile* above, const Tile* left) const
{
	if(above != nullptr || left != nullptr)
	{
		Tile* tile = above != nullptr ? Neighbour(pairs, *above, above->Bottom()) : Neighbour(pairs, *left, left->Right());
		for(size_t i = 0; i < ORIENTATIONS; i++)
		{
			bool t = above != nullptr ? tile->Top() == above->Bottom() : pairs.at(tile->Top()).size() == 1;
			bool l = left != nullptr ? tile->Left() == left->Right() : pairs.at(tile->Left()).size() == 1;
			if(t && l)
			{
				return tile;
			}
			tile->NextOrientation();
		}
	}
	return FindTopLeftTile(tiles, pairs);
}

Day20::Tile* Day20::FindTopLeftTile(std::vector<Day20::Tile>& tiles, const std::unordered_map<std::string, std::vector<Day20::Tile*>>& pairs) const
{
	for(Tile& tile : tiles)
	{
		for(int i = 0; i < ORIENTATIONS; i++)
		{
			if(pairs.at(tile.Top()).size() == 1 && pairs.at(tile.Left()).size() == 1)
			{
				return &tile;
			}
			tile.NextOrientation();
		}
	}
	return nullptr;
}

Day20::Tile* Day20::Neighbour(const std::unordered_map<std::string, std::vector<Tile*>>& pairs, const Tile& tile, const std::string& edge) const
{
	auto it = std::find_if(pairs.at(edge).begin(), pairs.at(edge).end(), [&](const Tile* t) { return t != &tile; });
	return it != pairs.at(edge).end() ? *it : nullptr;
}

Day20::Tile Day20::CreateImage(const std::vector<std::vector<Tile*>>& tiles) const
{
	size_t actualTileSize = tiles[0][0]->data.Width() - 2;
	int size = actualTileSize * tiles.size();
	Data data = Data(size, size);
	size_t x = 0;
	size_t y = 0;
	for(size_t ty = 0; ty < tiles.size(); ty++)
	{
		for(size_t tx = 0; tx < tiles.size(); tx++)
		{
			for(size_t ly = 0; ly < actualTileSize; ly++)
			{
				for(size_t lx = 0; lx < actualTileSize; lx++)
				{
					data.At(x + lx, y + ly) = tiles[tx][ty]->data(lx + 1, ly + 1);
				}
			}
			x += actualTileSize;
		}
		x = 0;
		y += actualTileSize;
	}
	return Tile(0, data);
}

int Day20::CountMonsters(const Tile& tile) const
{
	int c = 0;
	for(size_t i = 0; i < tile.data.Width() - monster[0].length(); i++)
	{
		for(size_t k = 0; k < tile.data.Height() - monster.size(); k++)
		{
			bool match = true;
			for(size_t y = 0; y < monster.size(); y++)
			{
				for(size_t x = 0; x < monster[0].length(); x++)
				{
					if(monster[y][x] == '#' && tile.data(i + x, k + y) != '#')
					{
						match = false;
						goto SHAPE_END;
					}
				}
			}
			SHAPE_END:
			c += match;
		}
	}
	return c;
}

std::vector<Day20::Tile> Day20::ParseInput() const
{
	std::vector<std::string> chunks = common::SplitStr(common::ReplaceStr(input.raw, "\r\n", "\n"), "\n\n");
	std::vector<Tile> tiles = {};
	for(const std::string& chunk : chunks)
	{
		std::vector<std::string> lines = common::SplitStr(chunk, "\n");
		int id = std::stoi(lines[0].substr(5, lines[0].size() - 6));
		Data data = Data(lines[1].size(), lines.size() - 1);
		for(size_t i = 1; i < lines.size(); i++)
		{
			for(size_t k = 0; k < lines[i].size(); k++)
			{
				data.At(k, i - 1) = lines[i][k];
			}
		}
		tiles.push_back(Tile(id, data));
	}
	return tiles;
}

void Day20::Tile::NextOrientation()
{
	orientation++;
	bool flip = orientation % 4 == 0;

	Data from = data;
	for(size_t col = 0; col < data.Width(); col++)
	{
		for(size_t row = 0; row < data.Height(); row++)
		{
			int x = data.Height() - 1 - row;
			int y = col;
			if(flip)
			{
				y = data.Width() - 1 - y;
			}
			data.At(col, row) = from(x, y);
		}
	}
}

std::string Day20::Tile::Row(int row) const
{
	std::string s = "";
	for(size_t i = 0; i < data.Width(); i++)
	{
		s += std::string(1, data(i, row));
	}
	return s;
}

std::string Day20::Tile::Col(int col) const
{
	std::string s = "";
	for(size_t i = 0; i < data.Height(); i++)
	{
		s += std::string(1, data(col, i));
	}
	return s;
}
