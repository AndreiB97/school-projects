#include "stdio.h"
#include "fstream"

using namespace std;

#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

// structura pentru ram

template <class T>
class ram {
public:
    unsigned int addr;
    T data;
};

// declarare vector ram si dimensiune
ram<int> ramArr[1000000];
int n;

// modificarea unei valori din ram
template <class T>
void updateData (unsigned int addr, T data) {
        int i = (addr-ramArr[0].addr)/4;
        ramArr[i].data = data;
    }

// returneaza valoarea de la adresa addr
template <class T>
T getData (unsigned int addr) {
    int i = (addr-ramArr[0].addr)/4;
    return ramArr[i].data;
}

// scrie continutul ramului in ram.out
void printRam() {
    FILE *ramOut = fopen("ram.out", "w");
    for (int i = 0; i < n; i++) {
        fprintf(ramOut, "%010d %010d\n", ramArr[i].addr, ramArr[i].data);
    }
}

// structura pentru un way

template <class T>
class way {
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

// structura pentru un set

template <class T>
class set {
public:
    way<T> w[2];

    int findFirst() {
        // returneaza indexul
        // primului way daca ambele sunt goale
        // way-ului liber daca unul dintre ele este liber
        // primului way adaugat in set daca ambele sunt ocupate
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
        // daca adresa addr se afla intr-unul din way-uri
        // returneaza indexul respectiv
        // daca nu returneaza indexul mai putin recent
        for (int i = 0; i <= 1; i++) {
            if (w[i].addr == addr) {
                return i;
            }
        }
        return findFirst();
    }

    void markDirty(unsigned int addr) {
        // marcheaza dirty valoarea de la adresa addr
        for (int i = 0; i <= 1; i++) {
            if (w[i].addr == addr) {
                w[i].dirty = true;
            }
        }
    }
};

// structura pentru cache L1

template <class T>
class L1 {
public:
    set<T> s[2048];

    unsigned int hashL1(unsigned int key) {
        unsigned int hash = 0;
        int p = 1;
        // elemina bitii 0 si 1
        key /= 4;
        for (int i = 2; i <= 12 && key; i++)  {
            // adauga bitii 2-12 in hash
            hash += (key%2)*p;
            p *= 2;
            key /= 2;
        }
        return hash;
    }

};

// structura pentru cache L2

template <class T>
class L2 {
public:
    set<T> s[8192];

    unsigned int hashL2(unsigned int key) {
        unsigned int hash = 0;
        int p = 1;
        // elimina bitii 0 si 1
        key /= 4;
        for (int i = 2; i <= 14 && key; i++)  {
            // adauga bitii 2-14 la hash
            hash += (key%2)*p;
            p *= 2;
            key /= 2;
        }
        return hash;
    }
};

// structura pentru procesor

template <class T>
class CPU {
    L1<T> core[2];
    L2<T> shared;
public:
    void read(int c, unsigned int addr) {
        unsigned int hL1 = core[c].hashL1(addr);
        int xL1 = core[c].s[hL1].findAddr(addr);

        // verifica daca datele nu exista in L1 sau sunt dirty
        if (core[c].s[hL1].w[xL1].data == -1 ||
            core[c].s[hL1].w[xL1].addr != addr ||
            core[c].s[hL1].w[xL1].dirty) {
                unsigned int hL2 = shared.hashL2(addr);
                int xL2 = shared.s[hL2].findAddr(addr);

                // verifica daca trebuie scos din L2
                if (shared.s[hL2].w[xL2].addr != addr &&
                    shared.s[hL2].w[xL2].data != -1) {
                        // scrie in ram
                        updateData(shared.s[hL2].w[xL2].addr,
                                   shared.s[hL2].w[xL2].data);
                        shared.s[hL2].w[xL2].data = -1;
                    }

                // verifica daca nu exista in L2
                if (shared.s[hL2].w[xL2].data == -1) {
                    // cauta in ram si adauga in L1 si L2
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
                    // copiaza din L2 in L1
                    core[c].s[hL1].w[xL1].data = shared.s[hL2].w[xL2].data;
                    core[c].s[hL1].w[xL1].addr = shared.s[hL2].w[xL2].addr;
                    core[c].s[hL1].w[xL1].last = true;
                    core[c].s[hL1].w[!xL1].last = false;
                    core[c].s[hL1].w[xL1].dirty = false;
                }
            }
    }

    void write(int c, unsigned int addr, T data) {
        unsigned int hL1 = core[c].hashL1(addr);
        int xL1 = core[c].s[hL1].findAddr(addr);

        unsigned int hL2 = shared.hashL2(addr);
        int xL2 = shared.s[hL2].findAddr(addr);

        // verifica daca nu  exista in L1 sau e dirty
        if (core[c].s[hL1].w[xL1].data == -1 ||
            core[c].s[hL1].w[xL1].addr != addr ||
            core[c].s[hL1].w[xL1].dirty) {
                unsigned int hL2 = shared.hashL2(addr);
                int xL2 = shared.s[hL2].findAddr(addr);

                //verifica daca trebuie scos din L2
                if (shared.s[hL2].w[xL2].addr != addr &&
                    shared.s[hL2].w[xL2].data != -1) {
                        updateData(shared.s[hL2].w[xL2].addr,
                                   shared.s[hL2].w[xL2].data);
                        shared.s[hL2].w[xL2].data = -1;
                    }

                // verifica daca nu exista in L2
                if (shared.s[hL2].w[xL2].data == -1) {
                    T ramData = getData<T>(addr);

                    // copiaza din ram in L1 si L2
                    shared.s[hL2].w[xL2].data = ramData;
                    shared.s[hL2].w[xL2].addr = addr;
                    shared.s[hL2].w[xL2].last = true;
                    shared.s[hL2].w[!xL2].last = false;

                    core[c].s[hL1].w[xL1].data = ramData;
                    core[c].s[hL1].w[xL1].addr = addr;
                    core[c].s[hL1].w[xL1].last = true;
                    core[c].s[hL1].w[!xL1].last = false;
                    core[c].s[hL1].w[xL1].dirty = false;

                    // actualizeaza in L1 si L2
                    shared.s[hL2].w[xL2].data = data;
                    core[c].s[hL1].w[xL1].data = data;

                    // marcare dirty
                    core[!c].s[hL1].markDirty(addr);
                } else {
                    // copiaza din L2 in L1
                    core[c].s[hL1].w[xL1].data = shared.s[hL2].w[xL2].data;
                    core[c].s[hL1].w[xL1].addr = shared.s[hL2].w[xL2].addr;
                    core[c].s[hL1].w[xL1].last = true;
                    core[c].s[hL1].w[!xL1].last = false;
                    core[c].s[hL1].w[xL1].dirty = false;

                    // actualizeaza in L1 si L2
                    shared.s[hL2].w[xL2].data = data;
                    core[c].s[hL1].w[xL1].data = data;

                    // marcare dirty
                    core[!c].s[hL1].markDirty(addr);
                }
            } else {
                // actualizeaza in L1 si L2
                core[c].s[hL1].w[xL1].data = data;
                shared.s[hL2].w[xL2].data = core[c].s[hL1].w[xL1].data;

                // marcare dirty
                core[!c].s[hL1].markDirty(addr);
            }
    }

    void print(){
        // scrie memoria in fisiere
        FILE *cacheOut = fopen("cache.out", "w");

        printRam();

        // scrie datele din L1
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

        // scrie datele din L2
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
