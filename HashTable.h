//
// Created by Sumanth on 5/3/2017.
//

#ifndef PROJ5_HASHTABLE_H
#define PROJ5_HASHTABLE_H

#include <stdexcept>      // std::out_of_range


class HashTable {


public:

    unsigned int hashCode(const char *str);
    //default constructor, default size is 101
    HashTable(int n=101) ;
    ~HashTable() ;


private:

    bool reHash; //used to see if we need to rehash
    unsigned  int capacity1, capacity2;
};


#endif //PROJ5_HASHTABLE_H
