#ifndef CELLS_H
#define CELLS_H
// MINESWEEPER.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "MovingCursor.h"
#include "Windows.h"
using namespace std;
class cell : public MovingCursor {
private:
	const char HIDDEN_CHAR = '-';
	const char EMPTY_CHAR = ' ';
	const char ZERO_BOMBS_CHAR = '0';
	const char BOMB_CHAR = '*';
	const char FLAG_CHAR = 'M';
	char adj_bombs_count = '0';
	bool bomb = false;
	bool revealed = false;
	char cell_character = HIDDEN_CHAR;
	bool marker = false;
public:
	void setBombCount(int c) {
		if (!bomb) {
			adj_bombs_count = static_cast<char>(c);
		}
	}
	char getBombCount() const {
		return adj_bombs_count;
	}
	char getNoAdjacentsChar()const {
		return ZERO_BOMBS_CHAR;
	}
	void setBombTrue() {
		bomb = true;

	}
	void show() {   //for end of game to show the character, without actually revealing cell because that messes up score 
		if (bomb) {
			cell_character = BOMB_CHAR;
		}
		else {
			cell_character = adj_bombs_count;
		}
	}
	
	bool isBomb() const {
		return bomb;
	}
	void setFlagged() { //allows you flag and unflag cells, also doesn't allow flagging on revealed cells
		
		if (cell_character == FLAG_CHAR) {
			if (revealed) {
				cell_character = adj_bombs_count;
			}
			else {
				cell_character = HIDDEN_CHAR;
			}
		}
		else {
			if (!revealed) {
				cell_character = FLAG_CHAR;
			}
		}
	}
	void setCharacter(char c) {
		cell_character = c;
	}
	bool getRevealStatus() const {
		return revealed;
	}
	bool revealCell() {
		revealed = true;
		if (bomb) {
			cell_character = BOMB_CHAR;
		}
		else {
			cell_character = adj_bombs_count;
		}
		return bomb;
	}
	char getCharacter(int location) {

		char c = cell_character;

		if (location == CURSOR_LOCATION) {
			c = showCursor();
		}
		else if(revealed && adj_bombs_count == ZERO_BOMBS_CHAR)
			{
			c = EMPTY_CHAR;
		}
		return c;
	}
};
#endif //CELLS_H

