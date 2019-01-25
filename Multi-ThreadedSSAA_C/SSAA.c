#include "SSAA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFLEN 255

int num_threads;
int resize_factor;

void readInput(const char * fileName, image *img) {
    // read image properties and allocate memory
    FILE *in = fopen(fileName, "r");
    char buff[BUFFLEN];
    char *word;
    int row, col;

    // read and set type
    fgets(buff, 255, in);
    img->type = buff[1] - '0';

    // read and set width and height
    fgets(buff, 255, in);

    word = strtok(buff, " \n");
    img->width = atoi(word);

    word = strtok(NULL, " \n");
    img->height = atoi(word);

    // read and set maxval
    fgets(buff, 255, in);
    img->maxval = atoi(buff);

    // read and allocate memory
    if (img->type == COLOR) {
        unsigned char byteBuffer[3 * img->width];

        img->red = (unsigned char **)malloc(sizeof(unsigned char *) * img->height);
        img->green = (unsigned char **)malloc(sizeof(unsigned char *) * img->height);
        img->blue = (unsigned char **)malloc(sizeof(unsigned char *) * img->height);
        img->gray = NULL;


        for (row = 0; row < img->height; row++) {
            img->red[row] = (unsigned char *)malloc(sizeof(unsigned char) * img->width);
            img->green[row] = (unsigned char *)malloc(sizeof(unsigned char) * img->width);
            img->blue[row] = (unsigned char *)malloc(sizeof(unsigned char) * img->width);

            fread(byteBuffer, sizeof(unsigned char), 3 * img->width, in);

            for (col = 0; col < img->width; col++) {
                img->red[row][col] = byteBuffer[col * 3];
                img->green[row][col] = byteBuffer[col * 3 + 1];
                img->blue[row][col] = byteBuffer[col * 3 + 2];
            }
        }

    } else if (img->type == GRAYSCALE) {
        img->gray = malloc(sizeof(unsigned char *) * img->height);
        img->red = NULL;
        img->green = NULL;
        img->blue = NULL;

        for (row = 0; row < img->height; row++) {
            img->gray[row] = malloc(sizeof(unsigned char) * img->width);

            fread(img->gray[row], sizeof(unsigned char), img->width, in);
        }
    }

    fclose(in);
}

void writeData(const char * fileName, image *img) {
    // write image and free memory
    FILE *out = fopen(fileName, "w");
    int row, col;
    // print type
    fprintf(out, "P%d\n", img->type);

    // print width and height
    fprintf(out, "%d %d\n", img->width, img->height);

    // print maxval
    fprintf(out, "%d\n", img->maxval);

    // print channel data & free memory
    if (img->type == COLOR) {
        unsigned char byteBuffer[3 * img->width];

        for (row = 0; row < img->height; row++) {
            for (col = 0; col < img->width; col++) {
                byteBuffer[col * 3] = img->red[row][col];
                byteBuffer[col * 3 + 1] = img->green[row][col];
                byteBuffer[col * 3 + 2] = img->blue[row][col];
            }

            fwrite(byteBuffer, sizeof(unsigned char), 3 * img->width, out);

            free(img->red[row]);
            free(img->green[row]);
            free(img->blue[row]);
        }

        free(img->red);
        free(img->green);
        free(img->blue);
    } else if (img->type == GRAYSCALE) {
        for (row = 0; row < img->height; row++) {
            fwrite(img->gray[row], sizeof(unsigned char), img->width, out);

            free(img->gray[row]);
        }

        free(img->gray);
    }

    fclose(out);
}

void resize(image *in, image * out) {
    pthread_t t_id[num_threads];
    int i;
    args arguments[num_threads];

    // prepare output image properties
    out->maxval = in->maxval;
    out->type = in->type;
    out->height = in->height / resize_factor;
    out->width = in->width / resize_factor;

    // prepare threaded function's arguments
    for (i = 0; i < num_threads; i++) {
        arguments[i].in = in;
        arguments[i].out = out;
        arguments[i].thread_id = i;
    }

    // allocate memory for output image
    if (in->type == COLOR) {
        out->red = (unsigned char **)malloc(sizeof(unsigned char *) * out->height);
        out->green = (unsigned char **)malloc(sizeof(unsigned char *) * out->height);
        out->blue = (unsigned char **)malloc(sizeof(unsigned char *) * out->height);
        out->gray = NULL;

        for (i = 0; i < out->height; i++) {
            out->red[i] = (unsigned char *)malloc(sizeof(unsigned char) * out->width);
            out->green[i] = (unsigned char *)malloc(sizeof(unsigned char) * out->width);
            out->blue[i] = (unsigned char *)malloc(sizeof(unsigned char) * out->width);
        }
    } else if (in->type == GRAYSCALE) {
        out->red = NULL;
        out->green = NULL;
        out->blue = NULL;
        out->gray = (unsigned char **)malloc(sizeof(unsigned char *) * out->height);

        for (i = 0; i < out->height; i++) {
            out->gray[i] = (unsigned char *)malloc(sizeof(unsigned char) * out->width);
        }
    }

    // start threads
    // function is based on resize factor
    if (resize_factor % 2 == 0) {
        for (i = 0; i < num_threads; i++) {
            pthread_create(&(t_id[i]), NULL, evenResize, &(arguments[i]));
        }
    } else {
        for (i = 0; i < num_threads; i++) {
            pthread_create(&(t_id[i]), NULL, gaussResize, &(arguments[i]));
        }
    }

    // rejoin main thread
    for (i = 0; i < num_threads; i++) {
        pthread_join(t_id[i], NULL);
    }
}

int multGauss(unsigned char **M, int start_row, int start_col) {
    // calculate resulting pixel color after gauss multiplication
    int sum = 0;
    int row, col;
    char gauss[3][3] = {{1, 2, 1},
                        {2, 4, 2},
                        {1, 2, 1}};

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            sum += M[row + start_row][col + start_col] * gauss[row][col];
        }
    }

    return sum / 16;
}

void* gaussResize(void *var) {
    // resize using gauss kernel
    args *arguments = (args*) var;
    int c = 0;
    int block_row, block_col;

    // each thread processes a pixel and then jumps over num_threads pixels
    if (arguments->in->type == COLOR) {
        // calculate row and col index
        block_row = (arguments->thread_id + c) / arguments->out->width;
        block_col = (arguments->thread_id + c) % arguments->out->width;

        // process pixel blocks
        while (block_row < arguments->out->height) {
            arguments->out->red[block_row][block_col] = multGauss(arguments->in->red,
                                                                  block_row * resize_factor, block_col * resize_factor);
            arguments->out->green[block_row][block_col] = multGauss(arguments->in->green,
                                                                    block_row * resize_factor, block_col * resize_factor);
            arguments->out->blue[block_row][block_col] = multGauss(arguments->in->blue,
                                                                   block_row * resize_factor, block_col * resize_factor);

            // calculate new row and col index
            c += num_threads;
            block_row = (arguments->thread_id + c) / arguments->out->width;
            block_col = (arguments->thread_id + c) % arguments->out->width;
        }
    } else if (arguments->in->type == GRAYSCALE) {
        // calculate row and col index
        block_row = (arguments->thread_id + c) / arguments->out->width;
        block_col = (arguments->thread_id + c) % arguments->out->width;

        // process pixel blocks
        while (block_row < arguments->out->height) {
            arguments->out->gray[block_row][block_col] = multGauss(arguments->in->gray,
                                                                   block_row * resize_factor, block_col * resize_factor);

            // calculate new row and col index
            c += num_threads;
            block_row = (arguments->thread_id + c) / arguments->out->width;
            block_col = (arguments->thread_id + c) % arguments->out->width;
        }
    }
}

void* evenResize(void* var) {
    // resize using even resize vactor
    args *arguments = (args*) var;
    int row, col;
    int c = 0;
    int block_row, block_col;

    // each thread processes a pixel and then jumps over num_threads pixels
    if (arguments->in->type == COLOR) {
        // calculate row and col index
        block_row = (arguments->thread_id + c) / arguments->out->width;
        block_col = (arguments->thread_id + c) % arguments->out->width;

        while (block_row < arguments->out->height) {
            // get average from (Tid + c) / w , (Tid + c) % w to (Tid + c + 1) / w, (Tid + c + 1) % w
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int num = 0;

            // calculate average pixel value
            for (row = block_row * resize_factor; row < (block_row + 1) * resize_factor; row++) {
                    for (col = block_col * resize_factor; col < (block_col + 1) * resize_factor; col++) {
                            sumRed += arguments->in->red[row][col];
                            sumGreen += arguments->in->green[row][col];
                            sumBlue += arguments->in->blue[row][col];
                            num++;
                        }
                }

            arguments->out->red[block_row][block_col] = sumRed / num;
            arguments->out->green[block_row][block_col] = sumGreen / num;
            arguments->out->blue[block_row][block_col] = sumBlue / num;

            // calculate new row and col index
            c += num_threads;
            block_row = (arguments->thread_id + c) / arguments->out->width;
            block_col = (arguments->thread_id + c) % arguments->out->width;
        }
    } else if (arguments->in->type == GRAYSCALE) {
        // calculate row and col index
        block_row = (arguments->thread_id + c) / arguments->out->width;
        block_col = (arguments->thread_id + c) % arguments->out->width;

        while (block_row < arguments->out->height) {
            // get average from (Tid + c) / w , (Tid + c) % w to (Tid + c + 1) / w, (Tid + c + 1) % w
            int sumGray = 0;
            int num = 0;

            // calculate average pixel value
            for (row = block_row * resize_factor; row < (block_row + 1) * resize_factor; row++) {
                    for (col = block_col * resize_factor; col < (block_col + 1) * resize_factor; col++) {
                            sumGray += arguments->in->gray[row][col];
                            num++;
                        }
                }

            arguments->out->gray[block_row][block_col] = sumGray / num;

            // calculate new row and col index
            c += num_threads;
            block_row = (arguments->thread_id + c) / arguments->out->width;
            block_col = (arguments->thread_id + c) % arguments->out->width;
        }
    }
}
