#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


monimage* read_pgm_p2(char* filename, monimage* img) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        exit(1);
    }

    char line[3];
    fscanf(file, "%s\n", &img->mod);
    // fgets(line, sizeof(line), file);
    // if (line[0] != 'P' || line[1] != '2') {
    //     fprintf(stderr, "Format de fichier non valide. PGM P2 attendu.\n");
    //     exit(1);
    // }

    // strncpy(img->mod, "P2", 3);
    fscanf(file, "%d %d\n", &img->largeur, &img->hauteur);
    fscanf(file, "%d\n", &img->max);

    img->pixel = (int**)malloc(img->hauteur * sizeof(int*));
    for (int i = 0; i < img->hauteur; i++) {
        img->pixel[i] = (int*)malloc(img->largeur * sizeof(int));
    }

    for (int i = 0; i < img->hauteur; i++) {
        for (int j = 0; j < img->largeur; j++) {
            fscanf(file, "%lc", &img->pixel[i][j]);
        }
    }

    fclose(file);
    printf("Image lue avec succès : %dx%d\n", img->largeur, img->hauteur);
    return img;
}

void write_pgm_p2(char* filename, monimage* img) {
    FILE* file = fopen(filename, "wb");
    // if (file == NULL) {
    //     fprintf(stderr, "Impossible de créer le fichier %s\n", filename);
    //     exit(1);
    // }

    fprintf(file, "%s\n%d %d\n%d\n", img->mod, img->largeur, img->hauteur, img->max);
    for (int i = 0; i < img->hauteur; i++) {
        for (int j = 0; j < img->largeur; j++) {
            // fprintf(file, "%d ", img->pixel[i][j]);
            fwrite(&img->pixel[i][j], 1, 1, file);

        }
        // fprintf(file, "\n");
    }

    fclose(file);
    printf("Image écrite avec succès : %s\n", filename);
}

void calculate_histogram(monimage* img, int* histogram) {
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }
    for (int i = 0; i < img->hauteur; i++) {
        for (int j = 0; j < img->largeur; j++) {
            histogram[img->pixel[i][j]]++;
        }
    }
}

int otsu_threshold(int* histogram, int total_pixels) {
    int sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += i * histogram[i];
    }
    int sumB = 0;
    int wB = 0;
    int wF = 0;
    float max_variance = 0.0;
    int threshold = 0;
    for (int i = 0; i < 256; i++) {
        wB += histogram[i];
        if (wB == 0) continue;
        wF = total_pixels - wB;
        if (wF == 0) break;
        sumB += i * histogram[i];
        float mB = (float)sumB / wB;
        float mF = (float)(sum - sumB) / wF;
        float variance_between = (float)wB * (float)wF * (mB - mF) * (mB - mF);
        if (variance_between > max_variance) {
            max_variance = variance_between;
            threshold = i;
        }
    }
    return threshold;
}

monimage* otsu_thresholding(monimage* input, monimage* output) {
    int histogram[256];
    int total_pixels = input->largeur * input->hauteur;

    calculate_histogram(input, histogram);

    int threshold = otsu_threshold(histogram, total_pixels);
    printf("Seuil optimal trouvé : %d\n", threshold);

    for (int i = 0; i < input->hauteur; i++) {
        for (int j = 0; j < input->largeur; j++) {
            output->pixel[i][j] = (input->pixel[i][j] > threshold) ? 255 : 0;
        }
    }
    return output;
}

int abs_custom(int x) {
    return x < 0 ? -x : x;
}

monimage* kmeans_segmentation(monimage* input, monimage* output, int k) {
    int width = input->largeur;
    int height = input->hauteur;
    int total_pixels = width * height;

    // Allouer de la mémoire pour les centres et les étiquettes
    int* centers = (int*)malloc(k * sizeof(int));
    int* labels = (int*)malloc(total_pixels * sizeof(int));

    // Initialiser les centres de manière aléatoire
    for (int i = 0; i < k; i++) {
        centers[i] = rand() % 256;
    }

    int iterations = 0;
    int changed;

    do {
        changed = 0;

        // Assigner chaque pixel au centre le plus proche
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixel_value = input->pixel[y][x];
                int min_distance = INT_MAX;
                int best_center = 0;

                for (int i = 0; i < k; i++) {
                    int distance = abs_custom(pixel_value - centers[i]);
                    if (distance < min_distance) {
                        min_distance = distance;
                        best_center = i;
                    }
                }

                int index = y * width + x;
                if (labels[index] != best_center) {
                    labels[index] = best_center;
                    changed = 1;
                }
            }
        }

        // Mettre à jour les centres
        int* cluster_sizes = (int*)calloc(k, sizeof(int));
        int* new_centers = (int*)calloc(k, sizeof(int));

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = y * width + x;
                int label = labels[index];
                new_centers[label] += input->pixel[y][x];
                cluster_sizes[label]++;
            }
        }

        for (int i = 0; i < k; i++) {
            if (cluster_sizes[i] > 0) {
                centers[i] = new_centers[i] / cluster_sizes[i];
            }
        }

        free(cluster_sizes);
        free(new_centers);

        iterations++;
    } while (changed && iterations < MAX_ITERATIONS);

    // Créer l'image segmentée
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            output->pixel[y][x] = centers[labels[index]];
        }
    }

    free(centers);
    free(labels);

    printf("Segmentation K-means terminée en %d itérations\n", iterations);
    return output;
}
// Fonction pour vérifier l'homogénéité d'une région
// Fonction pour calculer la variance d'une région
long long calculate_variance(Region* region) {
    long long mean = region->sum / region->count;
    return (region->sum_sq / region->count) - (mean * mean);
}

// Fonction pour vérifier si une région doit être divisée
int should_split(Region* region) {
    return calculate_variance(region) > SPLIT_THRESHOLD;
}

// Fonction pour vérifier si deux régions peuvent être fusionnées
int can_merge(Region* r1, Region* r2) {
    long long total_sum = r1->sum + r2->sum;
    long long total_sum_sq = r1->sum_sq + r2->sum_sq;
    int total_count = r1->count + r2->count;
    long long mean = total_sum / total_count;
    long long variance = (total_sum_sq / total_count) - (mean * mean);
    return variance <= MERGE_THRESHOLD;
}

// Fonction pour diviser une région
void split_region(monimage* img, Region* region, Region* queue, int* queue_size) {
    if (region->width <= MIN_REGION_SIZE || region->height <= MIN_REGION_SIZE) {
        return;
    }

    int half_width = region->width / 2;
    int half_height = region->height / 2;

    Region top_left = {region->x, region->y, half_width, half_height, 0, 0, 0};
    Region top_right = {region->x + half_width, region->y, region->width - half_width, half_height, 0, 0, 0};
    Region bottom_left = {region->x, region->y + half_height, half_width, region->height - half_height, 0, 0, 0};
    Region bottom_right = {region->x + half_width, region->y + half_height, region->width - half_width, region->height - half_height, 0, 0, 0};

    // Calcul des sommes et sommes des carrés pour chaque sous-région
    for (int y = region->y; y < region->y + region->height; y++) {
        for (int x = region->x; x < region->x + region->width; x++) {
            int pixel = img->pixel[y][x];
            Region* sub_region;
            if (y < region->y + half_height) {
                if (x < region->x + half_width) sub_region = &top_left;
                else sub_region = &top_right;
            } else {
                if (x < region->x + half_width) sub_region = &bottom_left;
                else sub_region = &bottom_right;
            }
            sub_region->sum += pixel;
            sub_region->sum_sq += (long long)pixel * pixel;
            sub_region->count++;
        }
    }

    queue[(*queue_size)++] = top_left;
    queue[(*queue_size)++] = top_right;
    queue[(*queue_size)++] = bottom_left;
    queue[(*queue_size)++] = bottom_right;
}

// Fonction principale de segmentation par division-fusion
monimage* split_and_merge_segmentation(monimage* input, monimage* output) {
    int width = input->largeur;
    int height = input->hauteur;

    // Initialiser la file d'attente des régions
    Region* queue = malloc(width * height * sizeof(Region));
    int queue_size = 0;

    // Commencer avec l'image entière comme région initiale
    Region initial_region = {0, 0, width, height, 0, 0, 0};
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel = input->pixel[y][x];
            initial_region.sum += pixel;
            initial_region.sum_sq += (long long)pixel * pixel;
            initial_region.count++;
        }
    }
    queue[queue_size++] = initial_region;

    // Étape de division
    while (queue_size > 0) {
        Region current_region = queue[--queue_size];
        if (should_split(&current_region)) {
            split_region(input, &current_region, queue, &queue_size);
        } else {
            // Marquer la région comme homogène dans l'image de sortie
            int avg_color = (int)(current_region.sum / current_region.count);
            for (int y = current_region.y; y < current_region.y + current_region.height; y++) {
                for (int x = current_region.x; x < current_region.x + current_region.width; x++) {
                    output->pixel[y][x] = avg_color;
                }
            }
        }
    }

    // Étape de fusion (simplifiée)
    // Note: Cette implémentation est simplifiée et ne fusionne que les régions adjacentes dans l'ordre de parcours
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width - 1; x++) {
            Region r1 = {x, y, 1, 1, input->pixel[y][x], (long long)input->pixel[y][x] * input->pixel[y][x], 1};
            Region r2 = {x+1, y, 1, 1, input->pixel[y][x+1], (long long)input->pixel[y][x+1] * input->pixel[y][x+1], 1};
            if (can_merge(&r1, &r2)) {
                output->pixel[y][x+1] = output->pixel[y][x];
            }
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height - 1; y++) {
            Region r1 = {x, y, 1, 1, output->pixel[y][x], (long long)output->pixel[y][x] * output->pixel[y][x], 1};
            Region r2 = {x, y+1, 1, 1, output->pixel[y+1][x], (long long)output->pixel[y+1][x] * output->pixel[y+1][x], 1};
            if (can_merge(&r1, &r2)) {
                output->pixel[y+1][x] = output->pixel[y][x];
            }
        }
    }

    free(queue);
    return output;;
}

