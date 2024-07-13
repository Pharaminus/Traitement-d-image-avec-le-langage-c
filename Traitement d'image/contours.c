#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_WIDTH 1024
#define MAX_HEIGHT 1024

typedef struct {
    int width;
    int height;
    int max_value;
    unsigned char data[MAX_HEIGHT][MAX_WIDTH];
} Image;

// Function prototypes
void read_pgm(const char *filename, Image *image);
void write_pgm(const char *filename, const Image *image);
void roberts_filter(const Image *input, Image *output);

// Reading PGM file
void read_pgm(const char *filename, Image *image) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(1);
    }

    fscanf(file, "P5\n%d %d\n%d\n", &image->width, &image->height, &image->max_value);
    fread(image->data, sizeof(unsigned char), image->width * image->height, file);
    fclose(file);
}

// Writing PGM file
void write_pgm(const char *filename, const Image *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(1);
    }

    fprintf(file, "P5\n%d %d\n%d\n", image->width, image->height, image->max_value);
    fwrite(image->data, sizeof(unsigned char), image->width * image->height, file);
    fclose(file);
}

// Roberts filter
void roberts_filter(const Image *input, Image *output) {
    int gx, gy;
    for (int y = 0; y < input->height - 1; y++) {
        for (int x = 0; x < input->width - 1; x++) {
            gx = input->data[y][x] - input->data[y + 1][x + 1];
            gy = input->data[y + 1][x] - input->data[y][x + 1];
            output->data[y][x] = (unsigned char) sqrt(gx * gx + gy * gy);
        }
    }
}

// Main function
int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s ImageFold/1.pgm output_linear_1.pgm\n", argv[0]);
        return 1;
    }

    Image input, output;
    read_pgm(argv[1], &input);
    roberts_filter(&input, &output);
    write_pgm(argv[2], &output);

    return 0;
}
