
#include<iostream>
#include<string>
#include"gameBoard.h"
#pragma once

using namespace std;


class Fields
{
	struct fields {
		string name;
		int xCoord;
		int yCoord;
		string fieldState; //tiger, goat or empty
		string connections; //eg. for a1 field connections would be like: "a2 b1 b2"
	};

private:

	fields mFields[5][5];
	gameBoard mGameBoard;

	bool isGoatBlocked(fields mField);


public:
	Fields() {};
	~Fields() {};

	void initializePanws();
	bool isBlocked(fields currentField);
	bool placeGoat(string field);
	bool moveTigerFrom(string from);
	bool moveTigerTo(string from, string to, int *goatsBeaten);
	bool moveGoatFrom(string from);
	bool moveGoatTo(string from, string to);
	bool areAllTigersBlocked();
	bool areAllGoatsBlocked();
	string calculateBestOption(int pawnType, int goatsRemain, int operationType);
	string getBoardState();
	bool setBoardState(string boardState);
	void updateBoard();
	void clearAllPawns();
};

