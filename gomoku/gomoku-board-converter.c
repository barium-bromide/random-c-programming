#include <stdio.h>

char stringBoard[49] = "X------"
                       "-X-----"
                       "--XXXX-"
                       "--XXX--"
                       "--XX---"
                       "--X----"
                       "-------";

char spareStringBoard[49];

void stringBoardToDecimal(char stringBoard[], unsigned long long *pboard, unsigned long long *pplayerBoard)
{
    unsigned long long board = 0;
    unsigned long long playerBoard = 0;
    for (int i = 0; i < 49; ++i)
    {
        if (stringBoard[i] == 'X' || stringBoard[i] == 'O')
        {
            board |= (1ULL << i);
            if (stringBoard[i] == 'X')
                playerBoard |= (1ULL << i);
        }
    }
    *pboard = board;
    *pplayerBoard = playerBoard;
}

void decimalToStringBoard(unsigned long long board, unsigned long long playerBoard, char *stringBoard)
{
    for (int i = 0; i < 49; ++i)
    {
        if (board & (1ULL << i))
        {
            if (playerBoard & (1ULL << i))
                stringBoard[i] = 'X';
            else
                stringBoard[i] = 'O';
        }
        else
            stringBoard[i] = '-';
    }
}

int main()
{
    unsigned long long board, playerBoard;
    board = 26392575123968;
    playerBoard = 26392575123968;
    decimalToStringBoard(board, playerBoard, spareStringBoard);
    for (int i = 0; i < 49; ++i)
    {
        printf("%c", spareStringBoard[i]);
        if ((i + 1) % 7 == 0)
            printf("\n");
    }

    // stringBoardToDecimal(stringBoard, &board, &playerBoard);
    // printf("board: %llu\n", board);
    // printf("playerBoard: %llu\n", playerBoard);
    return 0;
}