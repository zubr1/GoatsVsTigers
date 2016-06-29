#pragma once
#include "GameMode.h"
#include "gameBoard.h"
#include "Fields.h"


#include<iostream>
#include <string>
#include<conio.h>

using namespace std;


class Multiplayer :
	public GameMode
{
private:
	int xInput;
	int yInput;
	int goatsRemain;
	int goatsBeaten;
	bool exitGame;
	string boardState;
	bool gameLoaded;
	int isComputer; // 0 no, 1 yes

	gameBoard mScreen;
	Fields mBoard;
	int playerPawns;// 0 - goats, 1 - tigers
	int actualTurn;//0- goats, 1 - tigers

	string goatsPlayerName;
	string tigersPlayerName;
	int player1Points;
	int player2Points;

	bool goatsTurn();
	bool tigersTurn();

	string inputValidation(string message, int pawnType, int operationType);
	int playNewPart();
	void prepareNextRound();

public:
	Multiplayer();
	//Multiplayer(int playAs);
	Multiplayer(int isComputer, int actualTurn, int goatsRemain, int goatsBeaten, string boardState, string goatsPlayerName, string tigersPlayerName, bool gameLoaded, int player1Points, int player2Points);
	~Multiplayer();


	void start();
};

