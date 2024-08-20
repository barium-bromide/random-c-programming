#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROWS 7
#define COLS 7
#define GRID_SIZE 49
#define MAX 1000
#define MIN -1000
#define WINNING_LENGTH 5

struct Move
{
    int row, col;
};

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void printboard(char stringBoard[])
{
    for (int i = 0; i < 7; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            printf("%c", stringBoard[7 * i + j]);
        }
        printf("\n");
    }
}

short noMovesLeft(int board)
{
    return (board == (1ULL << (GRID_SIZE)) - 1);
}

int evaluate(unsigned long long board, unsigned long long playerBoard)
{
    // Define masks for rows, columns, and diagonals
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            // Row check
            if (col <= COLS - WINNING_LENGTH)
            {
                unsigned long long rowMask = 31 << (row * COLS + col);
                if ((playerBoard & rowMask) == rowMask)
                    return 10;
            }
            // Column check
            if (row <= ROWS - WINNING_LENGTH)
            {
                unsigned long long colMask = 0;
                colMask |= (270549121ULL << (row * COLS + col));
                if ((playerBoard & colMask) == colMask)
                    return 10;
            }
            // Diagonal check (bottom-right and bottom-left)
            if (row <= ROWS - WINNING_LENGTH && col <= COLS - WINNING_LENGTH)
            {
                unsigned long long diagMask = 0;
                unsigned long long antiDiagMask = 0;
                diagMask |= (4311810305ULL << (row * COLS + col));
                antiDiagMask |= (272696336ULL << (row * COLS + col));
                if (((playerBoard & diagMask) == diagMask) || ((playerBoard & antiDiagMask) == antiDiagMask))
                    return 10;
            }
        }
    }

    return 0;
}

int negamax(unsigned long long board, unsigned long long playerBoard, int depth, int alpha, int beta)
{
    int score = evaluate(board, playerBoard);
    if (score == 10 || score == -10 || depth == 0 || noMovesLeft(board))
        return score;

    int best = MIN;
    playerBoard = board ^ playerBoard;
    for (short i = 0; i < GRID_SIZE; ++i)
    {
        unsigned long long place = 1 << i;
        if (!(board & place))
        {
            // Negate the result of the recursive call and swap the roles
            int val = 0;
            val = -negamax((board | place), (playerBoard | place), depth - 1, -beta, -alpha);
            if (depth == 2 && val == 0)
                printf("Val %d , depth %d\n", val, depth);
            if (depth == 1 && val != 0)
                printf("Val %d , depth %d\n", val, depth);
            best = (best > val) ? best : val;
            alpha = (alpha > best) ? alpha : best;
            if (beta <= alpha)
                break;
        }
    }
    return best;
}

// Find the best move for the AI
struct Move findBestMove(unsigned long long board, unsigned long long playerBoard)
{
    int bestval = MIN;
    struct Move bestmove = {-1, -1};

    for (short i = 0; i < GRID_SIZE; ++i)
    {
        if (!(board & (1ULL << i)))
        {
            board |= (1ULL << i);
            playerBoard |= (1ULL << i);

            int moveVal = negamax(board, playerBoard, 2, MIN, MAX);
            board ^= (1ULL << i);
            playerBoard ^= (1ULL << i);

            if (moveVal > bestval)
            {
                bestmove.row = i / COLS;
                bestmove.col = i % COLS;
                bestval = moveVal;
            }
        }
    }
    printf("Best move value: %d\n", bestval);
    return bestmove;
}

int main()
{
    char stringBoard[GRID_SIZE];
    memset(stringBoard, '-', GRID_SIZE);

    unsigned long long board = 120;
    unsigned long long playerBoard = 0;

    double startTime = (float)clock() / CLOCKS_PER_SEC;
    struct Move bestmove = findBestMove(board, playerBoard);
    double endTime = (float)clock() / CLOCKS_PER_SEC;
    double timeElapsed = endTime - startTime;
    printf("Best move is (%d , %d)\n", bestmove.col, bestmove.row);
    printf("%lf\n", timeElapsed);
    // struct Move you;
    // while (!noMovesLeft(board))
    // {
    //     printf("X: ");
    //     scanf("%d", &(you.col));
    //     int c;
    //     clear_input_buffer();
    //     printf("Y:");
    //     scanf("%d", &(you.row));
    //     clear_input_buffer();
    //     int position = you.row * COLS + you.col;
    //     board |= (1ULL << position);
    //     playerBoard |= (1ULL << position);
    //     stringBoard[position] = 'X';

    //     if (noMovesLeft(board))
    //         break;

    //     struct Move bestmove = findBestMove(board, (board ^ playerBoard));
    //     int bestPosition = bestmove.row * COLS + bestmove.col;
    //     board |= (1ULL << bestPosition);
    //     stringBoard[bestPosition] = 'O';

    //     printboard(stringBoard);
    // }
    return 0;
}