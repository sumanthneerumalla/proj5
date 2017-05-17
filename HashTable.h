//
// Created by Sumanth on 5/3/2017.
//

#ifndef PROJ5_HASHTABLE_H
#define PROJ5_HASHTABLE_H

#include <stdexcept>      // std::out_of_range
#include <iostream>
#include "primes.h"
#include <string.h>

using namespace std;

class HashTable {
public:

    static char * const DELETED ;

    unsigned int hashCode(const char *str);

    //default constructor, default size is 101
    HashTable(int n = 101);

    ~HashTable();

    HashTable(HashTable &other);

    HashTable &operator=(HashTable &rhs);

    int binarySearch (const int arr[], int num, int size) ;


    void insert(const char *str);

    //inserts only into the second array, used for moving over
    void otherInsert(const char* str);

    void beginRehashing1();

    bool find(const char *str);

    void giveUp();

    char *remove(const char *str);

    void dump();

    //grading functions

    bool isRehashing();

    int tableSize(int table = 0);

    int size(int table = 0);

    const char *at(int index, int table = 0);

    int findStartOfCluster(int index);

    //moves everything in the given table at that index into the destination table, returns moved items
    int migrateCluster(int index);

    void checkToRehash();

    void finishMigrate();


private:

    bool isReHashingBool; //used to see if we need to rehash
    int capacity1, capacity2,capacity3,totalCapacity;
     int size1,size2,size3,totalSize;

    char** Table1;
    char** Table2;
};


#endif //PROJ5_HASHTABLE_H
