#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM   100
#define N_VEC 8

enum state{FLOOR = '.', EMPTY = 'L', OCCUP = '#'};
int adj[] = {[FLOOR] = 0, [EMPTY] = 0, [OCCUP] = 0, [' '] = 0};

// (x, y) although it doesn't matter
const int vec[N_VEC][2] = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1,  0},          {1,  0},
    {-1,  1}, {0,  1}, {1,  1}
};

/**
 * check immediate adjacencies of the point x, y in grid
 * we only care about occupied seats
 */
char checkadj(int x, int y, int w, int h, char grid[DIM][DIM])
{
    int n, dx, dy;
    char ret = grid[y][x];

    adj[OCCUP] = 0;
    if (grid[y][x] != FLOOR)
    {
        for (n = 0; n < N_VEC; n++)
        {
            dx = x + vec[n][0];
            dy = y + vec[n][1];
            if (dx < w && dx >= 0 && dy < h && dy >= 0) {adj[(int) grid[dy][dx]]++;}
        }

        if      (grid[y][x] == EMPTY && adj[OCCUP] == 0) {ret = OCCUP;}
        else if (grid[y][x] == OCCUP && adj[OCCUP] >= 4) {ret = EMPTY;}
    }

    return ret;
}

/**
 * check what tiles are visible from the point x, y along each of the 8 directions
 * in the grid, the only tile we actually care to count is occupied ones (OCCUP)
 *
 * when checking tiles, you stop on the first visible tile that isn't the floor as that blocks
 * the view of the origin tile
 */
char checkdiag(int x, int y, int w, int h, char grid[DIM][DIM])
{
    int dx, dy, n, fnd;
    char ret = grid[y][x];

    adj[OCCUP] = 0;
    if (grid[y][x] != FLOOR)
    {
        for (n = 0; n < N_VEC; n++)
        {
            fnd = 0;
            dx = x + vec[n][0];
            dy = y + vec[n][1];
            while (!fnd && dx < w && dx >= 0 && dy < h && dy >= 0)
            {
                // stop on first tile that isn't the floor!
                if (grid[dy][dx] != FLOOR)
                {
                    adj[(int) grid[dy][dx]]++;
                    fnd = 1;
                }

                dx += vec[n][0];
                dy += vec[n][1];
            }
        }

        if      (grid[y][x] == EMPTY && adj[OCCUP] == 0) {ret = OCCUP;}
        else if (grid[y][x] == OCCUP && adj[OCCUP] >= 5) {ret = EMPTY;}
    }

    return ret;
}

/**
 * run 'cellular automata'
 * flag set to 1 for pt1, 2 for pt2
 */
int solve(char grid[DIM][DIM], int w, int h, int flag)
{
    int i, j, fnd, sum;
    char newgrid[DIM][DIM];
    memset(newgrid, ' ', sizeof(newgrid));

    fnd = 0;
    while (!fnd)
    {
        for (i = 0; i < h; i++)
        {
            for (j = 0; j < w; j++)
            {
                if      (flag == 1) {newgrid[i][j] = checkadj(j, i, w, h, grid);}  // pt1
                else if (flag == 2) {newgrid[i][j] = checkdiag(j, i, w, h, grid);} // pt2
            }
        }

        i = 0;
        while (!memcmp(grid, newgrid, sizeof(newgrid)) && i < h) {i++;}
        if (i == h) {fnd = 1;}
        else        {memcpy(grid, newgrid, sizeof(newgrid));}
    }
    
    sum = 0;
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            if (newgrid[i][j] == OCCUP) {sum++;}
        }
    }
    return sum;
}

/**
 * These arrays are NOT null-terminated if you want to print them
 */
int main()
{
    int w, h;
    char line[DIM + 2];
    // exterior of size 1 around the perimeter to make calculation easier
    char grid[DIM][DIM];
    char orig[DIM][DIM];

    memset(grid, ' ', sizeof(grid));

    h = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        // trim new-line, no null-terminator
        if (!h) {w = strlen(line) - 1;}
        memcpy(grid[h++], &line, w);
    }

    memcpy(orig, grid, sizeof(orig));
    printf("part 1: %d seats occupied\n", solve(grid, w, h, 1));
    memcpy(grid, orig, sizeof(grid));
    printf("part 2: %d seats occupied\n", solve(grid, w, h, 2));

    return 0;
}
