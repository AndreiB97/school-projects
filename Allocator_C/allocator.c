#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char *arena;
int n;

void init(int dim);

void fin();

void dump();

void alloc(int dim);

void Free(int index);

void Fill(int index, int dim, int value);

void shFree();

void shUsage();

void shAlloc();

int main() {
    char *com = NULL, *p = NULL;
    int dim, i, index, value;
    if ((com = malloc(30*sizeof(unsigned char))) != NULL) {
        while(fgets(com, 30, stdin)) {
            // parse the commands
            p = strtok(com, " \n");
            if (strcmp(p, "INITIALIZE") == 0) {
                p = strtok(NULL, " ");
                n = 0;
                for (i = 0; i < strlen(p)-1; i++) {
                    n = n*10 + (p[i] - '0');
                }
                init(n);
            } else if (strcmp(p, "FINALIZE") == 0) {
                fin();
                break;
            } else if (strcmp(p, "DUMP") == 0) {
                dump();
            } else if (strcmp(p, "ALLOC") == 0) {
                p = strtok(NULL, " ");
                dim = 0;
                for (i = 0; i < strlen(p)-1; i++) {
                    dim = dim*10 + (p[i] - '0');
                }
                alloc(dim);
            } else if (strcmp(p, "FREE") == 0) {
                p = strtok(NULL, " ");
                index = 0;
                for (i = 0; i < strlen(p)-1; i++) {
                    index = index*10 + (p[i] - '0');
                }
                Free(index);
            } else if (strcmp(p, "FILL") == 0) {
                p = strtok(NULL, " \n");
                index = 0;
                for (i = 0; i < strlen(p); i++) {
                    index = index*10 + (p[i] - '0');
                }
                p = strtok(NULL, " \n");
                dim = 0;
                for (i = 0; i < strlen(p); i++) {
                    dim = dim*10 + (p[i] - '0');
                }
                p = strtok(NULL, " \n");
                value = 0;
                for (i = 0; i < strlen(p); i++) {
                    value = value*10 + (p[i] - '0');
                }
                Fill(index, dim, value);
            } else if (strcmp(p, "SHOW") == 0) {
                p = strtok(NULL, " \n");
                if (strcmp(p, "FREE") == 0) {
                    shFree();
                } else if (strcmp(p, "USAGE") == 0) {
                    shUsage();
                } else if (strcmp(p, "ALLOCATIONS") == 0) {
                    shAlloc();
                }
            }
        }
    }
    return 0;
}

void init(int dim) {
    // initialize memory
    arena = calloc(dim, sizeof(unsigned char));
}

void fin() {
    // free memory
    free(arena);
}

void dump() {
    // print content of memory on screen
    int i, j;
    for (i = 0; i < n; i+=16) {
        printf("%08X", i);
        printf("\t");
        for (j = i; j < i+8 && j < n; j++) {
            printf("%02X ", arena[j]);
        }
        printf(" ");
        for (; j < i+16 && j < n; j++) {
            printf("%02X ", arena[j]);
        }
        printf("\n");
    }
    printf("%08X", n);
    printf("\n");
}

void alloc(int dim) {
    // allocates a block of dim bytes
    int next, start, print, block, aux, i;

    dim += 12;
    start = arena[0]+arena[1]*256+arena[2]*256*256+arena[3]*256*256*256;
    // check if the allocation is possible
    if (dim > n-4) {
        printf("0\n");
    } else {
        // allocates block in the first empty spot
        // prints it's position in memory and sets the management area
        // if no empty area was found prints 0
        if (start == 0) {
            printf("16\n");
            if (dim/256/256/256) {
                arena[15] = dim/256/256/256;
                dim /= 256;
            }
            if (dim/256/256) {
                arena[14] = dim/256/256;
                dim /= 256;
            }
            if (dim/256) {
                arena[13] = dim/256;
                dim /= 256;
            }
            arena[12] = dim;
            arena[0] = 4;
        } else if (start-4 >= dim) {
            printf("16\n");
            if (dim/256/256/256) {
                arena[15] = dim/256/256/256;
                dim /= 256;
            }
            if (dim/256/256) {
                arena[14] = dim/256/256;
                dim /= 256;
            }
            if (dim/256) {
                arena[13] = dim/256;
                dim /= 256;
            }
            arena[12] = dim;

            for (i = 0; i < 4; i++) {
                arena[i+4] = arena[i];
            }

            arena[0] = 4;
            arena[1] = 0;
            arena[2] = 0;
            arena[3] = 0;

            next = arena[4]+arena[5]*256+arena[6]*256*256+arena[7]*256*256*256;

            for (i = 0; i < 4; i++) {
                arena[next+4+i] = arena[i];
            }
        } else {
            next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
            block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256*256;
            print = 0;
            while (next != 0 && print == 0) {
                if (next-start-block >= dim) {
                    printf("%d\n", start+block+12);
                    print = 1;
                    for (i = 0; i < 4; i++) {
                        arena[start+block+i] = arena[start+i];
                        arena[start+block+4+i] = arena[next+4+i];
                    }

                    if (dim/256/256/256) {
                        arena[start+block+11] = dim/256/256/256;
                        dim /= 256;
                    }
                    if (dim/256/256) {
                        arena[start+block+10] = dim/256/256;
                        dim /= 256;
                    }
                    if (dim/256) {
                        arena[start+block+9] = dim/256;
                        dim /= 256;
                    }
                    arena[start+block+8] = dim;

                    aux = start+block;
                    if (aux/256/256/256) {
                        arena[start+3] = aux/256/256/256;
                        aux /= 256;
                    }
                    if (aux/256/256) {
                        arena[start+2] = aux/256/256;
                        aux /= 256;
                    }
                    if (aux/256) {
                        arena[start+1] = aux/256;
                        aux /= 256;
                    }
                    arena[start] = aux;

                    for (i = 0; i < 4; i++) {
                        arena[next+4+i] = arena[start+i];
                    }
                } else {
                    start = next;
                    next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
                    block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256*256;
                }
            }

            if (print == 0) {
                if (n-start-block >= dim) {
                    printf("%d\n", start+block+12);
                    if (dim/256/256/256) {
                        arena[start+block+11] = dim/256/256/256;
                        dim /= 256;
                    }
                    if (dim/256/256) {
                        arena[start+block+10] = dim/256/256;
                        dim /= 256;
                    }
                    if (dim/256) {
                        arena[start+block+9] = dim/256;
                        dim /= 256;
                    }
                    arena[start+block+8] = dim;

                    aux = start;
                    if (aux/256/256/256) {
                        arena[start+block+7] = aux/256/256/256;
                        aux /= 256;
                    }
                    if (aux/256/256) {
                        arena[start+block+6] = aux/256/256;
                        aux /= 256;
                    }
                    if (aux/256) {
                        arena[start+block+5] = aux/256;
                        aux /=256;
                    }
                    arena[start+block+4] = aux;

                    aux = start+block;
                    if (aux/256/256/256) {
                        arena[start+3] = aux/256/256/256;
                        aux /= 256;
                    }
                    if (aux/256/256) {
                        arena[start+2] = aux/256/256;
                        aux /= 256;
                    }
                    if (aux/256) {
                        arena[start+1] = aux/256;
                        aux /= 256;
                    }
                    arena[start] = aux;
                } else {
                    printf("0\n");
                }
            }
        }
    }
}

void Free(int index) {
    int start, next, prev, i;

    start = index-12;
    next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
    prev = arena[start+4]+arena[start+5]*256+arena[start+6]*256*256+arena[start+7]*256*256*256;

    for (i = 0; i < 4; i++) {
        arena[prev+i] = arena[start+i];
        if (next) {
            arena[next+4+i] = arena[start+4+i];
        }
    }
}

void Fill(int index, int dim, int value) {
    // fills dim bytes from index with value
    // can overwrite management area
    while(dim) {
        arena[index+dim-1] = value;
        dim--;
    }
}

void shFree() {
    // prints how many bytes are free in memory
    int liber = 0, reg = 0;
    int start, next, block;
    start = arena[0]+arena[1]*256+arena[2]*256*256+arena[3]*256*256*256;
    if (start) {
        if (start-4) {
            reg++;
            liber += start-4;
        }
        next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
        block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256;
        while (next) {
            if (next-start-block) {
                reg++;
                liber += next-start-block;
            }
            start = next;
            next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
            block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256;
        }
        if (n-start-block) {
            reg++;
            liber += n-start-block;
        }
        printf("%d blocks (%d bytes) free\n", reg, liber);
    } else {
        printf("1 blocks (%d bytes) free\n", n-4);
    }
}

void shUsage() {
    // prints how many bytes are ussed, efficiency and fragmentation
    int start, next, block, reg = 0, bytes = 0, liber = 0;
    start = arena[0]+arena[1]*256+arena[2]*256*256+arena[3]*256*256*256;
    if (start) {
        next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
        block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256;
        bytes += block-12;
        reg++;
        if (start-4) {
            liber++;
        }
        while (next) {
            reg++;
            if (next-start-block) {
                liber++;
            }
            start = next;
            next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
            block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256;
            bytes += block-12;
        }
        if (n-start-block) {
            liber++;
        }
        printf("%d blocks (%d bytes) used\n%d%% efficiency\n%d%% fragmentation\n",
               reg, bytes, (int)((float)bytes/(float)(bytes+4+12*reg)*100), (int)((float)(liber-1)/(float)reg*100));
    } else {
        printf("0 blocks (0 bytes) used\n0%% efficiency\n0%% fragmentation\n");
    }
}

void shAlloc() {
    // prints all free/occupied areas
    int start, next, block;
    start = arena[0]+arena[1]*256+arena[2]*256*256+arena[3]*256*256*256;
    printf("OCCUPIED 4 bytes\n");
    if (start) {
        next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
        block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256;
        if (start-4) {
            printf("FREE %d bytes\n", start-4);
        }
        printf("OCCUPIED %d bytes\n", block);
        while (next) {
            if (next-start-block) {
                printf("FREE %d bytes\n", next-start-block);
            }
            start = next;
            next = arena[start]+arena[start+1]*256+arena[start+2]*256*256+arena[start+3]*256*256*256;
            block = arena[start+8]+arena[start+9]*256+arena[start+10]*256*256+arena[start+11]*256*256;
            printf("OCCUPIED %d bytes\n", block);
        }
        if (n-start-block) {
            printf("FREE %d bytes\n", n-start-block);
        }
    } else {
        printf("FREE %d bytes\n", n-4);
    }
}

