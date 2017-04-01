#include <limits>
#include <fstream>
#include <algorithm>
#include "Fib2584Ai.h"
#include "Fib2584/GameBoard.h"

using namespace std;
Fib2584Ai::Fib2584Ai()
{
	ifstream fin("feature.dat");
    if (fin) {
        feature = new float [FTSIZE];
        fin.read(reinterpret_cast<char*>(feature), sizeof(float) * FTSIZE);
		fin.close();
    } else
        feature = new float [FTSIZE] ();
}

Fib2584Ai::~Fib2584Ai() {
//  ofstream fout("feature.dat", ifstream::binary);
//	fout.write(reinterpret_cast<char*>(feature), sizeof(float) * FTSIZE);
//	fout.close(); 
   delete [] feature;
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	srand(time(NULL));
	turn = 0;
	return;
}

int Fib2584Ai::generateEvilMove(int board[4][4]) {
	myGameBoard countBoard(board);
	int CountEmptyTile = countBoard.countEmptyTile();
	if (CountEmptyTile == 16) {
		turn = 0;
	}
	int evilTile = (turn == 3 ? 3 : 1);
	int pos = -1;
	if (false) {
		vector<int> emptyTile;
		for (int i = 0; i < 16; ++i)
			if (board[i/4][i%4] == 0) emptyTile.push_back(i);
		pos = emptyTile[rand() % emptyTile.size()];
	} else {
		float worstScore = 99999999;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (board[i][j] == 0) {
					board[i][j] = evilTile;
					if (maxScore(board) < worstScore) {
						worstScore = maxScore(board);
						pos = i*4 + j;
					}
					board[i][j] = 0;
				}
			}
		}
	}
	turn = (turn + 1) % 4;
	return pos;
}

float Fib2584Ai::maxScore(int board[4][4]) {
	myGameBoard b(board);
	float bestScore = 0.0;
	for (int i = 0; i < 4; ++i) {
		myGameBoard tb(b);
		int reward = tb.move(static_cast<MoveDirection>(i));
		if (tb == b) continue;
		float vt = getVT(tb.getVector());
		if (vt + reward > bestScore)
			bestScore = vt + reward;
	}
	return bestScore;
}
/*
int Fib2584Ai::generateEvilMove(int board[4][4]) {
	int level = 2;
	int evilTile = count == 3 ? 0x3 : 0x1;
	float worstVT = numeric_limits<float>::max();
	int worstPlace = -1;
	vector<int> bestVec;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) {
				board[i][j] = evilTile;	
				float AiVal = getAiVal(board, level, count, -1, numeric_limits<float>::max());
				if (AiVal < worstVT) {
					worstVT = AiVal;
					worstPlace = i * 4 + j;
				}
				board[i][j] = 0;
			}
		}
	}
	count = (count + 1) % 4;
	return worstPlace;
}
*/
float Fib2584Ai::getAiVal(int board[4][4], int level, int count, float alpha, float beta){
	float bestVT = alpha;
	for (int i = 0; i < 4; i++) {
		myGameBoard myBoard(board);
		myGameBoard Origin = myBoard;
		int reward = myBoard.move(static_cast<MoveDirection>(i));
		if (Origin == myBoard) continue;
		int tmpBoard[4][4];
		myBoard.getArrayBoard(tmpBoard);
		bestVT = max(bestVT, reward + getEvilVal(tmpBoard, level - 1, (count + 1) % 4, -1, numeric_limits<float>::max()));
		if (bestVT > beta) break;
	}
	return bestVT;
}

float Fib2584Ai::getEvilVal(int board[4][4], int level, int count, float alpha, float beta){
	if (level == 0) {
		myGameBoard myBoard(board);
		vector<int> Vector = myBoard.getVector();
		return getVT(Vector);
	}
	int EvilTile = (count == 3 ? 3 : 1);	
	//float worstVT = numeric_limits<float>::max();
	float worstVT = beta;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) {
				board[i][j] = EvilTile;
				worstVT = min(worstVT, getAiVal(board, level, count, -1, numeric_limits<float>::max()));
				board[i][j] = 0;
				if (worstVT < alpha) return worstVT;
			}
		}
	}
	return worstVT;
}

MoveDirection Fib2584Ai::generateMove(int board[4][4])
{	
	float bestVT = 0.0;
	vector<int> bestVec;
	MoveDirection bestMove = static_cast<MoveDirection>(rand() % 4);
	myGameBoard myBoard(board);
	for (int i = 0; i < 4; i++) {
		myGameBoard tempBoard = myBoard;
		int reward = tempBoard.move(static_cast<MoveDirection>(i));
		if (myBoard == tempBoard) continue;
		vector<int> tempVec = tempBoard.getVector();
		float tempVT = getVT(tempVec);
		if (bestVT < tempVT + reward) {
			bestVec = tempVec;
			bestVT = tempVT + reward;
			bestMove = static_cast<MoveDirection>(i);
		}
	}
//	Learning(bestVT);
	preVec = bestVec;
	return static_cast<MoveDirection>(bestMove);
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
 //   Learning (0.0);
	turn = 0;
	preVec.clear();
	return;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/

float Fib2584Ai::getVT(const vector<int> &index) {
	float VT = 0.0;
	for (auto&& i: index) {
		VT += feature[i];
	}
	return VT;
}

int cmp(Score_TileNum a, Score_TileNum b) {
		return a.score > b.score;
}

void Fib2584Ai::Learning(const float & VT) {
	const float delta = VT - getVT(preVec);
	for (auto&& i: preVec) {
		feature[i] += delta * ALPHA_N;
	}
}


void search(myGameBoard myBoard, myGameBoard originBoard, int levels, int ori_level, vector<Score_TileNum> &score_tilenum, int dir, int score) {
	if (levels == 0) {
		int tilenum = myBoard.countEmptyTile();
		score_tilenum.push_back(Score_TileNum(dir, score, tilenum));
		return;
	} else  {
		for (int i = 0; i < 4; i++) {
			myGameBoard temp = myBoard;
			int score_m = temp.move(static_cast<MoveDirection>(i));
			score_m += temp.getWeightScore();
			if (levels == ori_level) {
				dir = i;
				if (temp == originBoard)
					continue;
			}
			if (myBoard == temp);
			else temp.addRandomTile();
			search(temp, originBoard, levels - 1, ori_level, score_tilenum, dir, score + score_m);
		}
	}
	return;
}
