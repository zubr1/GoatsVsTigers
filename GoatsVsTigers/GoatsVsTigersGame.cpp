#include<iostream>
#include<ctime>
#include<string>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
#include <sstream> 

#include"Multiplayer.h"
#include"SinglePlayer.h"
#include"GoatsVsTigersGame.h"
#include"gameBoard.h"

#pragma warning(disable:4996)

using namespace std;

GoatsVsTigersGame::GoatsVsTigersGame() {
}

GoatsVsTigersGame::~GoatsVsTigersGame() {
}

//clear the whole screen
void GoatsVsTigersGame::clearScreen() {
	system("cls");
}

//get player name from user(s), check properity, and finally return it
string GoatsVsTigersGame::playersNames(string pawns) {
	while (true) {
		clearScreen();
		mScreen.showText(pawns+" player name (max 12 characters, without spaces):", 15, 12);

		string playerName ;
		getline(cin, playerName);


		if (playerName.length() >= 13) {
			mScreen.refreshActualTurnText("Too long player name (max 12 characters)", (FOREGROUND_RED | FOREGROUND_INTENSITY), 20, 15);
			Sleep(500);
			continue;
		}else if(playerName.length() < 3) {
			mScreen.refreshActualTurnText("Too short player name (min 3 characters)", (FOREGROUND_RED | FOREGROUND_INTENSITY), 20, 15);
			Sleep(500);
			continue;
		}
		else if(playerName.find(' ')!= string::npos || playerName.find('	') != string::npos){
			mScreen.refreshActualTurnText("Player name cannot contain spaces", (FOREGROUND_RED | FOREGROUND_INTENSITY), 20, 15);
			Sleep(500);
			continue;
		}else if (playerName=="bot") {
			mScreen.refreshActualTurnText("Player cannot be named \"bot\"", (FOREGROUND_RED | FOREGROUND_INTENSITY), 20, 15);
			Sleep(500);
			continue;
		}

		return playerName;
	}
}

//Multiplayer game. Get twice player name (for both players), and start new multiplayer game with usual data
void GoatsVsTigersGame::playerVsPlayerGame() {
	string goatsPlayerName = playersNames("goats");
	string tigersPlayerName;

	while (true) {
		tigersPlayerName = playersNames("tigers");
		if (tigersPlayerName == goatsPlayerName) {//additionally check if players have the same name
			mScreen.refreshActualTurnText("Players cannot have the same name. Please put another name", (FOREGROUND_RED | FOREGROUND_INTENSITY), 20, 15);
			Sleep(500);
			continue;
		}
		break;
	}

	Multiplayer mGame(0, 0, 20, 0, "", goatsPlayerName, tigersPlayerName, false, 0, 0);//standard data for multiplayer
	GameMode *newGame = &mGame;
	((Multiplayer*)newGame)->start();
}

//SinglePlayer game. Gives user choice of whether to begin as goats or as tigers. 
//Get player name, and start new single player with usual data.
void GoatsVsTigersGame::playerVsComputerGame() {
	int playAs = 0;//0-goats, 1-tigers
	while (true) {
		clearScreen();
		string playerVsComputerStrings[3] = { "[1] Play as goats" , "[2] Play as tigers" , "[ESC] Back" };
		mScreen.showMenu(playerVsComputerStrings, 3, 2);
		int input = _getch();
		if (input == 49) {
			playAs = 0; //goats
			break;
		}else if (input == 50) {
			playAs = 1; //tigers
			break;
		}else if (input == 27) {
			return;
		}
	}
	clearScreen();

	string playerName = playersNames("");

	SinglePlayer mGame(1, 0, playAs, 20, 0, "", playerName, playerName, false, 0, 0);//standard data for single player (computer name will be changed in SinglePlayer class)
	GameMode *newGame = &mGame;
	((SinglePlayer*)newGame)->start();
}

//Give user choice of wether to play game with computer (single player), or with second user. 
void GoatsVsTigersGame::playGame() {
	while (true) {
		clearScreen();
		string playGameStrings[3] = { "[1] Single Player" , "[2] Multiplayer" , "[ESC] Back"};
		mScreen.showMenu(playGameStrings, 3, 2);
		switch (_getch())
		{
		case 49:
			playerVsComputerGame();
			return;
			break;
		case 50:
			playerVsPlayerGame();
			return;
			break;
		case 27:
			return;
			break;
		}
	}
}

//check if read data (only integers) has permitted values
bool GoatsVsTigersGame::intsFileValidation(int mFileData[]) {
	for (int i = 0; i < 3; i++) {
		if (mFileData[i] < 0 || mFileData[i]>1)
			return false;
	}
	if ((mFileData[3] < 0 || mFileData[3] > 20) || (mFileData[4] < 0 || mFileData[4] > 4)
		&& (mFileData[5] < 0 || mFileData[5]>1) || (mFileData[6] < 0 || mFileData[6]>1)) {
		return false;
	}
	return true;
}

//players names validation
bool GoatsVsTigersGame::stringsFileValidation(string goatsPlayerName, string tigersPlayerName, int mFileData[]) {
	//checks lenght of players names, and names repetition
	if (goatsPlayerName.length() < 3 || goatsPlayerName.length() > 12 || tigersPlayerName.length() < 3 || tigersPlayerName.length() > 12
		|| goatsPlayerName == tigersPlayerName) {
		return false;
	}
	//checks if proper player has "bot" name
	if (mFileData[0] == 1) {//if it is single player game
		if ((mFileData[5] + mFileData[6]) == 0) {//if it is first round
			if (mFileData[2] == 0 && (goatsPlayerName == "bot" || tigersPlayerName != "bot") || 
				mFileData[2] == 1 && (tigersPlayerName == "bot" || goatsPlayerName != "bot")) {
				return false;
			}
		}
		else {//it is second round
			if (mFileData[2] == 0 && (goatsPlayerName != "bot" || tigersPlayerName == "bot") ||
				mFileData[2] == 1 && (tigersPlayerName != "bot" || goatsPlayerName == "bot")) {
				return false;
			}
		}
	}
	else {
		if (goatsPlayerName == "bot" || tigersPlayerName == "bot") {
			return false;
		}
	}
	return true;
}

//Load saved game. If there is no saved game, proper message will be displayed. It always loads last SAVED game.
void GoatsVsTigersGame::loadGame() {
	clearScreen();
	FILE *stream;
	//open file
	if ((stream = fopen("saved_data.txt", "r")) == NULL) {
		string playGameStrings[2] = { "There is no saved game" , "Press any key to back to the menu..."};
		mScreen.showMenu(playGameStrings, 2, 2);
		_getch();
		return;
	}
	int fileData[7]; // 0-isComputer, 1-actualTurn, 2-playerPawns, 3-goatsRemain, 4-goatsBeaten, 5-player1Points, 6-player2Points
	char mBoardState[300];
	char mGoatsPlayerName[12];
	char mTigersPlayerName[12];
	char bufferForEnter;

	//read data
	for (int i = 0; i < 7; i++) {
		int a;
		fscanf_s(stream, "%d%c", &a, &bufferForEnter);
		fileData[i] = a;
	}
	fgets(mBoardState, 300, stream);
	fscanf_s(stream, "%s", mGoatsPlayerName, _countof(mGoatsPlayerName));
	fscanf_s(stream, "%s", mTigersPlayerName, _countof(mTigersPlayerName));

	if (stream != NULL) {
		fclose(stream);
	}

	string boardState(mBoardState);
	string goatsPlayerName(mGoatsPlayerName);
	string tigersPlayerName(mTigersPlayerName);

	//Check int data compatibility. Strings data will be checked later in Fields class
	if (!intsFileValidation(fileData)) {
		string loadGameStrings[2] = { "Error while loading data..." , "Press any key to back to the menu..." };
		mScreen.showMenu(loadGameStrings, 2, 2);
		_getch();
		return;
	}

	//players names validation
	if (!stringsFileValidation(goatsPlayerName, tigersPlayerName, fileData)) {
		string loadGameStrings[2] = { "Error while loading data..." , "Press any key to back to the menu..." };
		mScreen.showMenu(loadGameStrings, 2, 2);
		_getch();
		return;
	}

	if (fileData[0] == 0) {//checks if loaded data is single or multiplayer game, and starts new instance with loaded data
		Multiplayer mGame(fileData[0], fileData[1], fileData[3], fileData[4], boardState, goatsPlayerName, tigersPlayerName, true, fileData[5], fileData[6]);
		GameMode *newGame = &mGame;
		((Multiplayer*)newGame)->start();
	}
	else {
		SinglePlayer mGame(fileData[0], fileData[1], fileData[2], fileData[3], fileData[4], boardState, goatsPlayerName, tigersPlayerName, true, fileData[5], fileData[6]);
		GameMode *newGame = &mGame;
		((SinglePlayer*)newGame)->start();
	}
}

//simply displays errror message when gamesHistory.txt file is corrupted
void GoatsVsTigersGame::showError() {
	clearScreen();
	string rankingStrings[2] = { "Error: file corrupted", "Press any key to back to the menu..." };
	mScreen.showMenu(rankingStrings, 2, 2);
	_getch();
}

//Read games history from file and then display it. If any of file component is invalid, user can't read whole history
void GoatsVsTigersGame::showGamesHistory() {
	clearScreen();

	FILE *stream;
	//open file
	if ((stream = fopen("gamesHistory.txt", "r")) == NULL) {
		string gamesHistoryStrings[2] = { "There is no scores to display.", "Press any key to back to the menu..." };
		mScreen.showMenu(gamesHistoryStrings, 2, 2);
	}
	else {
		mScreen.showText("Games:",10,5);
		mScreen.showText("Result:", 60, 5);

		int counter = 0;
		char firstPlayer[30];
		char secondPlayer[30];
		int firstPlayerValue = 0;
		int secondPlayerValue = 0;
		int flag = 0;
		char firstPlayerArr[150][30];
		char secondPlayerArr[150][30];
		int firstPlayerValueArr[150];
		int secondPlayerValueArr[150];
		//read data
		while (true) {
			if (fscanf_s(stream, "%s", firstPlayerArr[counter], _countof(firstPlayerArr[counter])) == EOF) {
				break;
			}
			
			flag = fscanf_s(stream, "%s", secondPlayerArr[counter], _countof(secondPlayerArr[counter]));
			if (flag == NULL || flag == EOF) {
				showError();
				return;
			}
			flag = fscanf_s(stream, "%d", &firstPlayerValue);
			if (flag == NULL || flag == EOF || firstPlayerValue>2 || firstPlayerValue<0) {
				showError();
				return;
			}
			flag = fscanf_s(stream, "%d", &secondPlayerValue);
			if (flag == NULL || flag == EOF || secondPlayerValue>2 || secondPlayerValue<0) {
				showError();
				return;
			}
			if (firstPlayerValue + secondPlayerValue != 2) {
				showError();
				return;
			}

			firstPlayerValueArr[counter] = firstPlayerValue;
			secondPlayerValueArr[counter] = secondPlayerValue;
			counter++;
		}


		//display last 20 games
		int oldestGame = 0;
		if (counter > 20) {
			oldestGame = counter - 20;
		}

		for (int i = counter-1; i >= oldestGame; i--) {
			mScreen.showText("", 10, counter + 7);
			string firstPlStr(firstPlayerArr[i]);
			string secondPlStr(secondPlayerArr[i]);
			string mString = firstPlStr + " vs " + secondPlStr;
			mScreen.showText(mString, 10, 7 + (counter-i));
			mString = to_string(firstPlayerValueArr[i]) + ":" + to_string(secondPlayerValueArr[i]);
			mScreen.showText(mString, 61, 7 + (counter-i));
		}

		

		if (stream != NULL) {
			fclose(stream);
		}
	}
	_getch();
}

//Read instruction from file and then display it.
void GoatsVsTigersGame::showInstruction() {
	clearScreen();

	FILE *stream;
	//open file
	if ((stream = fopen("instruction.txt", "r")) == NULL) {//if there is no file with instruction, display message with link to it
		string instructionStrings[4] = { "Can't find instruction.", "You can always visit :", "https://en.wikipedia.org/wiki/Bagh-Chal", "Press any key to back to the menu..." };
		mScreen.showMenu(instructionStrings, 4, 2);
	}
	else {
		char c;

		//read data
		while ((c = fgetc(stream)) != EOF) {
			cout << (char)c;
			if (!_kbhit()) {//Displays data gradually. If user press any key, it immediately displays whole instruction.
				Sleep(23);
			}
		}
		if (stream != NULL) {
			fclose(stream);
		}
	}
	while (_kbhit()) _getch();
	_getch();
}

//simply shows intro (gradually)
void GoatsVsTigersGame::intro() {
	int speed = 70;
	mScreen.showIntro("Michal Cendrowski Presents...", speed);
	Sleep(600);
	while (_kbhit()) _getch();//clears buffer
}

//Run main menu. It gives user choice of whether to play new game, load game, show games history, show instruction or exit game
void GoatsVsTigersGame::run() {
	intro();
	while (true) {
		clearScreen();
		string mainMenuStrings[5] = { "[1] New game" , "[2] Load last saved game", "[3] Games history" , "[4] Instruction" , "[ESC] Exit" };
		mScreen.showMenu(mainMenuStrings, 5, 2);

		while (_kbhit()) _getch();
		switch (_getch()){
		case 49:
			playGame();
			break;
		case 50:
			loadGame();
			break;
		case 51:
			showGamesHistory();
			break;
		case 52:
			showInstruction();
			break;
		case 27:
			return;
			break;
		}
	}
	return;
}