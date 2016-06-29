#include "gameBoard.h"
#include<Windows.h>
#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
#include <algorithm>
#include <conio.h>

#define WHITE 	(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define YELLOW  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CYAN	(FOREGROUND_BLUE | FOREGROUND_GREEN)
#define MAGNETA	(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define BLUE	(FOREGROUND_BLUE | CYAN | FOREGROUND_INTENSITY)
#define RED	(FOREGROUND_RED | FOREGROUND_INTENSITY)

//the center of screen
#define centerX 32
#define centerY 9

using namespace std;

//template which sets coursor to the specified position
template <class T1, class T2>
BOOL setxy(T1 x, T2 y)
{
	COORD c = { x,y };
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void gameBoard::player1Color() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
}

void gameBoard::player2Color() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
}

void gameBoard::defaultColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}

//Show text intro gradually
void gameBoard::showIntro(string introText, int introSpeed) {
	setxy(centerX, centerY);

	for (int i = 0; i < introText.length(); i++) {
		if (i == 17) {
			setxy(centerX+2, centerY+1);
		}
		cout << introText[i];
		if (!_kbhit()) {//Displays data gradually. If user press any key, it immediately displays whole instruction.
			Sleep(introSpeed);
		}
	}
}

//simply displays text packed into an array
void gameBoard::showMenu(string menuStrings[], int arrayLength, int gap) {
	for (int i = 0; i < arrayLength; i++) {
		setxy(centerX, centerY + i*gap);
		cout << menuStrings[i];
	}
}

//clears text and displays the new one
void gameBoard::refreshActualTurnText(string mText, int color, short x, short y) {
	//clear previous text
	setxy(x, y);
	for (int i = 0; i < 23; i++) {
		cout << " ";
	}
	setxy(x, y);

	//display new text
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	cout << mText;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}

//Simply displays provided text
void gameBoard::showText(string mText, int x, int y) {
	cout.clear();
	cout.flush();
	setxy(x, y);
	cout << mText;
}

//Simply displays provided text colored with specified color. Text is displayed below game board area
void gameBoard::showText(string mText, int color) {
	setxy(xInput, yInput);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	cout << mText;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}

//Simply displays provided 2 strings colored with specified colors. Text is displayed below game board area
void gameBoard::showText(string mFirstText, int color, string mSecondText, int secondColor) {
	setxy(xInput, yInput);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	cout << mFirstText;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), secondColor);
	cout <<" "<< mSecondText;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}

//Simply displays provided strings below game board area
void gameBoard::showText(string mText) {
	setxy(xInput, yInput);
	cout << mText;
}

//Clear text below game board area up to 80 characters
void gameBoard::clearInputText() {
	setxy(xInput, yInput);
	for (int i = 0; i < 80; i++) {
		cout << " ";
	}
	setxy(xInput, yInput);//set coursor to the "begin" position
}


//Draws whole board frame
void gameBoard::DrawRect(int x, int y, int width, int height, int curPosX, int curPosY)
{
	int slashCode = 47;
	int backslashCode = 92;
	int heightWallCode = 186; 
	int widthWallCode = 205;
	int westWallCode = 185;
	int southWallCode = 202;
	int northWallCode = 203;
	int eastWallCode = 204;
	int crossCode = 206;

	//top of board
	setxy(x, y); 
	cout << char(201);
	for (int i = 1; i < width; i++) {
		if (i == width/4 || i==width/2 || i==width*3/4) {
			cout << char(northWallCode);
		}else{
			cout << char(widthWallCode);
		}
	}
	cout << char(187);

	//bottom of board
	setxy(x, height + y); 
	cout << char(200);
	for (int i = 1; i < width; i++) {
		if (i == width / 4 || i == width / 2 || i == width * 3 / 4) {
			cout << char(southWallCode);
		}
		else {
			cout << char(widthWallCode);
		}
	}
	
	//east and west sides of board
	cout << char(188);
	for (int i = y + 1; i < height + y; i++)
	{

		setxy(x, i); cout << char(heightWallCode);
		setxy(x + width/4, i); cout << char(heightWallCode);
		setxy(x + width/2, i); cout << char(heightWallCode);
		setxy(x + width*3/4, i); cout << char(heightWallCode);
		setxy(x + width, i); cout << char(heightWallCode);
	}

	//grid
	for (int i = 0; i <= width; i++) {
		if (i == 0) {
			setxy(x + i, y + height / 4); cout << char(eastWallCode);
			setxy(x + i, y + height / 2); cout << char(eastWallCode);
			setxy(x + i, y + height * 3 / 4); cout << char(eastWallCode);
		}
		else if (i == width) {
			setxy(x + i, y + height / 4); cout << char(westWallCode);
			setxy(x + i, y + height / 2); cout << char(westWallCode);
			setxy(x + i, y + height * 3 / 4); cout << char(westWallCode);
		}
		else if (i == width / 4 || i == width / 2 || i == width * 3 / 4) {
			setxy(x + i, y + height / 4); cout << char(crossCode);
			setxy(x + i, y + height / 2); cout << char(crossCode);
			setxy(x + i, y + height * 3 / 4); cout << char(crossCode);
		}
		else {
			setxy(x + i, y + height / 4); cout << char(widthWallCode);
			setxy(x + i, y + height / 2); cout << char(widthWallCode);
			setxy(x + i, y + height * 3 / 4); cout << char(widthWallCode);
		}
	}

	//diagonals
	for (int i = 1; i < height; i++) {
		if (i != height / 4 && i != height / 2 && i != height * 3 / 4) {
			if (i == 1) {
				setxy(x + 2, y + i); cout << char(backslashCode);

				setxy(x + width - i - 1, y + i); cout  << char(slashCode);
			}
			else if(i<height/4){
				setxy(x + i + i, y + i); cout << char(backslashCode);

				setxy(x + width - i - i, y + i); cout  << char(slashCode);

			}
			else if (i < height / 2) {
				setxy(x + i + i, y + i); cout << char(backslashCode);
				setxy(x + i + i - width/4, y + i + height/4); cout << char(backslashCode);
				setxy(x + i + i + width/4, y + i - height/4); cout << char(backslashCode);

				setxy(x + width - i - i, y + i); cout  << char(slashCode);
				setxy(x + width - i - i + width / 4, y + i + height / 4); cout  << char(slashCode);
				setxy(x + width - i - i - width / 4, y + i - height / 4); cout  << char(slashCode);
			}
			else if (i < height *3/ 4) {
				setxy(x + i + i, y + i); cout << char(backslashCode);
				setxy(x + i + i - width/4, y + i + height/4); cout << char(backslashCode);
				setxy(x + i + i + width/4, y + i - height/4); cout << char(backslashCode);

				setxy(x + width - i - i, y + i); cout <<   char(slashCode);
				setxy(x + width - i - i + width / 4, y + i + height / 4); cout  << char(slashCode);
				setxy(x + width - i - i - width / 4, y + i - height / 4); cout << char(slashCode);
			}
			else {
				setxy(x + i + i, y + i); cout << char(backslashCode);

				setxy(x + width - i - i, y + i); cout  << char(slashCode);
			}
		}
	}

	//coordinates
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	for (int i = 0; i < 5; i++) {
		setxy(x - 3, y + i*height / 4); cout << char(97+i);//a, b, c, d, e
		setxy(x + i*width / 4, y + height + 3); cout << char(49+i);// 1, 2, 3, 4, 5
	}
	defaultColor();
	
	setxy(curPosX, curPosY);
}

//Draw game board at the begining of new game.
void gameBoard::showBoard() {
	//draw game board
	setxy(1, 1);
	DrawRect(xBoard,yBoard,width,height, 32, 22);
}

//Draw a goat which consists of 9 blue chars.
void gameBoard::drawGoat(int x, int y) {
	int goatChar = 36;// $ char

	player1Color();
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			setxy(x + i, y + j);
			cout << char(goatChar);
		}
	}
	defaultColor();
}

//Draw a tiger which consists of 9 green chars.
void gameBoard::drawTiger(int x, int y) {
	int tigerChar = 64;// @ char

	player2Color();
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			setxy(x + i, y + j);
			cout << char(tigerChar);
		}
	}
	defaultColor();
}

//Display info about pawns, current game state: actual turn, goats beaten or placed (depends of current turn).
void gameBoard::showKey(string goatsPlayerName, string tigersPlayerName, int part) {
	//key inscription
	setxy(9, 3);
	cout << "Key:";

	//tiger pawn key
	int tigerChar = 64;

	player2Color();
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			setxy(3 + i, 6 + j);
			cout << char(tigerChar);
		}
	}
	defaultColor();
	setxy(7, 6);
	cout << "- tiger pawn";

	//goat pawn key
	int goatChar = 36;

	player1Color();
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			setxy(3 + i, 10 + j);
			cout << char(goatChar);
		}
	}
	defaultColor();
	setxy(7, 10);
	cout << "- goat pawn";

	//players
	setxy(3, 15);
	player1Color();
	cout << goatsPlayerName<<": goats";
	player2Color();
	setxy(3, 16);
	cout << tigersPlayerName<<": tigers";

	setxy(3,22);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MAGNETA);
	cout << "Part: " << part;

	defaultColor();
}

//clears specified element
void gameBoard::clearPawn(int x, int y) {
	int spaceChar = 32;
	for (int k = -1; k < 2; k++) {
		for (int l = -1; l < 2; l++) {
			setxy(x + k, y + l);
			cout << char(spaceChar);
		}
	}
}

int gameBoard::getXInput() {
	return xInput;
}

int gameBoard::getYInput() {
	return yInput;
}

int gameBoard::getXBoard() {
	return xBoard;
}

int gameBoard::getYBoard() {
	return yBoard;
}

int gameBoard::getWidth() {
	return width;
}

int gameBoard::getHeight() {
	return height;
}
