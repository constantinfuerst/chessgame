#pragma once

#include "pch.h"

class chessmen {
public:
	typedef std::array<unsigned int, 2> position;
	typedef unsigned int x_pos;
	typedef unsigned int y_pos;
	typedef unsigned int chessmen_id;

	enum color {
		white = 0, black = 1
	};
	enum chessfigure {
		pawn = 0, rook = 1, knight = 2, bishop = 3, queen = 4, king = 5
	};
	enum position_status {
		empty = 0, friendly = 1, enemy = 2
	};

	chessfigure figure;
	position current_position = { 0, 0 };
	color player_color;
	bool hasMoved = FALSE;

	virtual std::vector<position> possibleMoves(std::vector<chessmen*>& chessmen);;
	static position_status positiocheck(std::vector<chessmen*>& chessmen, position pos, color player);

protected:
	chessmen(color color_input, position position_input, chessfigure figure_input) {
		current_position[0] = position_input[0];
		current_position[1] = position_input[1];
		player_color = color_input;
		figure = figure_input;
	}
};

inline std::vector<chessmen::position> chessmen::possibleMoves(std::vector<chessmen*>& chessmen) {
	std::vector<position> emptyreturn;
	return emptyreturn;
}

inline chessmen::position_status chessmen::positiocheck(std::vector<chessmen*>& chessmen, position pos, color player) {
	for (size_t i = 0; i < chessmen.size(); i++) {
		if (chessmen[i]->current_position[0] == pos[0] && chessmen[i]->current_position[1] == pos[1]) {
			if (chessmen[i]->player_color == player) {
				return friendly;
			}
			else {
				return enemy;
			}
		}
	}
	return empty;
}

class pawn : public chessmen {
public:
	pawn(color color_input, position position_input, chessfigure figure_input) : chessmen(color_input, position_input, figure_input){}

	virtual std::vector<position> possibleMoves(std::vector <chessmen*>& chessmen) override {
		std::vector<position> returnpos;
		position hitleft;
		position hitright;
		position onestep;
		position twostep;
		if (player_color == black) {
			hitleft[0] = current_position[0] - 1;
			hitleft[1] = current_position[1] - 1;
			hitright[0] = current_position[0] + 1;
			hitright[1] = current_position[1] - 1;
			onestep[0] = current_position[0];
			onestep[1] = current_position[1] - 1;
			twostep[0] = current_position[0];
			twostep[1] = current_position[1] - 2;
		}
		else {
			hitleft[0] = current_position[0] - 1;
			hitleft[1] = current_position[1] + 1;
			hitright[0] = current_position[0] + 1;
			hitright[1] = current_position[1] + 1;
			onestep[0] = current_position[0];
			onestep[1] = current_position[1] + 1;
			twostep[0] = current_position[0];
			twostep[1] = current_position[1] + 2;
		}
		if (hasMoved == FALSE) {
			if (positiocheck(chessmen, twostep, player_color) == empty) {
				returnpos.push_back(twostep);
			}
		}
		if (current_position[1] <= 6 && current_position[1] >= 1) {
			if (positiocheck(chessmen, onestep, player_color) == empty) {
				returnpos.push_back(onestep);
			}
		}
		if (positiocheck(chessmen, hitleft, player_color) == enemy && current_position[0] > 0) {
			returnpos.push_back(hitleft);
		}
		if (positiocheck(chessmen, hitright, player_color) == enemy && current_position[0] < 7) {
			returnpos.push_back(hitright);
		}
		return returnpos;
	}
};

class rook : public chessmen {
public:
	rook(color color_input, position position_input, chessfigure figure_input) : chessmen(color_input, position_input, figure_input) {}

	virtual std::vector<position> possibleMoves(std::vector <chessmen*>& chessmen) override {
		std::vector<position> returnpos;
		{
			{
				unsigned int x = current_position[0] + 1;
				while (x <= 7) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++;
				}
			}
			{
				unsigned int x = current_position[0] - 1;
				while (x >= 0) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++;
				}
			}
			{
				unsigned int y = current_position[0] + 1;
				while (y <= 7) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					y++;
				}
			}
			{
				unsigned int y = current_position[0] - 1;
				while (y >= 0) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					y--;
				}
			}
		}
		return returnpos;
	}
};

class knight : public chessmen {
public:
	knight(color color_input, position position_input, chessfigure figure_input) : chessmen(color_input, position_input, figure_input) {}

	virtual std::vector<position> possibleMoves(std::vector <chessmen*>& chessmen) override {
		std::vector<position> returnpos;
		if (current_position[0] + 2 <= 7) {
			if (current_position[1] + 1 <= 7) {
				const position pos = { current_position[0] + 2, current_position[1] + 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[1] - 1 >= 0) {
				const position pos = { current_position[0] + 2, current_position[1] - 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
		}
		if (current_position[0] - 2 >= 0) {
			if (current_position[1] + 1 <= 7) {
				const position pos = { current_position[0] - 2, current_position[1] + 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[1] - 1 >= 0) {
				const position pos = { current_position[0] - 2, current_position[1] - 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
		}
		if (current_position[1] + 2 <= 7) {
			if (current_position[0] + 1 <= 7) {
				const position pos = { current_position[0] + 1, current_position[1] + 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[0] - 1 >= 0) {
				const position pos = { current_position[0] - 1, current_position[1] + 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
		}
		if (current_position[1] - 2 >= 0) {
			if (current_position[0] + 1 <= 7) {
				const position pos = { current_position[0] + 1, current_position[1] - 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[0] - 1 >= 0) {
				const position pos = { current_position[0] - 1, current_position[1] - 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly) {
					returnpos.push_back(pos);
				}
			}
		}
		return returnpos;
	}
};

class bishop : public chessmen {
public:
	bishop(color color_input, position position_input, chessfigure figure_input) : chessmen(color_input, position_input, figure_input) {}

	virtual std::vector<position> possibleMoves(std::vector <chessmen*>& chessmen) override {
		std::vector<position> returnpos;
		{
			{
				unsigned int x = current_position[0] + 1; unsigned int y = current_position[1] + 1;
				while (x <= 7 && y <= 7) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++; y++;
				}
			}
			{
				unsigned int x = current_position[0] - 1; unsigned int y = current_position[1] + 1;
				while (x >= 0 && y <= 7) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x--; y++;
				}
			}
			{
				unsigned int x = current_position[0] + 1; unsigned int y = current_position[1] - 1;
				while (x <= 7 && y >= 0) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++; y--;
				}
			}
			{
				unsigned int x = current_position[0] - 1; unsigned int y = current_position[1] - 1;
				while (x >= 0 && y >= 0) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x--; y--;
				}
			}

		}
		return returnpos;
	}
};

class queen : public chessmen {
public:
	queen(color color_input, position position_input, chessfigure figure_input) : chessmen(color_input, position_input, figure_input) {}

	virtual std::vector<position> possibleMoves(std::vector <chessmen*>& chessmen) override {
		std::vector<position> returnpos;
		{
			{
				unsigned int x = current_position[0] + 1; unsigned int y = current_position[1] + 1;
				while (x <= 7 && y <= 7) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++; y++;
				}
			}
			{
				unsigned int x = current_position[0] - 1; unsigned int y = current_position[1] + 1;
				while (x >= 0 && y <= 7) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x--; y++;
				}
			}
			{
				unsigned int x = current_position[0] + 1; unsigned int y = current_position[1] - 1;
				while (x <= 7 && y >= 0) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++; y--;
				}
			}
			{
				unsigned int x = current_position[0] - 1; unsigned int y = current_position[1] - 1;
				while (x >= 0 && y >= 0) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x--; y--;
				}
			}
			{
				unsigned int x = current_position[0] + 1;
				while (x <= 7) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++;
				}
			}
			{
				unsigned int x = current_position[0] - 1;
				while (x >= 0) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					x++;
				}
			}
			{
				unsigned int y = current_position[0] + 1;
				while (y <= 7) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					y++;
				}
			}
			{
				unsigned int y = current_position[0] - 1;
				while (y >= 0) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) != friendly) {
						returnpos.push_back(pos);
					}
					else {
						break;
					}
					y--;
				}
			}
		}
		return returnpos;
	}
};

class king : public chessmen {
public:
	king(color color_input, position position_input, chessfigure figure_input) : chessmen(color_input, position_input, figure_input) {}

	virtual std::vector<position> possibleMoves(std::vector <chessmen*>& chessmen) override {
		std::vector<position> returnpos;
		{
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if (current_position[0] + i <= 7 && current_position[1] + j <= 7) {
						const position pos = { current_position[0] + i ,current_position[1] + j };
						if (positiocheck(chessmen, pos, player_color) != friendly) {
							returnpos.push_back(pos);
						}
					}
				}
			}
		}
		return returnpos;
	}
};