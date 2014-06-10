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
	Moop(const Moop& b);
	std::string toString();
	bool play_square(int, int, int);
	bool play_square(int&, int&);
	bool move_is_valid(int, int, int);
	bool check_or_flip_path(int, int, int, int, int, bool);
	int get_square(int, int);
	int score();
	bool full_board();
	bool has_valid_move(int);
  float piece_difference();
  int corner_occupancy();
  float corner_closeness();
  bool is_frontier_disc(int, int);      
  float frontier_disc_ratio();
  int board_position_value();
  float mobility();
  float evaluation_output();
};

/********************************************************
* Function Declarations
********************************************************/
int minValue(Moop*, int, int, int, clock_t, int, int);
int maxValue(Moop*, int, int, int, clock_t, int, int);
void play();
bool make_simple_cpu_move(Moop*, int);
bool cpu_MiniMax_Move(Moop*, int, int&, int&);
