#pragma once
#include<Windows.h>
#include<iostream>
#include<string>

using namespace std;


class gameBoard
{
private:
	//coords for messages below game board area
	const int xInput;
	const int yInput;

	//coords for top-left game board corner
	const int xBoard;
	const int yBoard;

	//width and height of game board
	const int width;
	const int height;

	void defaultColor();
	void player1Color();
	void player2Color();

public:
	gameBoard() : xInput(3), yInput(28), xBoard(29), yBoard(3), width(40), height(20) {};
	~gameBoard() {};
	void showBoard();
	void showKey(string goatsPlayerName, string tigersPlayerName, int part);

	int getXInput();
	int getYInput();
	int getXBoard();
	int getYBoard();
	int getWidth();
	int getHeight();

	void showIntro(string introText, int introSpeed);
	void showMenu(string menuStrings[], int arrayLength, int gap);
	void refreshActualTurnText(string mText, int color, short x, short y);
	void showText(string mText, int x, int y);
	void showText(string mText, int color);
	void showText(string mText);
	void showText(string mFirstText, int color, string mSecondText, int secondColor);
	void clearInputText();

	void drawGoat(int x, int y);
	void drawTiger(int x, int y);
	void DrawRect(int x, int y, int width, int height, int curPosX, int curPosY);
	void clearPawn(int x, int y);
};

