#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RANGE    25
#define MAX_LINE 20
#define MAX_INP  1000

/**
 * need -std=c99 for this program
 */
int main()
{
    int i, j, k, len, fnd;
    int64_t sum, invalid, min, max;
    char line[MAX_LINE];
    int64_t *value = malloc(sizeof(*value) * MAX_INP);
    if (!value) {return -1;}

    len = 0;
    while (fgets(line, sizeof(line), stdin) != NULL) {value[len++] = atoll(line);}

    // O(n * RANGE ^ 2) rip
    for (i = RANGE; i < len; i++)
    {
        fnd = 0;
        for (j = i - RANGE; j < i && !fnd; j++)
        {
            for (k = j + 1; k < i && !fnd; k++)
            {
                if (value[j] + value[k] == value[i]) {fnd = 1;}
            }
        }

        if (!fnd) {break;} // invalid number found
    }
    invalid = value[i];
    printf("part 1: %I64d is not the pair sum of the prior %d numbers\n", invalid, RANGE);

    fnd = max = 0;
    min = INT64_MAX;
    for (i = 0; i < len && !fnd; i++)
    {
        sum = value[i];
        for (j = i + 1; j < len && sum < invalid; j++) {sum += value[j];}
        if (sum == invalid)
        {
            fnd = 1;
            for (k = i; k < j; k++)
            {
                if (value[k] < min) {min = value[k];}
                if (value[k] > max) {max = value[k];}
            }
        }
    }
    printf("part 2: %I64d encryption weakness\n", min + max);

    return 0;
}
