#include "stdio.h"
#include "fstream"

#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

using namespace std;


template <class T>
class ram {
	// ram data structure
public:
    unsigned int addr;
    T data;
};

// ram vector
ram<int> ramArr[1000000];
int n;

template <class T>
void updateData (unsigned int addr, T data) {
		// update data in ram
        int i = (addr-ramArr[0].addr)/4;
        ramArr[i].data = data;
    }

template <class T>
T getData (unsigned int addr) {
	// get data at addr from ram
    int i = (addr-ramArr[0].addr)/4;
    return ramArr[i].data;
}

void printRam() {
	// write contents of ram in ram.out
    FILE *ramOut = fopen("ram.out", "w");
    for (int i = 0; i < n; i++) {
        fprintf(ramOut, "%010d %010d\n", ramArr[i].addr, ramArr[i].data);
    }
}

template <class T>
class way {
	// way data structure used for set
public:
    unsigned int addr;
    T data;
    bool last;
    bool dirty;

    way() {
        last = true;
        data = -1;
    }
};

template <class T>
class set {
	// set data structure used for cache
public:
    way<T> w[2];

    int findFirst() {
        // returns which way was used first
        if (w[0].data == -1) {
            return 0;
        }
        if (w[1].data == -1) {
            return 1;
        }
        if (w[0].last == false) {
            return 0;
        } else {
            return 1;
        }
    }

    int findAddr(unsigned int addr) {
        // looks for addr in current set
        for (int i = 0; i <= 1; i++) {
            if (w[i].addr == addr) {
                return i;
            }
        }
        return findFirst();
    }

    void markDirty(unsigned int addr) {
        // marks the value at addr as dirty
        for (int i = 0; i <= 1; i++) {
            if (w[i].addr == addr) {
                w[i].dirty = true;
            }
        }
    }
};

template <class T>
class L1 {
	// data structure for L1 cache
public:
    set<T> s[2048];

    unsigned int hashL1(unsigned int key) {
		// hashing function
        unsigned int hash = 0;
        int p = 1;
        
        key /= 4;
        for (int i = 2; i <= 12 && key; i++)  {
            hash += (key%2)*p;
            p *= 2;
            key /= 2;
        }
        return hash;
    }

};

template <class T>
class L2 {
	// data structure for L2 cache
public:
    set<T> s[8192];

    unsigned int hashL2(unsigned int key) {
		// hashing function
        unsigned int hash = 0;
        int p = 1;

        key /= 4;
        for (int i = 2; i <= 14 && key; i++)  {
            hash += (key%2)*p;
            p *= 2;
            key /= 2;
        }
        return hash;
    }
};

template <class T>
class CPU {
	// data structure for CPU
    L1<T> core[2];
    L2<T> shared;
public:
    void read(int c, unsigned int addr) {
		// implements the read operations
        unsigned int hL1 = core[c].hashL1(addr);
        int xL1 = core[c].s[hL1].findAddr(addr);

        if (core[c].s[hL1].w[xL1].data == -1 ||
            core[c].s[hL1].w[xL1].addr != addr ||
            core[c].s[hL1].w[xL1].dirty) {
                unsigned int hL2 = shared.hashL2(addr);
                int xL2 = shared.s[hL2].findAddr(addr);

                if (shared.s[hL2].w[xL2].addr != addr &&
                    shared.s[hL2].w[xL2].data != -1) {
                        updateData(shared.s[hL2].w[xL2].addr,
                                   shared.s[hL2].w[xL2].data);
                        shared.s[hL2].w[xL2].data = -1;
                    }

                if (shared.s[hL2].w[xL2].data == -1) {
                    T ramData = getData<T>(addr);

                    shared.s[hL2].w[xL2].data = ramData;
                    shared.s[hL2].w[xL2].addr = addr;
                    shared.s[hL2].w[xL2].last = true;
                    shared.s[hL2].w[!xL2].last = false;

                    core[c].s[hL1].w[xL1].data = ramData;
                    core[c].s[hL1].w[xL1].addr = addr;
                    core[c].s[hL1].w[xL1].last = true;
                    core[c].s[hL1].w[!xL1].last = false;
                    core[c].s[hL1].w[xL1].dirty = false;
                } else {
                    core[c].s[hL1].w[xL1].data = shared.s[hL2].w[xL2].data;
                    core[c].s[hL1].w[xL1].addr = shared.s[hL2].w[xL2].addr;
                    core[c].s[hL1].w[xL1].last = true;
                    core[c].s[hL1].w[!xL1].last = false;
                    core[c].s[hL1].w[xL1].dirty = false;
                }
            }
    }

    void write(int c, unsigned int addr, T data) {\
		// implements the write operations
        unsigned int hL1 = core[c].hashL1(addr);
        int xL1 = core[c].s[hL1].findAddr(addr);

        unsigned int hL2 = shared.hashL2(addr);
        int xL2 = shared.s[hL2].findAddr(addr);

        if (core[c].s[hL1].w[xL1].data == -1 ||
            core[c].s[hL1].w[xL1].addr != addr ||
            core[c].s[hL1].w[xL1].dirty) {
                unsigned int hL2 = shared.hashL2(addr);
                int xL2 = shared.s[hL2].findAddr(addr);

                if (shared.s[hL2].w[xL2].addr != addr &&
                    shared.s[hL2].w[xL2].data != -1) {
                        updateData(shared.s[hL2].w[xL2].addr,
                                   shared.s[hL2].w[xL2].data);
                        shared.s[hL2].w[xL2].data = -1;
                    }

                if (shared.s[hL2].w[xL2].data == -1) {
                    T ramData = getData<T>(addr);

                    shared.s[hL2].w[xL2].data = ramData;
                    shared.s[hL2].w[xL2].addr = addr;
                    shared.s[hL2].w[xL2].last = true;
                    shared.s[hL2].w[!xL2].last = false;

                    core[c].s[hL1].w[xL1].data = ramData;
                    core[c].s[hL1].w[xL1].addr = addr;
                    core[c].s[hL1].w[xL1].last = true;
                    core[c].s[hL1].w[!xL1].last = false;
                    core[c].s[hL1].w[xL1].dirty = false;

                    shared.s[hL2].w[xL2].data = data;
                    core[c].s[hL1].w[xL1].data = data;

                    core[!c].s[hL1].markDirty(addr);
                } else {
                    core[c].s[hL1].w[xL1].data = shared.s[hL2].w[xL2].data;
                    core[c].s[hL1].w[xL1].addr = shared.s[hL2].w[xL2].addr;
                    core[c].s[hL1].w[xL1].last = true;
                    core[c].s[hL1].w[!xL1].last = false;
                    core[c].s[hL1].w[xL1].dirty = false;

                    shared.s[hL2].w[xL2].data = data;
                    core[c].s[hL1].w[xL1].data = data;

                    core[!c].s[hL1].markDirty(addr);
                }
            } else {
                core[c].s[hL1].w[xL1].data = data;
                shared.s[hL2].w[xL2].data = core[c].s[hL1].w[xL1].data;

                core[!c].s[hL1].markDirty(addr);
            }
    }

    void print(){
        // writes the contents of ram and cache
        FILE *cacheOut = fopen("cache.out", "w");

        printRam();

        for (int i = 0; i <= 1; i++) {
            for (int j = 0; j < 2048; j++) {
                for (int k = 0; k <= 1; k++) {
                    if (core[i].s[j].w[k].data != -1) {
                        fprintf(cacheOut, "%d %d %d %d", j, k,
                                core[i].s[j].w[k].addr, core[i].s[j].w[k].data);
                        if (core[i].s[j].w[k].dirty) {
                            fprintf(cacheOut, " *");
                        }
                        fprintf(cacheOut, "\n");
                    }
                }
            }
            fprintf(cacheOut, "\n");
        }

        for (int i = 0; i < 8192; i++) {
            for (int j = 0; j <= 1; j++) {
                if (shared.s[i].w[j].data != -1) {
                    fprintf(cacheOut, "%d %d %d %d\n", i, j,
                            shared.s[i].w[j].addr, shared.s[i].w[j].data);
                }
            }
        }
        fclose(cacheOut);
    }
};


#endif // CACHE_H_INCLUDED
