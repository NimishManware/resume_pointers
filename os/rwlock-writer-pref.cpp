#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  pthread_mutex_init(&rw->thing,NULL);
  rw->numRead = 0;
  rw->numWrit = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->thing);
  // cout<<"read lock, numRead is "<<rw->numRead<<" and numWrite is "<<rw->numWrit<<endl;
  if (rw->numWrit > 0) {
    // cout<<"here\n";
    pthread_mutex_unlock(&rw->thing);
  }
  else {
    pthread_mutex_unlock(&rw->thing);
    rw->numRead++;
    return;
  }
  while (rw->numWrit > 0) {
    ;
  }
  rw->numRead++;
  return;
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  // cout<<"read unlock called, numRead is "<<rw->numRead<<endl;
  rw->numRead--;
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->thing);
  rw->numWrit++;
  // cout<<"write lock, numRead is "<<rw->numRead<<endl;
  if (rw->numRead > 0) {
    // cout<<"write lock failed, numRead is "<<rw->numRead<<endl;
    pthread_mutex_unlock(&rw->thing);
  }
  else {
    // cout<<"write lock over, numRead is "<<rw->numRead<<endl;
    return;
  }
  while (rw->numRead > 0 || rw->numWrit > 0) {
    pthread_mutex_lock(&rw->thing);
    // cout<<"write lock, numRead is "<<rw->numRead<<endl;
    if (rw->numRead > 0) {
      // cout<<"write lock failed, numRead is "<<rw->numRead<<endl;
      pthread_mutex_unlock(&rw->thing);
    }
    else {
      // cout<<"write lock over, numRead is "<<rw->numRead<<endl;
      return;
    }
  }  
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  // cout<<"write unlock, numRead is "<<rw->numRead<<endl;
  rw->numWrit--;
  pthread_mutex_unlock(&rw->thing);
}
