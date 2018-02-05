#include "stdio.h"
#include "fstream"
#include "cache.h"
using namespace std;

// adaugat comm

// scris README

template <class T>
void readOp(CPU<T> cache) {
    int c;
    char op;
    unsigned int addr;
    T data;

    ifstream ramIn("ram.in");

    // stocare ram

    while (ramIn >> addr >> data) {
        ramArr[n].addr = addr;
        ramArr[n].data = data;
        n++;
    }

    ramIn.close();

    ifstream operationsIn("operations.in");

    //citire si executare operatii
    while(operationsIn >> c >> op >> addr) {
        if (op == 'w') {
            operationsIn >> data;
            cache.write(c, addr, data);
        } else {
            cache.read(c, addr);
        }
    }

    operationsIn.close();

    cache.print();
}

int main()
{
    CPU<int> cache;
    readOp(cache);
    return 0;
}
