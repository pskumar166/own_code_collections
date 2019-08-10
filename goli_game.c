#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRD_SQR_MTRX 7
#define SUCCESS 0
#define FAILURE 1
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

int brdData[BRD_SQR_MTRX][BRD_SQR_MTRX] = {{0}};
int brdDataInit[BRD_SQR_MTRX][BRD_SQR_MTRX] = {{0}};
int maxVal = 0;
int movement = 0;
int currRow = 0;
int currCol = 0;

void initBrdDataValues()
{
	int i,j;
	int val = 1;
	for(i=0; i<BRD_SQR_MTRX; i++) {
		for(j=0; j<BRD_SQR_MTRX; j++) {
			if(((i <= 4) && (i >= 2)) ||
				((j <= 4) && (j >= 2))) {
				brdDataInit[i][j] = val;
				brdData[i][j] = val;
				val++;
			}	
		}
	}
	maxVal = val;
	brdData[3][3] = 'X'; 
}

void printBrdData()
{
	int i,j;
	system("clear");
	printf(" ===============================================================\n");
	for(i=0; i<BRD_SQR_MTRX; i++) {
		printf("|\t");
		for(j=0; j<BRD_SQR_MTRX; j++) {
			if(brdData[i][j] == 0) {
				printf("\t");
			} else if(brdData[i][j] == 'X') {
				printf("X\t");
			} else {
				printf("%d\t",brdData[i][j]);
			}
		}
		printf("|\n");
	}
	printf(" ===============================================================\n");
}

void updateBrdData(int val, int dir)
{
	int i,j;

	i = currRow;
	j = currCol;

	brdData[i][j] = 'X';
	switch(dir) {
		case 1:
			i--;
			brdData[i][j] = 'X';
			i--;
			brdData[i][j] = brdDataInit[i][j];
			break;
		case 2:
			j--;
			brdData[i][j] = 'X';
			j--;
			brdData[i][j] = brdDataInit[i][j];
			break;
		case 3:
			i++;
			brdData[i][j] = 'X';
			i++;
			brdData[i][j] = brdDataInit[i][j];
			break;
		case 4:
			j++;
			brdData[i][j] = 'X';
			j++;
			brdData[i][j] = brdDataInit[i][j];
			break;
	}
	movement++;
}

int validateVal(int val)
{
	int i,j;

	/********** Validate value ***********/
	if((val <= 0) || (val >= maxVal)) {
		printf("Enter the value within its range\n");
		return FAILURE;
	}

	for(i=0; i<BRD_SQR_MTRX; i++) {
		for(j=0; j<BRD_SQR_MTRX; j++) {
			if(brdDataInit[i][j] == val) {
				if(brdData[i][j] == 'X') {
					printf("Selected empty location.Please select proper value\n");
					return FAILURE;
				} else {
					currRow = i;
					currCol = j;
					return SUCCESS;
				}
			}
		}
	}

	return SUCCESS;
}

int validateDir(char *arrow, int *dir)
{
	int i,j, err1=0, err2=0;
	i = currRow;
	j = currCol;

	/********** Validate direction ***********/
	if((arrow[0] == 27) && (arrow[1] == 91))
	{
		switch(arrow[2]) {
			case UP:
				*dir = 1;
				if(i <= 1) {
					err1 = 1;
					break;
				}
				if((brdData[i-1][j] == 'X') ||
					(brdData[i-2][j] != 'X'))
				{
					err2 = 1;
				}
				break;
			case LEFT:
				*dir = 2;
				if(j <= 1) {
					err1 = 1;
					break;
				}
				if((brdData[i][j-1] == 'X') ||
					(brdData[i][j-2] != 'X'))
				{
					err2 = 1;
				}
				break;
			case DOWN:
				*dir = 3;
				if(i >= 5) {
					err1 = 1;
					break;
				}
				if((brdData[i+1][j] == 'X') ||
					(brdData[i+2][j] != 'X'))
				{
					err2 = 1;
				}
				break;
			case RIGHT:
				*dir = 4;
				if(j >= 5) {
					err1 = 1;
					break;
				}
				if((brdData[i][j+1] == 'X') ||
					(brdData[i][j+2] != 'X'))
				{
					err2 = 1;
				}
				break;
			default:
				printf("Entered invalid direction\n");
				return FAILURE;
		}

		if(err1) {
			printf("Entered direction is out of board range\n");
			return FAILURE;
		} else if (err2) {
			printf("Can't move the selected value in this direction\n");
			return FAILURE;
		}
	} else {
		printf("Entered invalid direction\n");
		return FAILURE;
	} 

	return SUCCESS;
}

int getGameStatus()
{
	int i,j;

	for(i=0; i<BRD_SQR_MTRX; i++) {
		for(j=0; j<BRD_SQR_MTRX; j++) {
			if((brdData[i][j] != 0) && (brdData[i][j] != 'X')) {
				/* Upside check */
				if(i >= 2) {
					if((brdData[i-1][j] != 'X') && (brdData[i-1][j] != 0) &&
						(brdData[i-2][j] == 'X')) {
						return SUCCESS;
					}
				}

				/* Left side check */
				if(j >= 2) {
					if((brdData[i][j-1] != 'X') && (brdData[i][j-1] != 0) &&
						(brdData[i][j-2] == 'X')) {
						return SUCCESS;
					}
				}

				/* Downside check */
				if(i <= 4) {
					if((brdData[i+1][j] != 'X') && (brdData[i+1][j] != 0) &&
						(brdData[i+2][j] == 'X')) {
						return SUCCESS;
					}
				}

				/* Right side check */
				if(j <= 4) {
					if((brdData[i][j+1] != 'X') && (brdData[i][j+1] != 0) &&
						(brdData[i][j+2] == 'X')) {
						return SUCCESS;
					}
				}
			}
		}
	}

	if(movement == 32) {
		printf("Congrats...YOU HAVE WON THE GAME !!!\n");
	} else {
		printf("Sorry...YOU HAVE LOST THE GAME !!!\n");
	}
	return FAILURE;
}

int main()
{
	int val,dir;
	initBrdDataValues();
	char arrow[20] = {0};
	
	while(1) {
		printBrdData();

		if(getGameStatus() == FAILURE) {
			break;
		}
get_input:
		printf("Enter the value to move: ");
		scanf("%d%c",&val);

		if(validateVal(val) == FAILURE) {
			goto get_input;
		}

		memset(arrow, 0, 20);
		printf("Enter the direction to move\n");
		scanf("%[^\n]%*c",arrow);

		if(validateDir(arrow, &dir) == FAILURE) {
			goto get_input;
		}
		
		updateBrdData(val, dir);
	}
	return SUCCESS;
}
