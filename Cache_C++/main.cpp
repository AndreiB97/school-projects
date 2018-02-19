#include "stdio.h"
#include "fstream"
#include "cache.h"

using namespace std;

template <class T>
void readOp(CPU<T> cache) {
	// reads the operations from operations.in and executes them
    int c;
    char op;
    unsigned int addr;
    T data;

    ifstream ramIn("ram.in");

    while (ramIn >> addr >> data) {
        ramArr[n].addr = addr;
        ramArr[n].data = data;
        n++;
    }

    ramIn.close();

    ifstream operationsIn("operations.in");

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
