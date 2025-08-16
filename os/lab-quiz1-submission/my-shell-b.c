#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
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


	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();


		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

        /*handling &&*/
        int is_double = 0;
        for(int i=0 ; i< MAX_NUM_TOKENS ; i++){
            if(tokens[i] == NULL){
                break;
            }
            else if(strcmp(tokens[i] , "&&") == 0){
                is_double = 1;
                break;
            }
        }
    
   
       //do whatever you want with the commands, here we just print them
        //simplyfying assuption that one command to execute
        
        int pid = fork();
        
        if(pid == 0){
            
            if(is_double == 0){
                 //implemented for echo $? but is not working echo $PATH working
                 if(strcmp(tokens[0] , "echo") == 0  && tokens[1][0] == '$'){
                     char* str = (char*)malloc(sizeof(tokens[1])+1);
                     strcpy(str , tokens[1]+1);
                     char* env = getenv(str);
                     tokens[1] = env;
                     execvp(tokens[0] , tokens);
                  }
                 // else execute code as usual
                 else{
                    int x = execvp(tokens[0] , tokens);
                    if(x == -1){
                       printf("command not found\n");
                       exit(1); // command failed
                    }
                    exit(0);
                }

            }

            else if(is_double == 1){
                  int iter = 1;
                  int prev[MAX_NUM_TOKENS];
                  prev[0] = 0;

                  for(int i=1 ; i< MAX_NUM_TOKENS ; i++){
                     prev[i] = -1;
                  }

                  for(int i=0 ; i<MAX_NUM_TOKENS ; i++){
                       if(tokens[i] == NULL){
                           break;
                       }
                       else if(strcmp(tokens[i] , "&&") == 0){
                           tokens[i] = NULL;
                           prev[iter++] = i+1;
                       }
                  }

                  // prev is calculated
                  int p[MAX_NUM_TOKENS][2];

                  for(int j=0 ; j<MAX_NUM_TOKENS ; j++){
                       pipe(p[j]);
                  }

                  for(int i=0 ; i<MAX_NUM_TOKENS ; i++){
                      if(prev[i] == -1){
                        break;
                      }
                      else{
                         int g = fork();
                         if(g == 0){
                            int x = execvp(*(tokens+prev[i]) , tokens+prev[i]);
                            
                            if(x == -1){
                                printf("command not found \n");
                            }
                           
                            close(p[i][0]);
                            write(p[i][1] , &x , sizeof(int));
                            close(p[i][1]);
                            
                            exit(1);
                            /*
                            i am now thinking it would have been easy using by using return status
                            WIFEXITED AND WIFEXITSTATUS  but my approach is also good , the only thing is 
                            on running cat "not existing file" exec returns that command was correct
                            but it throwed error, that why my output is not comming correct

                            run ls && ks && ps it would run fine
                            
                            */
                         }
                         else{
                            wait(NULL);
                            int success;
                            close(p[i][1]);
                            read(p[i][0] , &success , sizeof(int));
                            close(p[i][0]);
                            if(success == -1){
                                exit(1);
                            }
                            
                              
                         }
                      }
                  }
            }

            exit(0);
        }
        else{
            wait(NULL);
        }
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
