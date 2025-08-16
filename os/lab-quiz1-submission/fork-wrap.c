#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int time_limit = 1;


int fork_wrap(){
  //modified
  int r = fork();
  if(r>0){
    int time = 0;
    while(time < time_limit){
        sleep(1);
        time++;
    }
    kill(r , SIGINT);
  }
  return r;
}

int main(){
  int r = fork_wrap();
  if(r == 0){
    printf("child %d\n", getpid());
    sleep(10);
  }
  else{
    printf("I am parent\n");
    wait(NULL);
    printf("reaped child\n");
  }
}
