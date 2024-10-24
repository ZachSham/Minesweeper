#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <random>
#include <mutex>
#include <fstream>
#include <sstream>
#include "windows.h"
#include "board.h"
#include "MovingCursor.h"

using namespace std;
const int height = 20;
const int width = 20;


int main() {
	string username, difficulty;
	int percentage = 10; //base percentage

	cout << "What is your player name? " << endl;
	getline(cin, username);
	cout << "Type in a difficulty, easy, medium,or hard (non-case sensitive): " << endl;
	getline(cin, difficulty);

	for (int i = 0; i < difficulty.size(); i++) {
		
		difficulty.at(i) = tolower(difficulty.at(i));
	}
	if (difficulty == "medium") { //if difficulty selected was easy percentage stays the same
		percentage = 20;
	}
	else if (difficulty == "hard") {
		percentage = 30;
	}

	ofstream playerData("PlayerData.txt", ios::app);
	if (playerData) {
		playerData << username << " played on " << difficulty << " and uncovered: "; // saves players name into file
		playerData.close();
	}
	else {
		cout << "File did not open!";
	}

	bool game_running = true;
	int time = 30; //auto_update_interval
	int finalScore;
	board minesweeper(height, width, percentage); //defined as consts
	minesweeper.setBombs(); // sets the bombs based on percentage and width/height
	minesweeper.count(); // Counts adjacents for all cells/tiles
	system("cls");
	thread thread_obj(&board::c_update, &minesweeper, time); //thread for cursor
	//update
	do {
		system("pause>nul"); //wait for key
		if (GetAsyncKeyState(VK_DOWN)) {
			minesweeper.move_down();
			//minesweeper.Draw(false);
		}
		else if (GetAsyncKeyState(VK_UP)) {
			minesweeper.move_up();
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			minesweeper.move_right();
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			minesweeper.move_left();
		}
		else if (GetAsyncKeyState(VK_SPACE)) {

			game_running = minesweeper.shoot();
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			game_running = false;
			//....TODO : Add case for pressing letter M for

		}
		else if (GetAsyncKeyState('M')) {
			minesweeper.flagDisplay();
		}
	} while (game_running);
	//thread_obj.detach(); //stops cursor updates
	system("cls");
	

	minesweeper.Draw(true);
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
