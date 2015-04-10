#define COUNT_FUNC 8
#define COUNT_STEP 4
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4

#define DEBUG_MODE 0
#define PLAY_MODE 1
#define GA_DEBUG_MODE 0

typedef int Board[BOARD_HEIGHT][BOARD_WIDTH];
typedef int (*Movement)(Board, Board);
typedef int (*ScoreFuncPtr)(Board);

int maxLevel = 1;
int TotalScore = 0;
//int weightParameter[COUNT_FUNC] = {54,70,35,-5,-20,68,48,-44};
