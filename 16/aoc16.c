#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_CHK     2 // a-b (1) or c-d (2)
#define N_RULE    20
#define MAX_LINE  200
#define MAX_RULE  300
#define DELIN     ","

const char *rulestr[] = {
    "departure location", "departure station", "departure platform", "departure track", "departure date", "departure time",
    "arrival location", "arrival station", "arrival platform", "arrival track",
    "class", "duration", "price", "route", "row", "seat", "train", "type", "wagon", "zone"
};

int main()
{
    int i, iteration;
    int map[N_RULE] = {0};
    int rule[N_RULE][2 * N_CHK];
    int (*ticket)[N_RULE]; // 0 is your ticket
    char line[MAX_LINE], name[MAX_LINE / 2];
    char *parse;

    ticket = calloc(MAX_RULE, sizeof(*ticket));
    if (!ticket) {return -1;}

    // might be able to solve it with bitmap masks
    i = iteration = 0;
    while (fgets(line, sizeof(line), stdin))
    {
        if (line[i] == '\n')
        {
            iteration++;
            fgets(line, sizeof(line), stdin); // throwaway
            continue;
        }

        if (!iteration)
        {
            sscanf(line, "%[^:] %d-%d or %d-%d\n", name, &rule[i][0], &rule[i][1], &rule[i][2], &rule[i][3]);
            i++;
        }
        else if (iteration == 1) // own ticket
        {
            parse = strtok(line, DELIN);
            while (parse != NULL)
            {
                parse = strtok(NULL, DELIN);
            }
        }
        else // other tickets
        {
        }
    }

    free(ticket);

    return 0;
}

