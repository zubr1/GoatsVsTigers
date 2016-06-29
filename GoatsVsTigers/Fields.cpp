#include "Fields.h"
#include"gameBoard.h"
#include<Windows.h>
#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
#include <algorithm>
#include <cstdlib>

using namespace std;

//Initialize game board fields with default data (all fields are empty)
void Fields::initializePanws() {
	//initialize playable fields
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			mFields[i][j].xCoord = mGameBoard.getXBoard() + i*mGameBoard.getWidth() / 4;
			mFields[i][j].yCoord = mGameBoard.getYBoard() + j*mGameBoard.getHeight() / 4;
			mFields[i][j].fieldState = "empty";
			stringstream ss;
			ss << char(97 + j);
			ss << char(49 + i);
			ss >> mFields[i][j].name;
		}
	}

	//create connections between fields
	mFields[0][0].connections = "a2 b2 b1";
	mFields[0][1].connections = "a1 b2 c1";
	mFields[0][2].connections = "b1 b2 c2 d2 d1";
	mFields[0][3].connections = "c1 d2 e1";
	mFields[0][4].connections = "d1 d2 e2";
	mFields[1][0].connections = "a1 b2 a3";
	mFields[1][1].connections = "a1 a2 a3 b1 b3 c1 c2 c3";
	mFields[1][2].connections = "b2 c1 c3 d2";
	mFields[1][3].connections = "c1 c2 c3 d1 d3 e1 e2 e3";
	mFields[1][4].connections = "d2 e1 e3";
	mFields[2][0].connections = "a2 a4 b2 b3 b4";
	mFields[2][1].connections = "a3 b2 b4 c3";
	mFields[2][2].connections = "b2 b3 b4 c2 c4 d2 d3 d4";
	mFields[2][3].connections = "c3 d2 d4 e3";
	mFields[2][4].connections = "d2 d3 d4 e2 e4";
	mFields[3][0].connections = "a3 a5 b4";
	mFields[3][1].connections = "a3 a4 a5 b3 b5 c3 c4 c5";
	mFields[3][2].connections = "b4 c3 c5 d4";
	mFields[3][3].connections = "c3 c4 c5 d3 d5 e3 e4 e5";
	mFields[3][4].connections = "d4 e3 e5";
	mFields[4][0].connections = "a4 b4 b5";
	mFields[4][1].connections = "a5 b4 c5";
	mFields[4][2].connections = "b4 b5 c4 d4 d5";
	mFields[4][3].connections = "c5 d4 e5";
	mFields[4][4].connections = "d4 d5 e4";

	//initialize 4 tigers
	mFields[0][0].fieldState = mFields[0][4].fieldState = mFields[4][0].fieldState = mFields[4][4].fieldState = "tiger";
}

//Check if tiger is blocked. If yes-returns true, no-returns false.
bool Fields::isBlocked(fields currentField) {
	//Checks if adjoining fields are empty. Even if one of them is empty returns false. Also checks if tiger can beat goat.
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (currentField.connections.find(mFields[i][j].name) != string::npos) {
				if (mFields[i][j].fieldState == "empty") {
					return false;
				}
				else if (mFields[i][j].fieldState == "goat") {	//checks if tiger can beat goat

					int xVector = mFields[i][j].xCoord - currentField.xCoord;
					int yVector = mFields[i][j].yCoord - currentField.yCoord;

					//finds potential field for tiger behind the goat
					for (int k = 0; k < 5; k++) {
						for (int l = 0; l < 5; l++) {
							if (mFields[k][l].xCoord == (currentField.xCoord + 2 * xVector) && mFields[k][l].yCoord == (currentField.yCoord + 2 * yVector)
								&& mFields[k][l].fieldState == "empty") {
								//We found new field for tiger. Now he can beat goat.
								return false;
							}
						}
					}
				}
			}
		}
	}
	//Algoritm didn't find possible move for tiger, so pawn is blocked.
	return true;
}

//Checks if goat is blocked. Even if one of the adjoining fields is empty returns false. Otherwise returns true.
bool Fields::isGoatBlocked(fields mField) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mField.connections.find(mFields[i][j].name) != string::npos) {
				if (mFields[i][j].fieldState == "empty") {
					return false;
				}
			}
		}
	}
	//Algoritm didn't find possible move for goat, so pawn is blocked.
	return true;
}

//Check if player can place new goat on board. New goats can be placed only on empty fields.
bool Fields::placeGoat(string field) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].name == field) {
				if (mFields[i][j].fieldState == "empty") {
					mFields[i][j].fieldState = "goat";
				}
				else {
					return false;
				}
				mGameBoard.drawGoat(mFields[i][j].xCoord, mFields[i][j].yCoord);
				return true;
			}
		}
	}
	return false;//for security purposes
}

//Check if tiger pawn can be moved from specified position. 
//If pawn is blocked, or doesn't exist on this field, method returns false. Otherwise true.
bool Fields::moveTigerFrom(string from) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].name == from) {
				if (mFields[i][j].fieldState == "tiger") {
					bool blocked = isBlocked(mFields[i][j]);//check if tiger is blocked

					if (blocked) {
						return false;
					}
					else {
						return true;
					}
				}
				else {
					return false;
				}
			}
		}
	}
	return false;//for security purposes
}

//Check if tiger pawn can be moved to a specified position from specified position.
//It also checks if tiger can beat goat. If so, goatsBeaten pointer increments value in game instance.
bool Fields::moveTigerTo(string from, string to, int *goatsBeaten) {
	//find destination field
	int iDest = 0;
	int jDest = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].name == to) {
				iDest = i;
				jDest = j;
				//If destination field is not empty, we dont need to perform rest operation. Move will be always invalid.
				if (mFields[iDest][jDest].fieldState != "empty") {
					return false;
				}
				break;
			}
		}
	}

	//find "from" field, and make move
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].name == from) {
				int xVector = mFields[iDest][jDest].xCoord - mFields[i][j].xCoord;
				int yVector = mFields[iDest][jDest].yCoord - mFields[i][j].yCoord;

				//coordinates for potential field with goat that can be beaten
				int xGoat = 0;
				int yGoat = 0;
				bool foundCoord = false;
				bool breakLoop = false;

				//check if this potential field is between current tiger field and possible target field for tiger
				if ((abs(xVector)>(mGameBoard.getWidth() + 1) / 4) && (abs(xVector)<(mGameBoard.getWidth() + 2) / 2) ||
					(abs(yVector)>(mGameBoard.getHeight() + 1) / 4) && (abs(yVector)<(mGameBoard.getHeight() + 2) / 2)) {
					
					//coords for potential field
					xVector /= 2;
					yVector /= 2;
					for (int m = 0; m < 5; m++) {
						for (int n = 0; n < 5; n++) {
							if (mFields[m][n].xCoord == (mFields[i][j].xCoord + xVector) &&
								mFields[m][n].yCoord == (mFields[i][j].yCoord + yVector)) {//finds potential field (using vector)
								if (mFields[m][n].fieldState == "goat") {//check if this field is goat
									//We found field with goat to beat;
									xGoat = m;
									yGoat = n;
									foundCoord = true;
									breakLoop = true;
									break;
								}
							}
						}
						if (breakLoop) {
							break;
						}
					}
					if (!foundCoord) {
						return false;
					}
				}
				//check if target field is connected with current tiger field, and is empty
				if (mFields[i][j].connections.find(to) != string::npos && mFields[iDest][jDest].fieldState == "empty") {
					mFields[i][j].fieldState = "empty";//remove tiger from current field
					mFields[iDest][jDest].fieldState = "tiger";//place tiger on a new field
					updateBoard();
					return true;
				}
				else if (foundCoord) {//if we found potential field, we have to check if current tiger field is connected with "goat to beat" field
					if (mFields[i][j].connections.find(mFields[xGoat][yGoat].name) != string::npos) {
						mFields[xGoat][yGoat].fieldState = "empty"; //remove beaten goat
						mFields[i][j].fieldState = "empty"; //move tiger from this field
						mFields[iDest][jDest].fieldState = "tiger"; //move tiger onto this field
						(*goatsBeaten)++;
						updateBoard();
						return true;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
		}
	}
	return false;//for security purposes
}

//in fact it checks if goat is not blocked, and if provided field is "goat field"
bool Fields::moveGoatFrom(string from) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].name == from) {
				if (mFields[i][j].fieldState == "goat") {
					bool blocked = isGoatBlocked(mFields[i][j]);

					if (blocked) {
						return false;
					}
					else {
						return true;
					}
				}
				else {
					return false;
				}
			}
		}
	}
	return false; //for security purposes
}

//Check if goat can be moved from specified position to a specified position
bool Fields::moveGoatTo(string from, string to) {
	//find destination field
	int iDest = 0;
	int jDest = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].name == to) {
				iDest = i;
				jDest = j;
				break;
			}
		}
	}

	//find "from field", and make move
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].name == from) {
				if (mFields[i][j].connections.find(to) != string::npos && mFields[iDest][jDest].fieldState == "empty") {//if bordering field is empty, make a move
					mFields[i][j].fieldState = "empty";//remove goat from current field
					mFields[iDest][jDest].fieldState = "goat";//put goat on a new field
					updateBoard();
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	return false;//for security purposes
}

// Scan each field and check if all tigers on a board are blocked
bool Fields::areAllTigersBlocked() {
	int counter = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].fieldState == "tiger") {
				if (isBlocked(mFields[i][j])) {
					counter++;
				}
			}
		}
	}

	if (counter >= 4) {
		return true;//all tigers is blocked, because there are always 4 tigers on a board
	}
	else {
		return false;
	}
}

//Check if all goats are blocked. If it's true tigers wins the game.
bool Fields::areAllGoatsBlocked() {
	int goatsCounter = 0;
	int blockedGoatsCounter = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].fieldState == "goat") {
				++goatsCounter;
				if (isGoatBlocked(mFields[i][j])) {
					++blockedGoatsCounter;
				}
			}

		}
	}

	if (goatsCounter == blockedGoatsCounter && goatsCounter!=0) {//goatsCounter!=0 prevents the analize of situation when tiger beats goat in his first move
		return true;//all goats are blocked
	}
	else {
		return false;
	}
}

//In fact it's not return best option for computer, but only random option
string Fields::calculateBestOption(int pawnType, int goatsRemain, int operationType) {//operationType: 0 - from, 1 - to
	string pawn;
	if (pawnType == 0) {
		pawn = "goat";
	}
	else {
		pawn = "tiger";
	}


	//array with shuffled data
	int randArr[25];
	for (int i = 0; i < 25; i++) {
		randArr[i] = i;
	}
	
	random_shuffle(begin(randArr), end(randArr));

	fields newFields[25];
	int counter = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			newFields[counter] = mFields[i][j];
			++counter;
		}
	}

	//now we can get access to game fields with randomized order
	for (int i = 0; i < 25; i++) {
		if (pawnType == 0) {//goats
			if (goatsRemain != 0) {//to type
				if (newFields[randArr[i]].fieldState == "empty") {
					return newFields[randArr[i]].name;
				}
			}
			else {
				if (operationType == 0) {//from type
					if (newFields[randArr[i]].fieldState == "goat") {
						bool blocked = isGoatBlocked(newFields[randArr[i]]);
						if (!blocked) {
							return newFields[randArr[i]].name;
						}
					}
				}
				else {//to type
					if (newFields[randArr[i]].fieldState == "empty") {
						return newFields[randArr[i]].name;
					}
				}
			}
		}
		else {//tigers
			if (operationType == 0) {//from type
				if (newFields[randArr[i]].fieldState == "tiger") {
					bool blocked = isBlocked(newFields[randArr[i]]);
					if (!blocked) {
						return newFields[randArr[i]].name;
					}
				}
			}
			else {//to type
				if (newFields[randArr[i]].fieldState == "empty") {
					return newFields[randArr[i]].name;
				}
			}
		}
	}

	return newFields[randArr[0]].name;//For security purposes. Value should always be returned before this statement.
}

//Goes through each element, and add it's name to a one long string
string Fields::getBoardState() {
	string boardState = "";
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			boardState += (mFields[i][j].name + ": " + mFields[i][j].fieldState + "  ");
		}
	}

	return boardState;
}

//Game board data validation. false- error, string is invalid
bool Fields::setBoardState(string boardState) {
	int goats = 0;
	int tigers = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {

			size_t found = boardState.find(mFields[i][j].name);

			if (found != string::npos) {
				string substr = boardState.substr(found + 4);
				string untilSpace = substr.substr(substr.find(' '));
				string substrOfSubstr = substr.substr(0, (substr.length() - untilSpace.length()));
				if (substrOfSubstr == "empty" || substrOfSubstr == "goat" || substrOfSubstr == "tiger") {
					mFields[i][j].fieldState = substrOfSubstr;//update field
					if (substrOfSubstr == "tiger") {
						tigers++;
					}else if(substrOfSubstr=="goats"){
						goats++;
					}
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}



	if (tigers != 4 || goats > 20) {
		return false;
	}
	else {
		return true;
	}
}

//simply draws actual board state
void Fields::updateBoard() {
	clearAllPawns();
	mGameBoard.DrawRect(mGameBoard.getXBoard(), mGameBoard.getYBoard(), mGameBoard.getWidth(), mGameBoard.getHeight(), 32, 22);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mFields[i][j].fieldState == "tiger") {
				mGameBoard.drawTiger(mFields[i][j].xCoord, mFields[i][j].yCoord);
			}
			else if (mFields[i][j].fieldState == "goat") {
				mGameBoard.drawGoat(mFields[i][j].xCoord, mFields[i][j].yCoord);
			}
		}
	}
}

//clears all pawns on the board, also clears pieces of board frame
void Fields::clearAllPawns() {
	//goes through each element
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			mGameBoard.clearPawn(mFields[i][j].xCoord, mFields[i][j].yCoord);
		}
	}
}
