#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio_ext.h>

#define MATRIX 4

int box[MATRIX][MATRIX] = {{0}};
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

    printf("Keys= W:up S:down A:left D:right\n");
}

int generateFourBitRandNum()
{
    int randNum = 0;
    srand(time(0));
    randNum = rand();
    randNum = ((randNum % (MATRIX * MATRIX)) & 0x0F);
    return randNum;
}

void getRandIndx(int *i, int *j)
{
    int randPos = 0;
    while(1)
    {
        randPos = generateFourBitRandNum();
        *i = randPos/MATRIX;
        *j = randPos%MATRIX;
        if(0 == box[*i][*j])
        {
            break;
        }
    }
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
    int i,j;

    /* Generate random four bit counter value.
     * This value will be used to decide whether to 
     * introduce 2 or 4 in a box. If counter is zero
     * then value will be introduced as 4, else it will
     * be 2*/
    count = generateFourBitRandNum();

    /* Randomly assign the two box value with 2 or 4 */
    /* First Box */
    updateOneNewValue();

    /* Second box */
    updateOneNewValue();
}

char getInput()
{
    char input, dummy;

    while(1)
    {
        __fpurge(stdin);
        input = getchar();
        scanf("%c",&dummy);
        if(dummy != '\n')
        {
            printf("Invalid input.Enter again\n");
            continue;
        }

        if((input != 'w') && (input != 's') && (input != 'a') 
                && (input != 'd'))
        {
            printf("Invalid input.Enter again\n");
            continue;
        }
        break;
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

void addNumToUpside()
{
    int i=0, j=0, ref=0;
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
}

void addNumToDownside()
{
    int i=MATRIX-1, j=MATRIX-1, ref=0;
    int isNonZero = 0;
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
}

void addNumToRightside()
{
    int i=0, j=0, ref=0;
    int isNonZero = 0;
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
}

void addNumToLeftside()
{
    int i=MATRIX-1, j=MATRIX-1, ref=0;
    int isNonZero = 0;
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
                addNumToUpside();
                break;
            }
            case 's':
            {
                addNumToDownside();
                break;
            }
            case 'd':
            {
                addNumToLeftside();
                break;
            }
            case 'a':
            {
                addNumToRightside();
                break;
            }
        }

        updateOneNewValue();
    }
    return 0;
}
