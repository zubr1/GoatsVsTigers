#pragma once
#include "gameBoard.h"

class GoatsVsTigersGame
{
private:
	gameBoard mScreen;

	bool intsFileValidation(int mFileData[]);
	bool stringsFileValidation(string goatsPlayerName, string tigersPlayerName, int mFileData[]);
	void showGamesHistory();
	void showInstruction();
	void loadGame();
	void playGame();
	void playerVsComputerGame();
	void playerVsPlayerGame();
	void intro();
	void clearScreen();
	string playersNames(string pawns);
	void showError();
public:
	GoatsVsTigersGame();
	~GoatsVsTigersGame();

	void run();
	

};