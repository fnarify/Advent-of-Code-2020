#include <stdio.h>
#include <stdlib.h>

#define LEN 30000000

/**
 * version using single array
 */
int main()
{
    int i, cur, next;
    int base[] = {1, 20, 8, 12, 0, 14};

    // at most you need LEN space, or max(base) + 1 as the values are bounded
    int *last = calloc(LEN, sizeof(*last));
    if (!last) {return -1;}

    for (i = 0; i < sizeof(base) / sizeof(*base) - 1; i++) {last[base[i]] = i + 1;}

    cur = base[i];
    for ( ; i < LEN - 1; i++)
    {
        if (last[cur]) {next = i - last[cur] + 1;}
        else           {next = 0;}
        last[cur] = i + 1;
        cur = next;

        if (i == 2018) {printf("part 1: %d\n", cur);}
    }
    printf("part 2: %d\n", cur);

    free(last);

    return 0;
}
