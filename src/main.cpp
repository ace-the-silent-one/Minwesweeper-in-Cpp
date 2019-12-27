#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;
void startGame();        // The main function which will call all other functions
void printMenu();       // Prints the main menu
void printLines(int);   //Prints blank lines  as per the argument passed
void setGameState();    // Sets the game to start, instructions or exit as per choice. If choice is invalid then calls startGame
void newGame();         // Start the game and print the board options
void printInstructions(); // Prints the instructions and rules of playing
void terminateGame();     // Ends the game
void initializeBoard();    // Initialize game board 1 or 2 as per currentChoice and generatesMines
void printBoard();         // prints the board on screen, based on currentChoice
bool isMined(int, int);   // Check for mines
void takeInput();       // take input from user
void checkInput(int, int, int);
bool isValid(int, int);
void showEmpty(int row, int col);
void setEmpty(int, int);
bool isEmpty(int, int);
bool isFlagged(int, int);
int Wincheck;
void printMines();
int countMines(int, int);       // Count the mines in vincinity of current box (row, col)
void rePlay();      //   If user wants to play again
int currentChoice;      // The choice user chooses, used throughout the functions
const int boardSize1 = 9;         // 9 x 9 board
char gameBoard1[boardSize1][boardSize1];  // 9 x 9 board
bool minedBoard1[boardSize1][boardSize1];  // To keep a track of mines without disturbing the main output
const int boardSize2 = 16;        // 16 x 16 board
char gameBoard2[boardSize2][boardSize2]; // 16 x 16 board
bool minedBoard2[boardSize2][boardSize2];
void printTop() {
	int count = 0;            // break the loop after print == signs twice
	printLines(3);
	for (int i = 1; i <= 25; i++) {
		cout << "=";
		if (i == 25) {
			if (count != 1)
				cout << "Minesweeper";
			i = 0;
			count++;          //increment the counter after printing Half the = signs
			if (count == 2)  // break the loop after print == signs twice
				break;
		}
	}
	printLines(2);
}
int main() {
	startGame();
	return 0;
}

void startGame() {
	Wincheck = 0;
	srand(time(NULL));  // Seed the time for random generator for mines each time new game starts
	printMenu();
	setGameState();
}

void printMenu() {
	system("cls");   // Clear the screen
	printTop();
	cout << "1) Start new game " << endl
		<< "2) Instructions " << endl
		<< "3) Exit";
	printLines(2);
	cout << "Enter your choice: ";  // Ask the user for input
	cin >> currentChoice;           // Store it globally
}

void printLines(int n) {
	for (int i = 0; i <= n; i++)
		cout << endl;
}

void setGameState() {
	switch (currentChoice) {  // Switch to the user's choice, if invalid, reset the program
	case 1:
		newGame();
		break;
	case 2:
		printInstructions();  // Prints detailed information on how to play the game
		break;
	case 3:
		terminateGame();  // Game quits
		break;
	default:
		startGame(); // Reset the game if user inputs invalid number
	}
}

void newGame() {
	system("cls");
	printTop();   // Print top, The main Ui of game
	cout << "Select the board size: " << endl
		<< " 1)  9 x  9   --    10 mines" << endl
		<< " 2) 16 x 16   --    40 mines" << endl
		<< " 3) Back";
	printLines(2);
	cout << "Enter your choice: ";
	cin >> currentChoice;
	if (currentChoice == 1 || currentChoice == 2) {
		initializeBoard();
		// To cheat or debug printMines();
		printBoard();
		takeInput();
	}
	else if (currentChoice == 3)
		startGame();
	else
		newGame();
}

void initializeBoard() {  // Set everything
	int randRow, randCol, count = 0;
	if (currentChoice == 1) {
		for (int row = 0; row < boardSize1; row++)
			for (int col = 0; col < boardSize1; col++) {     //Sets the board 9 x 9 to be printed
				gameBoard1[row][col] = '-';
				minedBoard1[row][col] = false;
			}

		while (true) {
			randRow = (rand() % 8);			//Random values from 0 - 8 
			randCol = (rand() % 8);			// The size of 9 x 9 board  
			if (!isMined(randRow, randCol)) {
				minedBoard1[randRow][randCol] = true;  //   Sets the board 16 x 16 mines on boolean board
				count++;
			}
			if (count == 10)				// As there are 10 mines on the 9 x 9 board
				break;
		}
	}
	else {
		for (int row = 0; row < boardSize2; row++)		//Sets the board 16 x 16 to be printed
			for (int col = 0; col < boardSize2; col++) {
				gameBoard2[row][col] = '-';
				minedBoard2[row][col] = false;
			}
		while (true) {
			randRow = (rand() % 15);			// Since the size of the board in array is 16
			randCol = (rand() % 15);			// The size of the board in array is 16
			if (!isMined(randRow, randCol)) {
				minedBoard2[randRow][randCol] = true;  // Sets the board 16 x 16 on the boolean board
				count++;
			}
			if (count == 40)   // As there are 40 total mines on the 16 x 16 board
				break;
		}
	}
}
void printBoard() {
	system("cls");
	printTop();
	if (currentChoice == 1) {
		cout << "    ";
		for (int i = 1; i <= boardSize1; i++)
			cout << setw(4) << i;
		printLines(2);
		for (int i = 0; i < boardSize1; i++) {
			cout << setw(4) << i + 1;
			for (int j = 0; j < boardSize1; j++) {                         // Prints the 9 x 9 board
				cout << setw(4) << gameBoard1[i][j];
			}
			cout << endl;
		}
	}

	else {
		cout << "    ";
		for (int i = 1; i <= boardSize2; i++)
			cout << setw(4) << i;
		printLines(2);
		for (int i = 0; i < boardSize2; i++) {
			cout << setw(4) << i + 1;
			for (int j = 0; j < boardSize2; j++) {
				cout << setw(4) << gameBoard2[i][j];			// Prints the 16 x 16 board
			}
			cout << endl;
		}
	}
}
void printInstructions() {  // The instructions
	system("cls");
	printTop();
	cout << "**The purpose of the game is to open all the cells of the board which do not contain a mine. You lose if you set open a mine cell\n\n"
		<< "**Every non-bomb cell you open will tell you the total number of bombs in the eight neighboring cells, \n\n"
		<< "**If there are no bombs in the eight neighboring cells of the opened cell, it will show E \n\n"
		<< "**If you think that a specefic cell has a mine, press 1 to flag it.\n\n"
		<< "**Once you have opened all the cells which are not mined and flagged all the mined cells, You win the game\n\n";
	system("pause");
	startGame();
}
void terminateGame() { exit(0); } // Exit the game
bool isMined(int row, int col) {              // Check if the given row and col of the board is mined or not
	if (currentChoice == 1) {
		return minedBoard1[row][col];
	}
	else {
		return minedBoard2[row][col];
	}
}

bool isFlagged(int row, int col) {
	if (currentChoice == 1) {
		if (gameBoard1[row][col] == 'F')
			return true;
		else
			return false;
	}
	else {
		if (gameBoard2[row][col] == 'F')
			return true;
		else
			return false;
	}
}

void takeInput() {
	bool win = false;
	Wincheck = 0;
	if (currentChoice == 1) {
		for (int i = 0; i < boardSize1; i++) {
			for (int j = 0; j < boardSize1; j++) {
				if (gameBoard1[i][j] == '-')
					Wincheck++;
			}
		}
		if (Wincheck == 0)
			win = true;
		else
			win = false;
	}
	else {
		for (int i = 0; i < boardSize2; i++) {
			for (int j = 0; j < boardSize2; j++) {
				if (gameBoard2[i][j] == '-')
					Wincheck++;
			}
		}
		if (Wincheck == 0)
			win = true;
		else
			win = false;
	}
	if (win) {  // Check if user has opened enough cells and flagged the mines
		printLines(3);
		cout << "You Win";
		rePlay();
	}
	else
	{
		int num = 1;
		int row, column;  // Input the  row and column from user, 
		int operation;     // Input the required operation, i-e Open, Flag, UnFlag
		printLines(2);
		do {
			cout << "Enter your choice (Row) (Column): ";
			cin >> row >> column;	// Ask for the row, column
		} while (!isValid(row - 1, column - 1));  // If row and column are out of bounds, ask for input again
		do {
			cout << "Enter your desired option: " << endl
				<< " 1 -- to flag " << endl
				<< " 2 -- to open " << endl
				<< " 3 -- to remove flag   ";
			cin >> operation; // Ask for the desired operation
		} while (operation < 1 || operation > 3);  // If the operation is invalid, ask for input again
		checkInput(row, column, operation);
	}
}
bool isValid(int row, int col)
{
	// Returns true if row number and column number 
	// is in range 
	if (currentChoice == 1)
		return (row >= 0) && (row < boardSize1) &&
		(col >= 0) && (col < boardSize1);
	else return (row >= 0) && (row < boardSize2) &&
		(col >= 0) && (col < boardSize2);
}

int countMines(int row, int col) {
	if (isValid(row, col)) {
		int minesCount = 0;
		if (isValid(row - 1, col))if (isMined(row - 1, col)) { minesCount++; }
		if (isValid(row - 1, col - 1))if (isMined(row - 1, col - 1)) { minesCount++; }
		if (isValid(row - 1, col + 1))if (isMined(row - 1, col + 1)) { minesCount++; }
		if (isValid(row, col + 1))if (isMined(row, col + 1)) { minesCount++; }
		if (isValid(row, col - 1))if (isMined(row, col - 1)) { minesCount++; }
		if (isValid(row + 1, col))if (isMined(row + 1, col)) { minesCount++; }
		if (isValid(row + 1, col - 1))if (isMined(row + 1, col - 1)) { minesCount++; }
		if (isValid(row + 1, col + 1))if (isMined(row + 1, col + 1)) { minesCount++; }
		return minesCount;
	}
}

bool isEmpty(int row, int col) {
	int num = countMines(row, col);
	if (num == 0)
		return true;
	else return false;
}

void setEmpty(int row, int col) {
	if (currentChoice == 1)
		gameBoard1[row][col] = 'E';
	else
		gameBoard2[row][col] = 'E';
}

void setMineCount(int row, int col) {
	int num = countMines(row, col);
	if (num == 0) {
		setEmpty(row, col);
	}
	else
		currentChoice == 1 ? gameBoard1[row][col] = num + '0' : gameBoard2[row][col] = num + '0';
}

void showEmpty(int row, int col) {
	if (isValid(row, col)) {
		int num = 0;
		if (isValid(row - 1, col) && !isMined(row - 1, col)) {
			setMineCount(row - 1, col);  // North
			if (isEmpty(row - 1, col)) {		// WORKS
				setEmpty(row - 1, col);
				showEmpty(row - 1, col);
			}
		}
		else;
		if (isValid(row - 1, col - 1) && !isMined(row - 1, col - 1)) {
			setMineCount(row - 1, col - 1);
			if (isEmpty(row - 1, col - 1)) {		//North West
				setEmpty(row - 1, col - 1);			// WORKS
				showEmpty(row - 1, col - 1);
			}
		}
		if (isValid(row - 1, col + 1) && !isMined(row - 1, col + 1)) {
			setMineCount(row - 1, col + 1);		//North East
			if (isEmpty(row - 1, col + 1)) {
				setEmpty(row - 1, col + 1);				// WORKS
				showEmpty(row - 1, col + 1);
			}
		}
		if (isValid(row, col + 1) && !isMined(row, col + 1)) {   // Works
			setMineCount(row, col + 1);
			if (isEmpty(row, col + 1)) {				//East
				setEmpty(row, col + 1);				// Works
				showEmpty(row, col + 1);
			}
		}
		if (isValid(row, col - 1) && !isMined(row, col - 1)) {
			setMineCount(row, col - 1);
		}
		if (isValid(row + 1, col) && !isMined(row + 1, col)) {
			setMineCount(row + 1, col);
		}
		if (isValid(row + 1, col - 1) && !isMined(row + 1, col - 1)) {
			setMineCount(row + 1, col - 1);
		}
		if (isValid(row + 1, col + 1) && !isMined(row + 1, col + 1)) {
			setMineCount(row + 1, col + 1);
		}
	}
	else;
}

void rePlay() {
	printLines(2);
	cout << "Press 1 to go back to main menu: " << endl   // Ask for user's opinion whether to exit or play again
		<< "Press 2 to exit: ";
	cin >> currentChoice;  // Exit or restart the game, based on input
	if (currentChoice == 1)
		startGame();
	else terminateGame();
}

void checkInput(int row, int col, int operation) {
	int num;
	if (isMined(row - 1, col - 1) && operation == 2) {
		currentChoice == 1 ? gameBoard1[row - 1][col - 1] = '*' : gameBoard2[row - 1][col - 1] = '*';
		printMines();
		printBoard();
		cout << "You Opened a mine field: " << endl
			<< "Game Over!";
		rePlay();
	}
	else if (!isMined(row - 1, col - 1) && operation == 2) {
		if (!isFlagged(row - 1, col - 1)) {
			num = countMines(row - 1, col - 1);
			if (num == 0) {
				setEmpty(row - 1, col - 1);
				showEmpty(row - 1, col - 1);
			}

			else
				setMineCount(row - 1, col - 1);
			printBoard();
			takeInput();
		}
		else {
			printBoard();
			takeInput();
		}
	}
	//
	else if (operation == 1) {
		if (currentChoice == 1)
			if (gameBoard1[row - 1][col - 1] == '-' || gameBoard1[row - 1][col - 1] == '*') {  //Only flag it, if it has not been opened
				gameBoard1[row - 1][col - 1] = 'F';
			}
			else;	// To sort the else below with the outer if
		else {
			if (gameBoard2[row - 1][col - 1] == '-') {  //Only flag it, if it has not been opened
				gameBoard2[row - 1][col - 1] = 'F';
			}
		}
		printBoard();
		takeInput();
	}
	else {			//IF user wants to remove the flag
		if (currentChoice == 1)
			if (gameBoard1[row - 1][col - 1] == 'F') {  // Remove the flag only if it is flagged  
				gameBoard1[row - 1][col - 1] = '-';
			}
			else;	// To sort the else below with the outer if
		else {
			if (gameBoard2[row - 1][col - 1] == 'F') {  // Remove the flag only if it is flagged  
				gameBoard2[row - 1][col - 1] = '-';
			}
		}
		printBoard();
		takeInput();
	}
}

void printMines() {
	if (currentChoice == 1)
		for (int i = 0; i < boardSize1; i++) {
			for (int j = 0; j < boardSize1; j++)
				if (minedBoard1[i][j])
					gameBoard1[i][j] = '*';
		}
	else
		for (int i = 0; i < boardSize2; i++) {
			for (int j = 0; j < boardSize2; j++)
				if (minedBoard2[i][j])
					gameBoard2[i][j] = '*';
		}
}