#pragma once
#include<iostream>
#include <string>
#include<conio.h>
#include"gameBoard.h"
#include"Fields.h"

class GameMode
{

private:
	gameBoard mScreen;
protected:
	int xInput;
	int yInput;
	int goatsRemain;
	int goatsBeaten;
	bool exitGame;
	string boardState;
	bool gameLoaded;
	int isComputer; // 0 no, 1 yes

	Fields mBoard;
	int playerPawns;// 0 - goats, 1 - tigers
	int actualTurn;//0- goats, 1 - tigers

	string goatsPlayerName;
	string tigersPlayerName;
	int player1Points;
	int player2Points;

	void saveGameHistory(string player1Name, string player2Name, int player1Score, int player2Score);
	void clearScreen();
	virtual bool tigersTurn() = 0;
	virtual bool goatsTurn() = 0;
	virtual string inputValidation(string message, int pawnType, int operationType) = 0;
	void doYouWantToSave(int isComputer, int actualTurn, int playerPawns, int goatsRemain, int goatsBeaten, gameBoard mScreen, string goatsPlayerName, string tigersPlayerName, Fields mBoard, int player1Points, int player2Points);
	void saveProgress(int isComputer, int actualTurn, int playerPawns, int goatsRemain, int goatsBeaten, gameBoard mScreen, string goatsPlayerName, string tigersPlayerName, Fields mBoard, int player1Points, int player2Points);
public:
	GameMode();
	~GameMode();
	virtual void start() = 0;

};

