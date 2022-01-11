#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 20
#define MAX_INST 800

enum inst{ACC, JMP, NOP};
const int map[] = {['a'] = ACC, ['j'] = JMP, ['n'] = NOP};
const char *codes[] = {[ACC] = "acc", [JMP] = "jmp", [NOP] = "nop"};

struct Code {
    int *op, *arg, *hist;
    int acc, size;
};
typedef struct Code Code;

/**
 * code is non-self-modifying which makes this all easier
 */
int solve(Code *prog, int start)
{
    int ip = start, offset = 0;
    while (!prog->hist[ip] && ip < prog->size)
    {
        switch (prog->op[ip])
        {
            case ACC:
                prog->acc += prog->arg[ip];
                offset = 1;
                break;
            case JMP:
                offset = prog->arg[ip];
                break;
            case NOP:
                offset = 1;
                break;
            default:
                printf("unknown opcode\n");
                break;
        }
        prog->hist[ip] = 1;
        ip += offset;
    }
    return ip;
}

int main()
{
    int i, orig, oldacc, offset;
    int *oldhist;
    char line[MAX_LINE], name[MAX_LINE / 2], value[MAX_LINE];

    Code *prog = malloc(sizeof(*prog));
    if (!prog) {return -1;}
    prog->op = malloc(sizeof(prog->op) * MAX_INST);
    prog->arg = malloc(sizeof(prog->arg) * MAX_INST);
    prog->hist = calloc(MAX_INST, sizeof(prog->hist));
    if (!prog->op || !prog->arg || !prog->hist) {return -2;}
    prog->acc = 0;
    prog->size = 0;

    i = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (i >= MAX_INST) {printf("not enough space\n"); return -3;}

        sscanf(line, "%s %s\n", name, value);
        // we can currently just use the first letter of the opcode
        // as there aren't enough that we need to use strcmp yet...
        prog->op[i] = map[(int) name[0]];
        prog->arg[i] = atoi(value);
        i++;
    }
    prog->size = i;

    solve(prog, 0);
    printf("part 1: %d acc value\n", prog->acc);

    // check for corrupted opcode, swap only 1 JMP or NOP instruction
    // pretty simply brute force implementation
    // smarter way might be working out the graph
    oldhist = malloc(sizeof(*oldhist) * MAX_INST);
    if (!oldhist) {return -4;}
    i = prog->acc = 0;
    memset(prog->hist, 0, MAX_INST);
    while (i < prog->size)
    {
        orig = prog->op[i];
        switch (prog->op[i])
        {
            case ACC:
                prog->acc += prog->arg[i];
                offset = 1;
                break;
            case JMP: // swap to NOP
                offset = 1;
                break;
            case NOP: // swap to JMP
                offset = prog->arg[i];
                break;
            default:
                printf("unknown opcode\n");
                break;
        }
        prog->hist[i] = 1;
        i += offset;

        // store history, check if it terminates at the end of the program
        if (orig != ACC)
        {
            memcpy(oldhist, prog->hist, MAX_INST);
            oldacc = prog->acc;

            if (solve(prog, i) >= prog->size)
            {
                printf("part 2: %d acc value\n", prog->acc);
                break;
            }

            prog->acc = oldacc;
            memcpy(prog->hist, oldhist, MAX_INST);
            i -= offset;
            i += (orig == JMP ? prog->arg[i] : 1);
        }
    }

    free(prog->op);
    free(prog->arg);
    free(prog->hist);
    free(oldhist);

    return 0;
}
