#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define N_BITS   36
#define MAX_LINE 100
#define MEM_SIZE 100000

struct Map{
    uint64_t *addr, *values;
    int size;
};
typedef struct Map Map;

/**
 * check whether our location is set in map->addr
 * if so update the value at that index in map->values
 * otherwise add a new (addr, values) key pair
 *
 * return the changed index + 1 otherwise 0
 */
int contain(uint64_t loc, uint64_t set, Map *map)
{
    int i, fnd;
    fnd = 0;
    for (i = 0; i < map->size && !fnd; i++)
    {
        if (loc == map->addr[i])
        {
            fnd = i + 1;
            map->values[i] = set;
        }
    }

    if (map->size >= MEM_SIZE)
    {
        printf("out of memory to add new address\n");
        fnd = -1;
    }
    else if (!fnd)
    {
        map->addr[map->size] = loc;
        map->values[map->size] = set;
        map->size++;
    }

    return fnd;
}

/**
 * finds all permutations of the N_BITS long string perm
 * permutations are only taken over char 'X'
 * base is preset to our masked value
 */
void permutations(uint64_t base, char *perm, int bit, uint64_t set, Map *map)
{
    if (bit == N_BITS)
    {
        contain(base, set, map);
    }
    else if (perm[bit] == 'X')
    {
        permutations(base, perm, bit + 1, set, map); // pre-set to 0
        base |= 1LL << (N_BITS - bit - 1); // set to 1
        permutations(base, perm, bit + 1, set, map);
    }
    else
    {
        permutations(base, perm, bit + 1, set, map);
    }
}

/**
 * needs -std=c99
 * using windows print format identifiers
 *
 * this partially only works without running out of memory due
 * to the face that at most 10 X's are set in the mask, meaning we at
 * most need 2^10 == 1024 spots of memory per mask, if most of the values
 * are 'X' in the mask, then you'd be looking at 70+GB of memory needed
 * -- ergo, don't run the first test input he gives in part 1
 *
 *  part 2 uses about 75k entries in map, meaning the contain search is
 *  very slow, running it as a hash could be possible but even more
 *  memory is required
 */
int main()
{
    int i, index;
    uint64_t val, base;
    char line[MAX_LINE], mask[MAX_LINE], floating[N_BITS + 1];
    uint64_t *memory;

    Map *map = malloc(sizeof(*map));
    if (!map) {return -1;}

    memory = calloc(MEM_SIZE, sizeof(*memory));
    map->addr = malloc(MEM_SIZE * sizeof(*map->addr));
    map->values = calloc(MEM_SIZE, sizeof(*map->values));
    map->size = 0;
    if (!memory || !map->addr || !map->values) {return -2;}

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (line[1] == 'a') // mask
        {
            sscanf(line, "mask = %s\n", mask);
        }
        else if (line[1] == 'e') // mem[]
        {
            sscanf(line, "mem[%d] = %I64u\n", &index, &val); // %I64u windows, %llu linux etc

            // pt2 apply mask to address, retain Xs then generate all permutations needed 2^n
            // where n is the amount of X's in the mask string
            strcpy(floating, mask);
            base = 0;
            for (i = 0; i < N_BITS; i++)
            {
                if (floating[i] == '0' && (index & (1LL << (N_BITS - i - 1))))
                {
                    floating[i] = '1';
                }
                if (floating[i] == '1') {base |= 1LL << (N_BITS - i - 1);}
            }
            permutations(base, floating, 0, val, map);

            // pt1 apply to value
            for (i = 0; i < N_BITS; i++)
            {
                if      (mask[i] == '0') {val &= ~(1LL << (N_BITS - i - 1));} // LL needed for 64-bit results
                else if (mask[i] == '1') {val |= 1LL << (N_BITS - i - 1);}
            }
            memory[index] = val;
        }
    }

    val = 0;
    for (i = 0; i < MEM_SIZE; i++) {val += memory[i];}
    printf("part 1: %I64u\n", val);

    val = 0;
    for (i = 0; i < map->size; i++) {val += map->values[i];}
    printf("part 2: %I64u\n", val);

    free(memory);
    free(map->addr);
    free(map->values);
    free(map);

    return 0;
}
