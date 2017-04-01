#ifndef __FIB2584AI_H__
#define __FIB2584AI_H__

#include <cstdlib>
#include <ctime>
#include <vector>
#include "myGameBoard.h"
#include "Fib2584/Typedefs.h"
#include <cmath>
using namespace std;

const int FTSIZE = 4 * 22 * 22 * 22 * 22 * 22;
const float ALPHA = 0.0021;
const float ALPHA_N = ALPHA / sqrt(32);

class Fib2584Ai
{
public:
	Fib2584Ai();
	~Fib2584Ai();
	// initialize ai
	void initialize(int argc, char* argv[]);
	// generate one move
	int generateEvilMove(int board[4][4]);
	// do some action when game over
	MoveDirection generateMove(int board[4][4]);
	void gameOver(int board[4][4], int iScore);
	float getVT(const vector<int> &);
	void Learning(const float &);
	float getAiVal(int board[4][4], int level, int count, float alpha, float beta);
	float getEvilVal(int board[4][4], int level, int count, float alpha, float beta);
	float maxScore(int board[4][4]);
	

	/**********************************
	You can implement any additional functions
	or define any variables you may need.
	**********************************/
private:
	float* feature;
	vector<int> preVec;
	int count = 0;
	int turn; 
};
//start
struct Score_TileNum{
	public:
		Score_TileNum(int dir_, int score_, int tilenum_) {
			dir = dir_;
			score = score_;
			tilenum = tilenum_;
		}
		int dir;
		int score;
		int tilenum;
};
int cmp(Score_TileNum a, Score_TileNum b);
void search(myGameBoard myBoard, myGameBoard originBoard, int levels, int ori_level, vector<Score_TileNum> &score_tilenum, int dir, int score);
//end
#endif
