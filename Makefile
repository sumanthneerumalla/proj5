CXXFLAGS = -Wall -g

Driver.out: HashTable.cpp HashTable.h words.h primes.h test1.cpp
		g++ $(CXXFLAGS) HashTable.cpp HashTable.h words.h primes.h test1.cpp -o Driver.out

make all:
	make Driver.out


make clean:
		rm -rf *.o
		rm -rf *~
		rm -rf *.out

make run:
		make;
		./Driver.out