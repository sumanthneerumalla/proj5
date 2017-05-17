//
// Created by Sumanth on 5/3/2017.
//

#include <cstdlib>
#include "HashTable.h"

char *const HashTable::DELETED = (char *) 1;

unsigned int HashTable::hashCode(const char *str) {
    unsigned int val = 0;
    const unsigned int thirtyThree = 33;  // magic number from textbook

    int i = 0;
    while (str[i] != '\0') {
        val = val * thirtyThree + str[i];
        i++;
    }
    return val;
}

//default constructor
HashTable::HashTable(int n) {

    if (n > 199999) {
        throw std::out_of_range("Requesting too big of a heap, retry");
    }
    //initialize sizes of the variables used for arrays
    capacity1 = totalCapacity = n;
    size1 = 0;
    capacity2 = capacity3 = -1;
    size2 = size3 = -1;

    int nextPrime = binarySearch(primes, n, numPrimes);
    Table1 = new char *[nextPrime]();

    capacity1 = nextPrime;


    isReHashingBool = false;
}

bool HashTable::isRehashing() {
    return isReHashingBool;
}

int HashTable::tableSize(int table) {
    if (table == 0) {
        return capacity1;//return size of the old table
    } else if (table == 1) {
        return capacity2;
    } else
        (
                throw std::out_of_range("Try requesting another size ")

        );

}


int HashTable::size(int table) {
    return totalSize;//returns the size of the table so far
}

const char *HashTable::at(int index, int table) {
    if (table == 0) {
        //check the capacity of the first table
        if ((index < 0) or (index >= capacity1)) {
            throw std::out_of_range("index out of range");
        }
        return Table1[index];

    } else {

        //otherwise check the size of the second table
        if ((index < 0) or (index >= capacity2)) {
            throw std::out_of_range("index out of range");

        }
        return Table2[index];
    }


}

//binarySearch()
//use binary search in the array in order to find index of the next largest prime
int HashTable::binarySearch(const int *arr, int num, int size) {
    int low, high, mid;
    low = 0;
    high = size - 1;

    int location = -1;

    bool success = false;
    while ((success != true) and (low <= high)) {
/* Calculate mid index */
        mid = (low + high) / 2;
/* If element is found at mid index
then return the index */
        if (arr[mid] == num) {
            success = true;
            location = mid;
        } else if (arr[mid] > num && arr[mid - 1] < num) {
            success = true;
            location = mid;

        } else if (arr[mid] > num) {
            high = mid - 1;
        } else if (arr[mid] < num) {
            low = mid + 1;
        }
    }
    return arr[location];
}

void HashTable::dump() {
    cout << "HashTable #1: size = " << size1 << " tableSize = " << capacity1 << endl;
    for (int i = 0; i < capacity1; ++i) {
        cout << "H1[" << i << "]";
        if (Table1[i] == "\0") {
            //print nothing
            cout << endl;
        } else if (Table1[i] == DELETED) {
            //print nothing
            cout << "DELETED" << endl;
        } else {
            //print out the string thats at that array location
            cout << Table1[i] << endl;
        }


    }

    if (isRehashing()) {
        //print second table only if we're rehashing
        for (int i = 0; i < capacity1; ++i) {
            cout << "H2[" << i << "] = ";
            if (Table2[i] == "\0") {
                //print nothing
                cout << endl;
            } else if (Table2[i] == DELETED) {
                //print nothing
                cout << "DELETED" << endl;
            } else {
                //print out the string thats at that array location
                cout << Table2[i] << endl;
            }

        }

    }
}

HashTable::~HashTable() {

    for (int i = 0; i < capacity1; ++i) {
        free(Table1[i]);
    }
    if (isRehashing()) {
        for (int i = 0; i < capacity2; ++i) {
            free(Table2[i]);
        }
    }
}

void HashTable::insert(const char *str) {

    if (!find(str)) {

        totalSize++;

        checkToRehash();

        int hashValue = hashCode(str);

        //if we are not rehashing we enter into the first array, otherwise we enter into the second array
        if (!isRehashing()) {
            //update the size of individual table
            size1++;

            //use modulo to find appropriate index
            int insertionPoint, insertionPointCopy;
            insertionPoint = insertionPointCopy = hashValue % capacity1;
            bool success = false;
            int count = 0;
            while ((success == false) or (count < 10)) {
                //this condition just loops back to beginning if the end of the loop happens
                if (insertionPoint == capacity1) {
                    insertionPoint = 0;
                }
                if ((Table1[insertionPoint] == NULL) or (Table1[insertionPoint] == DELETED)) {
                    //we insert if the array index is null or, the location there was deleted recently
                    Table1[insertionPoint] = strdup(str);
                    success = true;
                } else {
                    count++;
                }
                if (count >= 10) {
                    isReHashingBool = true;
                }
            }


        } else {
            //update the size of individual table
            size1++;
            //use modulo to find appropriate index
            int insertionPoint = hashValue % (capacity2 - 1);
            bool success = false;

        }

    }
    //do nothing if it already exists in the data structure

}

void HashTable::beginRehashing1() {
//do only if is rehashing AND table 2 doesn't already exist
    if (isRehashing() && (Table2 == NULL)) {

        //this will only be called if rehashing needs to start.
        int FOURTIMES = 4;//the new table will be 4 times as large.
        capacity2 = FOURTIMES * capacity1;

        int nextPrime = binarySearch(primes, capacity2, numPrimes);

        //make the new table with the larger size
        Table2 = new char *[nextPrime]();
    }

}

int HashTable::findStartOfCluster(int index) {
    bool startFound = false;
    char **myTable;
    int myCapacity;
    if (isRehashing()) {
        myTable = Table2;
        myCapacity = capacity2;
    } else {
        myTable = Table1;
        myCapacity = capacity1;
    }

    while (startFound == false) {
        //keep decrementing index until a null has been found. Then return
        if (myTable[index] == NULL) {
            startFound = true;
            return index + 1;
        } else if (index == 0) {
            //roll over to the end of the list
            index = myCapacity;
        } else {
            index--;
        }
    }
}

int HashTable::migrateCluster(int index) {
    // we keep a count of items migrated to the destination array, so we can decrement the size later
    int count = 0;
    bool success = false;

    //loop through table1 cluster until we hit a null, moving over into the table 2 cluster
    while ((success == false)) {
        char *str = Table1[index];//get the string

        if (str == NULL) {
            break;//stop copying cluster over if we hit the end of the cluster
        }//otherwise we are still in the cluster and need to move this over
        else if (index == capacity1) {
            index = 0; //loop over to beginning of table if we hit the end of the table
        } else {
            //insert specifically into table two then free the memory
            otherInsert(str);
            free(str);
            index++;
            count++;
        }
    }

//return the number of items moved in this process. not really used, but done for testing
    return count;
}

//searches the arrays for the string.
bool HashTable::find(const char *str) {

    int hashValue = hashCode(str);
    int insertCopy;
    int insertionPoint = insertCopy = hashValue % capacity1;

    //find where it would be inserted in the first table

    bool success = false;
    int count = 0;
    while ((success == false)) {
        //we either find it
        if (strcmp(str, Table1[insertionPoint]) == 0) {
            return true;
            //if the spot is taken, or deleted, we keep searching the cluster
        } else if ((Table1[insertionPoint] != NULL) or (Table1[insertionPoint] == DELETED)) {
            count++;
            insertionPoint++;
        } else if (Table1[insertionPoint] == NULL) {
            //end the loop if we cant find it
            break;
        }

    }

    if (count > 1 && isRehashing()) {
        //if we haven't found it but found a cluster AND we're rehashing, we move over the cluster to Table 2
        int start = findStartOfCluster(insertCopy);
        migrateCluster(start);


        //then we search the second table
        int insertionPoint1 = hashValue % capacity2;

        //find where it would be inserted in the first table

        bool success1 = false;
        while ((success1 == false)) {
            //we either find it
            if (strcmp(str, Table1[insertionPoint1]) == 0) {
                return true;
                //if the spot is taken, or deleted, we keep searching the cluster
            } else if ((Table1[insertionPoint1] != NULL) or (Table1[insertionPoint1] == DELETED)) {
                insertionPoint1++;
            } else if (Table1[insertionPoint1] == NULL) {
                //end the loop if we cant find it
                break;
            }

        }

    }

    //after this point, we have not found anything in either table
    return false;

}

void HashTable::checkToRehash() {
    if (!isRehashing()) {
        float loadFactor = size1 / capacity1;
        if (loadFactor > 0.5) {
            //we start rehashing if the load factor is over 50%
            isReHashingBool = true;
        }
        //this will make the second table 4 times as large as the first table
        beginRehashing1();
    } else {//otherwise we are rehashing, and we need to check for <3%
        float loadFactor = size1 / capacity1;

        if (loadFactor < 0.03 && isRehashing()) {
            //we stop rehashing if the load factor is less than 3%
            isReHashingBool = false;
            //call the function to copy over everything in table1 to table2
            finishMigrate();
        }

    }
}

//inserts only into the second array, used for moving over
void HashTable::otherInsert(const char *str) {
    if (!find(str)) {
        size2++;
        int hashValue = hashCode(str);
        int insertionPoint = hashValue % capacity2;

        bool success = false;
        int count = 0;
        while ((success == false) or (count < 10)) {
            //this condition just loops back to beginning if the end of the loop happens
            if (insertionPoint == capacity1) {
                insertionPoint = 0;
            }
            if ((Table2[insertionPoint] == NULL) or (Table2[insertionPoint] == DELETED)) {
                //we insert if the array index is null or, the location there was deleted recently
                Table2[insertionPoint] = strdup(str);
                success = true;
            } else {
                count++;
            }
            if (count >= 10) {
                giveUp();
                //after giving up, insert into the first table, isRehashing will be false at this point.
                insert(str);
            }
        }
    }
}

void HashTable::giveUp() {

    //this will only be called if rehashing needs to start.
    int FOURTIMES = 4;//the new table will be 4 times as large as the two tables added.
    capacity3 = FOURTIMES * (capacity1 + capacity2);

    int nextPrime = binarySearch(primes, capacity3, numPrimes);

    //make the new table with the larger size
    char **Table3 = new char *[nextPrime]();

    for (int i = 0; i < capacity1; ++i) {
        //move everything in the first table over
        char *str = Table1[i];
        int hashValue = hashCode(str);
        int insertionPoint = hashValue % capacity3;
        bool success = false;
        while ((success == false)) {
            //this condition just loops back to beginning if the end of the loop happens
            if (insertionPoint == capacity3) {
                insertionPoint = 0;
            }
            if ((Table3[insertionPoint] == NULL)) {
                //we insert if the array index is null
                Table3[insertionPoint] = strdup(str);
                success = true;
                free(Table1[i]);
                //free the memory after moving it over
            } else {
                //keep going until we find an empty spot
                insertionPoint++;
            }

        }

    }
    //move everything in the second table over
    for (int i = 0; i < capacity2; ++i) {
        char *str = Table2[i];
        int hashValue = hashCode(str);
        int insertionPoint = hashValue % capacity3;
        bool success = false;
        while ((success == false)) {
            //this condition just loops back to beginning if the end of the loop happens
            if (insertionPoint == capacity3) {
                insertionPoint = 0;
            }
            if ((Table3[insertionPoint] == NULL)) {
                //we insert if the array index is null
                Table3[insertionPoint] = strdup(str);
                success = true;
                free(Table2[i]);
                //free the memory after moving it over
            } else {
                //keep going until we find an empty spot
                insertionPoint++;
            }

        }

    }


    //at this point everything in both tables have been moved over to the third table, we can free the two tables
    free(Table2);
    free(Table1);
    Table1 = Table3;
    //Table 1 is now the new larger table
    capacity1 = capacity3;
    capacity2 = 0;
    size1 = size1 + size2;
    totalCapacity = capacity3;
    totalSize = size1;
    isReHashingBool = false;

}

void HashTable::finishMigrate() {
    for (int i = 0; i < capacity1; ++i) {
        //move everything in the first table over into second table
        char *str = Table1[i];
        //use function specifically for inserting into table 2, which calls give up if necessary
        otherInsert(str);
        //dont forget to free the memory at that spot, and then set to null, not deleted
        free(Table1[i]);
        Table1[i] = NULL;
        size1--;
        size2++;
    }
}

