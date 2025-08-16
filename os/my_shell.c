#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <stdbool.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)

/* Splits the string by space and returns the array of tokens
*
*/

bool kyaAgliLinePeJaye = false;

void sigHandler (int signal) {
	if (signal == SIGCHLD) {
		printf("oyeee\n");
		wait(NULL);
	}
	kyaAgliLinePeJaye = true;
}

int findString (char **tokens, char *apna, int numToks) {
	int retVal = -1;
	for (int i=0; i<numToks; i++) {
		if (strcmp(tokens[i],apna) == 0) {
			retVal = i;
		}
	}
	return retVal;
}

int findFreeIndex (int waitKiskeLiyeKarRhe[64], int n) {
	//n = 64
	for (int i=0; i<64; i++) {
		if (waitKiskeLiyeKarRhe[i] == -1) {
			return i;
		}
	}
	return -1;
}

char **tokenizeForAmpersand(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == '&' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}



int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	bool pehlaNasha = false;
	int waitKiskeLiyeKarRhe[64];
	for (int i=0; i<64; i++) {
		waitKiskeLiyeKarRhe[i] = -1;
	}

	while(1) {			
		signal(SIGINT,sigHandler);
		//signal(SIGCHLD,sigHandler);
		int seriesOrPar = -1;
		if (pehlaNasha) {
			for (int i=0; i<64; i++) {
				int status;
				//printf("%d\n",waitKiskeLiyeKarRhe[i]);
				if (waitKiskeLiyeKarRhe[i] > 0) {
					pid_t result = waitpid(waitKiskeLiyeKarRhe[i],&status,WNOHANG);
					if (result != 0) {
						//pehlaNasha = false;
						//kill(waitKiskeLiyeKarRhe[i],9);
						waitKiskeLiyeKarRhe[i] = -1;
					}
				}
			}
		}
		if (kyaAgliLinePeJaye) {
			kyaAgliLinePeJaye = false;
			printf("\n");
			continue;
		}
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		//printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		char backupLine[MAX_INPUT_SIZE];
		for (int i=0; i<MAX_INPUT_SIZE; i++) {
			backupLine[i] = line[i];
		}
		tokens = tokenize(line);

       //do whatever you want with the commands, here we just print them
		//get numToks ok but bhow hto

		int tokCount = 0;
		for (int j=0; tokens[j] != NULL; j++) {
			tokCount++;
		}

		if (tokens[0] == NULL) {
			continue;
		}

		for (int i=0; i<tokCount; i++) {
			if (strcmp(tokens[i],"&&") == 0) {
				seriesOrPar = 0; //0 -> series
			}
			else if (strcmp(tokens[i],"&&&") == 0) {
				seriesOrPar = 1; //1 -> parallel
			}
		}

		//for(i=0;tokens[i]!=NULL;i++){
		if (tokens[0][0] == 'c' && tokens[0][1] == 'd' && tokens[0][2] == '\0') {
			chdir(tokens[1]);
		}
		else if (tokens[1] == NULL && tokens[0][0] == 'e' && tokens[0][1] == 'x' && tokens[0][2] == 'i' && tokens[0][3] == 't') {
			for (int i=0; i<64; i++) {
				if (waitKiskeLiyeKarRhe[i] != -1) {
					kill(waitKiskeLiyeKarRhe[i],9);
				}
				else {
					break;
				}
			}
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);
			return 0;
		}
		else {
			if (seriesOrPar == 0) {
				//code
				//first tokenize the line and try to do some shizz
				char **subCommands = (char**)malloc(MAX_NUM_TOKENS*sizeof(char*));
				int tokPointer = 0;
				for (int i=0; i<= tokCount; i++) {
					if (kyaAgliLinePeJaye) {
						break;
					}
					//printf("%s\n",tokens[i]);
					if (i == tokCount) {
						int ret = fork();
						if (ret == 0) {
							execvp(subCommands[0],subCommands);
							printf("bhai bruh\n");
							exit(0);
						}
						else {
							waitpid(ret,NULL,W_OK);
						}
						continue;
					}
					if (strcmp(tokens[i],"&&") == 0) {
						//do stuff
						subCommands[tokPointer] = NULL;
						int tmp = tokPointer;
						for (int i= tokPointer + 1; i<MAX_NUM_TOKENS; i++) {
							subCommands[i] = NULL;//just to make sure
						}
						tokPointer = 0;
						int ret = fork();
						if (ret == 0) {
							execvp(subCommands[0],subCommands);
							printf("bhai gadhagiri mat kar\n");
							exit(0);
						}
						else {
							waitpid(ret,NULL,W_OK);
						}
					}
					else {
						//printf("yaha pohoche\n");
						//subCommands[i] = tokens[i]
						subCommands[tokPointer] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
						strcpy(subCommands[tokPointer++],tokens[i]);
						//printf("%s",subCommands[tokPointer-1]);
					}
				}
				for (int i=0; subCommands[i] != NULL; i++) {
					free(subCommands[i]);
				}
				free(subCommands);

				seriesOrPar = -1;
				for (int i=0; tokens[i] != NULL; i++) {
					free(tokens[i]);
				}
				free(tokens);
				continue;
			}
			else if (seriesOrPar == 1) {
				if (kyaAgliLinePeJaye) {
						break;
				}
				char **subCommands = (char**)malloc(MAX_NUM_TOKENS*sizeof(char*));
				int tokPointer = 0;
				int listOfThingsToWaitFor[64];
				for (int i=0; i<64; i++) {
					listOfThingsToWaitFor[i] = -1;
				}
				for (int i=0; i<= tokCount; i++) {
					//printf("%s\n",tokens[i]);
					if (i == tokCount) {
						int ret = fork();
						if (ret == 0) {
							execvp(subCommands[0],subCommands);
							printf("bhai bruh\n");
							exit(0);
						}
						else {
							listOfThingsToWaitFor[findFreeIndex(listOfThingsToWaitFor,64)] = ret;
						}
						continue;
					}
					if (strcmp(tokens[i],"&&&") == 0) {
						//do stuff
						subCommands[tokPointer] = NULL;
						int tmp = tokPointer;
						for (int i= tokPointer + 1; i<MAX_NUM_TOKENS; i++) {
							subCommands[i] = NULL;//just to make sure
						}
						tokPointer = 0;
						int ret = fork();
						if (ret == 0) {
							execvp(subCommands[0],subCommands);
							printf("bhai gadhagiri mat kar\n");
							exit(0);
						}
						else {
							listOfThingsToWaitFor[findFreeIndex(listOfThingsToWaitFor,64)] = ret;
						}
					}
					else {
						//printf("yaha pohoche\n");
						//subCommands[i] = tokens[i]
						subCommands[tokPointer] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
						strcpy(subCommands[tokPointer++],tokens[i]);
						//printf("%s",subCommands[tokPointer-1]);
					}
				}
				for (int i=0; i<64; i++) {
					if (listOfThingsToWaitFor[i] != -1) {
						waitpid(listOfThingsToWaitFor[i],NULL,W_OK);
					}
				}
				for (int i=0; subCommands[i] != NULL; i++) {
					free(subCommands[i]);
				}
				free(subCommands);

				seriesOrPar = -1;
				for (int i=0; tokens[i] != NULL; i++) {
					free(tokens[i]);
				}
				free(tokens);
				continue;
			}
			if (findString(tokens,"&",tokCount) != -1) {
				//printf("idhar aa rhe\r\n");
				//part B
				//lets split this lol
				int whereAnd = findString(tokens,"&",tokCount);
				if (whereAnd == tokCount - 1) { // if its something like A & B & C &
					
					//find subcommands
					char **subCommands = tokenizeForAmpersand(backupLine);
					int numSubCommands = 0;
					for (int i=0; subCommands[i] != NULL; i++) {
						numSubCommands++;
					}
					for (int i=0; i<numSubCommands; i++) {
						char **tokenizedSubCommand = tokenize(subCommands[i]);
						int ret = fork();
						if (ret == 0) {
							setpgid(0,0);
							execvp(tokenizedSubCommand[0],tokenizedSubCommand);
							printf("bruh gadhagiri mat kar\r\n");
							exit(0);
						}
						else {
							signal(SIGINT,sigHandler);
							waitKiskeLiyeKarRhe[findFreeIndex(waitKiskeLiyeKarRhe,64)] = ret;
							pehlaNasha = true;
						}
						//free tokenizedSubCommands
						for (int i=0; tokenizedSubCommand[i] != NULL;i++) {
							free(tokenizedSubCommand[i]);
						}
						free(tokenizedSubCommand);
					}
					//free subCommands
					for (int i=0; subCommands[i] != NULL; i++) {
						free(subCommands[i]);
					}
					free(subCommands);
				}
				else { // A & B & C
				/*
					char **subCommands = tokenizeForAmpersand(backupLine);
					int numSubCommands = 0;
					for (int i=0; subCommands[i] != NULL; i++) {
						numSubCommands++;
					}
					for (int i=0; i<numSubCommands; i++) {
						char **tokenizedSubCommand = tokenize(subCommands[i]);
						int ret = fork();
						if (ret == 0) {
							if (i != numSubCommands - 1) {
								setpgid(0,0);
							}
							execvp(tokenizedSubCommand[0],tokenizedSubCommand);
							printf("bruh gadhagiri mat kar\r\n");
							exit(0);
						}
						else {
							signal(SIGINT,sigHandler);
							if (i != numSubCommands - 1) {
								waitKiskeLiyeKarRhe[findFreeIndex(waitKiskeLiyeKarRhe,64)] = ret;
								pehlaNasha = true;
							}
							else {
								waitpid(ret,NULL,W_OK);
							}
						}
						for (int i=0; tokenizedSubCommand[i] != NULL;i++) {
							free(tokenizedSubCommand[i]);
						}
						free(tokenizedSubCommand);
					}
					for (int i=0; subCommands[i] != NULL; i++) {
						free(subCommands[i]);
					}
					free(subCommands);
				*/
					char **subCommands = (char **)malloc(MAX_NUM_TOKENS*sizeof(char*));
					int tokPointer = 0;
					for (int i=0; i<=tokCount; i++) {
						if (i == tokCount) {
							int ret = fork();
							if (ret == 0) {
								execvp(subCommands[0],subCommands);
								printf("bruh gadhagiri mat kar\n");
								exit(0);
							}
							else {
								waitpid(ret,NULL,W_OK);
							}
						}
						else if (strcmp(tokens[i],"&") == 0) {
							subCommands[tokPointer] = NULL;
							int tmp = tokPointer;
							for (int j=tokPointer+1; j<MAX_NUM_TOKENS; j++) {
								subCommands[j] = NULL;
							}
							tokPointer = 0;
							int ret = fork();
							if (ret == 0) {
								execvp(subCommands[0],subCommands);
								printf("bruh gadhagiri mat kar\n");
								exit(0);
							}
							else {
								waitKiskeLiyeKarRhe[findFreeIndex(waitKiskeLiyeKarRhe,64)] = ret;
								pehlaNasha = true;
							}
						}
						else {
							subCommands[tokPointer] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
							strcpy(subCommands[tokPointer++],tokens[i]);
						}
					}
					for (int i=0; subCommands[i] != NULL; i++) {
						free(subCommands[i]);
					}
					free(subCommands);
				}
				
			}
			else {
				int ret = fork();
				if (ret == 0) {
					execvp(tokens[0],tokens);
					printf("bruh gadhagiri mat kar\r\n");
					exit(0);
				}
				else {
					signal(SIGINT,sigHandler);
					waitpid(ret,NULL,W_OK);
					//pehlaNasha = false;
				}
			}
		}
		//}
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
