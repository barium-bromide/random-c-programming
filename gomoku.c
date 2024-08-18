#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

short noMovesLeft(int board)
{
    return (board == (1ULL << (GRID_SIZE)) - 1);
}

int evaluate(unsigned long long board, unsigned long long playerBoard)
{
    unsigned long long enemyBoard = board ^ playerBoard;

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
                if ((enemyBoard & rowMask) == rowMask)
                    return -10;
            }
            // Column check
            if (row <= ROWS - WINNING_LENGTH)
            {
                unsigned long long colMask = 0;
                colMask |= (270549121ULL << (row * COLS + col));
                if ((playerBoard & colMask) == colMask)
                    return 10;
                if ((enemyBoard & colMask) == colMask)
                    return -10;
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
                if (((enemyBoard & diagMask) == diagMask) || ((enemyBoard & antiDiagMask) == antiDiagMask))
                    return -10;
            }
        }
    }

    return 0;
}

// Minimax algorithm with alpha-beta pruning
int minimax(unsigned long long board, unsigned long long playerBoard, int depth, short isMax, int alpha, int beta)
{
    int score = evaluate(board, playerBoard);
    if (score == 10 || score == -10 || depth == 0 || noMovesLeft(board))
        return score;

    if (isMax)
    {
        int best = MIN;
        for (short i = 0; i < GRID_SIZE; ++i)
        {
            if (!(board & (1ULL << i)))
            {
                board |= (1ULL << i);
                playerBoard |= (1ULL << i);
                int val = minimax(board, (board ^ playerBoard), depth - 1, 0, alpha, beta);
                best = (best > val) ? best : val;
                alpha = (alpha > best) ? alpha : best;
                if (beta <= alpha)
                    break;
                board ^= (1ULL << i);
                playerBoard ^= (1ULL << i);
            }
        }
        return best;
    }
    else
    {
        int best = MAX;
        for (short i = 0; i < GRID_SIZE; ++i)
        {
            if (!(board & (1ULL << i)))
            {
                board |= (1ULL << i);
                playerBoard |= (1ULL << i);
                int val = minimax(board, (board ^ playerBoard), depth - 1, 1, alpha, beta);
                best = (best < val) ? best : val;
                beta = (beta < best) ? beta : best;
                if (beta <= alpha)
                    break;
                board ^= (1ULL << i);
                playerBoard ^= (1ULL << i);
            }
        }
        return best;
    }
}

int negamax(unsigned long long board, unsigned long long playerBoard, int depth, int alpha, int beta)
{
    int score = evaluate(board, playerBoard);
    if (score == 10 || score == -10 || depth == 0 || noMovesLeft(board))
        return score;

    int best = MIN;
    for (short i = 0; i < GRID_SIZE; ++i)
    {
        if (!(board & (1ULL << i)))
        {
            board |= (1ULL << i);
            playerBoard |= (1ULL << i);
            // Negate the result of the recursive call and swap the roles
            int val = -negamax(board, (board ^ playerBoard), depth - 1, -beta, -alpha);
            best = (best < val) ? best : val;
            alpha = (alpha < best) ? alpha : best;
            if (beta <= alpha)
                break;
            board ^= (1ULL << i);
            playerBoard ^= (1ULL << i);
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

            int moveVal = negamax(board, playerBoard, 5, MIN, MAX);

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
    unsigned long long board = 60;
    unsigned long long playerBoard = 60;
    double startTime = (float)clock() / CLOCKS_PER_SEC;
    struct Move bestmove = findBestMove(board, playerBoard);
    double endTime = (float)clock() / CLOCKS_PER_SEC;
    double timeElapsed = endTime - startTime;
    printf("Best move is (%d , %d)\n", bestmove.col, bestmove.row);
    printf("%lf", timeElapsed);
    return 0;
}