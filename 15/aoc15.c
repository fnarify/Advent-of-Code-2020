#include <stdio.h>
#include <stdlib.h>

#define LEN 30000000

int main()
{
    int i;
    int base[] = {1, 20, 8, 12, 0, 14};

    // technically don't need these, just need to store current and next value
    int *val = malloc((LEN + 1) * sizeof(*val));
    // at most you need LEN space, or max(base) + 1 as the values are bounded
    int *last = calloc(LEN, sizeof(*last));
    if (!val || !last) {return -1;}

    for (i = 0; i < sizeof(base) / sizeof(*base); i++)
    {
        val[i] = base[i];
        last[base[i]] = i + 1; // index of last value
    }

    i--;
    last[base[i]] = 0;
    for ( ; i < LEN; i++)
    {
        if (last[val[i]]) {val[i + 1] = i - last[val[i]] + 1;}
        else              {val[i + 1] = 0;}
        last[val[i]] = i + 1;
    }
    printf("part 1: %d\n", val[2019]);
    printf("part 2: %d\n", val[LEN - 1]);

    free(val);
    free(last);

    return 0;
}
