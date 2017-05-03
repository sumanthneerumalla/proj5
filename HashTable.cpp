//
// Created by Sumanth on 5/3/2017.
//

#include "HashTable.h"

unsigned int HashTable::hashCode(const char *str) {
    unsigned int val = 0 ;
    const unsigned int thirtyThree = 33 ;  // magic number from textbook

    int i = 0 ;
    while (str[i] != '\0') {
        val = val * thirtyThree + str[i] ;
        i++ ;
    }
    return val ;
}

HashTable::HashTable(int n) {

    if (n > 199999){
        throw std::out_of_range("Requesting too big of a heap, retry");
    }

}


