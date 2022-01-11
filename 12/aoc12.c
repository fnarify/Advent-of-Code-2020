#include <stdio.h>
#include <stdlib.h>

#define MAX_IN 10

enum action{FORWARD = 'F', NORTH = 'N', SOUTH = 'S', EAST = 'E', WEST = 'W', LEFT = 'L', RIGHT = 'R'};

int direction[][2] = {
    [NORTH] = {0, -1},
    [SOUTH] = {0, 1},
    [EAST]  = {1, 0},
    [WEST]  = {-1, 0}
};

int rotate[][3] = {
    [NORTH] = {EAST, SOUTH, WEST},
    [SOUTH] = {WEST, NORTH, EAST},
    [EAST]  = {SOUTH, WEST, NORTH},
    [WEST]  = {NORTH, EAST, SOUTH}
};

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[])
{
    char move, facing;
    int origx, origy, wayptx, waypty, x, y, val;
    char line[MAX_IN];
    FILE *fp;
    
    if (argc != 2) {printf("filename needed as argv\n"); return -1;}
    fp = fopen(argv[1], "r");
    if (!fp) {printf("cannot open file named %s\n", argv[1]); return -2;}

    facing = EAST;
    origx = origy = x = y = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%c%d\n", &move, &val) == 2)
        {
            switch(move)
            {
                case FORWARD:
                    x += direction[(int) facing][0] * val;
                    y += direction[(int) facing][1] * val;
                    break;
                case NORTH: case SOUTH: case EAST: case WEST:
                    x += direction[(int) move][0] * val;
                    y += direction[(int) move][1] * val;
                    break;
                case LEFT:
                    val = -val + 360; // fall through
                case RIGHT:
                    val %= 360;
                    facing = rotate[(int) facing][(val / 90) - 1];
                    break;
            }
        }
    }
    printf("part 1: %d distance from origin\n", abs(x - origx) + abs(y - origy));

    fseek(fp, 0, SEEK_SET);

    // waypoint moves relative with the ship
    origx = origy = x = y = 0;
    wayptx = 10;
    waypty = -1;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%c%d\n", &move, &val) == 2)
        {
            switch(move)
            {
                case FORWARD:
                    x += val * wayptx;
                    y += val * waypty;
                    break;
                case NORTH: case SOUTH: case EAST: case WEST:
                    wayptx += direction[(int) move][0] * val;
                    waypty += direction[(int) move][1] * val;
                    break;
                case LEFT: // fall through
                    val *= -1;
                case RIGHT:
                    // normalise angles, then rotate vector projection(!) from origin
                    val = (val + 360) % 360;
                    if      (val == 90)  {swap(&wayptx, &waypty); wayptx *= -1;}
                    else if (val == 180) {wayptx *= -1; waypty *= -1;}
                    else if (val == 270) {swap(&wayptx, &waypty); waypty *= -1;}
                    break;
            }
        }
    }
    printf("part 2: %d distance from origin\n", abs(x - origx) + abs(y - origy));

    fclose(fp);

    return 0;
}
