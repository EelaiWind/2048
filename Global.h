#define COUNT_FUNC 10
#define COUNT_STEP 4
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4
#define ROUND 10

#define DEBUG_MODE 0
#define PLAY_MODE 1
#define GA_DEBUG_MODE 0
#define INPUT_MODE 0
#define EVALE_MODE 0

typedef int Board[BOARD_HEIGHT][BOARD_WIDTH];
typedef int (*Movement)(Board, Board);
typedef int (*ScoreFuncPtr)(Board);

int TotalScore = 0;
int sign[2][COUNT_FUNC] = {{1,1,1,1,1,1,1,1,1,1},{1,1,1,-1,1,1,1,-1,1,-1}};
