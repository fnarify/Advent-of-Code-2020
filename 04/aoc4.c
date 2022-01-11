#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DELIN    " :"
#define E_SIZE   30
#define MAX_LINE 200
#define MAX_ENT  1000

enum field{BYR = 1 << 0, IYR = 1 << 1, EYR = 1 << 2, HGT = 1 << 3, HCL = 1 << 4, ECL = 1 << 5, PID = 1 << 6, CID = 1 << 7};

struct Passport {
    int birth, issue, exp, cid, valid;
    char height[E_SIZE], hair[E_SIZE], eye[E_SIZE], pid[E_SIZE];
};
typedef struct Passport Passport;

/**
 * valid if only all fields are present or just country id (cid) is missing
 */
int isvalid(int chk)
{
    static const int chksum = BYR | IYR | EYR | HGT | HCL | ECL | PID;
    if (chk == chksum || chk == (chksum | CID)) {return 1;}
    else {return 0;}
}

int main()
{
    int i, n, nvalid, flag, hval, ret;
    char line[MAX_LINE], type[E_SIZE];
    char *parse;
    Passport *pass;

    pass = calloc(MAX_ENT, sizeof(*pass));
    if (!pass) {printf("not enough memory\n"); return -1;}

    n = nvalid = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        // new entry only double new-line, watch out for the last entry
        if (line[0] == '\n')
        {
            nvalid += isvalid(pass[n++].valid);
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0'; // remove trailing new-line
        parse = strtok(line, DELIN);
        while (parse != NULL)
        {
            // retain type info
            strcpy(type, parse);
            // parse is now our value
            parse = strtok(NULL, DELIN);
            if (parse == NULL) {printf("malfored data\n"); return -2;}

            if      (!strcmp(type, "byr")) 
            {
                pass[n].birth = atoi(parse);
                pass[n].valid += BYR;
            }
            else if (!strcmp(type, "iyr"))
            {
                pass[n].issue = atoi(parse);
                pass[n].valid += IYR;
            }
            else if (!strcmp(type, "eyr"))
            {
                pass[n].exp = atoi(parse);
                pass[n].valid += EYR;
            }
            else if (!strcmp(type, "hgt"))
            {
                strcpy(pass[n].height, parse);
                pass[n].valid += HGT;
            }
            else if (!strcmp(type, "hcl"))
            {
                strcpy(pass[n].hair, parse);
                pass[n].valid += HCL;
            }
            else if (!strcmp(type, "ecl"))
            {
                strcpy(pass[n].eye, parse);
                pass[n].valid += ECL;
            }
            else if (!strcmp(type, "pid"))
            {
                // as it can start with a 0
                strcpy(pass[n].pid, parse);
                pass[n].valid += PID;
            }
            else if (!strcmp(type, "cid"))
            {
                pass[n].cid = atoi(parse);
                pass[n].valid += CID;
            }
            parse = strtok(NULL, DELIN);
        }
    }
    nvalid += isvalid(pass[n++].valid); // last entry
    printf("part 1: %d valid passports out of %d\n", nvalid, n);

    nvalid = hval = ret = 0;
    for (i = 0; i < n; i++)
    {
        flag = 0;
        if (pass[i].valid & BYR)
        {
            if (pass[i].birth >= 1920 && pass[i].birth <= 2002) {flag += BYR;}
        }
        if (pass[i].valid & IYR)
        {
            if (pass[i].issue >= 2010 && pass[i].issue <= 2020) {flag += IYR;}
        }
        if (pass[i].valid & EYR)
        {
            if (pass[i].exp >= 2020 && pass[i].exp <= 2030) {flag += EYR;}
        }
        if (pass[i].valid & HGT)
        {
            ret = sscanf(pass[i].height, "%d%s", &hval, type);
            // must be in the form Xcm or Xin, where X is a number
            if (ret == 2)
            {
                if ( (!strcmp(type, "cm") && hval >= 150 && hval <= 193) ||
                     (!strcmp(type, "in") && hval >= 59 && hval <= 76) ) {flag += HGT;}
            }
        }
        if (pass[i].valid & HCL)
        {
            if (strlen(pass[i].hair) == 7 && pass[i].hair[0] == '#') {flag += HCL;}
        }
        if (pass[i].valid & ECL)
        {
            strcpy(type, pass[i].eye);
            if (!strcmp(type, "amb") || !strcmp(type, "blu") ||
                !strcmp(type, "brn") || !strcmp(type, "gry") ||
                !strcmp(type, "grn") || !strcmp(type, "hzl") ||
                !strcmp(type, "oth")) {flag += ECL;}
        }
        if (pass[i].valid & PID)
        {
            // more correct would be check it's all digits as well
            if (strlen(pass[i].pid) == 9) {flag += PID;}
        }

        nvalid += isvalid(flag);
    }
    printf("part 2: %d strict passwords valid out of %d\n", nvalid, n);

    free(pass);

    return 0;
}
