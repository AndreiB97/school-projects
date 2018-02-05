#include <stdio.h>

int reuniuneBaze(int a[400][400], int m, int n, int i, int j, int ct, int f);

int baza2(int v[200000], int x);

int power(int x);

int main () {
    int b, n, m, a, k = 0, v[200000], c, i, w[400][400] = {0};
    v[0] = 0;
    scanf("%d %d %d", &b, &m, &n);
    scanf("%d", &a);
    //task 1
    while (a != -1) {
        //construieste sirul in v
        if (k == 0) {
            k = b;
        }
        if (a == 0) {
            v[v[0]+1] = 0;
            v[0]++;
            k--;
        } else {
            while(1) {
                if (a < power(k)) {
                    c = baza2(v, a);
                    k -= v[0]+1-c;
                    break;
                } else {
                    a /= 2;
                }
            }
        }
        scanf("%d", &a);
    }
    //afiseaza sirul
    for (i = 1; i < v[0]+1; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
    //task 2
    int x, y, p;
    scanf("%d %d", &x, &y);
    while (x != -1 || y != -1) {
        x++;
        y++;
        //verifica daca coordonatele sunt bune
        if (y >= x && x <= v[0] && y > 0) {
            if (x <= 0) {
                x = 1;
            }
            if (y > v[0]) {
                y = v[0];
            }
            if (y-x > 30) {
                y = x+30;
            }
            if (y >= x) {
                a = 0;
                p = 1;
                //construieste numarul
                for (i = y; i >= x; i--) {
                    a = a+v[i]*p;
                    p *= 2;
                }
                if (a >= m*n) {
                    a = a%(m*n);
                }
                //afiseaza numarul
                printf("%d ", a);
                //marcheaza pozitia numarului
                //in matrice cu 1
                w[a/n][a%n] = 1;
            }
        }
        scanf("%d %d", &x, &y);
    }
    printf("\n");
    //task 3
    int j, ct, max = 0;
    //gaseste cea mai mare reuniune de baze
    for (i = 0; i <= m-1; i++) {
        for (j = 0; j <= n-1; j++) {
            if (w[i][j] == 1) {
                ct = reuniuneBaze(w, m, n, i, j, 0, 1);
                if (ct > max) {
                    max = ct;
                }
            }
        }
    }
    //afiseaza toate reuniunile maxime de baze
    for (i = 0; i <= m-1; i++) {
        for (j = 0; j<= n-1; j++) {
            if (w[i][j] == 2) {
                ct = reuniuneBaze(w, m, n, i, j, 0, 2);
                if (ct == max) {
                    printf("%d %d %d\n", i, j, max);
                }
            }
        }
    }

    return 0;
}

int reuniuneBaze(int a[400][400], int m, int n, int i, int j, int ct, int f) {
    //returneaza numarl de baze din reuniune
    if (a[i][j] == f) {
        a[i][j] = f+1;
        ct++;
        if (i+1 <= m-1) {
            ct = reuniuneBaze(a, m, n, i+1, j, ct, f);
        }
        if (i-1 >= 0) {
            ct = reuniuneBaze(a, m, n, i-1, j, ct, f);
        }
        if (j+1 <= n-1) {
            ct = reuniuneBaze(a, m, n, i, j+1, ct, f);
        }
        if (j-1 >= 0) {
            ct = reuniuneBaze(a, m, n, i, j-1, ct, f);
        }
    }
    return ct;
}

int baza2(int v[200000], int x) {
    //il intrasforma pe x din baza 10 in baza 2
    //il adauga la coada lui v
    //returneaza noua lungime a lui v
    int y = v[0]+1;
    while (x) {
        v[v[0]+1] = x%2;
        x /= 2;
        v[0]++;
    }
    int i, aux;
    for (i = y; i <= y+(v[0]+1-y)/2-1; i++){
        aux = v[i];
        v[i] = v[v[0]-i+y];
        v[v[0]-i+y] = aux;
    }
    return y;
}

int power(int x) {
    //returneaza 2 la puterea x
    if (x > 0) {
        return 2*power(x-1);
    }
    return 1;
}
