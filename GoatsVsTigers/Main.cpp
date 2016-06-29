#include "GoatsVsTigersGame.h"
#include<ctime>
#include <cstdlib>


int main() {
	std::srand(unsigned(std::time(0)));
	system("MODE CON: COLS=80 LINES=32"); // set screen size

	GoatsVsTigersGame mGame;
	mGame.run();
	return 0;
}