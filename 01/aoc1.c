#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 20
#define MAX_IN   3000

int main()
{
    int i, j, nval, temp, fnd1, fnd2;
    char line[MAX_LINE];
    char *hash;
    int *vals;

    hash = calloc(MAX_IN, sizeof(*hash));
    vals = malloc(sizeof(*vals) * MAX_IN);
    if (!hash || !vals) {return -1;}

    i = 0;
    while (fgets(line, sizeof(line), stdin) && i < MAX_IN)
    {
        temp = atoi(line);
        vals[i++] = temp;
        if (temp < MAX_IN) {hash[temp] = 1;}
        else {printf("val too large for hashmap\n");}
    }
    nval = i;

    fnd1 = fnd2 = 0;
    for (i = 0; i < nval; i++)
    {
        temp = 2020 - vals[i];
        if (temp && hash[temp] && !fnd1)
        {
            printf("part 1: %d at vals (%d,%d)\n", temp * vals[i], temp, vals[i]);
            fnd1 = 1;
        }

        for (j = i + 1; j < nval; j++)
        {
            temp = vals[i] + vals[j];
            if (temp < 2020 && hash[2020 - temp] && !fnd2)
            {
                printf("part 2: %d at vals (%d,%d,%d)\n",
                        (2020 - temp) * vals[i] * vals[j], vals[i], vals[j], 2020 - temp);
                fnd2 = 1;
            }
        }

        if (fnd1 && fnd2) {break;}
    }

    free(hash);
    free(vals);

    return 0;
}
