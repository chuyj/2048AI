#ifndef __myGAMEBOARD_H__
#define __myGAMEBOARD_H__

#include <iostream>
#include "Fib2584/Typedefs.h"
#include "Fib2584/MoveTable.h"
#include "Fib2584/BitBoard.h"
#include "Fib2584/Random.h"
#include <vector>
using namespace std;

class myGameBoard
{
public:
	static const int fibonacci_[32];
	//start
	int myweight[4][4] = {{15,14,13,12},{8,9,10,11},{7,6,5,4},{0,1,2,3}};
private:
	static Random random_;
public:
	myGameBoard();
	myGameBoard(int board[4][4]);
	void initialize();
	int move(MoveDirection moveDirection);
	void addRandomTile();
	bool terminated();
	void getArrayBoard(int board[4][4]);
	int getMaxTile();
	void showBoard();
	bool operator==(myGameBoard gameBoard);
	int countEmptyTile();
	static int getIndex(int Fib);
	vector<int> getVector();
	//start
	int getWeightScore();
private:
	BitBoard getRow(int row);
	BitBoard getColumn(int column);
	BitBoard restoreRow(BitBoard rowBits, int row);
	BitBoard restoreColumn(BitBoard columnBits, int column);
	int getTile(int row, int column);
	int getFibonacci(int index);
private:
	BitBoard board_;
};

#endif
