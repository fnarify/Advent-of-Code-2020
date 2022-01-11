#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#define MAX_IN 200
#define DELIN  ","

int main()
{
    int i, len, goal, time, mintime, minid;
    int id[MAX_IN / 2];
    int64_t base, offset;
    char line[MAX_IN];
    char *parse;

    len = 0;
    while (fgets(line, sizeof(line), stdin))
    {
        if (!len) {goal = atoi(line); len = 1;}
    }

    len = 0;
    parse = strtok(line, DELIN);
    while (parse != NULL)
    {
        if (parse[0] == 'x') {id[len++] = 0;}
        else                 {id[len++] = atoi(parse);}
        parse = strtok(NULL, DELIN);
    }

    mintime = INT_MAX;
    for (i = 0; i < len; i++)
    {
        if (id[i])
        {
            time = id[i];
            while (time < goal) {time += id[i];}

            if (time < mintime) {mintime = time; minid = id[i];}
        }
    }
    printf("part 1: %d\n", (mintime - goal) * minid);

    /**
     * chinese remainder theorem
     * the values are all primes, used as modulus
     * This still works as long as the timestamps are coprime
     *
     * (i + base) congruent 0 (mod id[i])
     * this is done for all timestaps (id[i]) in i, such that they are congruent to 0
     *
     * base is set up to be our step factor for valid values
     * we basically increment our index (i) by base until we find a value that
     * is congruent (equal) to our previous system of simultaneous equations
     *
     * Or just use wolfram to solve it...
     */
    base = 0;
    offset = 1;
    for (i = 0; i < len; i++)
    {
        if (id[i])
        {
            while ((base + i) % id[i] != 0) {base += offset;}
            offset *= id[i]; // lcm(offset, id[i]) if they're not coprime
        }
    }
    printf("part 2: %I64d\n", base);

    return 0;
}
