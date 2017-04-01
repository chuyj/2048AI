#include "myGameBoard.h"
#include <algorithm>
const int myGameBoard::fibonacci_[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};

Random myGameBoard::random_;

myGameBoard::myGameBoard():
board_(0)
{
}

myGameBoard::myGameBoard(int board[4][4]) {
	board_ = 0x0;	
	BitBoard mask(0x1f);
	for (int i = 0; i < 16; i++) {
		BitBoard Index(getIndex(board[i / 4][i % 4]));
		board_ <<= 5;
		board_ |= BitBoard(Index);
	}
}

void myGameBoard::initialize()
{
	board_ = 0;
	addRandomTile();
	addRandomTile();
}

int myGameBoard::move(MoveDirection moveDirection)
{
	BitBoard newBoard = 0;
	int score = 0;
	if(moveDirection == MOVE_UP || moveDirection == MOVE_DOWN) {
		for(int i = 0;i < 4;i++) {
			BitBoard tempColumn = 0;
			tempColumn = getColumn(i);
			newBoard |= restoreColumn(MoveTable::move_table.row_move_table_[moveDirection == MOVE_DOWN][tempColumn], i);
			score += MoveTable::move_table.row_move_score_table_[moveDirection == MOVE_DOWN][tempColumn];
		}
	}
	else {
		for(int i = 0;i < 4;i++) {
			BitBoard tempRow = 0;
			tempRow = getRow(i);
			newBoard |= restoreRow(MoveTable::move_table.row_move_table_[moveDirection == MOVE_RIGHT][tempRow], i);
			score += MoveTable::move_table.row_move_score_table_[moveDirection == MOVE_RIGHT][tempRow];
		}
	}
	board_ = newBoard;
	return score;
}

void myGameBoard::addRandomTile()
{
	int oneTileRate = 6;
	int emptyTileNum = countEmptyTile();
	int randomTileLocation = random_.get_rand_num() % emptyTileNum;
	BitBoard randomTile = (random_.get_rand_num() % 8 < oneTileRate)?0x1:0x3;
	int count = 0;
	for(BitBoard tileMask = 0x1f;tileMask != 0;tileMask <<= 5, randomTile <<= 5) {
		if((board_ & tileMask) != 0)
			continue;
		if(count == randomTileLocation) {
			board_ |= randomTile;
			break;
		}
		count++;
	}
}

int myGameBoard::countEmptyTile()
{
	int count = 0;
	BitBoard tileMask = 0x1f;
	for(;tileMask != 0;tileMask <<= 5) {
		if((board_ & tileMask) == 0)
			count++;
	}
	return count;
}

bool myGameBoard::terminated()
{
	bool movable = false;
	BitBoard tempColumn;
	BitBoard tempRow;
	for(int i = 0;i < 4;i++) {
		tempColumn = getColumn(i);
		movable |= MoveTable::move_table.row_can_move_[0][tempColumn];
		movable |= MoveTable::move_table.row_can_move_[1][tempColumn];
		tempRow = getRow(i);
		movable |= MoveTable::move_table.row_can_move_[0][tempRow];
		movable |= MoveTable::move_table.row_can_move_[1][tempRow];
		if(movable)
			break;
	}
	return !movable;
}

void myGameBoard::getArrayBoard(int board[4][4])
{
	BitBoard tempBoard = board_;
	for(int i = 0;i < 16;i++) {
		board[3 - (i / 4)][3 - (i % 4)] = fibonacci_[(int)(tempBoard & 0x1f)];
		tempBoard = tempBoard >> 5;
	}
}

int myGameBoard::getMaxTile()
{
	BitBoard countBoard = board_;
	int max_tile = 0;
	for(int i = 0;i < 16;i++) {
		if((countBoard & 0x1f) > max_tile)
			max_tile = countBoard & 0x1f;
		countBoard >>= 5;
	}
	return fibonacci_[max_tile];
}

BitBoard myGameBoard::getRow( int row )
{
	return board_ >> (row * 20) & 0xfffff;
}

BitBoard myGameBoard::getColumn( int column )
{
	BitBoard tempBoard = board_ >> (column * 5) & BitBoard(0x0001, 0xf0001f0001f0001f);
	return (tempBoard & BitBoard(0x0001, 0xf000000000000000)) >> 45 |
      	   (tempBoard & BitBoard(0x0000, 0x00001f0000000000)) >> 30 |
      	   (tempBoard & BitBoard(0x0000, 0x0000000001f00000)) >> 15 |
       	   (tempBoard & BitBoard(0x0000, 0x000000000000001f));
}

BitBoard myGameBoard::restoreRow( BitBoard rowBits, int row )
{
  	return rowBits << (row * 20);
}

BitBoard myGameBoard::restoreColumn( BitBoard columnBits, int column )
{
  	return ((columnBits & BitBoard(0xf8000)) << 45 |
            (columnBits & BitBoard(0x07c00)) << 30 |
            (columnBits & BitBoard(0x003e0)) << 15 |
            (columnBits & BitBoard(0x0001f)) ) << (column * 5);
}

int myGameBoard::getTile( int row, int column )
{
	int tile = ((board_ >> (row * 20) & 0xfffff) >> (column * 5)) & 0x1f;
	return fibonacci_[tile];
}

void myGameBoard::showBoard()
{
  	for(int row = 3;row >= 0;row--) {
   		for(int column = 3;column >= 0;column--)
      		cout << dec << getTile(row, column) << "\t";
    	cout << "\n";
  	}
}

bool myGameBoard::operator==(myGameBoard gameBoard)
{
	return board_ == gameBoard.board_;
}

int myGameBoard::getIndex(int Fib) {
	return lower_bound(fibonacci_, fibonacci_ + 32, Fib) - fibonacci_;
}

vector<int> myGameBoard::getVector() {
	int board[4][4], tb[2][4][4], cb[2][4][4];
	getArrayBoard(board);
	vector<int> index(32);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			cb[0][i][j] = getIndex(board[i][j]);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			cb[1][i][j] = cb[0][3-i][j];
	for (int r = 0; r < 4; r++) {
		for (int m = 0; m < 2; m++) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					tb[m][i][j] = cb[m][j][3-i];
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					cb[m][i][j] = tb[m][i][j];
			int idx = 8 * r + 5 * m;
			index[idx] = 0;
			index[idx] = index[idx] * 22 + cb[m][0][0];
			index[idx] = index[idx] * 22 + cb[m][0][1];
			index[idx] = index[idx] * 22 + cb[m][1][0];
			index[idx] = index[idx] * 22 + cb[m][2][0];
			index[idx] = index[idx] * 22 + cb[m][3][0];
			index[idx+1] = 1;
			index[idx+1] = index[idx+1] * 22 + cb[m][1][0];
			index[idx+1] = index[idx+1] * 22 + cb[m][2][0];
			index[idx+1] = index[idx+1] * 22 + cb[m][3][0];
			index[idx+1] = index[idx+1] * 22 + cb[m][2][1];
			index[idx+1] = index[idx+1] * 22 + cb[m][3][1];
			index[idx+2] = 2;
			index[idx+2] = index[idx+2] * 22 + cb[m][2][1];
			index[idx+2] = index[idx+2] * 22 + cb[m][3][1];
			index[idx+2] = index[idx+2] * 22 + cb[m][1][2];
			index[idx+2] = index[idx+2] * 22 + cb[m][2][2];
			index[idx+2] = index[idx+2] * 22 + cb[m][3][2];
			index[idx+3] = 3;
			index[idx+3] = index[idx+3] * 22 + cb[m][0][2];
			index[idx+3] = index[idx+3] * 22 + cb[m][0][3];
			index[idx+3] = index[idx+3] * 22 + cb[m][1][1];
			index[idx+3] = index[idx+3] * 22 + cb[m][1][2];
			index[idx+3] = index[idx+3] * 22 + cb[m][1][3];

				
		}
	}
	return index;
}
//start
int myGameBoard::getWeightScore() {
	int array[4][4] = {};
	getArrayBoard(array);
	int score = 0;
	for (int i = 0; i < 16; i++) {
		score += array[i / 4][i % 4] *myweight[i / 4][i % 4];
	}
	return score/5;
}
