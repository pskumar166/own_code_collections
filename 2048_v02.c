#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio_ext.h>
#include <string.h>

#define MATRIX 4

int box[MATRIX][MATRIX] = {{0}};
int freeBox[MATRIX * MATRIX] = {0};
int count = 0;

void printBoxValues()
{
    int i,j;
    system("clear");
    printf(" ------- ------- ------- -------\n");
    for (i=0; i<MATRIX; i++)
    {
        printf("|");
        for (j=0; j<MATRIX; j++)
        {
            if (box[i][j])
            {
                printf("%d\t|",box[i][j]);
            }
            else
            {
                printf("\t|");
            }
        }
        printf("\n ------- ------- ------- -------\n");
    }

    printf("Keys= W:up S:down A:left D:right h:halt\n");
}

int generateFourBitRandNum()
{
    int randNum = 0;
    srand(time(0));
    randNum = rand();
    randNum = ((randNum % (MATRIX * MATRIX)) & 0x0F);
    return randNum;
}

void updateFreeBoxValues()
{
    int i,j,k=0;

    memset(freeBox, 0, (sizeof(int) * MATRIX * MATRIX));
    for(i=0; i<MATRIX; i++)
    {
        for(j=0; j<MATRIX; j++)
        {
            if(box[i][j] == 0)
            {
                freeBox[k] = ((i * MATRIX) + j + 1);
                k++;
            }
        }
    }
}

void getRandIndx(int *i, int *j)
{
    int randCnt = 0, pos = 0;
    updateFreeBoxValues();

    randCnt = generateFourBitRandNum();
    for(pos=0; pos<(MATRIX*MATRIX); pos++)
    {
        if(freeBox[pos] == 0)
        {
            break;
        }
    }
    pos = randCnt%pos;
    *i = (freeBox[pos]-1) / MATRIX;
    *j = (freeBox[pos]-1) % MATRIX;
}

void updateOneNewValue()
{
    int i,j;

    getRandIndx(&i, &j);
    if(!count)
    {
        box[i][j] = 4;
        count = generateFourBitRandNum();
    }
    else
    {
        box[i][j] = 2;
        count--;
    }
}

void initializeBox()
{
    int i;

    /* Generate random four bit counter value.
     * This value will be used to decide whether to 
     * introduce 2 or 4 in a box. If counter is zero
     * then value will be introduced as 4, else it will
     * be 2*/
    count = generateFourBitRandNum();

    /* Randomly assign the two box value with 2 or 4 */
    /* First Box */
    updateOneNewValue();

    sleep(1);
    /* Second box */
    updateOneNewValue();
}

char getInput()
{
    char input;
    int in_cnt = 0;

    system("/bin/stty raw");
    while(1)
    {
        __fpurge(stdin);
        if((input = getchar()) != '.')
        {
            if((input != 'w') && (input != 's') && (input != 'a') 
                    && (input != 'd') && (input != 'h'))
            {
                printf("Invalid input.Enter again\n");
                in_cnt++;
                if(in_cnt > 3)
                {
                    break;
                }
                continue;
            }
        }
        break;
    }
    system("/bin/stty cooked");
    if(input == 'h')
    {
        exit(0);
    }
    return input;
}

int getEmpBoxCntAndWinStatus()
{
    int count = 0;
    int i, j;

    for(i=0; i<MATRIX; i++)
    {
        for(j=0; j<MATRIX; j++)
        {
            if(box[i][j] == 0)
            {
                count++;
            }
            else if(box[i][j] == 2048)
            {
                return 2048;
            }
        }
    }
    return count;
}

int addNumToUpside()
{
    int i=0, j=0, ref=0, ret=0;
    int isNonZero = 0;
    while(j < MATRIX)
    {
        i = 0;
        while(i < (MATRIX -1))
        {
            isNonZero = 0;
            if(box[i][j] == 0)
            {
                for(ref=i; ref< (MATRIX-1); ref++)
                {
                    if(box[ref+1][j] != 0)
                    {
                        isNonZero = 1;
                    }
                    box[ref][j] = box[ref+1][j];
                    box[ref+1][j] = 0;
                }
                if(isNonZero)
                {
                    ret=1;
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(box[i][j] == box[i+1][j])
                {
                    box[i][j] += box[i+1][j];
                    box[i+1][j] = 0;
                    for(ref=i+1; ref< (MATRIX-1); ref++)
                    {
                        box[ref][j] = box[ref+1][j];
                        box[ref+1][j] = 0;
                    }
                    ret=1;
                }
                else if(box[i+1][j] == 0)
                {
                    for(ref=i+1; ref< (MATRIX-1); ref++)
                    {
                        if(box[ref+1][j] != 0)
                        {
                            isNonZero = 1;
                        }
                        box[ref][j] = box[ref+1][j];
                        box[ref+1][j] = 0;
                    }
                    if(isNonZero)
                    {
                        ret=1;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            i++;
        }
        j++;
    }
    return ret;
}

int addNumToDownside()
{
    int i=MATRIX-1, j=MATRIX-1, ref=0;
    int isNonZero = 0, ret = 0;
    while(j >= 0)
    {
        i = MATRIX-1;
        while(i > 0)
        {
            isNonZero = 0;
            if(box[i][j] == 0)
            {
                for(ref=i; ref>0; ref--)
                {
                    if(box[ref-1][j] != 0)
                    {
                        isNonZero = 1;
                    }
                    box[ref][j] = box[ref-1][j];
                    box[ref-1][j] = 0;
                }
                if(isNonZero)
                {
                    ret=1;
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(box[i][j] == box[i-1][j])
                {
                    box[i][j] += box[i-1][j];
                    box[i-1][j] = 0;
                    for(ref=i-1; ref>0; ref--)
                    {
                        box[ref][j] = box[ref-1][j];
                        box[ref-1][j] = 0;
                    }
                    ret=1;
                }
                else if(box[i-1][j] == 0)
                {
                    for(ref=i-1; ref>0; ref--)
                    {
                        if(box[ref-1][j] != 0)
                        {
                            isNonZero = 1;
                        }
                        box[ref][j] = box[ref-1][j];
                        box[ref-1][j] = 0;
                    }
                    if(isNonZero)
                    {
                        ret=1;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            i--;
        }
        j--;
    }
    return ret;
}

int addNumToRightside()
{
    int i=0, j=0, ref=0;
    int isNonZero = 0, ret =0;
    while(i < MATRIX)
    {
        j = 0;
        while(j < (MATRIX -1))
        {
            isNonZero = 0;
            if(box[i][j] == 0)
            {
                for(ref=j; ref< (MATRIX-1); ref++)
                {
                    if(box[i][ref+1] != 0)
                    {
                        isNonZero = 1;
                    }
                    box[i][ref] = box[i][ref+1];
                    box[i][ref+1] = 0;
                }
                if(isNonZero)
                {
                    ret =1;
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(box[i][j] == box[i][j+1])
                {
                    box[i][j] += box[i][j+1];
                    box[i][j+1] = 0;
                    for(ref=j+1; ref< (MATRIX-1); ref++)
                    {
                        box[i][ref] = box[i][ref+1];
                        box[i][ref+1] = 0;
                    }
                    ret=1;
                }
                else if(box[i][j+1] == 0)
                {
                    for(ref=j+1; ref< (MATRIX-1); ref++)
                    {
                        if(box[i][ref+1] != 0)
                        {
                            isNonZero = 1;
                        }
                        box[i][ref] = box[i][ref+1];
                        box[i][ref+1] = 0;
                    }
                    if(isNonZero)
                    {
                        ret=1;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            j++;
        }
        i++;
    }
    return ret;
}

int addNumToLeftside()
{
    int i=MATRIX-1, j=MATRIX-1, ref=0;
    int isNonZero = 0, ret = 0;
    while(i >= 0)
    {
        j = MATRIX-1;
        while(j > 0)
        {
            isNonZero = 0;
            if(box[i][j] == 0)
            {
                for(ref=j; ref>0; ref--)
                {
                    if(box[i][ref-1] != 0)
                    {
                        isNonZero = 1;
                    }
                    box[i][ref] = box[i][ref-1];
                    box[i][ref-1] = 0;
                }
                if(isNonZero)
                {
                    ret=1;
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(box[i][j] == box[i][j-1])
                {
                    box[i][j] += box[i][j-1];
                    box[i][j-1] = 0;
                    for(ref=j-1; ref>0; ref--)
                    {
                        box[i][ref] = box[i][ref-1];
                        box[i][ref-1] = 0;
                    }
                    ret=1;
                }
                else if(box[i][j-1] == 0)
                {
                    for(ref=j-1; ref>0; ref--)
                    {
                        if(box[i][ref-1] != 0)
                        {
                            isNonZero = 1;
                        }
                        box[i][ref] = box[i][ref-1];
                        box[i][ref-1] = 0;
                    }
                    if(isNonZero)
                    {
                        ret=1;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            j--;
        }
        i--;
    }
    return ret;
}

int main()
{
    char input;
    int ret;
    initializeBox();
    while(1)
    {
        printBoxValues();
        ret = getEmpBoxCntAndWinStatus();
        if(ret == 2048)
        {
            printf("************You have Won the game************\n");
            break;
        }
        else if(ret == 0)
        {
            printf("************GAME OVER************\n");
            break;
        }
        input = getInput();

        switch(input)
        {
            case 'w':
            {
                ret = addNumToUpside();
                break;
            }
            case 's':
            {
                ret = addNumToDownside();
                break;
            }
            case 'd':
            {
                ret = addNumToLeftside();
                break;
            }
            case 'a':
            {
                ret = addNumToRightside();
                break;
            }
        }

        if(ret)
        {
            updateOneNewValue();
        }
    }
    return 0;
}
