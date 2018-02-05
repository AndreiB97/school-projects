#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int prim(int x);

int nrMagic(int x);

float pointIntensity(int x, int y,
                     int lightX, int lightY,
                     float lightIntensity);

float distanta(int x1, int x2, int y1, int y2);

int main() {
    int lightX, lightY;
    float lightSourceIntensity;
    int n, i, x;
    int treeX1, treeY1, treeX2, treeY2, treeX3, treeY3;
    int indice = 0, inaltime = 1001;
    float treeIntensity, intensitate = 0;
    double a, b, c;
	double angle;

    scanf("%d %d", &lightX, &lightY);
    scanf("%f", &lightSourceIntensity);
    scanf("%d", &n);

    scanf("%d %d", &treeX1, &treeY1);
    scanf("%d %d", &treeX2, &treeY2);

    for (i = 2; i < n; i++) {
        scanf("%d %d", &treeX3, &treeY3);
        treeIntensity = pointIntensity(treeX2, treeY2,
                                       lightX, lightY,
                                       lightSourceIntensity);
        x = (((int)treeIntensity)%10)*1000+
            (treeY1%10)*100+(treeY2%10)*10+treeY3%10;
        if (nrMagic(x)) {
            if (inaltime > treeY2) {
                inaltime = treeY2;
                indice = i-1;
                intensitate = treeIntensity;
                a = distanta(treeX1,treeY1,treeX2,treeY2);
                b = distanta(treeX2,treeY2,treeX3,treeY3);
                c = distanta(treeX1,treeY1,treeX3,treeY3);
            }
        }
        treeX1 = treeX2;
        treeY1 = treeY2;
        treeX2 = treeX3;
        treeY2 = treeY3;
    }
    if (indice == 0) {
        printf("There is no place for you in DwarfLand!\n");
    } else {
        printf("%d %.4f \n", indice, intensitate);
	 	angle = 180-acos((c*c-b*b-a*a)/(2*a*b))*180/PI;
    	if (angle > 90 && angle < 180) {
    	    printf("%.4lf IT CAN BE EXTENDED\n", angle);
	    } else {
			printf("%.4lf IT CAN NOT BE EXTENDED\n", angle);
		}
	}

    return 0;
}

int prim(int x) {
    //returneaza 1 daca x prim
    //altfel returneaza 0
    if (x == 1) {
        return 0;
    }
    int i;
    for (i = 2; i <= x/2; i++) {
        if (x%i == 0) {
            return 0;
        }
    }
    return 1;
}

int nrMagic(int x) {
    //returneaza 1 daca x nr magic
    //altfel returneaza 0
    while (x) {
        if (prim(x) == 0) {
            return 0;
        }
        x/=10;
    }
    return 1;
}

float pointIntensity(int x, int y,
                     int lightX, int lightY,
                     float lightIntensity) {
    //returneaza intensitatea luminii intr-un punct
    //cu coordonatele x si y
    float a = (x-lightX)*(x-lightX) + (y-lightY)*(y-lightY);
    return (1/a)*lightIntensity;
}

float distanta(int x1, int y1, int x2, int y2) {
    //returneaza distanta dintre 2 puncte
    //de coordonate x1,y1 si x2,y2
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
