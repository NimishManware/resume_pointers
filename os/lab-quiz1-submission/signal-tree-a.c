#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>

/*
 * Add following two lines before sending signal
 * ```
 * sleep(1);
 * printf("Sending signal! PID=%d\n",getpid());
 * ```
 * sleep is to ensure that all processes have started
 *
 * Make sure you add a `sleep(5)` in the process receiving the signal.
 * This is required to ensure that the process receiving the signal doesn't
 * terminate before the signal is sent.
 */

void signal_handler_sigint(int signo)
{
  if ( signo == SIGINT ) {
    printf("Received signal! PID=%d\n",getpid());
    fflush(stdout);
    exit(1);
  }
}

void handler2(int signo){

}

int main() 
{
  int k1,k2;
  printf("[P1] pid=%d\n",getpid());
  fflush(stdout);


  
  if ( signal(SIGINT,signal_handler_sigint) == SIG_ERR) {
    // You don't need to handle this error
    // This is just to ensure that signal handler is registered properly
    printf("Could not register signal handler");
    return 2;
  }
  
  k1 = fork();
  
  if ( k1 == 0 ) {
    printf("[P2] pid=%d\n",getpid());
    fflush(stdout);
    sleep(5);
  }
  else {
    k2 = fork();
    
    if ( k2 == 0 ) {
      printf("[P3] pid=%d\n",getpid());
      fflush(stdout);
    }
    else{
        sleep(1);
        printf("Sending signal! PID=%d\n",getpid());
        kill(k1 , SIGINT);
    }
  }
}
