#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
    pthread_mutex_init(&s->lock,NULL);
    pthread_cond_init(&s->signalVar,NULL);
    s->countVal = value;
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&s->lock);
    s->countVal--;
    if (s->countVal < 0) {
        pthread_cond_wait(&s->signalVar,&s->lock);
    }
    pthread_mutex_unlock(&s->lock);
}

void zem_up(zem_t *s) {
    pthread_mutex_lock(&s->lock);
    s->countVal++;
    pthread_cond_signal(&s->signalVar);
    pthread_mutex_unlock(&s->lock);
}
