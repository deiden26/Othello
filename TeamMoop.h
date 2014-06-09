/********************************************************
* Board Class
********************************************************/

class Moop {
	int squares[8][8];
	int cpuValue;
	int opponentValue;
	bool isEmpty;

public:
	Moop();
	std::string toString();
	bool play_square(int, int, int);
	bool play_square(int&, int&);
	bool move_is_valid(int, int, int);
	bool check_or_flip_path(int, int, int, int, int, bool);
	int get_square(int, int);
	int score();
	bool full_board();
	bool has_valid_move(int);
};

/********************************************************
* Function Declarations
********************************************************/
int minValue(Moop*, int, int, int, clock_t, int, int);
int maxValue(Moop*, int, int, int, clock_t, int, int);
void play();
bool make_simple_cpu_move(Moop*, int);
bool cpu_MiniMax_Move(Moop*, int, int&, int&);