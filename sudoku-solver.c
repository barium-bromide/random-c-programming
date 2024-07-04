// solve any sudoku

#include <stdio.h>

int is_valid(int sudoku[9][9], int row, int column, int number)
{
    for (int i = 0; i < 9; i++)
        if (sudoku[row][i] == number)
            return 0;

    for (int i = 0; i < 9; i++)
        if (sudoku[i][column] == number)
            return 0;

    int mini_grid_start_row = row - row % 3;
    int mini_grid_start_column = column - column % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (sudoku[i + mini_grid_start_row][j + mini_grid_start_column] == number)
                return 0;

    return 1;
}

int solve(int sudoku[9][9], int row, int column)
{
    if (row == 8 && column == 9)
        return 1;

    if (column == 9)
    {
        row++;
        column = 0;
    }
    if (sudoku[row][column] > 0)
        return solve(sudoku, row, column + 1);

    for (int i = 1; i < 10; i++)
    {
        if (is_valid(sudoku, row, column, i))
        {
            sudoku[row][column] = i;
            if (solve(sudoku, row, column + 1))
                return 1;
        }

        sudoku[row][column] = 0;
    }
    return 0;
}

int main()
{
    int sudoku[9][9];

    FILE *fp;
    char filename[] = "sudoku.txt";

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            fscanf(fp, "%1d", &sudoku[i][j]);
    }
    fclose(fp);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            printf("%d ", sudoku[i][j]);
        printf("\n");
    }

    if (solve(sudoku, 0, 0))
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
                printf("%d ", sudoku[i][j]);
            printf("\n");
        }
    }
    else
    {
        printf("impossible");
    }

    return 0;
}