#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 100

int main()
{
    int i, min, max, nvalid, nvalid2, numchar;
    char passchar;
    char line[MAX_LINE], pass[MAX_LINE];

    min = max = nvalid = nvalid2 = passchar = 0;
    while (fgets(line, sizeof(line), stdin))
    {
        if (sscanf(line, "%d-%d %c: %s\n", &min, &max, &passchar, pass) != EOF)
        {
            for (i = 0, numchar = 0; pass[i] != '\0'; i++)
            {
                if (pass[i] == passchar) {numchar++;}
            }
            if (numchar >= min && numchar <= max) {nvalid++;}

            if (i < max)
            {
                printf("max index larger than password\n");
                return -2;
            }

            nvalid2 += (pass[min - 1] == passchar) ^ (pass[max - 1] == passchar);
        }
    }
    printf("part 1: %d valid\npart 2: %d valid\n", nvalid, nvalid2);

    return 0;
}
