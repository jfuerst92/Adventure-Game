/*********************************************************************************
 *Author: Joseph Fuerst
 *Date 7/18/2016
 *Description: A game that generates a random dungeon of 7 rooms. It stores thes in
 *files that load when a player enters a room. The player must make their way from 
 *the start to the end in as little moves as possible.
 **********************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


//carries the value of the starting room number
int stNum;
//Contains information about each room. Used for writing to files. The actual game
//does not use these structs, and instead takes info directly from the files.
struct Room {
	char* roomName; //string containing name of room
	int rmNameNum; //name number, used to find the name in an array
	int rmNum; //the actual number of the room in the order it is assigned
	char* roomType; //the type of room. Either START_ROOM, MID_ROOM, or END_ROOM
	int conNameNum[6]; //The name numbers of its connections.
	int conCount;	//the number of connections
	int conID[6];	//The id numbers of the connections, matches up to other rooms' rmNums

};
/**************************************
Function: checkCon
checks the number given against the connection ids
of the connections for room rm. If rm is already connected
to that room, then return 1. Otherwise return 0.

***************************************/
int checkCon(struct Room rm, int check)
{
	int j;
	for (j = 0; j < rm.conCount; j++)
	{
		//printf("Doing checkCon Now:\n");
		//printf("check is %i\n", check);
		//printf("rm.conCount is %i\n", rm.conCount);
		//printf("rm.conID[%d] is %i\n", j, rm.conID[j]);
		if (check == rm.conID[j])
		{
			//printf("This room already has that connection.\n");
			return 1;
		}
			
	}
	//printf("No connection with that name!!\n");
	return 0;
}
/**************************************
Function: checkCount
returns 1 if the room being checked is already
full of connections. Otherwise returns 0.

***************************************/
int checkCount(struct Room rm)
{
	//printf("doing checkCount now:\n");
	int numOfCons = 0;
	int j;
	for (j = 0; j < rm.conCount; j++)
	{
		//printf("numOfCoins is %i\n", numOfCons);
		//printf("j is %i\n", j);
		if (rm.conID[j] != -1)
		{
			//printf("rm.conID[j] is full");
			numOfCons++;
		}
	}
	//printf("numOfCoins finally is %i\n", numOfCons);
	//printf("The conCount for this rm is %i\n", rm.conCount);
	if (numOfCons >= rm.conCount)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**************************************
Function: getName
Returns the name matching the ID given.

***************************************/
char* getName(char* namesArr[], int ID)
{
	return namesArr[ID];
}

/**************************************
Function: checkNames
Checks to make sure the current name is not
already used when making rooms.

***************************************/
int checkNames(int used[], int num)
{
    int a;
    for (a = 0; a < 7; a++)
    {
        if (used[a] == num)
        {
            return 1;
        }
    }
    return 0;
}

/**************************************
Function: mkRooms
This is a large function that makes the rooms, 
sets up their connections, and stores all of this
information in files. It needs to be run before 
the game is to be played, since the game relies
on the room files only to play.

***************************************/
void mkRooms()
{
	//the list of possible names
	char* names[10];
	names[0] = "Grotto";
	names[1] = "Taberna";
	names[2] = "Attic";
	names[3] = "Atrium";
	names[4] = "Vault";
	names[5] = "Sewers";
	names[6] = "Hovel";
	names[7] = "Cavern";
	names[8] = "Cellar";
	names[9] = "Other_Cellar";

	//to be used to ensure a decent number of connections that doesn't leave any out
	int conArr[] = {3, 3, 5, 4, 5, 4, 3};
	//the array of used names. This correlates to the indices of the names array.
    int used[] = {-1, -1, -1, -1, -1, -1, -1};
	//set up a random number
	int r;
	int rNum;
	int startSet = 0; //check to see if the start room is set
	int endSet = 0; //check to see if the end room is set
	struct Room rooms[7];
	int i;
	//set names, num of connections and type of rooms. We make 7 rooms in this case.
	for (i = 0; i < 7; i++)
	{
		//get a random room name
	    do
        {
            r = rand(); 
            rNum = r % 10;
        } while (checkNames(used, rNum) == 1);

		//assign the room name
		rooms[i].roomName=names[rNum];
		used[i] = rNum; //add room name to list of used room names

		//set the room number
		rooms[i].rmNum = i;
		rooms[i].rmNameNum = rNum; //set the rmNameNum to the number of the name in the names array
		//set the conIDs to -1, indicating empty
		int l;
		for (l = 0; l < 7; l++)
			rooms[i].conID[l] = -1;
		//assign type START_ROOM to the first room
		if (startSet == 0)
		{
			rooms[i].roomType = "START_ROOM";
			startSet = 1;
			//stNum = i;
			stNum = rooms[i].rmNameNum;
			//printf("no seg fault1\n");

		}
		//Assign type END_ROOM to the next room
		else if (endSet == 0)
		{
			rooms[i].roomType = "END_ROOM";
			endSet = 1;
			//printf("no seg fault2\n");
		}
		//all other rooms are mid rooms
		else
		{
			rooms[i].roomType = "MID_ROOM";
			//printf("no seg fault3\n");
			
		}
		
        /*
		r = rand();
		rNum = r % 4;
		rNum += 3;
		printf("%i\n", rNum);
		printf("no seg fault1\n");
		rooms[i].conCount = rNum;
		printf("no seg fault2\n");
		*/
		rooms[i].conCount = conArr[i];

	}
	
	//make the connections, by the end, the final room will be completely connected, so there is no need to connect it, because 
	//all previous connections will be full.
	for (i = 0; i < 6; i++)
	{
		//printf("**************WORKING ON ROOM %d********************\n", i);
	    //printf("no seg fault32\n");
		int j;
		//for each open connection in rooms[i]
		for (j = 0; j < rooms[i].conCount; j++)
		{
			//printf("rooms %d conCount is %d\n", i, rooms[i].conCount);
		   // printf("no seg fault4\n");
			//printf("The current conID is: %d\n", rooms[i].conID[j]);
			//check if the connection is open
			if (rooms[i].conID[j] == -1)
			{
				//printf("The current conID is: %d\n", rooms[i].conID[j]);
			   // printf("no seg fault5\n");
				//generate random number until it's not itself, doesnt already exist, or the rooms aren't full
				int fullCheck = 0; //checks if no more assignments can be made. 
				int chcon;
				int chcount;
				reCheck:
				do{

					r = rand();
					rNum = r % 7;
					
					fullCheck++;
					
					chcon = checkCon(rooms[i], rNum);
					chcount = checkCount(rooms[rNum]);
				}while ((rNum <= i || chcon == 1 || chcount == 1) && fullCheck < 100);
				//Here arises an odd situation. The first part of the do-while loop, checking that rNum <= i, fails at
				//certain times and lets a room connect to itself. I can't figure out why not, but this second check ensures 
				//even if a same number gets past the first check, it will not get past the second. It uses a goto statement
				//to go back up to the do while loop above. 
				//printf("i =: %d\n", i);
				//printf("rNum =: %d\n", rNum);
				if (i == rNum)
				{
					//printf("were going to recheck ==\n");
					goto reCheck;
				}
				/*
				else if (checkCon(rooms[i], rNum) == 1);
				{
					printf("were going to recheck checkCon\n");
					goto reCheck;
				}
				*/
				//printf("were not going to recheck\n");
				if (fullCheck <= 100) //if it has tried 100 times to find an open slot, and can find none, the connections are full
                {
                    rooms[i].conID[j] = rNum;
                    rooms[i].conNameNum[j] = rooms[rNum].rmNameNum;
                  
                    int k = 0;
					//get the next open connection.
                    for (k = 0; k <= rooms[rNum].conCount; k++)
					{
						if (rooms[rNum].conID[k] == -1)
						{
							break;	
						}
					}
					//assign the current room to the connections next connection
                    rooms[rNum].conID[k] = i;
                    rooms[rNum].conNameNum[k] = rooms[i].rmNameNum;
                }
				//there are no more open spaces, decrement the conCOunt because the current room can have no more.
                else
                {
                    rooms[i].conCount--;
                }
			}
		}
	}
	//Finally make files for all 7 rooms and store the gathered info inside.
	for (i = 0; i < 7; i++)
	{
		FILE * pFile;
		//get the route of the directory trey will be placed in, as well as the filenames.
		//the files are named the same as their room names.
		int pid = getpid();
		char route[20];
		sprintf(route, "fuerstj.rooms.%i", pid);
		char* fName = rooms[i].roomName;
		char* ext=".txt";
		char* path = route;
		char buf[40];
		sprintf(buf, "%s/%s%s", path, fName, ext);
		//open the file
		pFile = fopen (buf,"w");
		
		//if it can open successfully
		if (pFile!=NULL)
		{
			//store room name
			char* namePref = "ROOM NAME: ";
			char* roomName = rooms[i].roomName;
			fputs (namePref, pFile);
			fputs (roomName, pFile);
			fputs ("\n", pFile);
			//store the connections for that room
			int j = 0;
			int cNum = 1;
			while (rooms[i].conID[j] != -1)
			{
				char* conPref = "CONNECTION ";
				char* conSuf = ": ";
				int crNum = rooms[i].conNameNum[j];
				char* roomName = getName(names, crNum);
				fputs (conPref, pFile);
				fprintf (pFile, "%i", cNum);
				fputs (conSuf, pFile);
				fputs (roomName, pFile);
				fputs ("\n", pFile);
				j++;
				cNum++;
			}
			//store the room type
			fputs ("ROOM TYPE: ", pFile);
			fputs (rooms[i].roomType, pFile);
			
			fclose (pFile);
			
			
		}
		
		
		
		
	}
	
	
	
}
/**************************************
Function: checkUInput
checks the name that the user input vs 
the names in the list. If the name exits, 
then the entry is valid
***************************************/
int checkUInput(char* nameArr[], char* input)
{
	int i;
	int check = 0;
	for (i = 0; i < 10; i++)
	{
		if (strcmp(input, nameArr[i]) == 0)
		{
			return 1; //the name is correct
		}
	}
	return 0; //the name is not correct
}

/**************************************
Function: printRoom
prints contents ofthe room called 
fuerstj.rooms.pid.choice.txt

***************************************/
int printRoom(char* choice)
{
	FILE * pFile;
	//get route
	int pid = getpid();
	char route[20];
	sprintf(route, "fuerstj.rooms.%i", pid);
	char* fName = choice;
	char* ext=".txt";
	char* path = route;
	char buf[40];
	sprintf(buf, "%s/%s%s", path, fName, ext);
	//open the correct file
	pFile = fopen (buf,"r");
	
	//if the file opens correctly
	if (pFile!=NULL)
	{
		//first check the win condition. The room type is at the end of the file, so navigate there, go back enough spaces and read 
		//the text there. If it is equal to END_ROOM, then the game is over.
		fseek (pFile , -8 , SEEK_END);
		char type[15];
		fgets(type, 9, pFile);
		if (strcmp(type, "END_ROOM") == 0)
		{
			printf("CONGRATULATIONS YOU HAVE FOUND THE END ROOM!\n");
			return 1;
		}
		else
		{
			//print current location
			printf("\nCURRENT LOCATION: ");
			//go to the start of the file, and get the results of the line after the 11th character. So right after ROOM NAME:
			fseek ( pFile, 11, SEEK_SET);
			char curLoc[15];
			fgets(curLoc, 20, pFile);
			printf("%s", curLoc);
			//print the connections
			printf("POSSIBLE CONNECTIONS:\n");
			fseek ( pFile, 14, SEEK_CUR);
			char con[15];
			char cur[15];
			int chck = 0;  //check for comma
			//This iterates through the rest of the lines in the file, but throws out only the last one, which is room type
			while (fgets(con, sizeof con, pFile) != NULL)
			{
				strcpy(cur, con);
				if (fgets(con, sizeof con, pFile) != NULL)
				{
					if (chck == 1)
					{
						printf(", ");  //spacer
					}
					chck = 1;
					char* curN = strtok(cur, "\n");
					printf("%s", curN);
				}
			}
			printf(".\n"); //print period and break after connections
			printf("WHERE TO? >");  //ask user for input
			
			if (feof(pFile))
			{
			// hit end of file
			}
			else
			{
			// some other error interrupted the read
			}
		}
		return 0;
	}
	else
	{
		printf("error opening file..."); //cant open the file. SHould never happen
	}
	fclose(pFile);
}
/**************************************
Function: storeRoute
stores the visited rooms in an array route.
This needs to use the names array because 
otherwise the array would hold only pointers
to the last used room, which is not good.

***************************************/
int storeRoute(char* input, char* route[], char* names[])
{
	int i;
	for (i = 0; i < 10; i++)
	{
		//printf("input is %s\n", input);
		if (strcmp(input, names[i]) == 0)
		{
			//printf("match found\n");
			return i;
		}
	}
}

int main()
{
	srand(time(NULL));
	//names array for reference in main
	char *names[10];
	names[0] = "Grotto";
	names[1] = "Taberna";
	names[2] = "Attic";
	names[3] = "Atrium";
	names[4] = "Vault";
	names[5] = "Sewers";
	names[6] = "Hovel";
	names[7] = "Cavern";
	names[8] = "Cellar";
	names[9] = "Other_Cellar";
	
	//get the pid and construct the route
	struct stat st = {0};
	int id = getpid();
	char route[20];
	sprintf(route, "fuerstj.rooms.%i", id);
	//printf("%s\n", route);
	//make the directory if its valid
	if (stat(route, &st) == -1)
	{
		mkdir(route, 0700);
	}
	
	//make the room files.
	mkRooms();
	
	
	//printing name of first room
	printRoom(names[stNum]);
	//choice, choice 2 and test deal with getting the name of the user input, testing it and storing the visited room
	//in an array
	char* choice = names[stNum];
	char choice2[20];
	char test[20];
	char* visited[50];
	visited[0] = names[stNum]; //array of visited rooms
	int steps = 0; //number of steps taken, starts at 0
	int result = 0;
	//result checks the result of printRoom. If the result is 0, then the end room is not found
	//if result is 1, then the game is over.
	while (result == 0)
	{
		
		scanf ("%s",test);
		while (checkUInput(names, test) == 0)
		{
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
			
			result = printRoom(choice);
			scanf ("%s",test);
		}
		strcpy(choice2, test);
		result = printRoom(choice2);
		choice = choice2;
		steps++;
		//printf("steps = %d", steps);
		//printf("Choice saved in steps is %s\n", choice2);
		visited[steps] = names[storeRoute(choice2, visited, names)];
	}
	printf("YOU TOOK %d STEPS! YOUR PATH TO VICTORY WAS:\n", steps);
	int stepCount;
	for (stepCount = 0; stepCount <= steps; stepCount++)
	{
		printf("%s\n", visited[stepCount]);
	}
	
	
	
	return 0;
}
