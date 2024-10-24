#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <random>
#include <mutex>
#include "windows.h"
#include "MovingCursor.h"
#include "cell.h"
#include <fstream>
using namespace std;
class board {
private:
	mutex mtx;
	void ShowConsoleCursor(bool showFlag)
	{
		//-------do not change-------
		//hides the cursor, use only once at program start
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = showFlag; // set the cursor visibility
		SetConsoleCursorInfo(out, &cursorInfo);
	}
	BOOL gotoxy(const WORD x, const WORD y) {
		//-------do not change-------
		COORD xy;
		xy.X = x;
		xy.Y = y;
		return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
	}
	int height;
	int width;
	int totalCells;
	double highPercentage;
	int bombMax;
	int bombMin;
	bool gameWon;
	vector<cell> map;
	int bombCount;
	
public:
	board(int x, int y, int maxPercent) {
		height = y;
		width = x;
		bombCount = 0;
		totalCells = height * width;
		map.resize(totalCells);
		highPercentage = maxPercent / 100.0;
		bombMax = totalCells * (highPercentage);
		bombMin = totalCells * (highPercentage * .333);
		ShowConsoleCursor(false);
	}
	void setWidth(int w) {
		width = w;
	}
	void setHeight(int l) {
		height = l;
	}
	void setMaxDensity(int density) {
		highPercentage = density / 100;
		int max = width * highPercentage;
	}
	void setBombs() {
		random_device rd;
		default_random_engine generator(rd());
		uniform_int_distribution <int> dist(bombMin, bombMax);
		bombCount = dist(generator);
		for (int i = 0; i < bombCount; i++) {
			uniform_int_distribution <int> dist(0, totalCells - 1);
			int placement = dist(generator);
			map.at(placement).setBombTrue();
		}

	}
	//use mutex to make sure commands arent running at the same time
	void Draw(bool gameOver) {

		mtx.lock();
		static bool flag = false;
		gotoxy(0, 0);
		int score = revealedCount(); // how many they revealed
		int static comp;

		for (int i = 0; i < map.size(); i++) {

			if (gameOver) {
				map.at(i).show();
			}
			cout << map.at(i).getCharacter(i) << ' ';
			if (((i + 1) % width) == 0) {
				cout << endl;
			}
		}

		if (score != comp) {
			cout << endl << "Tiles found: " << score << endl; // scoreboard
		}

		ofstream playerData("PlayerData.txt", ios::app);

		
			if (score + bombCount == totalCells && !flag) { // if they revealed all non-bomb tiles they won
				cout << "You won!";
				ofstream playerData("PlayerData.txt", ios::app);
				if (playerData) {
					playerData << "Every single tile!" << endl;
					playerData.close();
					flag = true;
				}
				else {
					cout << "Filerror!";
				}
				

			}
			else if (gameOver && !flag) { //if game is over and all non bomb tiles werent revealed they lost
				cout << "You lost!";

				ofstream playerData("PlayerData.txt", ios::app);
				if (playerData) {
					playerData << score << endl;
					playerData.close();
					flag = true;
				}
				else {
					cout << "Fileerror";
				}
			}

		comp = score;

		mtx.unlock();
	}
	void move_down() {

		if ((map.at(0).getCLocation() < map.size() - width)) {

			map.at(0).setCLocation(map.at(0).getCLocation() + width);
		}
	}
	void move_up() {
		if ((map.at(0).getCLocation() >= width)) {
			map.at(0).setCLocation(map.at(0).getCLocation() - width);
		}
	}
	void move_right() {
		if ((map.at(0).getCLocation() < map.size() - 1)) {
			map.at(0).setCLocation(map.at(0).getCLocation() + 1);
		}
	}
	void move_left() {
		if ((map.at(0).getCLocation() > 0)) {
			map.at(0).setCLocation(map.at(0).getCLocation() - 1);
		}
	}
	void c_update(int time) {
		while (true) {
			this_thread::sleep_for(chrono::milliseconds(time));
			Draw(false);
		}
	}

	 bool shoot() {
		bool gameRunning = true;
		bool flag = false;
		vector <int> adjacent = { -1, +1, -width, -width - 1, -width + 1, +width, +width - 1, +width + 1 };
		vector <int> locations;
		int cursorLoc = map.at(0).getCLocation();
		int score = revealedCount();
		if (map.at(cursorLoc).isBomb() || score + 1 + bombCount == totalCells) {

			map.at(cursorLoc).revealCell();
			gameRunning = false; // if they revealed all tiles or hit a bomb game ends
		}
		else {

			if (!map.at(cursorLoc).getRevealStatus()) {
				map.at(cursorLoc).revealCell(); //mark the square

				if (map.at(cursorLoc).getBombCount() == map.at(0).getNoAdjacentsChar()) {

					for (int j = 0; j < adjacent.size(); j++) {
						if (cursorLoc + adjacent.at(j) >= 0 && cursorLoc + adjacent.at(j) < map.size()) {

							if (!map.at(cursorLoc + adjacent.at(j)).getRevealStatus()) {
								map.at(cursorLoc + adjacent.at(j)).revealCell();
								if (map.at(cursorLoc + adjacent.at(j)).getBombCount() == map.at(0).getNoAdjacentsChar()) {
									locations.push_back(cursorLoc + adjacent.at(j));
								}
							}
						}
					}


					int index = 0;
					do {
						if (map.at(locations.at(index)).getBombCount() == map.at(0).getNoAdjacentsChar()) {

							for (int i = 0; i < adjacent.size(); i++) {

								if (locations.at(index) + adjacent.at(i) >= 0 && locations.at(index) + adjacent.at(i) < map.size()) {
									if (!map.at(locations.at(index) + adjacent.at(i)).getRevealStatus()) {

										map.at(locations.at(index) + adjacent.at(i)).revealCell();

										if (map.at(locations.at(index) + adjacent.at(i)).getBombCount() == map.at(0).getNoAdjacentsChar()) {
											locations.push_back(locations.at(index) + adjacent.at(i));
										}

									}
								}
							}
							locations.erase(locations.begin());
						}
						else {
							map.at(locations.at(index)).revealCell();
							locations.erase(locations.begin());
						}
						

					} while ( locations.size() != 0) ;

				}
			}
		}
		

		return gameRunning;
	}
	

	void flagDisplay() {
		map.at(map.at(0).getCLocation()).setFlagged();
	}

	int revealedCount(){
		int count = 0;
		for (int i = 0; i < map.size(); i++) {
			if (!map.at(i).isBomb()) {
				count += map.at(i).getRevealStatus();
			}
		}
		return count;
	}

	void count() {
		vector <int> adjacent = { -1, +1, -width, -width - 1, -width + 1, +width, +width - 1, +width + 1 };
		int t_adjacentBombs = 0;

		for (int index = 0; index < map.size(); index++) {
			for (int i = 0; i < adjacent.size(); i++) {
				if (index + adjacent.at(i) < totalCells && index + adjacent.at(i) > 0) {
					t_adjacentBombs += map.at(index + adjacent.at(i)).isBomb();
				}
			}
			map.at(index).setBombCount('0' + t_adjacentBombs);
			t_adjacentBombs = 0;
		}
	}
};
#endif // !BOARD_H
