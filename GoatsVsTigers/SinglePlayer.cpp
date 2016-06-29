#include "SinglePlayer.h"
#include "gameBoard.h"
#include "Fields.h"
#include<iostream>
#include<string>
#include<conio.h>
#include <sstream>
#include <ctime>


#pragma warning(disable:4996)

#define RED	(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define WHITE 	(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)

#define centerX 32
#define centerY 9

using namespace std;


SinglePlayer::SinglePlayer() {
	goatsRemain = 20;
	goatsBeaten = 0;
	exitGame = false;
	actualTurn = 0;
	gameLoaded = false;
	isComputer = 1;
	player1Points = 0;
	player2Points = 0;
}

SinglePlayer::SinglePlayer(int playAs) { // 0 - goats, 1 - tigers
	playerPawns = playAs;
	goatsRemain = 20;
	goatsBeaten = 0;
	exitGame = false;
	actualTurn = 0;
	gameLoaded = false;
	isComputer = 1;
	player1Points = 0;
	player2Points = 0;
}

SinglePlayer::SinglePlayer(int isComputer, int actualTurn, int playerPawns, int goatsRemain, int goatsBeaten, string boardState, string goatsPlayerName, string tigersPlayerName, bool gameLoaded, int player1Points, int player2Points) {
	this->isComputer = isComputer;
	this->actualTurn = actualTurn;
	this->playerPawns = playerPawns;
	this->goatsRemain = goatsRemain;
	this->goatsBeaten = goatsBeaten;
	this->boardState = boardState;
	this->goatsPlayerName = goatsPlayerName;
	this->tigersPlayerName = tigersPlayerName;
	this->gameLoaded = gameLoaded;
	this->player1Points = player1Points;
	this->player2Points = player2Points;
	exitGame = false;
	gameLoaded = true;
}


SinglePlayer::~SinglePlayer() {
}

//gets input from player, and return validate coords
string SinglePlayer::inputValidation(string message, int pawnType, int operationType) {//operationType: 0 - from, 1 - to
	string mCoord = "";

	bool invalidInput = true;
	do {
		mScreen.clearInputText();
		mScreen.showText(message, WHITE);

		if (playerPawns==pawnType) {//if not computer turn
		//if(false){
			int temp;
			temp = _getche();

			//checks if user want to exit game
			if (temp == 27) {
				exitGame = true;
				return "";
			}

			if (temp != 97 && temp != 98 && temp != 99 && temp != 100 && temp != 101) { //if not a,b,c,d or e
				mScreen.clearInputText();
				mScreen.showText(message, WHITE, "invalid input", RED);
				if (playerPawns == pawnType) {
					Sleep(500);
					while (_kbhit()) _getch();//clear buffer
				}
				continue;
			}
			mCoord += temp;
			temp = NULL;
			temp = _getche();

			//checks if user want to exit game
			if (temp == 27) {
				exitGame = true;
				return "";
			}

			if (temp != 49 && temp != 50 && temp != 51 && temp != 52 && temp != 53) {//if not 1,2,3,4 or 5
				mScreen.clearInputText();
				mScreen.showText(message, WHITE, "invalid input", RED);
				if (playerPawns == pawnType) {
					Sleep(500);
					while (_kbhit()) _getch();//clear buffer
				}
				continue;
			}
			else {
				mCoord += temp;
				invalidInput = false;
			}
		}
		else {//computer turn, returns proper random coord
			mCoord = mBoard.calculateBestOption(pawnType, goatsRemain, operationType);//returns best move for pc player
			invalidInput = false;
		}
	} while (invalidInput);
	mScreen.clearInputText();
	return mCoord;
}

bool SinglePlayer::goatsTurn() {
	ostringstream ss;
	int goatPawns = 0;
	ss << goatsRemain;
	mScreen.refreshActualTurnText("", WHITE, 3, 18);//clear tigers text
	string turnMessage = "Turn: goats (" + ss.str() + ") left";
	mScreen.refreshActualTurnText(turnMessage, RED, 3, 19);
	if (goatsRemain > 0) { //first type of goats movement (player just have to place a goat on an empty field)
		while (true) {
			string coord;
			coord = inputValidation("Write the coordinates to place goat (eg. a2): ", goatPawns, 1);
			if (exitGame) {
				return false;
			}
			if (mBoard.placeGoat(coord)) {
				goatsRemain--;
				break;
			}
			else { //illegal move
				mScreen.clearInputText();
				mScreen.showText("Write the coordinates to place goat (eg. a2): ", WHITE, "illegal move", RED);
				if (playerPawns == goatPawns) {
					Sleep(500);
					while (_kbhit()) _getch();//clear buffer
				}
				continue;
			}
		}
	}
	else {//Second type of goats movement (after finishing placing all goat pawns). Now goats moves like tigers exclude beating.
		  //first check if goats are blocked
		if (mBoard.areAllGoatsBlocked()) {
			return true;
		}
		
		string coordinateFrom, coordinateTo;

		//goat from
		while (true) {
			coordinateFrom = inputValidation("Move goat from (eg. a1): ", goatPawns, 0);
			if (exitGame) {
				return false;
			}
			if (mBoard.moveGoatFrom(coordinateFrom)) {
				break;
			}
			else {
				mScreen.clearInputText();
				mScreen.showText("Move goat from (eg. a1): ", WHITE, "illegal move", RED);
				if (playerPawns == goatPawns) {
					Sleep(500);
					while (_kbhit()) _getch();//clear buffer
				}
				continue;
			}
		}

		//goat to
		while (true) {
			coordinateTo = inputValidation("Move goat to (eg. b2): ", goatPawns, 1);
			if (exitGame) {
				return false;
			}
			bool properMove = mBoard.moveGoatTo(coordinateFrom, coordinateTo);
			if (properMove) {
				break;
			}
			else {
				mScreen.clearInputText();
				mScreen.showText("Move goat to (eg. b2): ", WHITE, "illegal move", RED);
				if (playerPawns == goatPawns) {
					Sleep(500);
					while (_kbhit()) _getch();//clear buffer
				}
				continue;
			}
		}

	}

	//check if game has ended
	if (mBoard.areAllTigersBlocked()) {
		return true;
	}
	else {
		return false;
	}
}

bool SinglePlayer::tigersTurn() {
	string coordinateFrom, coordinateTo;
	int tigerPawns = 1;

	ostringstream ss;
	ss << goatsBeaten;
	string turnMessage = "(goats beaten:" + ss.str() + ")";
	mScreen.refreshActualTurnText("Turn: tigers", RED, 3, 18);
	mScreen.refreshActualTurnText(turnMessage, RED, 3, 19);
	
	//tiger from
	while (true) {
		coordinateFrom = inputValidation("Move tiger from (eg. a1): ", tigerPawns, 0);
		if (exitGame) {
			return false;
		}
		bool properMove = mBoard.moveTigerFrom(coordinateFrom);
		if (properMove) {
			break;
		}
		else {
			mScreen.clearInputText();
			mScreen.showText("Move tiger from (eg. a1): ", WHITE, "illegal move", RED);
			if (playerPawns == tigerPawns) {
				Sleep(500);
				while (_kbhit()) _getch();//clear buffer
			}
			continue;
		}
	}

	//tiger to
	while (true) {
		coordinateTo = inputValidation("Move tiger to (eg. a2): ", tigerPawns, 1);
		if (exitGame) {
			return false;
		}
		bool properMove = mBoard.moveTigerTo(coordinateFrom, coordinateTo, &goatsBeaten);
		if (properMove) {
			break;
		}
		else {
			mScreen.clearInputText();
			mScreen.showText("Move tiger to (eg. a2): ", WHITE, "illegal move", RED);
			if (playerPawns == tigerPawns) {
				Sleep(500);
				while (_kbhit()) _getch();//clear buffer
			}
			continue;
		}
	}

	//check if game has ended
	if (goatsBeaten >= 5) {
		return true;
	}
	else {
		return false;
	}
}

//Start new part (first or second) of whole game. 
int SinglePlayer::playNewPart() {
	clearScreen();
	//initialize board and key
	mBoard.initializePanws();
	if (gameLoaded) {
		bool correctData = mBoard.setBoardState(boardState);
		if (correctData == false) {
			return -1;
		}
	}

	mScreen.showBoard();
	if (player1Points + player2Points == 0) {
		mScreen.showKey(goatsPlayerName, tigersPlayerName, (player1Points + player2Points + 1));
	}
	else {
		mScreen.showKey(tigersPlayerName, goatsPlayerName, (player1Points + player2Points + 1));
	}	mBoard.updateBoard();

	bool endGame = false;
	//main game loop
	while (true) {
		if (actualTurn == 0) {//goats turn
			endGame = goatsTurn();
		}
		else {
			endGame = tigersTurn();
		}

		if (endGame) {
			break;
		}
		if (exitGame) {
			doYouWantToSave(isComputer, actualTurn, playerPawns, goatsRemain, goatsBeaten, mScreen, goatsPlayerName, tigersPlayerName, mBoard, player1Points, player2Points);
			return 0;
		}

		if (actualTurn == 0) {
			actualTurn = 1;
		}
		else {
			actualTurn = 0;
		}
	}

	//add point to a winner
	if (goatsBeaten>=5 || mBoard.areAllGoatsBlocked()) {
		if (player1Points + player2Points == 1) {
			player1Points++;
		}
		else {
			player2Points++;
		}
	}
	else {
		if (player1Points + player2Points == 1) {
			player2Points++;
		}
		else {
			player1Points++;
		}
	}

	return (player1Points + player2Points);
}

//makes default values again and spawn pawns
void SinglePlayer::prepareNextRound() {
	clearScreen();
	mScreen.showText("Round number 2 (players swaps pawns)", 25, 10);
	Sleep(3000);
	while (_kbhit()) _getch();//clear buffer
	//change values as default
	playerPawns = !(playerPawns);
	goatsRemain = 20;
	goatsBeaten = 0;
	actualTurn = 0;
	gameLoaded = false;
}

//starts new game
void SinglePlayer::start() {
	if (!gameLoaded) {
		//change one name to a "bot" name
		if (playerPawns == 0) {
			if (player1Points + player2Points == 0) {
				tigersPlayerName = "bot";
			}
			else {
				goatsPlayerName = "bot";
			}
		}
		else {
			if (player1Points + player2Points == 0) {
				goatsPlayerName = "bot";
			}
			else {
				tigersPlayerName = "bot";
			}
		}
	}
	while (true) {
		int returnedValue = playNewPart();
		if (returnedValue == -1) {
			clearScreen();
			string loadGameStrings[2] = { "Error while loading data..." , "Press any key to back to the menu..." };
			mScreen.showMenu(loadGameStrings, 2, 2);
			_getch();
			return;
		}else if(returnedValue == 2){
			clearScreen();
			string winnerStrings[2];
			//show win, draw or lose message
			if (playerPawns == 0) {//player played goats
				if (player1Points > player2Points) {//player lost
					winnerStrings[0] = "You lost...";
				}
				else if (player1Points == player2Points) {//draw
					winnerStrings[0] = "It's a draw!";
				}
				else {//player won
					winnerStrings[0] = "Congrats, You won!";
				}
			}else {//player played tigers
				if (player1Points > player2Points) {//player won
					winnerStrings[0] = "Congrats, You won!";
				}
				else if (player1Points == player2Points) {//draw
					winnerStrings[0] = "It's a draw!";
				}
				else {//player lost
					winnerStrings[0] = "You lost...";
				}
			}
			saveGameHistory(goatsPlayerName, tigersPlayerName, player1Points, player2Points);
			winnerStrings[1] = "Press any key to continue...";
			mScreen.showMenu(winnerStrings, 2, 2);
			_getch();
			return;
		}
		else if (returnedValue == 0) {
			return;
		}
		prepareNextRound();
	}
}





