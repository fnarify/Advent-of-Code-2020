#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_OUT   20
#define MAX_MEM   200000 // my input requires a size this large for unique hashes
#define MAX_LINE  300
#define DELIN     ","
#define SEARCH    "shiny gold"

int ndupe = 0;
int dupes[594] = {0}; // hard-coded at 594, need less space for actual operation

struct Bag {
    int contain[MAX_OUT]; // what this bag contains, stored as two values (number made, bag location)
    int outside[MAX_OUT]; // what contains this bag, just store index
    int consize; // how many different unique bags it can store
    int outsize; // how many different bags can store this bag
};
typedef struct Bag Bag;

int hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {hash = ((hash << 5) + hash) + c;} // hash * 33 + c

    return hash % MAX_MEM;
}

/**
 * null-string after n instances of whitespace are detected
 */
void shorten(char *s, int n)
{
    int i;
    for (i = 0; n && s[i] != '\0'; i++)
    {
        if (s[i] == ' ') {n--;}
    }
    s[i - 1] = '\0';
}

/**
 * return 1 if loc is located in the dupes array
 */
int duplicate(int loc)
{
    int i, ret = 0;
    for (i = 0; i < ndupe; i++)
    {
        if (loc == dupes[i]) {ret = 1;}
    }
    return ret;
}

/**
 * Solve how many bags can contain a bag at hash location loc
 * need to watch out for duplicates
 */
int solveout(Bag *bag, int loc)
{
    int i, sum, curloc;
    sum = bag[loc].outsize;
    for (i = 0; i < bag[loc].outsize; i++)
    {
        curloc = bag[loc].outside[i];
        if (!duplicate(curloc))
        {
            dupes[ndupe++] = curloc;
            sum += solveout(bag, curloc);
        }
        else {sum--;}
    }
    return sum;
}

/**
 * Solve how many bags can be contained inside a bag at location loc
 * duplicates should not matter
 * neither this nor the function above are tail-recursive so quite slow
 */
int solvecon(Bag *bag, int loc)
{
    int i, sum = 1;
    // jump by 2's
    for (i = 0; i < bag[loc].consize; i++)
    {
        sum += bag[loc].contain[i * 2] * solvecon(bag, bag[loc].contain[i * 2 + 1]);
    }
    return sum;
}

int main()
{
    int cnt, ret, loc, outloc;
    char line[MAX_LINE], input[MAX_LINE / 2], output[MAX_LINE];
    char *parse;
    const int searchloc = hash(SEARCH);

    Bag *bag = calloc(MAX_MEM, sizeof(struct Bag));
    if (!bag) {return -1;}

    while (fgets(line, sizeof(line), stdin))
    {
        // parse up to the first digit, could technically parse it via two separate strings
        ret = sscanf(line, "%[^0-9] %[^.]\n", input, output);
        shorten(input, 2);
        loc = hash(input);

        if (ret > 1)
        {
            // get number of bags and bag name for each component
            parse = strtok(output, DELIN);
            while (parse != NULL)
            {
                sscanf(parse, "%d", &cnt);
                while (!isalpha(*parse)) {parse++;} // remove starting whitespace etc
                strcpy(input, parse);
                shorten(input, 2);

                // better watch out for this multiplier! Definitely going to bite someone in the arse
                outloc = hash(input);
                if (bag[loc].consize >= MAX_OUT) {printf("out of space to store contains\n"); return -2;}
                bag[loc].contain[bag[loc].consize * 2]     = cnt;
                bag[loc].contain[bag[loc].consize * 2 + 1] = outloc;
                bag[loc].consize++;

                // reverse dependancy map
                bag[outloc].outside[bag[outloc].outsize] = loc;
                bag[outloc].outsize++;

                parse = strtok(NULL, DELIN);
            }
        }
    }
    printf("part 1: %d bags can contain at least one shiny gold bag\n", solveout(bag, searchloc));
    // -1 to account for the bag itself
    printf("part 2: %d bags can be contained in a shiny gold bag\n", solvecon(bag, searchloc) - 1);

    free(bag);

    return 0;
}

