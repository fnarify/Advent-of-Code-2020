#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 50
#define N_ROW    128
#define N_COL    8
#define ROW_SIZE 8
#define COL_SIZE 3
#define EMPTY    ' '
#define OCCUP    'X'

int main()
{
    int i, j, row, col, low, high, max, fnd;
    char line[MAX_LINE];

    char plane[N_ROW][N_COL];
    memset(plane, EMPTY, sizeof(plane));

    max = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        // remove trailing new-line
        line[strcspn(line, "\r\n")] = '\0';

        // 8 row characters, 3 column
        low = 0;
        high = N_ROW - 1;
        for (i = 0; i < ROW_SIZE; i++)
        {
            if      (line[i] == 'F') {high = (low + high) / 2;}
            else if (line[i] == 'B') {low = ((low + high) / 2) + 1;}
        }
        row = low;

        low = 0;
        high = N_COL - 1;
        for (i = -1; i < COL_SIZE; i++)
        {
            if      (line[i + ROW_SIZE] == 'R') {low = ((low + high) / 2) + 1;}
            else if (line[i + ROW_SIZE] == 'L') {high = (low + high) / 2;}
        }
        col = low;

        // could use binary encoding with how the input is defined
        if (row * 8 + col > max) {max = row * 8 + col;}

        plane[row][col] = OCCUP;
    }
    printf("part 1: %d highest seat id\n", max);

    /**
     * easy enough to do by eye, otherwise search for an element
     * with a seat taken at both adjacent positions (horizontal only)
     * technically not at the very front/back, so you could skip 'some' rows
     */
    fnd = 0;
    for (i = 0; i < N_ROW && !fnd; i++)
    {
        for (j = 1; j < N_COL - 1; j++)
        {
            if (plane[i][j] == EMPTY && plane[i][j - 1] == OCCUP && plane[i][j + 1] == OCCUP)
            {
                printf("part 2: %d my seat id\n", i * 8 + j);
                fnd = 1;
                break;
            }
        }
    }

    return 0;
}
