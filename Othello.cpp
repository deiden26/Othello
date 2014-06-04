// TicTacToe.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<sstream>
#include <exception>
#include<string>
#include <cctype>

using namespace std;

class Board {
	int squares[8][8];

public:
	Board();
	string toString();
	bool play_square(int, int, int);
	bool play_square(int&, int&);
	bool move_is_valid(int, int, int);
	bool check_or_flip_path(int, int, int, int, int, bool);
	int get_square(int, int);
	int score();
	bool full_board();
	bool has_valid_move(int);
};

Board::Board() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			squares[i][j] = 0;
	squares[3][3]=-1;
	squares[4][4]=-1;
	squares[3][4]=1;
	squares[4][3]=1;
}

string Board::toString() {
	stringstream s;
	char cforvalplusone[] = {'W', '_', 'B'};
	s << "  1 2 3 4 5 6 7 8" << endl;
	for(int i=0; i<8;i++) {
		s << i+1 << '|';
		for(int j=0; j<8; j++)
			s << cforvalplusone[squares[i][j]+1] << '|';
		s << endl;
	}
	return s.str();
}

//returns if player with val has some valid move in this configuration
bool Board::has_valid_move(int val) {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(move_is_valid(i+1, j+1, val))
				return true;
	return false;
}

//r and c zero indexed here
//checks whether path in direction rinc, cinc results in flips for val
//will actually flip the pieces along path when doFlips is true
bool Board::check_or_flip_path(int r, int c, int rinc, int cinc, int val, bool doFlips) {
	int pathr = r + rinc;
	int pathc = c + cinc;
	if(pathr < 0 || pathr > 7 || pathc < 0 || pathc > 7 || squares[pathr][pathc]!=-1*val)
		return false;
	//check for some chip of val's along the path:
	while(true) {
		pathr += rinc;
		pathc += cinc;
		if(pathr < 0 || pathr > 7 || pathc < 0 || pathc > 7 || squares[pathr][pathc]==0)
			return false;
		if(squares[pathr][pathc]==val) {
			if(doFlips) {
				pathr=r+rinc;
				pathc=c+cinc;
				while(squares[pathr][pathc]!=val) {
					squares[pathr][pathc]=val;
					pathr += rinc;
					pathc += cinc;
				}
			}
			return true;
		}
	}
	return false;	
}


//returns whether given move is valid in this configuration
bool Board::move_is_valid(int row, int col, int val) {
	int r = row-1;
	int c = col-1;
	if(r < 0 || r > 7 || c < 0 || c > 7)
		return false;
	//check whether space is occupied:
	if(squares[r][c]!=0)
		return false;
	//check that there is at least one path resulting in flips:
	for(int rinc = -1; rinc <= 1; rinc++)
		for(int cinc = -1; cinc <= 1; cinc++) {
			if(check_or_flip_path(r, c, rinc, cinc, val, false))
				return true;
		}
	return false;
}

//executes move if it is valid.  Returns false and does not update board otherwise
bool Board::play_square(int row, int col, int val) {
	if(!move_is_valid(row, col, val))
		return false;
	squares[row-1][col-1] = val;
	for(int rinc = -1; rinc <= 1; rinc++)
		for(int cinc = -1; cinc <= 1; cinc++) {
			check_or_flip_path(row-1, col-1, rinc, cinc, val, true);
		}
	return true;
}

//executes move if it is valid. Returns false and does not update board otherwise
//Makes computer make its move and returns the computer's move in row, and col
bool Board::play_square(int &row, int &col){
    return true;
}

bool Board::full_board() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(squares[i][j]==0)
				return false;
	return true;
}

//returns score, positive for X player's advantage
int Board::score() {
	int sum =0;
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			sum+=squares[i][j];
	return sum;
}

int Board::get_square(int row, int col) {
	return squares[row-1][col-1];
}

bool make_simple_cpu_move(Board * b, int cpuval) {
	for(int i=1; i<9;i++)
		for(int j=1; j<9; j++)
			if(b->get_square(i, j)==0)
				if(b->play_square(i, j, cpuval)) 
					return true;
	cout << "Computer passes." << endl;
	return false;
}

void play() {
	Board * b = new Board();
	int humanPlayer = 1;
	int cpuPlayer = -1;

	cout << b->toString();
	int consecutivePasses = 0;
	while(!b->full_board() && consecutivePasses<2) {
		int row, col;
		//check if player must pass:
		if(!b->has_valid_move(humanPlayer)) {
			cout << "You must pass." << endl;
			consecutivePasses++;
		}
		else {
            consecutivePasses = 0;
			cout << "Your move row (1-8): ";
			cin >> row;
			if(cin.fail()){
                 std::cerr<< "Illegal move."<<endl;
                 cin.clear();
                 cin.ignore();
                 continue;
            }
			cout << "Your move col (1-8): ";
			cin >> col;
			if(cin.fail()){
                 std::cerr<< "Illegal move."<<endl;
                 cin.clear();
                 cin.ignore();
                 continue;
            }
			if(!b->play_square(row, col, humanPlayer)) {
                  cout << "Illegal move." << endl;
			      continue;
            }        
		}
		//move for computer:
		if(b->full_board())
			break;
		else {
			cout << b->toString() << "..." << endl;
			if(make_simple_cpu_move(b, cpuPlayer))
				consecutivePasses=0;
			else
				consecutivePasses++;
			cout << b->toString();
		}
	}
	int score = b->score();
	if(score==0)
		cout << "Tie game." << endl;
	else if(score>0)
		cout << "Human wins by " << score << endl;
	else if(score<0)
		cout << "Computer wins by " << -score << endl;
	char a;
	cin >> a;
}

int main(int argc, char * argv[])
{
	play();
	return 0;
}

