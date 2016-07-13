#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

srand(time(NULL));

struct Room {
	char roomName[15];
	char fName[10];
	int rmNum;
	char roomType[15];
	int connections[5][2];
	int conCount;
};


int checkCon(struct Room rm, int check)
{
	for (j = 0; j < rm.conCount; j++)
	{
		if (check == rm.connections[j][1])
			return 1;
	}
	return 0;
}
//returns 1 if full
int checkCount(struct Room rm)
{
	int numOfCons;
	for (j = 0; j < rm.conCount; j++)
	{
		if (rm.connections[j][0] != NULL)
			numOfCons++;
	}
	if (numOfCons == rm.conCount)
			return 1;
	else
			return 0;
}


void mkRooms()
{
	const char *names[10];
	names[0] = "Grotto";
	names[1] = "Taberna";
	names[2] = "Attic"
	names[3] = "Atrium";
	names[4] = "Vault";
	names[5] = "Sewers";
	names[6] = "Hovel";
	names[7] = "Cavern";
	names[8] = "Cellar";
	names[9] = "Other Cellar";
	
	int r;
	int rNum;
	int startSet = 0
	int endSet = 0
	struct Room rooms[7]
	START_ROOM, END_ROOM, and MID_ROOM
	int i;
	for (i = 0; i < 7; i++)
	{
		r = rand();
		rNum = r % 10; 
		rooms[i].roomName=names[rNum];
		sprintf(rooms[i].fName, "ROOM_%i", i);
		rooms[i].rmNum = i;
		
		if (startSet == 0)
		{
			rooms[i].roomType = "START_ROOM";
		}
		else if (endSet == 0)
		{
			rooms[i].roomType = "END_ROOM";
		}
		else
			rooms[i].roomType = "MID_ROOM";
		
		r = rand();
		rNum = r % 4; 
		rNum += 4;
		printf("%i\n", rNum);
		rooms[i].conCount = rNum;
	}
	
	for (i = 0; i < 7; i++)
	{
		int j
		for (j = 0; j < rooms[i].conCount; j++)
		{
			if (rooms[i].connection[j][0] != NULL)
			{
				do{
				r = rand();
				rNum = r % 7; 
				}while (rNum == i || checkCon(rooms[i], rNum) == 1 || checkCount(rooms[rNum]) == 1)
				
				rooms[i].connections[j][0] = rooms[rNum].roomName;
				rooms[i].connections[j][1] = rNum;
				while (rooms[rNum].connections[j] != NULL)
			}
		}
		 
	}
	
}

int main()
{
	
	struct stat st = {0};
	int id = getpid();
	char route[20];
	sprintf(route, "fuerstj.rooms.%i", id);
	printf("%s\n", route);
	
	if (stat(route, &st) == -1) 
	{
		mkdir(route, 0700);
	}
	
	mkRooms();
	return 0;
}