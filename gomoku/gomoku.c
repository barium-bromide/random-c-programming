#undef _GLIBCXX_DEBUG  // disable run-time bound checking, etc
#pragma GCC optimize( \
    "Ofast,inline")  // Ofast =
                     // O3,fast-math,allow-store-data-races,no-protect-parens

#pragma GCC target("bmi,bmi2,lzcnt,popcnt")  // bit manipulation
#pragma GCC target("movbe")                  // byte swap
#pragma GCC target("aes,pclmul,rdrnd")       // encryption
#pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2")  // SIMD

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <x86intrin.h>

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

int noMovesLeft(unsigned long long board)
{
    // return (board == ((1ULL << GRID_SIZE) - 1));
    return __builtin_popcount(board) == GRID_SIZE;
}

unsigned long long possibleMask(unsigned long long board)
{
    return (((1ULL << GRID_SIZE) - 1ULL) ^ board);
}

unsigned long long winningMask(unsigned long long board, unsigned long long playerBoard)
{
    unsigned long long playMask = possibleMask(board);
    unsigned long long winningMask = 0;
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            if (col <= (COLS - WINNING_LENGTH + 1))
            {
                unsigned long long rowMask = 15ULL << (row * COLS + col);
                if ((playerBoard & rowMask) == rowMask)
                {
                    if (col != 0)
                        winningMask |= (playMask & (1ULL << (row * COLS + col - 1)));
                    if (col != (COLS - WINNING_LENGTH + 1))
                        winningMask |= (playMask & (1ULL << (row * COLS + col + 4)));
                }
            }
            if (row <= (ROWS - WINNING_LENGTH + 1))
            {
                unsigned long long colMask = 0;
                colMask |= (2113665ULL << (row * COLS + col));
                if ((playerBoard & colMask) == colMask)
                {
                    if (row != 0)
                    {
                        winningMask |= (playMask & (1ULL << ((row - 1) * COLS + col)));
                    }
                    if (row != (ROWS - WINNING_LENGTH + 1))
                        winningMask |= (playMask & (1ULL << ((row + 4) * COLS + col)));
                }
            }
            if (row <= (ROWS - WINNING_LENGTH + 1) && col <= (COLS - WINNING_LENGTH + 1))
            {
                unsigned long long diagMask = 0;
                unsigned long long antiDiagMask = 0;
                diagMask |= (16843009ULL << (row * COLS + col));
                antiDiagMask |= (2130440ULL << (row * COLS + col));
                if (((playerBoard & diagMask) == diagMask))
                {
                    if (row != 0 && col != 0)
                        winningMask |= (playMask & (1ULL << ((row - 1) * COLS + col - 1)));
                    if (row != (ROWS - WINNING_LENGTH + 1) && col != (COLS - WINNING_LENGTH + 1))
                        winningMask |= (playMask & (1ULL << ((row + 4) * COLS + col + 4)));
                }
                if (((playerBoard & antiDiagMask) == antiDiagMask))
                {
                    if (row != 0 && col != (COLS - WINNING_LENGTH + 1))
                        winningMask |= (playMask & (1ULL << ((row - 1) * COLS + col + 4)));
                    if (row != (ROWS - WINNING_LENGTH + 1) && col != 0)
                        winningMask |= (playMask & (1ULL << ((row + 4) * COLS + col - 1)));
                }
            }
        }
    }
    return winningMask;
}

void possibleMoves(unsigned long long board, unsigned long long *array)
{
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        if (!(board & (1ULL << i)))
            array[i] = i;
        else
            array[i] = -1;
    }
}

// int evaluate_win(unsigned long long playerBoard)
// {
//     // Define masks for rows, columns, and diagonals
//     for (int row = 0; row < ROWS; ++row)
//     {
//         for (int col = 0; col < COLS; ++col)
//         {
//             // Row check
//             if (col <= (COLS - WINNING_LENGTH))
//             {
//                 unsigned long long rowMask = 31ULL << (row * COLS + col);
//                 if ((playerBoard & rowMask) == rowMask)
//                     return 10;
//             }
//             // Column check
//             if (row <= (ROWS - WINNING_LENGTH))
//             {
//                 unsigned long long colMask = 0;
//                 colMask |= (270549121ULL << (row * COLS + col));
//                 if ((playerBoard & colMask) == colMask)
//                     return 10;
//             }
//             // Diagonal check (bottom-right and bottom-left)
//             if (row <= (ROWS - WINNING_LENGTH) && col <= (COLS - WINNING_LENGTH))
//             {
//                 unsigned long long diagMask = 0;
//                 unsigned long long antiDiagMask = 0;
//                 diagMask |= (4311810305ULL << (row * COLS + col));
//                 antiDiagMask |= (272696336ULL << (row * COLS + col));
//                 if (((playerBoard & diagMask) == diagMask) || ((playerBoard & antiDiagMask) == antiDiagMask))
//                     return 10;
//             }
//         }
//     }

//     return 0;
// }

int evaluate_win(unsigned long long playerBoard)
{
    // Define masks for rows, columns, and diagonals
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            const unsigned int position_to_compare = (unsigned int)(playerBoard >> (row * COLS + col));
            // Row check
            if (col <= (COLS - WINNING_LENGTH))
            {
                // unsigned int rowMask = 31;
                if ((position_to_compare & 31U) == 31U)
                // if (__builtin_popcount((position_to_compare & 31U)) == 5)
                    return 10;
            }
            // Column check
            if (row <= (ROWS - WINNING_LENGTH))
            {
                // unsigned long long colMask = 270549121ULL;
                if ((position_to_compare & 270549121U) == 270549121U)
                // if (__builtin_popcount((position_to_compare & 270549121U)) == 5)
                    return 10;
            }
            // Diagonal check (bottom-right and bottom-left)
            if (row <= (ROWS - WINNING_LENGTH) && col <= (COLS - WINNING_LENGTH))
            {
                // unsigned long long diagMask = 4311810305ULL;
                // unsigned long long antiDiagMask = 272696336U;
                if ((((unsigned long long)position_to_compare & 4311810305ULL) == 4311810305ULL) || ((position_to_compare & 272696336U) == 272696336U))
                // if (__builtin_popcount((position_to_compare & 4311810305ULL)) == 5 || __builtin_popcount((position_to_compare & 272696336U)) == 5)
                    return 10;
            }
        }
    }

    return 0;
}

int negamax(unsigned long long board, unsigned long long playerBoard, int depth, int alpha, int beta)
{
    int score = evaluate_win(playerBoard);
    if (score == 10 || depth == 0 || noMovesLeft(board))
        return -score;

    int best = MIN;
    playerBoard = board ^ playerBoard;
    // unsigned long long forceWinMask = winningMask(board, playerBoard);
    unsigned long long forceWinMask = 0;
    unsigned long long playMask;
    if (forceWinMask)
    {
        if (forceWinMask & (forceWinMask - 1))
            return 10;
        else
            return 9;
    }
    playMask = possibleMask(board);
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        if (1ULL & playMask >> i)
        {
            int val = -negamax((board | (1ULL << i)), (playerBoard | (1ULL << i)), depth - 1, -beta, -alpha);
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

    for (int i = 0; i < GRID_SIZE; ++i)
        if (!(board & (1ULL << i)))
            if (evaluate_win((playerBoard | (1ULL << i))))
            {
                bestmove.row = i / COLS;
                bestmove.col = i % COLS;
                return bestmove;
            }
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        if (!(board >> i & 1ULL))
        {
            int moveVal = -negamax((board | (1ULL << i)), (playerBoard | (1ULL << i)), 6, MIN, MAX);

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

    char nofight = 1;

    if (nofight)
    {
        unsigned long long board = 0;
        unsigned long long playerBoard = 0;
        double startTime = (float)clock() / CLOCKS_PER_SEC;
        struct Move bestmove = findBestMove(board, playerBoard);
        double endTime = (float)clock() / CLOCKS_PER_SEC;
        double timeElapsed = endTime - startTime;
        printf("Best move is (%d , %d)\n", bestmove.col, bestmove.row);
        printf("%lf\n", timeElapsed);
    }
    else
    {
        unsigned long long board = 0;
        unsigned long long playerBoard = 0;
        struct Move you;
        while (!noMovesLeft(board))
        {
            printf("X: ");
            scanf("%d", &(you.col));
            int c;
            clear_input_buffer();
            printf("Y:");
            scanf("%d", &(you.row));
            clear_input_buffer();
            int position = you.row * COLS + you.col;
            board |= (1ULL << position);
            playerBoard |= (1ULL << position);
            stringBoard[position] = 'X';
            printf("%llu %llu", board, playerBoard);

            if (noMovesLeft(board) || evaluate_win(playerBoard))
                break;

            double startTime = (float)clock() / CLOCKS_PER_SEC;
            struct Move bestmove = findBestMove(board, (board ^ playerBoard));
            double endTime = (float)clock() / CLOCKS_PER_SEC;
            double timeElapsed = endTime - startTime;
            printf("%lf\n", timeElapsed);
            int bestPosition = bestmove.row * COLS + bestmove.col;
            board |= (1ULL << bestPosition);
            stringBoard[bestPosition] = 'O';

            printboard(stringBoard);
        }
    }
    return 0;
}