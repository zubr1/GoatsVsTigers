#include "GameMode.h"
#include "gameBoard.h"
#include<iostream>
#include<string>
#include<conio.h>
#include <sstream>
#include <ctime>

#pragma warning(disable:4996)

#define RED	(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define WHITE 	(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)

using namespace std;

GameMode::GameMode(){
}

GameMode::~GameMode(){
}

//clear the whole screen
void GameMode::clearScreen() {
	system("cls");
}

//Save game result to a existing or not existing file (if file doesn't exist, create new one).
//Game result can be readed later in games history
void GameMode::saveGameHistory(string player1Name, string player2Name, int player1Score, int player2Score) {
	//open file
	FILE *stream;
	if ((stream = fopen("gamesHistory.txt", "a")) == NULL) {
		clearScreen();
		string gameHistoryStrings[1] = { "Can't save score."};
		mScreen.showMenu(gameHistoryStrings, 1, 2);
		Sleep(500);
		while (_kbhit()) _getch();//clears buffer
		return;
	}
	else {
		//save data
		fprintf(stream, "%s ", player1Name.c_str());
		fprintf(stream, "%s ", player2Name.c_str());
		fprintf(stream, "%d ", player1Score);
		fprintf(stream, "%d\n", player2Score);

		if (stream != NULL) {
			fclose(stream);
		}
	}
}

//Save current game. After execution of this method, saved game can be loaded via main menu.
void GameMode::saveProgress(int isComputer, int actualTurn, int playerPawns, int goatsRemain, int goatsBeaten, gameBoard mScreen, string goatsPlayerName, string tigersPlayerName, Fields mBoard, int player1Points, int player2Points) {
	FILE *stream;
	//open file
	if ((stream = fopen("saved_data.txt", "w")) == NULL) {
		clearScreen();
		string loadGameStrings[1] = { "Error while saving data..."};
		mScreen.showMenu(loadGameStrings, 1, 2);
		Sleep(500);
		while (_kbhit()) _getch();//clears buffer
		return;
	}
	else {
		//save data
		fprintf(stream, "%d\n", isComputer);
		fprintf(stream, "%d\n", actualTurn);
		fprintf(stream, "%d\n", playerPawns);
		fprintf(stream, "%d\n", goatsRemain);
		fprintf(stream, "%d\n", goatsBeaten);
		fprintf(stream, "%d\n", player1Points);
		fprintf(stream, "%d\n", player2Points);
		fprintf(stream, "%s\n", mBoard.getBoardState().c_str());
		fprintf(stream, "%s\n", goatsPlayerName.c_str());
		fprintf(stream, "%s\n", tigersPlayerName.c_str());

		if (stream != NULL) {
			fclose(stream);
		}
	}
}

//Ask user if he want to save current game. Keys: y-yes, n-no
void GameMode::doYouWantToSave(int isComputer, int actualTurn, int playerPawns, int goatsRemain, int goatsBeaten, gameBoard mScreen, string goatsPlayerName, string tigersPlayerName, Fields mBoard, int player1Points, int player2Points) {
	mScreen.showText("Do You want to save Your progress? (y/n)", 24, 13);
	while (true) {
		clearScreen();
		mScreen.showText("Do You want to save Your progress? (y/n)", 24, 13);

		switch (_getch()) {
		case 121://y
			saveProgress(isComputer, actualTurn, playerPawns, goatsRemain, goatsBeaten, mScreen, goatsPlayerName, tigersPlayerName, mBoard, player1Points, player2Points);
			break;
		case 110://n
			break;
		case 27://esc
			break;
		default:
			continue;
			break;
		}
		return;
	}
}
