// TicTacToe.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <cctype>
#include <vector>
#include <ctime>
#include "TeamMoop.h"

using namespace std;

/********************************************************
* Board Class Methods
********************************************************/

Moop::Moop() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			squares[i][j] = 0;
	squares[3][3]=-1;
	squares[4][4]=-1;
	squares[3][4]=1;
	squares[4][3]=1;
}

string Moop::toString() {
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
bool Moop::has_valid_move(int val) {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(move_is_valid(i+1, j+1, val))
				return true;
	return false;
}

//r and c zero indexed here
//checks whether path in direction rinc, cinc results in flips for val
//will actually flip the pieces along path when doFlips is true
bool Moop::check_or_flip_path(int r, int c, int rinc, int cinc, int val, bool doFlips) {
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
bool Moop::move_is_valid(int row, int col, int val) {
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
bool Moop::play_square(int row, int col, int val) {
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
bool Moop::play_square(int &row, int &col){
    return true;
}

bool Moop::full_board() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(squares[i][j]==0)
				return false;
	return true;
}

//returns score, positive for X player's advantage
int Moop::score() {
	int sum =0;
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			sum+=squares[i][j];
	return sum;
}

int Moop::get_square(int row, int col) {
	return squares[row-1][col-1];
}

/********************************************************
* Min / Max Functions
********************************************************/

int minValue(Moop* b, int cpuval, int alpha, int beta, clock_t tim, int depth, int maxDepth){

	/* Set opponent to have the opposite value of the CPU
	 * e.g. if CPU is Black, opponent is White */
	int oppoval = (cpuval * -1);

	float e_tim = ((float)clock()-tim)/CLOCKS_PER_SEC;

	// if termininumal case, return utility
	if(b->full_board() || (!b->has_valid_move(cpuval) && !b->has_valid_move(oppoval)) || e_tim > 14.95 || depth > maxDepth)
	{
		/* IF CPU is Black, return result of score function */
		if(cpuval == 1) return b->score();
		/* If CPU is White, return opposite of score function */
		else return (b->score() * -1);
	}

	// if no valid move exists for the oponent, let the cpu play again
	else if(!b->has_valid_move(oppoval))
	{
		return maxValue(b, cpuval, alpha, beta, tim, depth, maxDepth + 1);
	}

	// otherwise, recursive call to maxValue for all successors
	else
	{
		/* Create a vector of possible boards */
		vector <Moop*> test;
		/* Create a counter for the number of boards attempted */
		int count = 0;
		/* t is the value returned after trying a test move */
		int t;
		/* Min is the current mininumimum value returned */
		int mininum = 100;
		for(int i=1; i<9; i++){
			for(int j=1; j<9; j++){
				/* If a move is valid, try it */
				if(b->move_is_valid(i, j, oppoval)){
					/* Coppy current board into the array */
					test.push_back(new Moop(*b)); //may not work because test is a vector
					/* Play in the free square */
					test[count]->play_square(i, j, oppoval);
					/* Print current board */
					//cout << test[count]->toString();
					/* Find the maximumimum value from playing in the given square */
					t = maxValue(test[count], cpuval, alpha, beta, tim, depth, maxDepth + 1);
					/* If the current value t is less than the mininumimum value
					 * t is the new mininumimum value */
					if(t < mininum)
					{
						mininum = t;
						if (mininum <= alpha)
							return mininum;
						beta = min(beta, mininum);
					}
					count++;
				}
			}
		}
		/* Delete all of the test boards */
		for(int k=0; k<count; k++)
		{
			delete test[k];
		}
		/* Return the mininumimum value */
		return mininum;
	}
}

int maxValue(Moop* b, int cpuval, int alpha, int beta, clock_t tim, int depth, int maxDepth){
	// if termininumal case, return utility
	float e_tim = ((float)clock()-tim)/CLOCKS_PER_SEC;
	if(b->full_board() || (!b->has_valid_move(1) && !b->has_valid_move(-1)) || e_tim > 14.95 || depth > maxDepth)
	{
		/* IF CPU is Black, return result of score function */
		if(cpuval == 1) return b->score();
		/* If CPU is White, return opposite of score function */
		else return (b->score() * -1);
	}

	// if no valid move exists for the cpu, let the opponent play again
	else if(!b->has_valid_move(cpuval))
	{
		return minValue(b, cpuval, alpha, beta, tim, depth, maxDepth + 1);
	}

	// otherwise, recursive call to minValue for all successors
	else
	{
		/* Create a vector of possible boards */
		vector <Moop*> test;
		/* Create a counter for the number of boards attempted */
		int count = 0;
		/* t is the value returned after trying a test move */
		int t;
		/* Max is the current maximumimum value returned */
		int maximum = -100;
		for(int i=1; i<9; i++){
			for(int j=1; j<9; j++){
				/* If a move is valid, try it */
				if(b->move_is_valid(i, j, cpuval)){
					/* Copy current board into the array */
					test.push_back(new Moop(*b));
					/* Play in the free square */
					test[count]->play_square(i, j, cpuval);
					/* Print current board */
					//cout << test[count]->toString();
					/* Find the mininumimum value from playing in the given square */
					t = minValue(test[count], cpuval, alpha, beta, tim, depth, maxDepth + 1);
					/* If the current value t is greater than the maximumimum value
					 * t is the new maximumimum value */
					if(t > maximum) 
					{
						maximum = t;
						if (maximum >= beta)
							return maximum;
						alpha = max(alpha, maximum);
					}
					count++;
				}
			}
		}
		/* Delete all of the test boards */
		for(int k=0; k<count; k++)
		{
			delete test[k];
		}
		/* Return the maximumimum value */
		return maximum;
	}
}

/********************************************************
* CPU Move Function
********************************************************/
bool cpu_MiniMax_Move(Moop* b, int cpuval){

	if(!b->has_valid_move(cpuval))
	{
		cout << "Computer passes." << endl;
		return false;
	}

	/* Set initial alpha and beta */
	int alpha = -100;
	int beta = 100;
	/* Create a vector of possible boards */
	vector <Moop*> test;
	/* Create a counter for the number of boards attempted */
	int count = 0;
	/* Max is the current maximumimum value returned */
	int maximum = -100;
	/* t is the value returned after trying a test move */
	int t;
	/* Move stores the current best possible move */
	int move[2];

	//start timer to keep checking under 15 seconds
	clock_t tim = clock();
	float e_tim = ((float)clock()-tim)/CLOCKS_PER_SEC;

	//Depth tracker for iterative deepening
	int maxDepth = 0;
	
	while (e_tim < 14.95)
	{
		//get minValue for every possible move
		for(int i=1; i<9; i++){
			for(int j=1; j<9; j++){
				/* If a square is empty, try to play there */
				if(b->move_is_valid(i, j, cpuval)){
					/* Copy current board into the vector */
					test.push_back(new Moop(*b));
					/* Play in the free square */
					test[count]->play_square(i, j, cpuval);
					/* Find the mininumimum value from playing in the given square */
					t = minValue(test[count], cpuval, alpha, beta, tim, 1, maxDepth);
					//if minValue for a move is greater than current maximum,
					//CPU will make that move
					if(t > maximum)
					{
						maximum = t;
						move[0] = i;
						move[1] = j;
						if (maximum >= beta)
						{
							b->play_square(move[0], move[1], cpuval);
							return true;
						}
						alpha = max(alpha, maximum);
					}
					count++;
				}
			}
		}
		e_tim = ((float)clock()-tim)/CLOCKS_PER_SEC;
		maxDepth++;
	}
	/* Delete all of the test boards */
	for(int k=0; k<count; k++) delete test[k];
	//CPU making move
	b->play_square(move[0], move[1], cpuval);
	return true;
}

/********************************************************
* Play Function
********************************************************/

void play() {
	Moop * b = new Moop();
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
			if(cpu_MiniMax_Move(b, cpuPlayer))
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

/********************************************************
* Main Function
********************************************************/

int main(int argc, char * argv[])
{
	play();
	return 0;
}

/********************************************************
* Old Functions (To Be Deleted)
********************************************************/

bool make_simple_cpu_move(Moop * b, int cpuval) {
	for(int i=1; i<9;i++)
		for(int j=1; j<9; j++)
			if(b->get_square(i, j)==0)
				if(b->play_square(i, j, cpuval)) 
					return true;
	cout << "Computer passes." << endl;
	return false;
}

