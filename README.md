# Minesweeper

This project is a console-based Minesweeper game implemented in C++. It is designed to offer a full minesweeper-playing experience. I implemented this as part of a project for my Object-Oriented Programming class in my freshman year

## Features
Standard Rules: Implements all standard Minesweeper rules.

Flags: Allows you to mark down where you think potential bombs are.

Game Status: Show how many tiles have been revealed as game continues. 

PlayerData File: Saves player name and how many tiles they revealed, or if they (won) in a file called "PlayerData.txt".

Difficulty Selection: Player can select easy, medium , or hard, and based on that a diffrenet percentage of the map will be filled with bombs. 

## The project is implemented using
Object-Oriented Programming (OOP): Classes for piece cells, board, and cursor. Chose to implement it where each cell on the board can take attribute of being either a cursor, bomb, number adjacent to bomb, or empty. I called it cells for the class, but they should have been called tiles instead. 

Multithreading: Utilized for the cursor to make it change shape so that it has the effect of looking like it is moving.

Random Device Generator: To randomly select how many bombs will be on map based on a percentage of a map. Also to randomly select positions on the map that will be planted with bombs. 

## Future Goals
~ Loading Screen.

~ Make a more visually pleasing game with colors and actual tiles.

~ Add sound and graphic effects for bombs when bombs are hit. 

## Contact
Feel free to reach out if you have questions or suggestions!

Zach Shamieh zachshamieh@gmail.com
