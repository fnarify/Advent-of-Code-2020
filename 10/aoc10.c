#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define GAP      3
#define MAX_LINE 10
#define MAX_INP  150

/**
 * use sort n in vim before running this or equivalent, too lazy to use
 * quick sort or write a cheap selection sort right now
 *
 * need -std=c99
 */
int main()
{
    char line[MAX_LINE];
    int i, len, cur;
    int diff[4] = {0};
    int value[MAX_INP];
    int64_t *map;

    len = 0;
    value[len++] = 0;
    while (fgets(line, sizeof(line), stdin) != NULL) {value[len++] = atoi(line);}
    value[len] = value[len - 1] + GAP; // 3-joltage gap accounted for, final socket
    len++;

    cur = 0;
    for (i = 0; i < len; i++)
    {
        diff[value[i] - cur]++;
        cur = value[i];
    }
    printf("part 1: %d\n", diff[1] * diff[3]);

    map = calloc(value[len - 1] + 1, sizeof(*map));
    if (!map) {return -1;}

    // slide gap, max 3 prior dependencies
    // iterate over values, update map with number of branches of the last 3 (GAP) entries prior
    // basically like a triple sum fibonacci, best way to visualise it is to draw the first few sums
    for (i = 0; i < len; i++)
    {
        if      (value[i] == 0) {map[value[i]] = 1;}
        else if (value[i] == 1) {map[value[i]] = map[value[i] - 1];}
        else if (value[i] == 2) {map[value[i]] = map[value[i] - 1] + map[value[i] - 2];}
        else                    {map[value[i]] = map[value[i] - 1] + map[value[i] - 2] + map[value[i] - 3];}

        /** harder to read switch fall-though version
        switch(value[i])
        {
            default:
                map[value[i]] += map[value[i] - 3];
            case 2:
                map[value[i]] += map[value[i] - 2];
            case 1:
                map[value[i]] += map[value[i] - 1];
                break;
            case 0:
                map[value[i]] = 1;
                break;
        }
        */
    }

    // should typically be at the very last element of the list,
    // otherwise take the max value in map
    printf("part 2: %I64d\n", map[value[len - 1]]);

    free(map);

    return 0;
}
