#pragma once
#include "pch.h"

#define TRUE 1
#define FALSE 0

class chessmen {
public:
	typedef std::array<unsigned int, 2> position;
	typedef unsigned int x_pos;
	typedef unsigned int y_pos;
	typedef unsigned int chessmen_id;

	enum color {
		white = 0, black = 1
	};

	position current_position;
	color player_color;

	chessmen(color color_input, position position_input) {
		current_position = position_input;
		player_color = color_input;
	}
};

class pawn : public chessmen {
private:
	bool m_hasMoved = FALSE;
public:
	using chessmen::chessmen;
	std::vector<position> possibleMoves() {
		std::vector<position> returnpos;
		if (m_hasMoved == FALSE && current_position[1] <= 6) {
			returnpos.push_back(position{ current_position[0], current_position[1] + 2 });
		}
		if (current_position[1] <= 7) {
			returnpos.push_back(position{ current_position[0], current_position[1] + 1 });
		}
		return returnpos;
	}
};

class rook : public chessmen {
public:
	using chessmen::chessmen;
	std::vector<position> possibleMoves() {
		std::vector<position> returnpos;
		{
			{
				int x = current_position[0] + 1;
				while (x <= 7) {
					returnpos.push_back(position{ x, current_position[1] });
					x++;
				}
			}
			{
				int x = current_position[0] - 1;
				while (x >= 0) {
					returnpos.push_back(position{ x, current_position[1] });
					x++;
				}
			}
			{
				int y = current_position[0] + 1;
				while (y <= 7) {
					returnpos.push_back(position{ current_position[0], y });
					y++;
				}
			}
			{
				int y = current_position[0] - 1;
				while (y >= 0) {
					returnpos.push_back(position{ current_position[0], y });
					y--;
				}
			}
		}
		return returnpos;
	}
};

class knight : public chessmen {
public:
	using chessmen::chessmen;
	std::vector<position> possibleMoves() {
		std::vector<position> returnpos;
		if (current_position[0] + 2 <= 7) {
			if (current_position[1] + 1 <= 7) {
				returnpos.push_back(position{ current_position[0] + 2, current_position[1] + 1 });
			}
			if (current_position[1] - 1 >= 0) {
				returnpos.push_back(position{ current_position[0] + 2, current_position[1] - 1 });
			}
		}
		if (current_position[0] - 2 >= 0) {
			if (current_position[1] + 1 <= 7) {
				returnpos.push_back(position{ current_position[0] - 2, current_position[1] + 1 });
			}
			if (current_position[1] - 1 >= 0) {
				returnpos.push_back(position{ current_position[0] - 2, current_position[1] - 1 });
			}
		}
		if (current_position[1] + 2 <= 7) {
			if (current_position[0] + 1 <= 7) {
				returnpos.push_back(position{ current_position[0] + 1, current_position[1] + 2 });
			}
			if (current_position[0] - 1 >= 0) {
				returnpos.push_back(position{ current_position[0] - 1, current_position[1] + 2 });
			}
		}
		if (current_position[1] - 2 >= 0) {
			if (current_position[0] + 1 <= 7) {
				returnpos.push_back(position{ current_position[0] + 1, current_position[1] - 2 });
			}
			if (current_position[0] - 1 >= 0) {
				returnpos.push_back(position{ current_position[0] - 1, current_position[1] - 2 });
			}
		}
		return returnpos;
	}
};

class bishop : public chessmen {
public:
	using chessmen::chessmen;
	std::vector<position> possibleMoves() {
		std::vector<position> returnpos;
		{
			{
				int x = current_position[0] + 1; int y = current_position[1] + 1;
				while (x <= 7 && y <= 7) {
					returnpos.push_back(position{ x, y });
					x++; y++;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] + 1;
				while (x >= 0 && y <= 7) {
					returnpos.push_back(position{ x, y });
					x--; y++;
				}
			}
			{
				int x = current_position[0] + 1; int y = current_position[1] - 1;
				while (x <= 7 && y >= 0) {
					returnpos.push_back(position{ x, y });
					x++; y--;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] - 1;
				while (x >= 0 && y >= 0) {
					returnpos.push_back(position{ x, y });
					x--; y--;
				}
			}

		}
		return returnpos;
	}
};

class queen : public chessmen {
public:
	using chessmen::chessmen;
	std::vector<position> possibleMoves() {
		std::vector<position> returnpos;
		{
			{
				int x = current_position[0] + 1; int y = current_position[1] + 1;
				while (x <= 7 && y <= 7) {
					returnpos.push_back(position{ x, y });
					x++; y++;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] + 1;
				while (x >= 0 && y <= 7) {
					returnpos.push_back(position{ x, y });
					x--; y++;
				}
			}
			{
				int x = current_position[0] + 1; int y = current_position[1] - 1;
				while (x <= 7 && y >= 0) {
					returnpos.push_back(position{ x, y });
					x++; y--;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] - 1;
				while (x >= 0 && y >= 0) {
					returnpos.push_back(position{ x, y });
					x--; y--;
				}
			}
			{
				int x = current_position[0] + 1;
				while (x <= 7) {
					returnpos.push_back(position{ x, current_position[1] });
					x++;
				}
			}
			{
				int x = current_position[0] - 1;
				while (x >= 0) {
					returnpos.push_back(position{ x, current_position[1] });
					x++;
				}
			}
			{
				int y = current_position[0] + 1;
				while (y <= 7) {
					returnpos.push_back(position{ current_position[0], y });
					y++;
				}
			}
			{
				int y = current_position[0] - 1;
				while (y >= 0) {
					returnpos.push_back(position{ current_position[0], y });
					y--;
				}
			}
		}
		return returnpos;
	}
};

class king : public chessmen {
public:
	using chessmen::chessmen;
	std::vector<position> possibleMoves() {
		std::vector<position> returnpos;
		{
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if (current_position[0] + i <= 7 && current_position[1] + j <= 7) {
						returnpos.push_back(position{ current_position[0] + i ,current_position[1] + j });
					}
				}
			}
		}
		return returnpos;
	}
};