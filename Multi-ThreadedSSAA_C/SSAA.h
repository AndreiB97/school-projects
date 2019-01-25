#ifndef SSAA_H
#define SSAA_H

#define COLOR 6
#define GRAYSCALE 5

typedef struct {
    // image properties
    unsigned char type;
    unsigned int width;
    unsigned int height;
    unsigned char maxval;

    // channels
    unsigned char **red;
    unsigned char **green;
    unsigned char **blue;
    unsigned char **gray;
}image;

typedef struct {
    // threaded resize arguments
    int thread_id;
    image *in;
    image *out;
} args;

int multGauss(unsigned char **M, int start_row, int start_col);

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

void* evenResize(void *var);

void* gaussResize(void *var);

#endif /* SSAA_H */
