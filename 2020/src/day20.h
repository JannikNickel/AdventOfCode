#pragma once
#include "solutionbase.h"
#include "common/grid.h"
#include <vector>

struct Day20 : SolutionBase
{
	Day20(const Input& input) : SolutionBase(input, 20, "Jurassic Jigsaw") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using Data = common::Grid<char>;

	struct Tile
	{
		int id;
		Data data;
		int orientation = 0;

		Tile(int id, Data data) : id(id), data(data) { }

		std::string Top() const { return Row(0); }
		std::string Right() const { return Col(data.Width() - 1); }
		std::string Bottom() const { return Row(data.Height() - 1); }
		std::string Left() const { return Col(0); }

		void NextOrientation();
		std::string Row(int row) const;
		std::string Col(int col) const;
	};

	std::vector<std::vector<Tile*>> SolvePuzzle(std::vector<Tile>& tiles) const;
	Tile* PlaceTile(std::vector<Tile>& tiles, const std::unordered_map<std::string, std::vector<Tile*>>& pairs, const Tile* above, const Tile* left) const;
	Day20::Tile* FindTopLeftTile(std::vector<Day20::Tile>& tiles, const std::unordered_map<std::string, std::vector<Day20::Tile*>>& pairs) const;
	Tile* Neighbour(const std::unordered_map<std::string, std::vector<Tile*>>& pairs, const Tile& tile, const std::string& edge) const;
	Tile CreateImage(const std::vector<std::vector<Tile*>>& tiles) const;
	int CountMonsters(const Tile& tile) const;
	std::vector<Tile> ParseInput() const;
};
