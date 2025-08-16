#include "dictionary.h"
#include "math.h"
Dictionary::Dictionary(){
    N = DICT_SIZE;
    A = new Entry[N];
    for(int i = 0; i < N; i++){
        Entry* e = new Entry;
        e->key = new char[32];
        strcpy(e->key, "");
        e->value = (int)NULL;
        A[i] = *e;
    }
};


int Dictionary::hashValue(char key[]){
    // int size = sizeof(key)/ sizeof(key[0]);
    int size = 0;
    while(key[size] != '\0'){
        size++;
    }
    int hashValue = 0;
    int x = 33;
    // int x = 2;
    for(int i = size - 1; i >= 0; i--){
        int val = key[i] - 'a';
        hashValue = (val + x * hashValue) % N;
    }
    double a1 = (sqrt(5)-1)/2;
    double ka = double(hashValue)*a1;
    ka-=floor(ka);
    hashValue = floor(DICT_SIZE*ka);
    return hashValue;
}

int Dictionary::findFreeIndex(char key[]){
    int hash = hashValue(key);
    
    for(int i = 0; i < N; i++){
        int index = (hash + i)%N;
        
        if(strcmp(A[index].key,"") == 0){
            return index;
        }
        else if(strcmp(A[index].key,"TOMBSTONE") == 0){
            return index;
        }
    }
    return -1;
}

struct Entry* Dictionary::get(char key[]){
    int hash = hashValue(key);

    for(int i = 0; i < N; i++){
        int index = (hash + i)%N;

        if(strcmp(A[index].key,"") == 0){
            return NULL;
        }       

        if(strcmp(A[index].key,key) == 0){
            return &A[index];
        } 
    }

    return NULL;
}

bool Dictionary::put(Entry e){
    int freeIndex = findFreeIndex(e.key);
    if(freeIndex == -1){
        return false;
    }
    else{
        A[freeIndex] = e;
        return true;
    }

}

bool Dictionary::remove(char key[]){
    Entry *e = get(key);
    if(!e){
        return false;
    }
    else{
        strcpy(e->key, "TOMBSTONE");
        e->value = (int)NULL;
        return true;
    }
}
