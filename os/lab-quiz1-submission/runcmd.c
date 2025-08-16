#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Incorrect number of arguments\n");
        exit(0);
    }
    else{
        int f = fork();
        if(f == 0){
            execvp(argv[1] , argv+1);
        }
        else{
            wait(NULL);
        }
    }
}