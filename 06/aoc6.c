#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100
#define N_QUES   26

void check(int map[], int n, int *a, int *b)
{
    int i;
    for (i = 0; i < N_QUES; i++)
    {
        if (map[i])      {*a += 1;}
        if (map[i] == n) {*b += 1;}
        map[i] = 0;
    }
}

int main()
{
    int i, n, len, sum, same;
    char line[MAX_LINE];
    int map[N_QUES] = {0};

    sum = same = n = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (line[0] == '\n')
        {
            check(map, n, &sum, &same);
            n = 0;
        }
        else
        {
            len = strlen(line);
            for (i = 0; i < len - 1; i++) {map[line[i] - 'a']++;}
            n++;
        }
    }
    check(map, n, &sum, &same);
    printf("part 1: %d\npart 2: %d\n", sum, same);

    return 0;
}
