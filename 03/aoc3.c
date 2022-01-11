#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 400
#define OPEN '.'
#define TREE '#'

int checkslope(char map[MAX_HEIGHT][MAX_WIDTH], int h, int w, int dx, int dy)
{
    int i, j, ntree;
    for (i = dx, j = dy, ntree = 0; i < h; i += dx, j += dy)
    {
        j %= w;
        if (map[i][j] == TREE) {ntree++;}
    }
    return ntree;
}

int main()
{
    int w, h;
    int64_t sum;
    char map[MAX_HEIGHT][MAX_WIDTH];

    w = h = 0;
    while (fgets(map[h], sizeof(map[h]), stdin)) {h++;}
    w = strlen(map[0]) - 1;

    sum = checkslope(map, h, w, 1, 3);
    printf("part 1: %lld trees\n", sum);

    // overflows on 32-bit, needs c99 for no warnings
    sum *= checkslope(map, h, w, 1, 1) * checkslope(map, h, w, 1, 5) * checkslope(map, h, w, 1, 7) * checkslope(map, h, w, 2, 1);
    printf("part 2: %lld\n", sum);

    return 0;
}
