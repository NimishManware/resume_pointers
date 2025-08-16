#include <iostream>

/*
In this exercise, we will implement heap as discussed in the class.
We need to implement the following functions.


ONLY THIS FILE MUST BE MODIFIED FOR SUBMISSION

You may edit main.cpp for writing more test cases.
But your code must continue to work with the original main.cpp


THERE IS ONLY ONE TEST CASE ONLY FOR YOUR UNDERSTANDING.

-- You may need to generate more random tests to get your tests right
-- Print will not work until you have a good printing function
*/

 
#include "heap.h"


int Heap::parent(int i) {
  if(i%2 ==0){
    return (i-2)/2;
  }
  else{
    return (i-1)/2;
  }
}

int Heap::left(int i) {
  return 2*i+1;
}

int Heap::right(int i) {
  return 2*i+2;
}

int Heap::max() {
  return store[0];
}

void Heap::swap(int i, int j) {
  int temp = store[i];
  store[i] = store[j];
  store[j] = temp;
}

void Heap::insert(int v) {
  
  append(v);
  int t = sz-1;
  while( t>0 && store[parent(t)]<store[t] ){
       swap(parent(t),t);
       swap_count++;
       t = parent(t);
  }
}

void Heap::heapify(int i) {
  if(sz == 0){
    return;
  }
  int largest = i;
  unsigned int left = 2*i+1;
  unsigned int right = 2*i+2;
  if(left < sz && store[largest] < store[left] ){
    largest = left;
  }
  if(right < sz && store[largest] < store[right] ){
    largest = right;
  }

  if(largest == i){
    return;
  }
    swap(largest,i);
    swap_count++;
    heapify(largest);

}


void Heap::deleteMax() {
  swap(0,sz-1);
  swap_count++;
  sz = sz-1;
  heapify(0);
}

void Heap::buildHeap() {
  for(int i = sz-1 ; i>=0 ; i--){
    heapify(i);
  }
}

void Heap::heapSort() {
  buildHeap();
  while (sz>0) {deleteMax();}
}


